/*
 * Procesador.cpp
 *
 *  Created on: 7 de abr. de 2016
 *      Author: gusta
 */

#include "Procesador.h"

Procesador::Procesador() {
	this->clientList = NULL;
	this->screenWidth = 0;
	this->screenHeight = 0;
}

Procesador::Procesador(ClientList *clientList, int screenWidth, int screenHeight) {
	this->clientList = clientList;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}

int Procesador::processMessage(clientMsj message) {
	Client* client = this->clientList->getClientForName(message.id);
	int value;
	if(strcmp(message.value, "DER") == 0) {
		if((client->plane->getPosX() + client -> getPlane()->getWidth()) < this->screenWidth){
			client->plane->moveOneStepRight();
		}
		value = 1;
	}else if(strcmp(message.value, "IZQ") == 0) {
		if(client->plane->getPosX() > 0){
			client->plane->moveOneStepLeft();
		}
		value = 1;
	}else if(strcmp(message.value, "ABJ") == 0) {
		if((client->plane->getPosY() + client -> getPlane()->getHeigth()) < this->screenHeight){
			client->plane->moveOneStepDown();
		}
		value = 1;
	}else if(strcmp(message.value, "ARR") == 0) {
		if(client->plane->getPosY() > 0){
			client->plane->moveOneStepUp();
		}
		value = 1;
	}else if(strcmp(message.value, "DIS") == 0){
		//cout << "LLego dis" << endl;
		value = 2;
	}else if(strcmp(message.value, "alive") == 0){
		value = 3;
	}else if(strcmp(message.value, "RES") == 0){
		value = 4;
	}else if(strcmp(message.value, "ANIMATE") == 0){
		value = 5;
	}
	return value;
}

Procesador::~Procesador() {
	// TODO Auto-generated destructor stub
}

