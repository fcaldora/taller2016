/*
 * Explosion.h
 *
 *  Created on: May 30, 2016
 *      Author: facundo
 */

#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "DrawableObject.h"

class Explosion: public DrawableObject{
public:
	Explosion(int posX, int posY, bool big, int id);
	virtual ~Explosion();

	bool isFinished() const {
		return finished;
	}

	void setFinished(bool finished) {
		this->finished = finished;
	}

	void updateFrame();
private:
	bool finished;
};

#endif /* EXPLOSION_H_ */
