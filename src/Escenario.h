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
	virtual ~Escenario();

private:
	list<DrawableObject*> elements;
};

#endif /* ESCENARIO_H_ */
