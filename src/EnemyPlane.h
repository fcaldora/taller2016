/*
 * EnemyPlane.h
 *
 *  Created on: May 27, 2016
 *      Author: facundo
 */

#ifndef ENEMYPLANE_H_
#define ENEMYPLANE_H_
#include "DrawableObject.h"
#include "Formation.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "ClientList.h"

class EnemyPlane: public DrawableObject {
public:
	EnemyPlane();
	virtual ~EnemyPlane();

	const Formation* getFormation() {
		return formation;
	}

	void setFormation( Formation* formation) {
		this->formation = formation;
	}

	int getLifes() const {
		return lifes;
	}

	void setLifes(int lifes) {
		this->lifes = lifes;
	}

	int getScore() const {
		return score;
	}

	void setScore(int score) {
		this->score = score;
	}

	void move();
	void movePosition();
	void changeDirection();
	void die();
	void shoot();

	const string getFacingDirection()  {
		return facingDirection;
	}

	void setFacingDirection(string facingDirection) {
		this->facingDirection = facingDirection;
	}

	int getTimesFacingOneDirection() const {
		return timesFacingOneDirection;
	}

	void setTimesFacingOneDirection(int timesFacingOneDirection) {
		this->timesFacingOneDirection = timesFacingOneDirection;
	}
	bool notVisible(int width, int height);

	bool isCrazyMoves() const {
		return crazyMoves;
	}

	void setCrazyMoves(bool crazyMoves) {
		this->crazyMoves = crazyMoves;
	}

	int getSpeedFactor() const {
		return speedFactor;
	}

	void setSpeedFactor(int speedFactor) {
		this->speedFactor = speedFactor;
	}

	int collideWithClient(ClientList* clientList);

private:
	Formation* formation;
	int score;
	int lifes;
	string facingDirection;
	int timesFacingOneDirection;
	bool crazyMoves;
	int speedFactor;
};

#endif /* ENEMYPLANE_H_ */
