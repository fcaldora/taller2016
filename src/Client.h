/*
 * Client.h
 *
 *  Created on: Apr 24, 2016
 *      Author: facundo
 */

#ifndef CLIENT_H_
#define CLIENT_H_
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include "Avion.h"

using namespace std;

class Client {
public:
	Client(string name, int socketM, int socketKA, Avion* plane);
	virtual ~Client();

	string getName(){
		return name;
	}

	bool getConnnectionState(){
		return connected;
	}

	void setConnected(bool connected) {
		this->connected = connected;
	}

	void setName(const string& name) {
		this->name = name;
	}

	int getSocketKeepAlive() const {
		return socketKeepAlive;
	}

	void setSocketKeepAlive(int socketKeepAlive) {
		this->socketKeepAlive = socketKeepAlive;
	}

	int getSocketMessages() const {
		return socketMessages;
	}

	void setSocketMessages(int socketMessages) {
		this->socketMessages = socketMessages;
	}

	 Avion* getPlane()  {
		return plane;
	}

	Avion* plane;
	int clientID;
	int earnedPoints;
	bool isFirstTimeLogin;
	int currentNumberOfLifes;

private:
	string name;
	bool connected;
	int socketMessages;
	int socketKeepAlive;
};

#endif /* CLIENT_H_ */
