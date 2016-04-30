/*
 * Escenario.cpp
 *
 *  Created on: 24 de abr. de 2016
 *      Author: gusta
 */

#include "Escenario.h"

Escenario::Escenario() {
	this->id  = 0;
	this->width = 0;
	this->heigth = 0;
	this->actualPhotogram = 1;
	this->numberOfPhotograms = 1;
	this->posX = 0;
	this->posY = 0;
}

void Escenario::addElement(DrawableObject* element){
	elements.push_front(element);
}

Escenario::~Escenario() {
	// TODO Auto-generated destructor stub
}

