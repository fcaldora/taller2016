/*
 * Bullet.h
 *
 *  Created on: May 1, 2016
 *      Author: luciano
 */

#ifndef BULLET_H_
#define BULLET_H_

#include "DrawableObject.h"

class Bullet: public DrawableObject {
public:
	Bullet();
	Bullet(int id, int posX, int posY);
	virtual ~Bullet();

	void move();

	bool notVisible(int width, int height);
};

#endif /* BULLET_H_ */
