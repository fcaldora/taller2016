/*
 * Client.cpp
 *
 *  Created on: Apr 24, 2016
 *      Author: facundo
 */

#include "Client.h"

Client::Client(string name, int socketM, int socketKA, Avion* plane) {
	this->name = name;
	this->connected = true;
	this->socketKeepAlive = socketKA;
	this->socketMessages = socketM;
	this->plane = plane;
}

Client::~Client() {
	// TODO Auto-generated destructor stub
}


