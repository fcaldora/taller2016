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

#define MAXHOSTNAME 256
using namespace std;

list<clientMsj> messagesList;
list<ClientMessage> clientMessageList;

int readBlock(int fd, void* buffer, int len) {
	int ret = 0;
	int count = 0;
	while (count < len) {
		ret = read(fd, buffer + count, 1);
		if (ret <= 0) {
			return (-1);
		}
		count += 1;
	}
	return count;
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
	bool clientConnected = true;
	while(clientConnected){
		clientMsj msj;
		readBlock(socketConnection, &msj, sizeof(msj));
		cout << "socket connection: " << socketConnection << ", msj:" << msj.value << endl;
		if(strcmp (msj.value,"Disconnection") == 0){
			write(socketConnection, &msj, sizeof(msj));
			close(socketConnection);
			clientConnected = false;
		}
		write(socketConnection,&msj,sizeof(msj));
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	string fileName;

	if(argc != 2){
		fileName = "serverTest.txt";
		cout<<"Falta escribir el nombre del archivo, se usara uno por defecto"<<endl;
	} else {
		fileName = argv[1];
	}
	
	std::thread clientThreads[5];

	CargadorXML cargador;

	cargador.cargarServidor(fileName);
	//cargador.cargarServidor("serverTest.txt");

	XmlParser parser(cargador.getDocumento());
	struct sockaddr_in socketInfo;
	char sysHost[MAXHOSTNAME + 1]; // Hostname of this computer we are running on
	struct hostent *hPtr;
	int socketHandle;
	int portNumber = 8080;
	parser.obtenerPuertoSv(portNumber);
	int maxNumberOfClients;
	int numberOfCurrentAcceptedClients = 1;
	parser.obtenerMaxClientes(maxNumberOfClients);

	bzero(&socketInfo, sizeof(sockaddr_in));  // Clear structure memory

	// Get system information

	gethostname(sysHost, MAXHOSTNAME); // Get the name of this computer we are running on
	if ((hPtr = gethostbyname(sysHost)) == NULL) {
		cerr << "System hostname misconfigured." << endl;
		exit(EXIT_FAILURE);
	}

	// create socket

	if ((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		close(socketHandle);
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
		cout << "exit ";
		exit(EXIT_FAILURE);
	}
	if (listen(socketHandle, 5) == -1) {
		return -1;
	}
	//Creo una lista donde voy a guardar los threads

	//arranco el thread que va a ir desencolando los mensajes que recibe el server

	//std::thread messageReaderThread(readMsjs);

	while (1) {
		if (numberOfCurrentAcceptedClients < maxNumberOfClients) {
			cout << "Waiting for a client connection" << endl;
			int socketConnection = accept(socketHandle, NULL, NULL);
			cout << "connection received" << endl;
			//int rc = readBlock(socketConnection, &msj, 60);
			clientThreads[numberOfCurrentAcceptedClients] = std::thread(clientReader, socketConnection);
			numberOfCurrentAcceptedClients++;
		}
	}

	//messageReaderThread.join();
	for(int i=1; i< maxNumberOfClients; i++){
		clientThreads[i].join();
	}
	close(socketHandle);
	cout << "Close ";

	return EXIT_SUCCESS;
}
