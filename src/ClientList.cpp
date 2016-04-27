/*
 * ClientList.cpp
 *
 *  Created on: Apr 26, 2016
 *      Author: luciano
 */

#include "ClientList.h"

ClientList::ClientList() {
	// TODO Auto-generated constructor stub
}

void ClientList::addClient(Client *client) {
	this->clients.push_back(client);
}

bool ClientList::checkIfUserNameIsAlreadyInUse(char value[]) {
	std::list<Client*>::iterator it;
	for (it = this->clients.begin(); it != this->clients.end(); ++it) {
		if (strcmp((*it)->getName().c_str(), value) == 0) {
			return false;
		}
	}
	return true;
}

Client* ClientList::getClientForName(char name[]) {
	std::list<Client*>::iterator it;
	Client* response;
	for (it = this->clients.begin(); it != this->clients.end(); ++it) {
		if (strcmp((*it)->getName().c_str(), name) == 0) {
			response = (*it);
		}
	}
	return response;
}

Client* ClientList::getClientForSocket(int socket) {
	std::list<Client*>::iterator it;
	Client* response;
	for (it = this->clients.begin(); it != this->clients.end(); ++it) {
		if (socket == (*it)->getSocketMessages()) {
			response = (*it);
		}
	}
	return response;
}

ClientList::~ClientList() {
	for (Client *client : this->clients) {
		delete client;
	}
	// TODO Auto-generated destructor stub
}

