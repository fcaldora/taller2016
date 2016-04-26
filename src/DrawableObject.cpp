/*
 * Object.cpp
 *
 *  Created on: Apr 24, 2016
 *      Author: facundo
 */

#include "DrawableObject.h"

DrawableObject::DrawableObject() {
	id = 0;
	imagePath = "";
	actualPhotogram = 0;
	photograms = 0;
	posX = 0;
	posY = 200;
	width = 200;
	heigth = 200;
}

DrawableObject::~DrawableObject() {
	// TODO Auto-generated destructor stub
}

