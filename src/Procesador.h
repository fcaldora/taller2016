/*
 * Procesador.h
 *
 *  Created on: 7 de abr. de 2016
 *      Author: gusta
 */
#include <string>
#include "Constants.h"
#ifndef PROCESADOR_H_
#define PROCESADOR_H_

using namespace std;

#include "ClientList.h"
#include "GameManager.h"

class Procesador {
public:
	Procesador();
	Procesador(ClientList *clientList, int screenWidth, int screenHeight, GameManager *gameManager);
	virtual ~Procesador();

	void processMessage(clientMsj message);

	int getScreenHeight() const {
		return screenHeight;
	}

	void setScreenHeight(int screenHeight) {
		this->screenHeight = screenHeight;
	}

	int getScreenWidth() const {
		return screenWidth;
	}

	void setScreenWidth(int screenWidth) {
		this->screenWidth = screenWidth;
	}

private:
	ClientList *clientList;
	GameManager *gameManager;
	int screenWidth;
	int screenHeight;

	void processMovementMessage(clientMsj message);
	void processShootingMessage (clientMsj message);
};

#endif /* PROCESADOR_H_ */
