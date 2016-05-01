/*
 * Object.cpp
 *
 *  Created on: Apr 24, 2016
 *      Author: facundo
 */

#include "Object.h"

Object::Object() {
	// TODO Auto-generated constructor stub

}

Object::~Object() {
	// TODO Auto-generated destructor stub
}

void Object::move(){
	this->posY -= 1;
}

bool Object::notVisible(int width, int height){
	return(this->posX > (width + 5) || this->posY > (height+5) || this->posX < -5 || this->posY < -5);
}
