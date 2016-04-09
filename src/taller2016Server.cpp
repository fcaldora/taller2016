#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <sys/unistd.h>
#include <string.h>
#include <list>
#include <thread>
#include "ClientAttendant.h"
#include "ClientMessage.h"
#include "XmlParser.h"
#include "CargadorXML.h"
#include "ErrorLogWriter.h"
#include "Procesador.h"
#include <mutex>

#define MAXHOSTNAME 256
#define kServerTestFile "serverTest.txt"

using namespace std;
struct msjProcesado{
	clientMsj mensaje;
	int socket;
	bool procesado;
};

list<msjProcesado> messagesList;
std::mutex mutexColaMensajes;
std::thread clientEntrada[5];
std::thread clientSalida[5];
bool appShouldTerminate;


int sendMsj(int socket,int bytesAEnviar, clientMsj* mensaje){
	int enviados = 0;
	int res = 0;
	cout << "send message starts" << endl;

	while(enviados < bytesAEnviar){
		res = send(socket, &(mensaje)[enviados], bytesAEnviar - enviados, MSG_WAITALL);
		if (res == 0){ //Se cerró la conexion. Escribir en log de errores de conexion.
			return 0;
		}else if(res<0){ //Error en el envio del mensaje. Escribir en el log.
			return -1;
		}else{
			enviados += res;
		}
	}
	cout << "send message finish" << endl;

	return enviados;
}

int readMsj(int socket, int bytesARecibir, clientMsj* mensaje){
	int recibidos = 0;
	int totalBytesRecibidos = 0;
	while (recibidos < bytesARecibir){
		recibidos = recv(socket, &mensaje[totalBytesRecibidos], bytesARecibir - totalBytesRecibidos, MSG_WAITALL);
		if (recibidos < 0){
			shutdown(socket, SHUT_RDWR);
			return -1;
		}else if(recibidos == 0){//se corto la conexion desde el lado del servidor.
				shutdown(socket, SHUT_RDWR);
				return -1;
		}else{
			totalBytesRecibidos += recibidos;
		}
	}
	return 1;
}

void *procesarMensajes(list<msjProcesado> *msgList){
	Procesador procesador;
	msjProcesado *auxiliar;
	while (!appShouldTerminate){
		mutexColaMensajes.lock();
		auxiliar = &msgList->front();
		mutexColaMensajes.unlock();
		if(!msgList->empty() && (auxiliar->procesado == false) && !appShouldTerminate){
			clientMsj respuesta;
			strncpy(respuesta.id,auxiliar->mensaje.id, 20);
			strncpy(respuesta.type,auxiliar->mensaje.type,20);
			if(procesador.isMsgValid(auxiliar->mensaje.type, auxiliar->mensaje.value)){
				strncpy(respuesta.value,"Mensaje correcto",20);
				cout<<"Mensaje correcto"<<endl;
			}else{
				strncpy(respuesta.value,"Mensaje incorrecto",20);
				cout<<"Mensaje incorrecto"<<endl;
			}
			auxiliar->mensaje = respuesta;
		auxiliar->procesado = true;
		}
	}
	pthread_exit(NULL);
}

void *clientReader(int socketConnection, list<msjProcesado>* messagesList){
	int res = 0;
	while(!appShouldTerminate){
		clientMsj recibido;
		res = readMsj(socketConnection, sizeof(clientMsj), &recibido);
		if (res< 0){
			shutdown(socketConnection, SHUT_RDWR);
			appShouldTerminate = true;
		}else{
			cout << "socket connection: " << socketConnection << ", msj:" << recibido.value << endl;
			cout<<"Tipo: "<<recibido.type<<" con id: "<<recibido.id<<endl;
			clientMsj respuesta;
			msjProcesado mensaje;
			mensaje.mensaje = recibido;
			mensaje.procesado = false;
			mensaje.socket = socketConnection;
			strncpy(respuesta.id,mensaje.mensaje.id,sizeof(respuesta.id));
			strncpy(respuesta.type, mensaje.mensaje.type, sizeof(respuesta.type));
			strncpy(respuesta.value, mensaje.mensaje.value, sizeof(respuesta.value));

			mutexColaMensajes.lock();
			messagesList->push_front(mensaje);//CAMBIAR POR PUSH BACK!! PUSH FRONT SOLO PARA PROBAR
			mutexColaMensajes.unlock();

			cout<<"Numero de mensajes en la cola: "<<messagesList->size()<<endl;
		}
	}
	pthread_exit(NULL);
}

