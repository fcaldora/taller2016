/*
 * MenuPresenter.h
 *
 *  Created on: Apr 26, 2016
 *      Author: luciano
 */

#ifndef MENUPRESENTER_H_
#define MENUPRESENTER_H_

#include "Constants.h"
#include "GameManager.h"

class MenuPresenter {
public:
	MenuPresenter(bool appShouldTerminate, GameManager *gameManager);
	virtual ~MenuPresenter();

	void presentMenu();

private:
	bool appShouldTerminate;
	GameManager *gameManager;
};

#endif /* MENUPRESENTER_H_ */
