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

mensaje MessageBuilder::createBackgroundElementUpdateMessageForElement(DrawableObject *element){
	mensaje msg = this->createBackgroundElementBaseMessageForElement(element);
	strncpy(msg.action, "draw", 20);

	return msg;
}

mensaje MessageBuilder::createBackgroundElementCreationMessageForElement(DrawableObject *element){
	mensaje msg;
	memset(&msg, 0, sizeof(mensaje));

	msg = this->createBackgroundElementBaseMessageForElement(element);
	strncpy(msg.action, "create", 20);

	return msg;
}

mensaje MessageBuilder::createBackgroundElementBaseMessageForElement(DrawableObject *element){
	mensaje msg;
	msg.id = element->getId();
	msg.posX = element->getPosX();
	msg.posY = element->getPosY();
	strncpy(msg.imagePath, element->getPath().c_str(), 20);
	msg.height = element->getHeigth();
	msg.width = element->getWidth();
	msg.actualPhotogram = element->getActualPhotogram();
	msg.activeState = true;

	return msg;
}

mensaje MessageBuilder::createReconnectionMessageForClient(Client* client) {
	mensaje reconnection;
	strcpy(reconnection.action, "path");
	reconnection.id = client->plane->getId();
	strcpy(reconnection.imagePath, client->plane->getPath().c_str());
	reconnection.width = client->plane->getWidth();
	reconnection.height = client->plane->getHeigth();

	return reconnection;
}

mensaje MessageBuilder::createDisconnectionMessageForClient(Client* client) {
	mensaje disconnection;

	strcpy(disconnection.action, "path");
	disconnection.id = client->plane->getId();
	strcpy(disconnection.imagePath, "disconnected.png");
	disconnection.height = 68;
	disconnection.width = 68;

	return disconnection;
}

mensaje MessageBuilder::createUpdatePhotogramMessageForPlane(Avion* plane) {
	mensaje photogramMsg;

	strncpy(photogramMsg.action,"draw", kLongChar);
	photogramMsg.actualPhotogram = plane->getActualPhotogram();
	photogramMsg.id = plane->getId();
	photogramMsg.posX = plane->getPosX();
	photogramMsg.posY = plane->getPosY();

	return photogramMsg;
}

mensaje MessageBuilder::createExplosionMessage(Explosion* explosion){
	mensaje message;
	message.id = explosion->getId();
	strcpy(message.action,"create");
	strcpy(message.imagePath, explosion->getPath().c_str());
	message.height = explosion->getHeigth();
	message.width = explosion->getWidth();
	message.posX = explosion->getPosX();
	message.posY = explosion->getPosY();
	message.activeState = true;
	message.actualPhotogram = explosion->getActualPhotogram();

	return message;
}

MessageBuilder::~MessageBuilder() {
	// TODO Auto-generated destructor stub
}

