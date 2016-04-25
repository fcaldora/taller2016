/*
 * Escenario.h
 *
 *  Created on: 24 de abr. de 2016
 *      Author: gusta
 */
#include <stdlib.h>
#include "Sprite.h"
#include "Constants.h"
#include <list>

#ifndef ESCENARIO_H_
#define ESCENARIO_H_

using namespace std;

class Escenario {
public:
	Escenario();
	void setWidth(int width);
	int getWidth();
	void setHeight(int height);
	int getHeight();
	void setFondo(Sprite* fondo);
	void addElement(Sprite* element);
	Sprite* getElement(struct elemento element,unsigned int numElement);
	virtual ~Escenario();

private:
	int width;
	int height;
	Sprite* fondo;
	list<Sprite*> elements;
};

#endif /* ESCENARIO_H_ */
