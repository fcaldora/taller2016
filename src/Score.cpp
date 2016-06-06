/*
 * Score.cpp
 *
 *  Created on: May 31, 2016
 *      Author: facundo
 */

#include "Score.h"

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
	lastScore = score;
	return true;
}

int Score::getScoreYPosition(int height){
	return height - height/8;
}
