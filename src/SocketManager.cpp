/*
 * SocketManager.cpp
 *
 *  Created on: Apr 26, 2016
 *      Author: luciano
 */

#include "SocketManager.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <netdb.h>

#define kMaxHostName 256

SocketManager::SocketManager(LogWriter *logWriter, XmlParser *parser) {
	this->logWriter = logWriter;
	this->parser = parser;
	this->socketHandle = 0;
}

int SocketManager::createSocketConnection() {
	struct sockaddr_in socketInfo;
	char sysHost[kMaxHostName + 1]; // Hostname of this computer we are running on
	struct hostent *hPtr;
	int socketHandle;
	int portNumber = this->parser->getServerPort();
	bzero(&socketInfo, sizeof(sockaddr_in));  // Clear structure memory

	// Get system information

	gethostname(sysHost, kMaxHostName); // Get the name of this computer we are running on
	if ((hPtr = gethostbyname(sysHost)) == NULL) {
		cerr << "System hostname misconfigured." << endl;
		this->logWriter->writeError("Error al obtener el hostname");
		exit(EXIT_FAILURE);
	}

	// create socket

	if ((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		close(socketHandle);
		this->logWriter->writeError("Error al crear el socket");
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
		this->logWriter->writeError("Error en el bind");
		exit(EXIT_FAILURE);
	}
	if (listen(socketHandle, 5) == -1) {
		this->logWriter->writeError("Error en el listen");
		return EXIT_FAILURE;
	}

	this->socketHandle = socketHandle;

	return EXIT_SUCCESS;
}

SocketManager::~SocketManager() {
	// TODO Auto-generated destructor stub
}

