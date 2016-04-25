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

using namespace std;

class Client {
public:
	Client(string name, int socketM, int socketKA);
	virtual ~Client();
	string getName();
private:
	string name;
	int posX;
	int posY;
	bool connected;
	int socketMessages;
	int socketKeepAlive;
};

#endif /* CLIENT_H_ */
