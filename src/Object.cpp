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
	int upPlaneY = plane->getPosY() + plane->getHeigth();
	int leftBulletX = posX;
	int rightBulletX= posX + width;
	int upBulletY = posY;
	int downBulletY = posY + heigth;
	/*if(leftBulletX >= leftPlaneX && rightBulletX <= rightPlaneX)
		xCollision = true;
	else if(leftBulletX <= rightPlaneX && rightBulletX >= rightPlaneX)
		xCollision = true;
	else if(rightBulletX >= leftPlaneX && leftBulletX <= leftPlaneX)
		xCollision = true;

	if(upBulletY <= downPlaneY)
		yCollision = true;

	if(yCollision && xCollision)
		return true;
	return false;*/
	if(downBulletY < upPlaneY)
		return false;
	if(upBulletY > downPlaneY)
		return false;
	if(leftBulletX > rightPlaneX)
		return false;
	if(rightBulletX < leftPlaneX)
		return false;
	return true;

}

bool Object::haveCollision2(Client* client){
	bool xCollision = false;
	bool yCollision = false;
	int leftPlaneX = client->getPlane()->getPosX();
	int rightPlaneX = client->getPlane()->getPosX() + client->getPlane()->getWidth();
	int upPlaneY = client->getPlane()->getPosY();
	int downPlaneY = client->getPlane()->getPosY() + client->getPlane()->getHeigth();
	int leftBulletX = posX;
	int rightBulletX= posX + width;
	int downBulletY = posY + heigth;
	int upBulletY = posY;
	/*if(leftBulletX >= leftPlaneX && rightBulletX <= rightPlaneX)
		xCollision = true;
	else if(leftBulletX <= rightPlaneX && rightBulletX >= rightPlaneX)
		xCollision = true;
	else if(rightBulletX >= leftPlaneX && leftBulletX <= leftPlaneX)
		xCollision = true;

	if(downBulletY >= upPlaneY && upBulletY <= downPlaneY)
		yCollision = true;

	if(yCollision && xCollision)
		return true;
	return false;*/
	if(downPlaneY < upBulletY)
		return false;
	if(upPlaneY > downBulletY)
		return false;
	if(leftPlaneX > rightBulletX)
		return false;
	if(rightPlaneX < leftBulletX)
		return false;
	return true;

}

int Object::crashedWithPlane(list<EnemyPlane*> enemyPlanes){
	list<EnemyPlane*>::iterator it;
	for(it = enemyPlanes.begin(); it != enemyPlanes.end(); it++){
		if(this->haveCollision(*it)){
			(*it)->setLifes((*it)->getLifes() - 1);
			if((*it)->getFormation() != NULL){
				(*it)->getFormation()->setQuantity((*it)->getFormation()->getQuantity() - 1);
				if((*it)->getFormation()->isBonus()){
					//Primera vez seteo el client id
					if((*it)->getFormation()->getHitId() == -1) (*it)->getFormation()->setHitId(this->getClientId());
					(*it)->getFormation()->setBonus((*it)->getFormation()->getHitId() == this->getClientId());
					(*it)->getFormation()->setHitId(this->getClientId());
				}
			}
			if((*it)->getLifes() <= 0){
				return (*it)->getId();
			}
			return (*it)->getId();
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
					(*it)->setAlive(false);
					return (*it)->getPlane()->getId();
				}
				return (*it)->getPlane()->getId();
			}
		}
	}
	return -1;
}