void *responderCliente(int socket, list<msjProcesado> *msgList){
	msjProcesado auxiliar;
	int respuesta;

	while(!appShouldTerminate){
		mutexColaMensajes.lock();
		auxiliar = msgList->front();
		if((auxiliar.socket == socket) && (auxiliar.procesado == true)){
			msgList->pop_front();
			respuesta = sendMsj(socket, sizeof(auxiliar.mensaje), &(auxiliar.mensaje));
			if(respuesta<=0){
				appShouldTerminate = true;
			}
		}
		mutexColaMensajes.unlock();
	}

	pthread_exit(NULL);
}

void* waitForClientConnection(int numberOfCurrentAcceptedClients, int maxNumberOfClients, int socketHandle) {
	while (!appShouldTerminate) {
		if (numberOfCurrentAcceptedClients < maxNumberOfClients) {
			cout << "Waiting for a client connection" << endl;
			int socketConnection = accept(socketHandle, NULL, NULL);
			cout << "connection received" << endl;
			clientEntrada[numberOfCurrentAcceptedClients] = std::thread(clientReader, socketConnection, &messagesList);
			clientSalida[numberOfCurrentAcceptedClients] = std::thread(responderCliente, socketConnection, &messagesList);
			numberOfCurrentAcceptedClients++;
		}
	}
	pthread_exit(NULL);
}

void prepareForExit(XMLLoader *xmlLoader, XmlParser *xmlParser , ErrorLogWriter *errorLogWriter, string exitMessage){
	cout << exitMessage << endl;
	delete errorLogWriter;
	delete xmlLoader;
	delete xmlParser;
}

int main(int argc, char* argv[]) {
	const char* fileName;
	ErrorLogWriter *errorLogWriter = new ErrorLogWriter;
	XMLLoader *xmlLoader = new XMLLoader(errorLogWriter);
	int numberOfCurrentAcceptedClients = 0;
	if(argc != 2){
		fileName = kServerTestFile;
		string error = "Falta escribir el nombre del archivo, se usara uno por defecto";
		errorLogWriter->writeErrorInFile(error);
	} else {
		fileName = argv[1];
		if (!xmlLoader->serverXMLIsValid(fileName)){
			fileName = kServerTestFile;
		}
	}

	XmlParser *parser = new XmlParser(fileName);
	struct sockaddr_in socketInfo;
	char sysHost[MAXHOSTNAME + 1]; // Hostname of this computer we are running on
	struct hostent *hPtr;
	int socketHandle;
	int portNumber = parser->getServerPort();
	int maxNumberOfClients = parser->getMaxNumberOfClients();

	bzero(&socketInfo, sizeof(sockaddr_in));  // Clear structure memory

	// Get system information

	gethostname(sysHost, MAXHOSTNAME); // Get the name of this computer we are running on
	if ((hPtr = gethostbyname(sysHost)) == NULL) {
		cerr << "System hostname misconfigured." << endl;
		prepareForExit(xmlLoader, parser, errorLogWriter, "System hostname misconfigured exit");
		exit(EXIT_FAILURE);
	}

	// create socket

	if ((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		close(socketHandle);
		prepareForExit(xmlLoader, parser, errorLogWriter, "Socket Handle exit");
		exit(EXIT_FAILURE);
	}

	// Load system information into socket data structures

	socketInfo.sin_family = AF_INET;
	socketInfo.sin_addr.s_addr = htonl(INADDR_ANY); // Use any address available to the system
	socketInfo.sin_port = htons(portNumber);  // Set port number

	// Bind the socket to a local socket address
	if (bind(socketHandle, (struct sockaddr *) &socketInfo, sizeof(socketInfo))
			< 0) {
		close(socketHandle);
		perror("bind");
		prepareForExit(xmlLoader, parser, errorLogWriter, "bind exit ");
		exit(EXIT_FAILURE);
	}
	if (listen(socketHandle, 5) == -1) {
		prepareForExit(xmlLoader, parser, errorLogWriter, "listen exit ");
		return EXIT_FAILURE;
	}

	appShouldTerminate = false;
	std::thread procesadorMensajes(procesarMensajes, &messagesList);
	std::thread clientConnectionWaiter(waitForClientConnection, numberOfCurrentAcceptedClients, maxNumberOfClients, socketHandle);
	while (!appShouldTerminate) {
		cout << "Menu:" << endl;
		cout << "1. Salir" << endl;
		cout << "Ingresar opcion:" << endl;
		unsigned int userDidChooseOption;
		cin >> userDidChooseOption;
		if (userDidChooseOption == 1) {
			appShouldTerminate = true;
		} else {
			cout << "Opcion incorrecta" << endl;
		}
	}

	for(int i = 0; i < numberOfCurrentAcceptedClients; i++){
		clientEntrada[i].join();
		clientSalida[i].join();
	}
	procesadorMensajes.detach();

	clientConnectionWaiter.detach();

	close(socketHandle);
	prepareForExit(xmlLoader, parser, errorLogWriter, "Close");
	return EXIT_SUCCESS;
}
