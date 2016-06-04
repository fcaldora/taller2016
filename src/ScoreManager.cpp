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
			(*it)->setScore((*it)->getScore() + enemyPlane->getScore());
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
