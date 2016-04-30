/*
 * MessageBuilder.h
 *
 *  Created on: Apr 26, 2016
 *      Author: luciano
 */

#ifndef MESSAGEBUILDER_H_
#define MESSAGEBUILDER_H_

#include "Constants.h"
#include "Client.h"
#include "Escenario.h"

class MessageBuilder {
public:
	MessageBuilder();
	virtual ~MessageBuilder();

	clientMsj createSuccessfullyConnectedMessage();
	clientMsj createServerFullMessage();
	clientMsj createUserNameAlreadyInUseMessage();
	void createInitialMessageForClient(Client *client, mensaje* message);
	mensaje createPlaneMovementMessageForClient(Client *client);
	mensaje createInitBackgroundMessage(Escenario *escenario);
};

#endif /* MESSAGEBUILDER_H_ */
