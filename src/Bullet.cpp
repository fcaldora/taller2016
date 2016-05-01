/*
 * Bullet.cpp
 *
 *  Created on: May 1, 2016
 *      Author: luciano
 */

#include "Bullet.h"

Bullet::Bullet() {
	// TODO Auto-generated constructor stub
}

Bullet::Bullet(int id, int posX, int posY) {
	this->setId(id);
	this->setPath("bullet.png");
	this->setPosX(posX);
	this->setPosY(posY);
	this->setWidth(30);
	this->setHeigth(30);
	this->setStatus(true);
}

void Bullet::move(){
	this->posY -= 1;
}

bool Bullet::notVisible(int width, int height){
	return(this->posX > (width + 5) || this->posY > (height+5) || this->posX < -5 || this->posY < -5);
}

Bullet::~Bullet() {
	// TODO Auto-generated destructor stub
}

