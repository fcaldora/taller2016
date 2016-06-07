/*
 * Procesador.h
 *
 *  Created on: 7 de abr. de 2016
 *      Author: gusta
 */
#include <string>
#include "Constants.h"
#include <string.h>
#ifndef PROCESADOR_H_
#define PROCESADOR_H_

using namespace std;

#include "EnemyPlane.h"
#include "ClientList.h"

class GameManager;

class Procesador {
public:
	Procesador();
	Procesador(ClientList *clientList, int screenWidth, int screenHeight, GameManager *gameManager);
	virtual ~Procesador();

	void processMessage(clientMsj message);
	void processMenuMessageForClient(menuRequestMessage message, Client *client);

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
	void processEnemyBullet(EnemyPlane* enemyPlane);
	GameManager *gameManager;

private:
	ClientList *clientList;
	int screenWidth;
	int screenHeight;
	void processMovementMessage(clientMsj message);
	void processShootMessage(clientMsj message);
	void processKeepAliveMessage(clientMsj message);
	void processResetMessage(clientMsj message);
	void processAnimationMessage(clientMsj message);
	void processExitMessage(clientMsj message);
};

#endif /* PROCESADOR_H_ */
