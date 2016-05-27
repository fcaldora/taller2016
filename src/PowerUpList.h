/*
 * PowerUpList.h
 *
 *  Created on: 26 de may. de 2016
 *      Author: gusta
 */

#ifndef POWERUPLIST_H_
#define POWERUPLIST_H_
#include "PowerUp.h"
#include <list>

using namespace std;

class PowerUpList {
public:
	PowerUpList();
	void deletePowerUp(int powerUpId);
	void addPowerUp(PowerUp* powerUp);
	PowerUp* getPowerUp(int numberOfPowerUp);
	void movePowerUps();
	void setScrollingStep(int step);
	void setScenaryHeight(int height);
	int numberOfPowerUps();
	virtual ~PowerUpList();

private:
	list<PowerUp*> powerUpsList;
	int scrollingStep;
	int scenaryHeight;
	int scrollingOffset;
};


#endif /* POWERUPLIST_H_ */
