/*
 * Procesador.cpp
 *
 *  Created on: 7 de abr. de 2016
 *      Author: gusta
 */

#include "Procesador.h"

#include "MessageBuilder.h"

Procesador::Procesador() {
	this->clientList = NULL;
	this->gameManager = NULL;
	this->screenWidth = 0;
	this->screenHeight = 0;
}

Procesador::Procesador(ClientList *clientList, int screenWidth, int screenHeight, GameManager *gameManager) {
	this->clientList = clientList;
	this->gameManager = gameManager;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}

void Procesador::processMessage(clientMsj message) {
	if (strcmp(message.value, "movement") == 0) {
		processMovementMessage(message);
	} else if (strcmp(message.value, "shoot") == 0) {
		processShootingMessage(message);
	}
}

void Procesador::processShootingMessage (clientMsj message) {
	Client* client = this->clientList->getClientForName(message.id);
	Bullet *bullet = this->gameManager->createBulletForClient(client);
	mensaje *response = MessageBuilder().createBulletMessage(bullet);
	this->gameManager->broadcastMessage(response);
}

void Procesador::processMovementMessage(clientMsj message) {
	Client* client = this->clientList->getClientForName(message.id);
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

	mensaje *response = MessageBuilder().createPlaneMovementMessageForClient(client);

	this->gameManager->broadcastMessage(response);
}

Procesador::~Procesador() {
	// TODO Auto-generated destructor stub
}

