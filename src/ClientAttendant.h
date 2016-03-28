/*
 * ClientAttendant.h
 *
 *  Created on: Mar 27, 2016
 *      Author: luciano
 */

#include <list>
#include "ClientMessage.h"

#ifndef CLIENTATTENDANT_H_
#define CLIENTATTENDANT_H_

struct msj {
	int id;
	char type[20];
	char value[20];
};

class ClientAttendant {
private:
	int clientID;
	int clientSocket;
	std::list<ClientMessage>* messagesList;
	void createThreadForClient ();

public:
	ClientAttendant(int clientID, int clientSocket, std::list<ClientMessage>* messages); // clientSocket after accepting it.
	virtual ~ClientAttendant();
	void startListeningToClient ();
};

#endif /* CLIENTLISTENER_H_ */
