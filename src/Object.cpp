/*
 * Object.cpp
 *
 *  Created on: Apr 24, 2016
 *      Author: facundo
 */

#include "Object.h"

Object::Object() {
	this->actualPhotogram = 1;
	this->photograms = 1;
	this->step = 1;

}

Object::~Object() {
	// TODO Auto-generated destructor stub
}

void Object::move(){
	if(this->isEnemyBullet()){
		this->posY += step;
	}else{
		this->posY -= step;
	}
}

bool Object::notVisible(int width, int height){
	return(this->posX > (width + 5) || this->posY > (height+5) || this->posX < -5 || this->posY < -5);
}

bool Object::haveCollision(EnemyPlane* plane){
	bool xCollision = false;
	bool yCollision = false;
	int leftPlaneX = plane->getPosX();
	int rightPlaneX = plane->getPosX() + plane->getWidth();
	int downPlaneY = plane->getPosY() + plane->getHeigth();
	int leftBulletX = posX;
	int rightBulletX= posX + width;
	int upBulletY = posY;
	if(leftBulletX >= leftPlaneX && rightBulletX <= rightPlaneX)
		xCollision = true;
	else if(leftBulletX <= rightPlaneX && rightBulletX >= rightPlaneX)
		xCollision = true;
	else if(rightBulletX >= leftPlaneX && leftBulletX <= leftPlaneX)
		xCollision = true;

	if(upBulletY <= downPlaneY)
		yCollision = true;

	if(yCollision && xCollision)
		return true;
	return false;

}

bool Object::haveCollision2(Client* client){
	bool xCollision = false;
	bool yCollision = false;
	int leftPlaneX = client->getPlane()->getPosX();
	int rightPlaneX = client->getPlane()->getPosX() + client->getPlane()->getWidth();
	int upPlaneY = client->getPlane()->getPosY();
	int leftBulletX = posX;
	int rightBulletX= posX + width;
	int downBulletY = posY + heigth;
	if(leftBulletX >= leftPlaneX && rightBulletX <= rightPlaneX)
		xCollision = true;
	else if(leftBulletX <= rightPlaneX && rightBulletX >= rightPlaneX)
		xCollision = true;
	else if(rightBulletX >= leftPlaneX && leftBulletX <= leftPlaneX)
		xCollision = true;

	if(downBulletY >= upPlaneY)
		yCollision = true;

	if(yCollision && xCollision)
		return true;
	return false;

}

int Object::crashedWithPlane(list<EnemyPlane*> enemyPlanes){
	list<EnemyPlane*>::iterator it;
	for(it = enemyPlanes.begin(); it != enemyPlanes.end(); it++){
		if(this->haveCollision(*it)){
			(*it)->setLifes((*it)->getLifes() - 1);
			if((*it)->getLifes() <= 0){
				return (*it)->getId();
			}
			return -2;
		}
	}
	return -1;
}

int Object::crashedWithClient(ClientList* clientList){
	list<Client*>::iterator it;
	for(it = clientList->clients.begin(); it != clientList->clients.end(); it++){
		if((*it)->isAlive()){
			if(this->haveCollision2((*it))){
				(*it)->getPlane()->setLifes((*it)->getPlane()->getLifes() - 1);
				if((*it)->getPlane()->getLifes() <= 0){
					return (*it)->getPlane()->getId();
				}
				return -2;
			}
		}
	}
	return -1;
}
