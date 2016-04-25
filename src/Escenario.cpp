/*
 * Escenario.cpp
 *
 *  Created on: 24 de abr. de 2016
 *      Author: gusta
 */

#include "Escenario.h"

Escenario::Escenario() {
	width = 0;
	height = 0;
	fondo = NULL;
}

void Escenario::addElement(Sprite* element){
	elements.push_front(element);
}

void Escenario::setFondo(Sprite* fondo){
	this->fondo = fondo;
}

Sprite* Escenario::getElement(struct elemento element, unsigned int numElement){
	if(numElement > elements.size())
		return NULL;
	list<Sprite*>::iterator it = elements.begin();
	for(unsigned int contador = 0; contador<numElement; contador++){
		it++;
	}
	strncpy(element.id,(*it)->getId().c_str(), 20);
	element.x = (*it)->getWidth();
	element.y = (*it)->getHeight();

}

void Escenario::setHeight(int height){
	this->height = height;
}

int Escenario::getHeight(){
	return height;
}

void Escenario::setWidth(int width){
	this->width = width;
}

int Escenario::getWidth(){
	return width;
}

Escenario::~Escenario() {
	// TODO Auto-generated destructor stub
}

