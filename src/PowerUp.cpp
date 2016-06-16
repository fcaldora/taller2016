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
	int leftPlaneX = plane->getPosX();
	int rightPlaneX = plane->getPosX() + plane->getWidth();
	int upPlaneY = plane->getPosY();
	int downPlaneY = plane->getPosY() + plane->getHeigth();
	int leftPowerupX = posX;
	int rightPowerupX = posX + width;
	int upPowerupY = posY;
	int downPowerupY = posY + heigth;

	if(downPlaneY < upPowerupY)
		return false;
	if(upPlaneY > downPowerupY)
		return false;
	if(leftPlaneX > rightPowerupX)
		return false;
	if(rightPlaneX < leftPowerupX)
		return false;
	return true;
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

void PowerUp::applyPowerUp(Avion* avion, ScoreManager* score, list<EnemyPlane*> &enemyPlanes, Procesador* processor){
	list<EnemyPlane*>::iterator it;
	switch(this->type){
	case 1:
		avion->setDoubleShooting(true);
		break;
	case 2:
		for(it = enemyPlanes.begin(); it != enemyPlanes.end(); it++){
			if((*it)->isOnScreen(processor->getScreenWidth(), processor->getScreenHeight())){
				(*it)->setLifes(0);
				score->increaseDestroyScore(avion->getId(), (*it));
			}
		}
		break;
	case 3:
		cout<<"Sumar  "<<this->pointsToAdd<<" puntos"<<endl;
		score->increaseScoreForPowerUp(avion->getId(), pointsToAdd);
		break;
	}
}

PowerUp::~PowerUp() {}

