/*
 * BulletList.h
 *
 *  Created on: 12 de may. de 2016
 *      Author: gusta
 */
#include <mutex>
#include <list>
#include "Object.h"
#include "Constants.h"
#include "EnemyPlane.h"
#ifndef BULLETLIST_H_
#define BULLETLIST_H_

using namespace std;

class BulletList {
public:
	BulletList();
	void addElement(Object obj);
	void setIdOfFirstBullet(int id);
	void deleteElement(int id);
	Object getObject(unsigned int number);
	int getObjectId(int numberOfObject);
	int bulletQuantity();
	int getLastId();
	int getFirstId();
	void moveBullets();
	int bulletMessage(int bulletNumber, mensaje &msg, int ScreenWidth, int ScreenHeight, list<EnemyPlane*> enemyPlanes);
	list<Object>::iterator begin();
	list<Object>::iterator end();
	virtual ~BulletList();
	void setLastId(int id);
private:
	list<Object> objectList;
	std::mutex mutex;
	int firstBulletId;
	int lastBulletId;
};

#endif /* BULLETLIST_H_ */
