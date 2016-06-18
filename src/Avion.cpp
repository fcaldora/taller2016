/*
 * Avion.cpp
 *
 *  Created on: 24 de abr. de 2016
 *      Author: gusta
 */

#include "Avion.h"

Avion::Avion() :DrawableObject() {
	this->numberOfPhotograms = 1;
	this->actualPhotogram = 1;
	velDisparo = 0;
	velDesplazamiento = 0;
	this->isLooping = false;
	this->hasDoubleShooting = false;
	lastRollPhotogram = 8; //CARGAR DESDE XML?
	landingCounter = 0;
}

void Avion::setVelDesplazamiento(int velDesplazamiento){
	this->velDesplazamiento = velDesplazamiento;
}

bool Avion::doubleShooting(){
	return this->hasDoubleShooting;
}

void Avion::setDoubleShooting(bool hasDoubleShooting){
	this->hasDoubleShooting = hasDoubleShooting;
}

int Avion::getVelDesplazamiento(){
	return velDesplazamiento;
}

void Avion::setVelDisparo(int velDisparo){
	this->velDisparo = velDisparo;
}

int Avion::getVelDisparo(){
	return velDisparo;
}

void Avion::moveHorizontal(int movement){
	posX += movement;
}

void Avion::moveVerical(int movement){
	posY += movement;
}

void Avion::moveOneStepRight() {
	moveHorizontal(this->velDesplazamiento);
}

void Avion::moveOneStepLeft() {
	moveHorizontal(-this->velDesplazamiento);
}

void Avion::moveOneStepUp() {
	moveVerical(-this->velDesplazamiento);
}

void Avion::moveOneStepDown() {
	moveVerical(this->velDesplazamiento);
}

void Avion::setPhotogram(){
	if(this->numberOfPhotograms > 1)
		this->actualPhotogram++;
}

void Avion::aterrizar(int finishX){
	if(this->posX - finishX >= 0){
		this->moveOneStepLeft();
	}else if(this->posX - finishX <= 0){
		this->moveOneStepRight();
	}
	if(landingCounter == 0){
		actualPhotogram = lastRollPhotogram + 1;
		landingCounter++;
		return;
	}

	landingCounter++;

	if(actualPhotogram < numberOfPhotograms && landingCounter == 100){
		actualPhotogram++;
		landingCounter = 1;
	}else if(actualPhotogram == numberOfPhotograms){
		actualPhotogram = 1;
		landingCounter = 0;
	}
}

bool Avion::updatePhotogram(bool aterrizaje){
	if(this->actualPhotogram == 1) {
		this->isLooping = false;
		return false;
	}

	if(this->actualPhotogram < this->lastRollPhotogram){
		this->actualPhotogram++;
		this->isLooping = true;
		return true;
	}
	if(this->actualPhotogram == this->lastRollPhotogram){
		this->actualPhotogram = 1;
		this->isLooping = false;
		return true;
	}
	if(!aterrizaje & (actualPhotogram > lastRollPhotogram || actualPhotogram >= numberOfPhotograms)){
		actualPhotogram = 1;
		landingCounter = 0;
	}
	return false;
}

Avion::~Avion() {
}

