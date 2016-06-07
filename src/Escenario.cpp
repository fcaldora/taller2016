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
	this->portaAvionesY = 0;
	this->portaAvionesX = 0;
}

void Escenario::addElement(DrawableObject* element){
	element->setPosXInit(element->getPosX());
	element->setPosYInit(element->getPosY());
	elements.push_back(element);
}

void Escenario::setScrollingStep(double step){
	this->scrollingStep = step;
}

void Escenario::update(){
	//if(this->scrollingOffset > this->heigth ){
		//scrollingOffset = 0;
	//}else{
		scrollingOffset += scrollingStep;
	//}
	if(this->scrollingOffset >= this->stagesPositions.front()){
		this->stagesPositions.pop_front();
		this->portaAvionesY = this->stagesPositions.front();
	}
	this->posYActual += scrollingStep;
	this->posY = scrollingOffset;
	list<DrawableObject*>::iterator it;
	for (it = elements.begin(); it !=elements.end(); it++){
		(*it)->setPosY((*it)->getPosY() + scrollingStep);
	}
	for(int i = 0; i < powerUps.numberOfPowerUps(); i++){
		powerUps.getPowerUp(i)->setPosY(powerUps.getPowerUp(i)->getPosY() + scrollingStep);
	}
	//if(this->posYActual == this->heigth)
		//restart();
}

void Escenario::deleteElement(int elementId){
	list<DrawableObject*>::iterator it = elements.begin();
	bool found = false;
	while(!found && it != elements.end()){
		if((*it)->getId() == elementId){
			found = true;
			elements.erase(it);
		}else
			it++;
	}
}

void Escenario::addPowerUp(PowerUp* powerUp){
	powerUps.addPowerUp(powerUp);
}

void Escenario::deletePowerUp(int powerUpId){
	powerUps.deletePowerUp(powerUpId);
}

PowerUp* Escenario::getPowerUp(int numPowerUp){
	return(powerUps.getPowerUp(numPowerUp));
}

int Escenario::getNumberOfPowerUps(){
	return (powerUps.numberOfPowerUps());
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
	for(int i = 0; i < powerUps.numberOfPowerUps(); i++){
		powerUps.getPowerUp(i)->setPosY(-powerUps.getPowerUp(i)->getPosY() + windowHeight);
		powerUps.getPowerUp(i)->setPosYInit(-powerUps.getPowerUp(i)->getPosYInit() + windowHeight);
	}

}

DrawableObject* Escenario::getElement(unsigned int numElement){
	if(this->elements.size() < numElement){
		cout<<"Numero incorrecto de elemento"<<endl;
		return NULL;
	}
	list<DrawableObject*>::iterator it = elements.begin();
	for(unsigned int i = 0; i < numElement; i++){
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
	for(unsigned int i = 0; i < elements.size(); i++){
		(*it)->setPosX((*it)->getPosXInit());
		(*it)->setPosY((*it)->getPosYInit());
		it++;
	}
	for(int i = 0; i < powerUps.numberOfPowerUps(); i++){
		powerUps.getPowerUp(i)->setPosX(powerUps.getPowerUp(i)->getPosXInit());
		powerUps.getPowerUp(i)->setPosY(powerUps.getPowerUp(i)->getPosYInit());
	}
}

void Escenario::deleteElements(){
	this->elements.clear();
}

bool Escenario::hayQueAterrizar(){
	if(portaAvionesY < scrollingOffset + windowHeight ){
		return true;
	}
	return false;
}

void Escenario::setPosPortaAviones(int posX, int posY){
	this->portaAvionesX = posX;
	this->portaAvionesY = posY;
}

int Escenario::getPortaAvionesX(){
	return portaAvionesX;
}

int Escenario::getPortaAvionesY(){
	return portaAvionesY;
}

void Escenario::deletePowerUps(){
	this->powerUps.deletePowerUps();
}

void Escenario::setStagesPositions(XmlParser* parser){
	parser->getStagesPositions(stagesPositions);
}

bool Escenario::gameFinished(){
	if(scrollingOffset >= this->heigth)
		return true;
	return false;
}

Escenario::~Escenario() {

}

