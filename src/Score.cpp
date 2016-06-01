/*
 * Score.cpp
 *
 *  Created on: May 31, 2016
 *      Author: facundo
 */

#include "Score.h"

Score::Score() {
	// TODO Auto-generated constructor stub

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

int Score::getScoreYPosition(int height){
	return height - height/8;
}
