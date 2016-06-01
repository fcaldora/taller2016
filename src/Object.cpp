/*
 * Object.cpp
 *
 *  Created on: Apr 24, 2016
 *      Author: facundo
 */

#include "Object.h"

Object::Object() {
	this->actualPhotogram = 1;
	this->photograms = 1;
	this->step = 1;
}

Object::~Object() {
	// TODO Auto-generated destructor stub
}

void Object::move(){
	this->posY -= step;
}

bool Object::notVisible(int width, int height){
	return(this->posX > (width + 5) || this->posY > (height+5) || this->posX < -5 || this->posY < -5);
}
