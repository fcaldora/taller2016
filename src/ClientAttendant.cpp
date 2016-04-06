/*
 * ClientListener.cpp
 *
 *  Created on: Mar 27, 2016
 *      Author: luciano
 */
using namespace std;
#include <iostream>
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/socket.h>
#include <list>

#include "ClientAttendant.h"

pthread_t tid[2];
pthread_mutex_t lock;

struct arg_struct {
    int clientSocket;
    std::list<ClientMessage>* messages;
}args;

void* receiveClientMessages(void *arguments)
{
	//struct arg_struct *args = (struct arg_struct *)arguments;
	char buffer[1024];
	int clientSocket = args.clientSocket;

	int sizeOfMessageReceived = recv(clientSocket, buffer, 1024, 0);

	if (sizeOfMessageReceived > 0) {

	}

    pthread_mutex_lock(&lock);

    printf("\n Job %d started\n", clientSocket);

    for(unsigned long i=0; i<(0xFFFFFFFF);i++);

    printf("\n Job %d finished\n", clientSocket);

    pthread_mutex_unlock(&lock);

    return NULL;
}

ClientAttendant::ClientAttendant(int clientID, int clientSocket, std::list<ClientMessage>* m){//, list<clientMsj> messages) {
	cout<< endl <<"Init" << endl;
	this->clientID = clientID;
	this->clientSocket = clientSocket;
	this->messagesList = messagesList;
	this->createThreadForClient();
}

ClientAttendant::~ClientAttendant() {
	pthread_join(tid[this->clientID], NULL);
	pthread_mutex_destroy(&lock);
}

void ClientAttendant::createThreadForClient () {
	int err;

	if (pthread_mutex_init(&lock, NULL) != 0)
	{
		printf("\n mutex init failed\n");
	}

	args.clientSocket = this->clientSocket;
	args.messages = this->messagesList;

	err = pthread_create(&(tid[clientID]), NULL, &receiveClientMessages, NULL);
	if (err != 0)
		printf("\ncan't create thread :[%s]", strerror(err));
}

