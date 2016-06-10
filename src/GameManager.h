/*
 * GameManager.h
 *
 *  Created on: Apr 26, 2016
 *      Author: luciano
 */

#ifndef GAMEMANAGER_H_
#define GAMEMANAGER_H_

#include "XmlParser.h"
#include "CargadorXML.h"
#include "SocketManager.h"
#include "ClientList.h"
#include "Procesador.h"
#include "BulletList.h"
#include "EnemyPlane.h"

class MenuPresenter;

class GameManager {
public:
	GameManager();
	virtual ~GameManager();

	int initGameWithArguments(int argc, char* argv[]);

	void userDidChooseExitoption();

	void broadcastMessage(mensaje message);
	void restartGame();
	Object createBulletForClient(Client* client, int posX);
	void reloadGameFromXml();
	void createEnemyBullet(EnemyPlane* enemyPlane);
	Escenario* getScenary();
private:
	bool appShouldTerminate;
	MenuPresenter *menuPresenter;
	XmlParser *parser;
	XMLLoader *xmlLoader;
	SocketManager *socketManager;
	ClientList *clientList;
	Procesador *procesor;
	Escenario* escenario;

	void detachClientMessagesThreads();
};

#endif /* GAMEMANAGER_H_ */
