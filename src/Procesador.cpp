/*
 * Procesador.cpp
 *
 *  Created on: 7 de abr. de 2016
 *      Author: gusta
 */

#include "Procesador.h"

#include "MessageBuilder.h"

#define kOneMilisecond 0.001

class timer {
	private:
		unsigned long begTime;
	public:
		void start() {
			begTime = clock();
		}

		double elapsedTime() {
			return ((double) clock() - begTime) /(double) CLOCKS_PER_SEC;
		}

		bool isTimeout(unsigned long seconds) {
			return seconds >= elapsedTime();
		}
};

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
	if (strcmp(message.type, "movement") == 0) {
		processMovementMessage(message);
	} else if (strcmp(message.type, "shoot") == 0) {
		processShootingMessage(message);
	} else if (strcmp(message.type, "alive") == 0) {

	} else if (strcmp(message.type, "animation") == 0) {
		processAnimationMessage(message);
	}
}

void Procesador::processAnimationMessage (clientMsj message) {
	Client* client = this->clientList->getClientForName(message.id);

	sendAnimationMessage(client);

	while (client->plane->getActualPhotogramNumber() != 1) {
		double seconds = kOneMilisecond;
		timer t;
		t.start();
		while(true) {
			if(t.elapsedTime() >= seconds) {
				break;
			}
		}
		sendAnimationMessage(client);
	}
}

void Procesador::sendAnimationMessage(Client *client) {
	client->plane->incrementPhotogramNumber();
	mensaje *response = MessageBuilder().createClientPlaneLoopMessage(client);
	this->gameManager->broadcastMessage(response);
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

