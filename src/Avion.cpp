/*
 * Avion.cpp
 *
 *  Created on: 24 de abr. de 2016
 *      Author: gusta
 */

#include "Avion.h"

Avion::Avion() {
	velDesplazamiento = 0;
	velDisparo = 0;
	//ACA HAY QUE SETEAR LA POSICION INICIAL DEL AVION.
	posActualX = 0;
	posActualY = 0;
}

void Avion::setVelDesplazamiento(int velDesplazamiento){
	this->velDesplazamiento = velDesplazamiento;
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
	this->posActualX += movement;
}

void Avion::moveVerical(int movement){
	this->posActualY += movement;
}

int Avion::getPosX(){
	return posActualX;
}

int Avion::getPosY(){
	return posActualY;
}

Avion::~Avion() {
	// TODO Auto-generated destructor stub
}

