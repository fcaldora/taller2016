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

mensaje MessageBuilder::createInitialMessageForClient(Client *client) {
	mensaje message;
	strcpy(message.action, "create");
	strcpy(message.imagePath, client->getPlane()->getPath().c_str());
	message.id = client->getPlane()->getId();
	message.photograms = client->getPlane()->getPhotograms();
	message.actualPhotogram = client->getPlane()->getActualPhotogram();
	message.height = client->getPlane()->getHeigth();
	message.width = client->getPlane()->getWidth();
	message.posX = client->getPlane()->getPosX();
	message.posY = client->getPlane()->getPosY();
	message.activeState = true;
	return message;
}

mensaje MessageBuilder::createPlaneMovementMessageForClient(Client *client) {
	mensaje message;
	message.id = client->getPlane()->getId();
	strcpy(message.action, "draw");
	message.posX = client->plane->getPosX();
	message.posY = client->plane->getPosY();
	return message;
}

MessageBuilder::~MessageBuilder() {
	// TODO Auto-generated destructor stub
}

