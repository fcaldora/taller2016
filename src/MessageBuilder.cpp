/*
 * MessageBuilder.cpp
 *
 *  Created on: Apr 26, 2016
 *      Author: luciano
 */

#include "MessageBuilder.h"

MessageBuilder::MessageBuilder() {
	// TODO Auto-generated constructor stub
}

clientMsj MessageBuilder::createSuccessfullyConnectedMessage() {
	clientMsj message;
	strncpy(message.id, "0", kLongChar);
	strncpy(message.type, "connection_ok", kLongChar);
	strncpy(message.value, "Client connected", kLongChar);
	return message;
}

clientMsj MessageBuilder::createServerFullMessage() {
	clientMsj message;
	strncpy(message.id, "0", 20);
	strncpy(message.type, "server_full", 20);
	strncpy(message.value, "Try again later", 20);
	return message;
}

clientMsj MessageBuilder::createUserNameAlreadyInUseMessage() {
	clientMsj message;
	strncpy(message.id, "0", 20);
	strncpy(message.type, "error", 20);
	strncpy(message.value, "Name already used", 20);
	return message;
}


mensaje* MessageBuilder::createInitialMessageForClient(Client *client) {
	mensaje *message = new mensaje;

	strcpy(message->action, "create");
	strcpy(message->imagePath, client->getPlane()->getPath().c_str());
	message->id = client->getPlane()->getId();
	message->photograms = client->getPlane()->getPhotograms();
	message->actualPhotogram = client->getPlane()->getActualPhotogram();
	message->height = client->getPlane()->getHeigth();
	message->width = client->getPlane()->getWidth();
	message->posX = client->getPlane()->getPosX();
	message->posY = client->getPlane()->getPosY();
	message->activeState = true;

	return message;
}

mensaje MessageBuilder::createPlaneMovementMessageForClient(Client *client) {
	mensaje message;
	message.id = client->plane->getId();
	strncpy(message.action, "draw", kLongChar);
	message.posX = client->plane->getPosX();
	message.posY = client->plane->getPosY();
	return message;
}

mensaje* MessageBuilder::createInitBackgroundMessageForScenery(Escenario *escenario){
	mensaje *message = new mensaje;

	message->id = escenario->getId();
	strncpy(message->action,"create", kLongChar);
	strncpy(message->imagePath, escenario->getPath().c_str(), kLongChar);
	message->photograms = escenario->getPhotograms();
	message->actualPhotogram = escenario->getActualPhotogram();
	message->height = escenario->getHeigth();
	message->width = escenario->getWidth();
	message->posX = escenario->getPosX();
	message->posY = escenario->getPosY();
	message->activeState = true;

	return message;
}



MessageBuilder::~MessageBuilder() {
	// TODO Auto-generated destructor stub
}

