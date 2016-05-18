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
	DrawableObject* getElement(int numElement);
	int getNumberElements();
	virtual ~Escenario();
	void restart();
	void deleteElements();
private:
	list<DrawableObject*> elements;
	int posYActual;
	int scrollingStep;
	int scrollingOffset;
	int windowHeight;
};

#endif /* ESCENARIO_H_ */
