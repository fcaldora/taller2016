/*
 * PowerUp.cpp
 *
 *  Created on: 25 de may. de 2016
 *      Author: gusta
 */

#include "PowerUp.h"

PowerUp::PowerUp() {
	this->type = 0;
	this->pointsToAdd = 0;
}

void PowerUp::setPointsToAdd(int points){
	this->pointsToAdd = points;
}

bool PowerUp::haveCollision(Avion* plane){
	bool xCollision = false;
	bool yCollision = false;
	int leftPlaneX = plane->getPosX();
	int rightPlaneX = plane->getPosX() + plane->getWidth();
	int upPlaneY = plane->getPosY();
	int downPlaneY = plane->getPosY() + plane->getHeigth();
	int leftPowerupX = posX;
	int rightPowerupX = posX + width;
	int upPowerupY = posY;
	int downPowerupY = posY + heigth;

	if(leftPlaneX >= leftPowerupX && leftPlaneX <= rightPowerupX)
		xCollision = true;
	else if(rightPlaneX >= leftPowerupX && rightPlaneX <= rightPowerupX)
		xCollision = true;

	if(upPlaneY >= upPowerupY && upPlaneY <= downPowerupY)
		yCollision = true;
	else if(downPlaneY >= upPowerupY && downPlaneY <= downPowerupY)
		yCollision = true;

	if(yCollision && xCollision)
		return true;
	return false;
}

int PowerUp::getType(){
	return type;
}

int PowerUp::getPointsToAdd(){
	return pointsToAdd;
}

void PowerUp::setType(int type){
	this->type = type;
}

void PowerUp::applyPowerUp(Avion* avion){
	switch(this->type){
	case 1:
		avion->setDoubleShooting(true);
		break;
	case 2:
		//Matar a todos los enemigos que aparecen en pantalla y otorgarle los puntos
		//que correspondan al avion.
		break;
	case 3:
		//Aca deberia sumar puntos al cliente o al avion.
		cout<<"Sumar  "<<this->pointsToAdd<<" puntos"<<endl;
	}
}

PowerUp::~PowerUp() {}

