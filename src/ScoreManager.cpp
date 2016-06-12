/*
 * ScoreManager.cpp
 *
 *  Created on: May 31, 2016
 *      Author: facundo
 */

#include "ScoreManager.h"

ScoreManager::ScoreManager() {
	// TODO Auto-generated constructor stub

}

ScoreManager::~ScoreManager() {
	// TODO Auto-generated destructor stub
}

void ScoreManager::setScores(list<Score*> scores){
	this->scores = scores;
}

list<Score*> ScoreManager::getScores(){
	return this->scores;
}

void ScoreManager::increaseScore(int id, int hid, int score, int extraScore){

}


void ScoreManager::increaseDestroyScore(int id, EnemyPlane* enemyPlane){
	list<Score*>::iterator it;
	for(it = this->scores.begin(); it != this->scores.end(); it++){
		if((*it)->getId() == id){
			(*it)->setScore((*it)->getScore() + enemyPlane->getScore() + enemyPlane->getLastHitScore());
			if(enemyPlane->getFormation() != NULL){
				if(enemyPlane->getFormation()->getQuantity() == 0){
					enemyPlane->getFormation()->setDestroyed(true);
					if(enemyPlane->getFormation()->isBonus()){
						cout << "IT IS! here are your " << enemyPlane->getFormation()->getExtraPoints() << " extra points" << endl;
						(*it)->setScore((*it)->getScore() + enemyPlane->getFormation()->getExtraPoints());
					}
				}
			}
		}
	}
}

void ScoreManager::increaseScoreForHit(int id, EnemyPlane* enemyPlane){
	list<Score*>::iterator it;
	for(it = this->scores.begin(); it != this->scores.end(); it++){
		if((*it)->getId() == id){
			(*it)->setScore((*it)->getScore() + enemyPlane->getScore());
		}
	}
}

void ScoreManager::increaseScoreForPowerUp(int id, int powerUpBonus){
	list<Score*>::iterator it = scores.begin();
	bool encontrado = false;
	while(!encontrado && it != scores.end()){
		if((*it)->getId() == id){
			(*it)->setScore((*it)->getScore() + powerUpBonus);
			encontrado = true;
		}else{
			it++;
		}
	}
}

void ScoreManager::resetScores(){
	list<Score*>::iterator it;
	for(it = scores.begin(); it != scores.end(); it++){
		(*it)->setScore(0);
	}
}
