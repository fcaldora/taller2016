/*
 * Escenario.h
 *
 *  Created on: 24 de abr. de 2016
 *      Author: gusta
 */
#include <stdlib.h>
#include "Constants.h"
#include <list>
#include "DrawableObject.h"
#include "PowerUpList.h"
#include "PowerUp.h"
#ifndef ESCENARIO_H_
#define ESCENARIO_H_

using namespace std;

class Escenario: public DrawableObject {
public:
	Escenario();
	void addElement(DrawableObject* element);
	void update();
	void setScrollingStep(double step);
	void transformPositions();
	void setWindowHeight(int windowHeight);
	DrawableObject* getElement(unsigned int numElement);
	int getNumberElements();
	void deleteElement(int elementId);

	void addPowerUp(PowerUp* powerUp);
	void deletePowerUp(int powerUpId);
	void setPosPortaAviones(int posX, int posY);
	int getPortaAvionesX();
	int getPortaAvionesY();
	PowerUp* getPowerUp(int numPowerUp);
	int getNumberOfPowerUps();
	void deletePowerUps();
	virtual ~Escenario();
	bool hayQueAterrizar();
	void restart();
	void deleteElements();
private:
	list<DrawableObject*> elements;
	PowerUpList powerUps;
	int posYActual;
	int scrollingStep;
	int scrollingOffset;
	int windowHeight;
	int portaAvionesY;
	int portaAvionesX;
};

#endif /* ESCENARIO_H_ */
