/*
 * MenuPresenter.cpp
 *
 *  Created on: Apr 26, 2016
 *      Author: luciano
 */

#include "MenuPresenter.h"

MenuPresenter::MenuPresenter(bool appShouldTerminate, GameManager *gameManager) {
	this->appShouldTerminate = appShouldTerminate;
	this->gameManager = gameManager;
}

void MenuPresenter::presentMenu() {
	while (!appShouldTerminate) {
		cout << "Menu:" << endl;
		cout << "1. Salir" << endl;
		cout << "Ingresar opcion:" << endl;
		unsigned int userDidChooseOption;
		cin >> userDidChooseOption;
		if (userDidChooseOption == 1) {
			appShouldTerminate = true;
			this->gameManager->userDidChooseExitoption();
		} else {
			cout << "Opcion incorrecta" << endl;
		}
	}
}

MenuPresenter::~MenuPresenter() {
	// TODO Auto-generated destructor stub
}

