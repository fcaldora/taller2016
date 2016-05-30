/*
 * Explosion.cpp
 *
 *  Created on: May 30, 2016
 *      Author: facundo
 */

#include "Explosion.h"

Explosion::Explosion(int posX, int posY, bool big, int id) {
	this->setActualPhotogram(1);
	this->finished = false;
	if(big){
		this->setPhotograms(7);
		this->heigth = 67;
		this->width = 67;
		this->setPath("bigexplosion.png");
	}else{
		this->setPhotograms(6);
		this->heigth = 34;
		this->width = 34;
		this->setPath( "explosion.png");
	}
	this->id = id;
	this->posX = posX;
	this->posY = posY;
}

Explosion::~Explosion() {
	// TODO Auto-generated destructor stub
}

void Explosion::updateFrame(){
	if(this->actualPhotogram == this->numberOfPhotograms){
		this->finished = true;
	}else{
		this->actualPhotogram += 1;
	}
}
