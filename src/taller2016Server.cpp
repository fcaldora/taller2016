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
#include <pthread.h>
#define MAXHOSTNAME 256
using namespace std;

struct clientMsj {
	int id;
	char type[20];
	char value[20];
};

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

void* readMsjs(void *param) {
	clientMsj msj = { };
	list<clientMsj> msjs = *((list<clientMsj> *) param);
	while (1) {
		if (!msjs.empty()) {
			msj = msjs.front();
			cout << msj.id << endl;
			cout << msj.value << endl;
			cout << msj.type << endl;
			msjs.pop_front();
		}
	}
	return 0;
}

int main() {

	list<clientMsj> msjs;
	struct sockaddr_in socketInfo;
	char sysHost[MAXHOSTNAME + 1]; // Hostname of this computer we are running on
	struct hostent *hPtr;
	int socketHandle;
	int portNumber = 8080;

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
		exit(EXIT_FAILURE);
	}

	if (listen(socketHandle, 5) == -1) {
		return -1;
	}

	//arranco el thread que va a ir desencolando los mensajes que recibe el server
	pthread_t messageReaderThread;
	pthread_create(&messageReaderThread, NULL, readMsjs, &msjs);

	while (1) {
		clientMsj msj = { };
		int socketConnection = accept(socketHandle, NULL, NULL);
		int rc = readBlock(socketConnection, &msj, 44);
		cout << msj.id << endl;
		msjs.push_back(msj);
		cout << msjs.size() << endl;

	}
	pthread_join(messageReaderThread, NULL);
	close(socketHandle);
}
