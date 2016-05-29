/*
 * PowerUpList.cpp
 *
 *  Created on: 26 de may. de 2016
 *      Author: gusta
 */

#include "PowerUpList.h"

PowerUpList::PowerUpList() {
	scrollingStep = 0;
	scrollingOffset = 0;
	scenaryHeight = 0;
}

void PowerUpList::addPowerUp(PowerUp* powerUp){
	powerUp->setPosXInit(powerUp->getPosX());
	powerUp->setPosYInit(powerUp->getPosY());
	powerUpsList.push_back(powerUp);
}

void PowerUpList::deletePowerUp(int powerUpId){
	list<PowerUp*>::iterator it = powerUpsList.begin();
	bool found = false;
	while(!found && it != powerUpsList.end()){
		if((*it)->getId() == powerUpId){
			found = true;
			powerUpsList.erase(it);
		}else
			it++;
	}
}

PowerUp* PowerUpList::getPowerUp(unsigned int numberOfPowerUp){
	if(numberOfPowerUp > powerUpsList.size()){
		cout<<"Numero incorrecto de power Up"<<endl;
		return NULL;
	}
	list<PowerUp*>::iterator it = powerUpsList.begin();
	for(unsigned int i = 0; i < numberOfPowerUp; i++){
		it++;
	}
	return (*it);
}

void PowerUpList::movePowerUps(){
	list<PowerUp*>::iterator it;
	for(it = powerUpsList.begin(); it != powerUpsList.end(); it++){
		(*it)->setPosY((*it)->getPosY() + scrollingStep);
	}
	if(scrollingOffset >= scenaryHeight){
		scrollingOffset = 0;
		for(it = powerUpsList.begin(); it != powerUpsList.end(); it++){
			(*it)->setPosY((*it)->getPosYInit());
		}
	}
	else
		scrollingOffset += scrollingStep;
}

void PowerUpList::setScrollingStep(int step){
	scrollingStep = step;
}

void PowerUpList::setScenaryHeight(int height){
	scenaryHeight = height;
}

int PowerUpList::numberOfPowerUps(){
	return powerUpsList.size();
}

void PowerUpList::deletePowerUps(){
	powerUpsList.clear();
}

PowerUpList::~PowerUpList() {
	powerUpsList.clear();
}
