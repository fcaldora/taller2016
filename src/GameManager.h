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
#include "Bullet.h"

class MenuPresenter;
class Procesador;

class GameManager {
public:
	GameManager();
	virtual ~GameManager();

	int initGameWithArguments(int argc, char* argv[]);

	void userDidChooseExitoption();
	void broadcastMessage(mensaje *message);

	Bullet* createBulletForClient(Client *client);
private:
	bool appShouldTerminate;
	MenuPresenter *menuPresenter;
	XmlParser *parser;
	XMLLoader *xmlLoader;
	SocketManager *socketManager;
	ClientList *clientList;
	Procesador *procesor;
	Escenario *scenery;

	void detachClientMessagesThreads();
};

#endif /* GAMEMANAGER_H_ */
