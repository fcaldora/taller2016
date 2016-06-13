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
#include "Team.h"

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

	void addClientToTeamWithName(Client *client, string teamName);
	void createTeamWithNameForClient(string teamName, Client *client);

private:
	bool appShouldTerminate;
	MenuPresenter *menuPresenter;
	XmlParser *parser;
	XMLLoader *xmlLoader;
	SocketManager *socketManager;
	ClientList *clientList;
	Procesador *procesor;
	Escenario* escenario;
	vector<Team *> *teams;

	void detachClientMessagesThreads();
	void sendInitialGameInfo();
};

#endif /* GAMEMANAGER_H_ */
