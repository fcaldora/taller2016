/*
 * Messenger.h
 *
 *  Created on: 21 de may. de 2016
 *      Author: gusta
 */

#ifndef MESSENGER_H_
#define MESSENGER_H_
#include "Constants.h"
#include <sys/socket.h>
#include "ClientList.h"

class Messenger {
public:
	Messenger();
	virtual ~Messenger();
	int sendClientMsj(int socket, int bytesARecibir, clientMsj* mensaje);
	int sendMensaje(int socket, int bytesARecibir, mensaje* mensaje);
	int sendActionMsj(int socket, int bytesARecibir, actionMsj* mensaje);
	int sendUpdateMsj(int socket, int bytesARecibir, updateMsj* mensaje);
	int readClientMsj(int socket, int bytesAEnviar, clientMsj* mensaje);
	int sendDeleteMsj(int socket, int bytesAEnviar, deleteMsj* mensaje);
	void broadcastClientMsj(clientMsj msg, ClientList* clientList);
	void broadcastMensaje(mensaje msg, ClientList* clientList);
	void broadcastUpdateMsj(updateMsj msg, ClientList* clientList);
	void broadcastActionMsj(actionMsj msg, ClientList* clientList);
	void broadcastDeleteMsj(deleteMsj msg, ClientList* clientList);
};

#endif /* MESSENGER_H_ */
