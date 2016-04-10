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
#include "XmlParser.h"
#include "CargadorXML.h"
#include "LogWriter.h"
#include "Procesador.h"
#include <mutex>

#include "Constants.h"

#define kMaxHostName 256
#define kServerTestFile "serverTest.txt"

using namespace std;
struct msjProcesado{
	clientMsj mensaje;
	int socket;
	bool procesado;
};

list<msjProcesado> messagesList;
std::mutex mutexColaMensajes;
std::mutex writingInLogFileMutex;
std::thread clientEntrada[5];
std::thread clientSalida[5];
bool appShouldTerminate;
LogWriter *logWriter;

int sendMsj(int socket,int bytesAEnviar, clientMsj* mensaje){
	int enviados = 0;
	int res = 0;

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
		if(!msgList->empty() && (auxiliar->procesado == false) && !appShouldTerminate){
			clientMsj respuesta;
			strncpy(respuesta.id,auxiliar->mensaje.id, 20);
			strncpy(respuesta.type,auxiliar->mensaje.type,20);
			if(procesador.isMsgValid(auxiliar->mensaje.type, auxiliar->mensaje.value)){
				strncpy(respuesta.value,"Mensaje correcto",20);
			}else{
				strncpy(respuesta.value,"Mensaje incorrecto",20);
			}

			writingInLogFileMutex.lock();
			logWriter->writeMessageWasprossed(respuesta);
			writingInLogFileMutex.unlock();

			auxiliar->mensaje = respuesta;
		auxiliar->procesado = true;
		}
		mutexColaMensajes.unlock();
	}
	pthread_exit(NULL);
}

void *clientReader(int socketConnection, list<msjProcesado>* messagesList, int numberOfCurrentAcceptedClients){
	int res = 0;
	bool clientHasDisconnected = false;
	while(!appShouldTerminate && !clientHasDisconnected){
		clientMsj recibido;
		res = readMsj(socketConnection, sizeof(clientMsj), &recibido);
		if (res< 0){
			shutdown(socketConnection, SHUT_RDWR);
			clientHasDisconnected = true;
			numberOfCurrentAcceptedClients --;
		}else{
			writingInLogFileMutex.lock();
			logWriter->writeClientMessageReceviedFromSocketConnection(recibido, socketConnection);
			writingInLogFileMutex.unlock();
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
	bool clientHasDisconnected = false;

	while(!appShouldTerminate && !clientHasDisconnected){
		mutexColaMensajes.lock();
		auxiliar = msgList->front();
		if((auxiliar.socket == socket) && (auxiliar.procesado == true)){
			msgList->pop_front();
			respuesta = sendMsj(socket, sizeof(auxiliar.mensaje), &(auxiliar.mensaje));
			if(respuesta<=0){
				clientHasDisconnected = true;
			}
		}
		mutexColaMensajes.unlock();
	}

	pthread_exit(NULL);
}

void* waitForClientConnection(int numberOfCurrentAcceptedClients, int maxNumberOfClients, int socketHandle) {
	while (!appShouldTerminate) {
		logWriter->writeWaitingForClientConnection();
		int socketConnection = accept(socketHandle, NULL, NULL);
		logWriter->writeClientConnectionReceived();
		clientEntrada[numberOfCurrentAcceptedClients] = std::thread(
				clientReader, socketConnection, &messagesList,
				numberOfCurrentAcceptedClients);
		clientSalida[numberOfCurrentAcceptedClients] = std::thread(
				responderCliente, socketConnection, &messagesList);
		clientMsj message;
		if (numberOfCurrentAcceptedClients >= maxNumberOfClients) {
			strncpy(message.id,"0", 20);
			strncpy(message.type,"server_full",20);
			strncpy(message.value,"Try again later",20);
		}else{
			strncpy(message.id,"0", 20);
			strncpy(message.type,"connection_successful",20);
			strncpy(message.value,"Client successfully connected",20);
		}
		int response = sendMsj(socketConnection, sizeof(message), &(message));
		numberOfCurrentAcceptedClients++;

	}
	pthread_exit(NULL);
}

void prepareForExit(XMLLoader *xmlLoader, XmlParser *xmlParser , LogWriter *logWriter){
	delete logWriter;
	delete xmlLoader;
	delete xmlParser;
}

int main(int argc, char* argv[]) {
	const char* fileName;
	logWriter = new LogWriter;
	XMLLoader *xmlLoader = new XMLLoader(logWriter);
	int numberOfCurrentAcceptedClients = 0;
	if(argc != 2){
		fileName = kServerTestFile;
		logWriter->writeUserDidnotEnterFileName();
	} else {
		fileName = argv[1];
		if (!xmlLoader->serverXMLIsValid(fileName)){
			fileName = kServerTestFile;
		}
	}

	XmlParser *parser = new XmlParser(fileName);
	logWriter->setLogLevel(parser->getLogLevel());
	struct sockaddr_in socketInfo;
	char sysHost[kMaxHostName + 1]; // Hostname of this computer we are running on
	struct hostent *hPtr;
	int socketHandle;
	int portNumber = parser->getServerPort();
	int maxNumberOfClients = parser->getMaxNumberOfClients();

	bzero(&socketInfo, sizeof(sockaddr_in));  // Clear structure memory

	// Get system information

	gethostname(sysHost, kMaxHostName); // Get the name of this computer we are running on
	if ((hPtr = gethostbyname(sysHost)) == NULL) {
		cerr << "System hostname misconfigured." << endl;
		logWriter->writeError("Error al obtener el hostname");
		prepareForExit(xmlLoader, parser, logWriter);
		exit(EXIT_FAILURE);
	}

	// create socket

	if ((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		close(socketHandle);
		logWriter->writeError("Error al crear el socket");
		prepareForExit(xmlLoader, parser, logWriter);
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
		logWriter->writeError("Error en el bind");
		prepareForExit(xmlLoader, parser, logWriter);
		exit(EXIT_FAILURE);
	}
	if (listen(socketHandle, 5) == -1) {
		prepareForExit(xmlLoader, parser, logWriter);
		logWriter->writeError("Error en el listen");
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
	logWriter->writeUserDidFinishTheApp();
	prepareForExit(xmlLoader, parser, logWriter);
	return EXIT_SUCCESS;
}
