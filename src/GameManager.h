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

class MenuPresenter;

class GameManager {
public:
	GameManager();
	virtual ~GameManager();

	int initGameWithArguments(int argc, char* argv[]);

	void userDidChooseExitoption();

private:
	bool appShouldTerminate;
	MenuPresenter *menuPresenter;
	XmlParser *parser;
	XMLLoader *xmlLoader;
	SocketManager *socketManager;
	ClientList *clientList;

	void detachClientMessagesThreads();
};

#endif /* GAMEMANAGER_H_ */
