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

#define MAXHOSTNAME 256
#define kServerTestFile "serverTest.txt"

using namespace std;

list<clientMsj> messagesList;
list<ClientMessage> clientMessageList;

int sendMsj(int socket, string buffer){
	uint32_t enviados = 0;
	int res = 0;
	uint32_t bytesAEnviar = (buffer.size());
	res = send(socket, &bytesAEnviar, sizeof(uint32_t), MSG_WAITALL);//Envio la longitud del mensaje primero.
	while(enviados<bytesAEnviar){
		res = send(socket, &(buffer.c_str())[enviados], bytesAEnviar - enviados, MSG_WAITALL);
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

int readMsj(int socket, char* buffer){
	uint32_t totalBytesRecibidos = 0;
	int recibidos = 0;
	uint32_t numBytes;
	recv(socket, &numBytes, sizeof(uint32_t), MSG_WAITALL);
	int tamBuffer = numBytes;
	memset(buffer,0,20);
	while (totalBytesRecibidos < numBytes){
		recibidos = recv(socket, &buffer[totalBytesRecibidos], tamBuffer - totalBytesRecibidos, MSG_WAITALL);
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
	return 0;
}

void readMsjs() {
	clientMsj clientMessage = { };
	while (1) {
		if (!messagesList.empty()) {
			clientMessage = messagesList.front();
			cout << clientMessage.id << endl;
			cout << clientMessage.value << endl;
			cout << clientMessage.type << endl;
			messagesList.pop_front();
		}
	}
}

void* clientReader(int socketConnection){
	char buffer[20];
	int res = 0;
	bool fin = false;
	while(!fin){
		memset(buffer, 0, 20);
		res = readMsj(socketConnection, buffer);
		if (res< 0){
			shutdown(socketConnection, SHUT_RDWR);
			fin = true;
		}else{
			cout << "socket connection: " << socketConnection << ", msj:" << buffer << endl;
			string respuesta("MENSAJE RECIBIDO CORRECTAMENTE: " + string(buffer) + "\n.");
			sendMsj(socketConnection, respuesta);
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

	std::thread clientThreads[5];

	XmlParser *parser = new XmlParser(fileName);
	struct sockaddr_in socketInfo;
	char sysHost[MAXHOSTNAME + 1]; // Hostname of this computer we are running on
	struct hostent *hPtr;
	int socketHandle;
	int portNumber = parser->getServerPort();
	int maxNumberOfClients = parser->getMaxNumberOfClients();
	int numberOfCurrentAcceptedClients = 0;

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
	//Creo una lista donde voy a guardar los threads

	//arranco el thread que va a ir desencolando los mensajes que recibe el server

	//std::thread messageReaderThread(readMsjs);

	while (1) {
		if (numberOfCurrentAcceptedClients < maxNumberOfClients) {
			cout << "Waiting for a client connection" << endl;
			int socketConnection = accept(socketHandle, NULL, NULL);
			cout << "connection received" << endl;
			clientThreads[numberOfCurrentAcceptedClients] = std::thread(clientReader, socketConnection);
			numberOfCurrentAcceptedClients++;
		}
	}

	//messageReaderThread.join();
	for(int i=1; i< maxNumberOfClients; i++){
		clientThreads[i].join();
	}

	close(socketHandle);
	prepareForExit(xmlLoader, parser, errorLogWriter, "Close");
	return EXIT_SUCCESS;
}
