/*
 * EnemyPlane.cpp
 *
 *  Created on: May 27, 2016
 *      Author: facundo
 */

#include "EnemyPlane.h"

EnemyPlane::EnemyPlane() {
	pwUpPoints = 0;
	pwUpWidth = 0;
	pwUpHeight = 0;
	pwUpPath = "";
}

EnemyPlane::~EnemyPlane() {
	// TODO Auto-generated destructor stub
}

void EnemyPlane::die(){

}

void EnemyPlane::shoot(){

}

void EnemyPlane::changeDirection(){
	if(strcmp(this->facingDirection.c_str(), "NORTH") == 0){
		this->facingDirection = "NORTHEAST";
	}else if(strcmp(this->facingDirection.c_str(), "NORTHEAST") == 0){
		this->facingDirection = "EAST";
	}else if(strcmp(this->facingDirection.c_str(), "EAST") == 0){
		this->facingDirection = "SOUTHEAST";
	}else if(strcmp(this->facingDirection.c_str(), "SOUTHEAST") == 0){
		this->facingDirection = "SOUTH";
	}else if(strcmp(this->facingDirection.c_str(), "SOUTH") == 0){
		this->facingDirection = "SOUTHWEST";
	}else if(strcmp(this->facingDirection.c_str(), "SOUTHWEST") == 0){
		this->facingDirection = "WEST";
	}else if(strcmp(this->facingDirection.c_str(), "WEST") == 0){
		this->facingDirection = "NORTHWEST";
	}else if(strcmp(this->facingDirection.c_str(), "NORTHWEST") == 0){
		this->facingDirection = "NORTH";
	}
}

void EnemyPlane::movePosition(){
	if(strcmp(this->facingDirection.c_str(), "NORTH") == 0){
		if(this->isBigPlane() && posY < 10){
			this->facingDirection = "WEST";
		}
		this->posY -= 1;
	}else if(strcmp(this->facingDirection.c_str(), "NORTHEAST") == 0){
		this->posY -= 1;
		this->posX += 1;
	}else if(strcmp(this->facingDirection.c_str(), "EAST") == 0){
		if(this->isBigPlane() && posX > 700){
			this->facingDirection = "WEST";
		}
		this->posX += 1;
	}else if(strcmp(this->facingDirection.c_str(), "SOUTHEAST") == 0){
		this->posX += 1;
		this->posY += 1;
	}else if(strcmp(this->facingDirection.c_str(), "SOUTH") == 0){
		this->posY += 1;
	}else if(strcmp(this->facingDirection.c_str(), "SOUTHWEST") == 0){
		this->posY += 1;
		this->posX -= 1;
	}else if(strcmp(this->facingDirection.c_str(), "WEST") == 0){
		if(this->isBigPlane() && posX < 10){
			this->facingDirection = "EAST";
		}
		this->posX -= 1;
	}else if(strcmp(this->facingDirection.c_str(), "NORTHWEST") == 0){
		this->posX -= 1;
		this->posY -= 1;
	}
}

void EnemyPlane::move(){
	if(this->crazyMoves){
		if(this->timesFacingOneDirection > (rand()%150 + this->timesFacingOneDirection - 4)){
			this->changeDirection();
			this->timesFacingOneDirection = 1;
		}else{
			this->timesFacingOneDirection += 1;
		}
	}
	if(this->speedFactor > 5){
		this->movePosition();
		this->speedFactor = 0;
	}else{
		this->speedFactor += 1;
	}
}

bool EnemyPlane::notVisible(int width, int height){
	return(this->posX > (width + 50) || (this->posY > (height+50) && !this->isBigPlane()));
}

int EnemyPlane::collideWithClient(ClientList* clientList){
	list<Client*>::iterator it;
	bool xCollision = false;
	bool yCollision = false;
	for(it = clientList->clients.begin(); it != clientList->clients.end(); it++){
		xCollision = false;
		yCollision = false;
		if((*it)->getConnnectionState() && (*it)->isAlive()){

			int leftPlaneX = (*it)->getPlane()->getPosX();
			int rightPlaneX = (*it)->getPlane()->getPosX() + (*it)->getPlane()->getWidth();
			int upPlaneY = (*it)->getPlane()->getPosY();
			int downPlaneY = (*it)->getPlane()->getPosY() + (*it)->getPlane()->getHeigth();
			int leftClientX = posX;
			int rightClientX = posX + width;
			int upClientY = posY;
			int downClientY = posY + heigth;
			if(leftPlaneX >= leftClientX && leftPlaneX <= rightClientX)
				xCollision = true;
			else if(rightPlaneX >= leftClientX && rightPlaneX <= rightClientX)
				xCollision = true;
			else if(leftPlaneX >= leftClientX && rightPlaneX <= rightClientX)
				xCollision = true;

			if(upPlaneY >= upClientY && upPlaneY <= downClientY)
				yCollision = true;
			else if(downPlaneY >= upClientY && downPlaneY <= downClientY)
				yCollision = true;
			else if(downPlaneY <= downClientY && upPlaneY >= upClientY)
				yCollision = true;

			if(yCollision && xCollision){
				return (*it)->getPlane()->getId();
			}
		}
	}
	return -1;
}

bool EnemyPlane::isOnScreen(int width, int height){
	return (this->posX < width && this->posY < height && this->posX > 0 && this->posY > 0);
}
