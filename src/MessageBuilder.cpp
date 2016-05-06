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


void MessageBuilder::createInitialMessageForClient(Client *client, mensaje* message) {
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
}

mensaje MessageBuilder::createPlaneMovementMessageForClient(Client *client) {
	mensaje message;
	message.id = client->plane->getId();
	strncpy(message.action, "draw", kLongChar);
	message.posX = client->plane->getPosX();
	message.posY = client->plane->getPosY();
	message.actualPhotogram = client->plane->getActualPhotogram();
	return message;
}

mensaje MessageBuilder::createInitBackgroundMessage(Escenario *escenario){
	mensaje message;
	message.id = escenario->getId();
	strncpy(message.action,"create", kLongChar);
	strncpy(message.imagePath, escenario->getPath().c_str(), kLongChar);
	message.photograms = escenario->getPhotograms();
	message.actualPhotogram = escenario->getActualPhotogram();
	message.height = escenario->getHeigth();
	message.width = escenario->getWidth();
	message.posX = escenario->getPosX();
	message.posY = escenario->getPosY();
	message.activeState = true;
	return message;
}

mensaje MessageBuilder::createBulletMessage(Object* bullet){
	mensaje message;
	message.id = bullet->getId();
	strcpy(message.action,"create");
	strcpy(message.imagePath, bullet->getPath().c_str());
	message.height = bullet->getHeigth();
	message.width = bullet->getWidth();
	message.posX = bullet->getPosX();
	message.posY = bullet->getPosY();
	message.activeState = true;
	message.actualPhotogram = bullet->getActualPhotogram();
	return message;
}

mensaje MessageBuilder::createBackgroundUpdateMessage(Escenario* escenario){
	mensaje msg;
	strncpy(msg.action, "draw", 20);
	msg.id = escenario->getId();
	msg.posY = escenario->getPosY();
	msg.posX = escenario->getPosX();
	msg.actualPhotogram = escenario->getActualPhotogram();
	return msg;
}

mensaje MessageBuilder::createBackgroundElementUpdateMessage(Escenario* escenario, int numElement){
	mensaje msg;
	DrawableObject* auxObject;
	strncpy(msg.action, "draw", 20);
	auxObject = escenario->getElement(numElement);
	msg.id = auxObject->getId();
	msg.posX = auxObject->getPosX();
	msg.posY = auxObject->getPosY();
	strncpy(msg.imagePath, auxObject->getPath().c_str(), 20);
	msg.height = auxObject->getHeigth();
	msg.width = auxObject->getWidth();
	msg.actualPhotogram = auxObject->getActualPhotogram();
	msg.activeState = true;
	return msg;
}


MessageBuilder::~MessageBuilder() {
	// TODO Auto-generated destructor stub
}

