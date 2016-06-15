/*
 * Score.cpp
 *
 *  Created on: May 31, 2016
 *      Author: facundo
 */

#include "Score.h"
#include <iostream>

Score::Score() {
	score = 0;
	lastScore = 0;
	id = 0;
}

Score::~Score() {
	// TODO Auto-generated destructor stub
}

int Score::getScoreXPosition(int width){
	if(this->id == 1){
		return width/4;
	}else if(this->id == 2){
		return width - width/4;
	}
	return 50;
}

bool Score::hasChanged(){
	if (score == lastScore)
		return false;
	return true;
}

void Score::updateLastScore(){
	lastScore = score;
}

int Score::getLastScoreDifference(){
	return (score - lastScore);
}

int Score::getScoreYPosition(int height){
	return height - height/8;
}

int Score::getClientTeamId(){
	return clientTeamId;
}

void Score::setClientTeamId(int teamId){
	clientTeamId = teamId;
}
