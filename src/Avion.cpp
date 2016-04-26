/*
 * Avion.cpp
 *
 *  Created on: 24 de abr. de 2016
 *      Author: gusta
 */

#include "Avion.h"

Avion::Avion(int velDesplazamiento, int velDisparo, int numberOfPhotograms,
		int actualPhotogram, int postionX, int positionY, int height, int width, string imagePath) :DrawableObject() {
	this->velDesplazamiento = velDesplazamiento;
	this->velDisparo = velDisparo;
	this->numberOfPhotograms = numberOfPhotograms;
	this->actualPhotogram = actualPhotogram;
	this->posX = postionX;
	this->posY = positionY;
	this->heigth = height;
	this->width = width;
	this->imagePath = imagePath;
	avionSprite = NULL;
	disparoSprite = NULL;
	vueltaSprite = NULL;
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
	posX += movement;
}

void Avion::moveVerical(int movement){
	posY += movement;
}

void Avion::moveOneStepRight() {
	moveHorizontal(1);
}

void Avion::moveOneStepLeft() {
	moveHorizontal(-1);
}

void Avion::moveOneStepUp() {
	moveVerical(-1);
}

void Avion::moveOneStepDown() {
	moveVerical(1);
}

Avion::~Avion() {
	delete(avionSprite);
	delete(disparoSprite);
	delete(vueltaSprite);
	// TODO Auto-generated destructor stub
}

