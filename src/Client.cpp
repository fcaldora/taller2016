/*
 * Client.cpp
 *
 *  Created on: Apr 24, 2016
 *      Author: facundo
 */

#include "Client.h"

Client::Client(string name, int socketM, int socketKA) {
	this->name = name;
	this->posX = 0;
	this->posY = 0;
	this->connected = true;
	this->socketKeepAlive = socketKA;
	this->socketMessages = socketM;
	// TODO Auto-generated constructor stub

}

Client::~Client() {
	// TODO Auto-generated destructor stub
}


