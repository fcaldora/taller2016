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
	this->posYActual = 0;
	this->scrollingStep = 0.1;
	this->scrollingOffset = 0;
	this->windowHeight = 0;
}

void Escenario::addElement(DrawableObject* element){
	elements.push_back(element);
}

void Escenario::setScrollingStep(double step){
	this->scrollingStep = step;
}

void Escenario::update(){
	if(this->scrollingOffset > this->heigth ){
		scrollingOffset = 0;
	}else{
		scrollingOffset += scrollingStep;
	}

	this->posYActual += scrollingStep;
	this->posY = scrollingOffset;
	list<DrawableObject*>::iterator it;
	for (it = elements.begin(); it !=elements.end(); it++){
		(*it)->setPosY((*it)->getPosY() + scrollingStep);
	}
	if(this->posYActual == this->heigth)
		restart();
}

void Escenario::setWindowHeight(int height){
	this->windowHeight = height;
}

//Transformo coordenadas en Y. El juego como esta planteado tiene diferentes coordenadas en Y a las
//que utiliza el SDL. EL 0 en SDL esta en el tope de la ventana y crece hacia abajo, y el 0
// en el juego deberia estar en la base del escenario y crecer hacia arriba.
void Escenario::transformPositions(){
	list<DrawableObject*>::iterator it;
	for (it = elements.begin(); it !=elements.end(); it++){
		(*it)->setPosY(-(*it)->getPosY() + windowHeight);
		(*it)->setPosYInit(-(*it)->getPosYInit() + windowHeight);
	}
}

DrawableObject* Escenario::getElement(int numElement){
	if(this->elements.size() < numElement){
		cout<<"Numero incorrecto de elemento"<<endl;
		return NULL;
	}
	list<DrawableObject*>::iterator it = elements.begin();
	for(int i = 0; i < numElement; i++){
		it++;
	}
	return (*it);
}

int Escenario::getNumberElements(){
	return (elements.size());
}

void Escenario::restart(){
	this->scrollingOffset = 0;
	this->posYActual = 0;
	list<DrawableObject*>::iterator it = elements.begin();
	for(int i = 0; i < elements.size(); i++){
		(*it)->setPosX((*it)->getPosXInit());
		(*it)->setPosY((*it)->getPosYInit());
		it++;
	}
}

Escenario::~Escenario() {

}

