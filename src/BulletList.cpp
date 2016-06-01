/*
 * BulletList.cpp
 *
 *  Created on: 12 de may. de 2016
 *      Author: gusta
 */

#include "BulletList.h"

BulletList::BulletList() {
	// TODO Auto-generated constructor stub

}

void BulletList::addElement(Object obj){
	this->mutex.lock();
	this->objectList.push_back(obj);
	this->lastBulletId++;
	this->mutex.unlock();
}

int BulletList::getLastId(){
	return this->lastBulletId;
}

void BulletList::deleteElement(int id){
	list<Object>::iterator it = this->begin();
	bool encontrado = false;
	this->mutex.lock();
	while(!encontrado && it != this->end()){
		if((*it).getId() == id){
			this->objectList.erase(it);
			encontrado = true;
		}else{
			it++;
		}
	}
	this->mutex.unlock();
}

int BulletList::getFirstId(){
	return this->firstBulletId;
}

void BulletList::setIdOfFirstBullet(int id){
	this->firstBulletId = id;
	this->lastBulletId = id;
}

int BulletList::bulletQuantity(){
	return (this->objectList.size());
}

//DEBE ENCONTRAR EL OBJETO POR ID, EN VEZ DE POR POSICION EN LA LISTA
Object BulletList::getObject(unsigned int number){
	Object obj;
	if(number > this->bulletQuantity()){
		obj.setStatus(false);
		return obj;
	}
	list<Object>::iterator it = this->objectList.begin();
	for(unsigned int i = 0; i < number; i++)
		it++;
	return (*it);
}

void BulletList::moveBullets(){
	list<Object>::iterator objectIt;
	for(objectIt = this->begin(); objectIt != this->end(); objectIt++){
		if((*objectIt).isStatic()){
			(*objectIt).move();
		}
	}
}

int BulletList::bulletMessage(int bulletNumber, mensaje &msg, int width, int height, list<EnemyPlane*> enemyPlanes){
//HARDCODADO EL BULLETID + 7. MODIFICAR!
	Object object = this->getObject(bulletNumber);
	int id = object.crashedWithPlane(enemyPlanes);
	if((object.notVisible(width, height) || id != -1) && object.isStatic()){
		strcpy(msg.action, "delete");
		msg.id = object.getId();
		object.setStatus(false);
		this->deleteElement(object.getId());
	}else if(object.isStatic()){
		strcpy(msg.action, "draw");
		msg.id = object.getId();
		msg.posX = object.getPosX();
		msg.posY = object.getPosY();
		msg.actualPhotogram = object.getActualPhotogram();
	}else{//object is not static
		strncpy(msg.action, "Bullet deleted", 20);
	}
	return id;
}

list<Object>::iterator BulletList::begin(){
	return this->objectList.begin();
}

list<Object>::iterator BulletList::end(){
	return this->objectList.end();
}

void BulletList::setLastId(int id){
	this->lastBulletId = id;
}

BulletList::~BulletList() {
	// TODO Auto-generated destructor stub
}

