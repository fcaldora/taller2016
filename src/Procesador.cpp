/*
 * Procesador.cpp
 *
 *  Created on: 7 de abr. de 2016
 *      Author: gusta
 */

#include "Procesador.h"

#include "MessageBuilder.h"
#include "GameManager.h"

Procesador::Procesador() {
	this->clientList = NULL;
	this->screenWidth = 0;
	this->screenHeight = 0;
	this->gameManager = NULL;
}

Procesador::Procesador(ClientList *clientList, int screenWidth, int screenHeight, GameManager *gameManager) {
	this->clientList = clientList;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->gameManager = gameManager;
}

void Procesador::processMessage(clientMsj message) {
	if(strcmp(message.type, "movement") == 0) {
		this->processMovementMessage(message);
		return;
	}

	if(strcmp(message.type, "shoot") == 0){
		this->processShootMessage(message);
		return;
	}

	if(strcmp(message.type, "alive") == 0){
		this->processKeepAliveMessage(message);
		return;
	}

	if(strcmp(message.type, "reset") == 0){
		this->processResetMessage(message);
		return;
	}

	if(strcmp(message.type, "animation") == 0){
		this->processAnimationMessage(message);
		return;
	}

	if(strcmp(message.type, "close") == 0){
		this->processExitMessage(message);
		return;
	}
}

void Procesador::processMovementMessage(clientMsj message) {
	Client* client = this->clientList->getClientForName(message.id);
	if (client->plane->isLooping)
		return;

	if(strcmp(message.value, "DER") == 0) {
		if((client->plane->getPosX() + client -> getPlane()->getWidth()) < this->screenWidth){
			client->plane->moveOneStepRight();
		}
	}else if(strcmp(message.value, "IZQ") == 0) {
		if(client->plane->getPosX() > 0){
			client->plane->moveOneStepLeft();
		}
	}else if(strcmp(message.value, "ABJ") == 0) {
		if((client->plane->getPosY() + client -> getPlane()->getHeigth()) < this->screenHeight){
			client->plane->moveOneStepDown();
		}
	}else if(strcmp(message.value, "ARR") == 0) {
		if(client->plane->getPosY() > 0){
			client->plane->moveOneStepUp();
		}
	}
	mensaje response = MessageBuilder().createPlaneMovementMessageForClient(client);
	this->gameManager->broadcastMessage(response);
}

void Procesador::processShootMessage(clientMsj message) {
	Client* client = this->clientList->getClientForName(message.id);
	if (client->plane->isLooping)
		return;
	if(client->plane->doubleShooting()){
		int leftPlane = client->plane->getPosX();
		int rightPlane = client->plane->getPosX() + client->plane->getWidth()/2;
		Object bulletLeft = this->gameManager->createBulletForClient(client, leftPlane);
		Object bulletRight = this->gameManager->createBulletForClient(client, rightPlane);
		mensaje responseLeft = MessageBuilder().createBulletMessage(&bulletLeft);
		mensaje responseRight = MessageBuilder().createBulletMessage(&bulletRight);
		this->gameManager->broadcastMessage(responseLeft);
		this->gameManager->broadcastMessage(responseRight);
	}else{
		int middlePlane = client->plane->getPosX() + client->plane->getWidth()/2 - 15;
		Object bullet = this->gameManager->createBulletForClient(client, middlePlane);
		mensaje response = MessageBuilder().createBulletMessage(&bullet);
		this->gameManager->broadcastMessage(response);
	}
	mensaje soundMessage = MessageBuilder().createBulletSoundMessage(message);
	this->gameManager->broadcastMessage(soundMessage);
}

void Procesador::processKeepAliveMessage(clientMsj message) {

}

void Procesador::processResetMessage(clientMsj message) {
	this->gameManager->restartGame();
	mensaje msjReset;
	memset(&msjReset, 0, sizeof(mensaje));
	strcpy(msjReset.action, "reset");
	this->gameManager->broadcastMessage(msjReset);
	this->gameManager->reloadGameFromXml();
}

void Procesador::processAnimationMessage(clientMsj message) {
	Client* client = this->clientList->getClientForName(message.id);
	if (client->plane->isLooping)
		return;

	client->plane->setPhotogram();
	mensaje response = MessageBuilder().createPlaneMovementMessageForClient(client);
	this->gameManager->broadcastMessage(response);
}

void Procesador::processExitMessage(clientMsj message){
	mensaje closeMsj;
	memset(&closeMsj, 0, sizeof(mensaje));
	strcpy(closeMsj.action, "close");
	this->gameManager->broadcastMessage(closeMsj);
}

Procesador::~Procesador() {
	// TODO Auto-generated destructor stub
}

