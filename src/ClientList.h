/*
 * ClientList.h
 *
 *  Created on: Apr 26, 2016
 *      Author: luciano
 */

#ifndef CLIENTLIST_H_
#define CLIENTLIST_H_

#include "Constants.h"
#include "Client.h"

class ClientList {
public:
	ClientList();
	virtual ~ClientList();

	list<Client*> clients;

	void addClient(Client *client);
	bool checkIfUserNameIsAlreadyInUse(char value[]);
	Client* getClientForName(char name[]);
	Client* getClientForSocket(int socket);
	Client* getClientForPlaneId(int id);
};

#endif /* CLIENTLIST_H_ */
