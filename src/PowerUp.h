/*
 * PowerUp.h
 *
 *  Created on: 25 de may. de 2016
 *      Author: gusta
 */

#ifndef POWERUP_H_
#define POWERUP_H_
#include <iostream>
#include "Avion.h"

using namespace std;
class PowerUp: public DrawableObject {
public:
	PowerUp();
	virtual ~PowerUp();
	bool haveCollision(Avion* plane);
	void setType(int type);
	void setPointsToAdd(int points);
	int getType();
	int getPointsToAdd();
	void applyPowerUp(Avion* avion);

private:
	int type;
	int pointsToAdd; //Solo se usa para el tipo 3 de sumar puntos.

};

#endif /* POWERUP_H_ */
