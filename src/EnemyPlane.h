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

	Formation* getFormation() {
		return formation;
	}

	void setFormation( Formation* formation) {
		this->formation = formation;
	}

	int getLifes() {
		return lifes;
	}

	void setLifes(int lifes) {
		this->lifes = lifes;
	}

	int getScore() {
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

	int getTimesFacingOneDirection()  {
		return timesFacingOneDirection;
	}

	void setTimesFacingOneDirection(int timesFacingOneDirection) {
		this->timesFacingOneDirection = timesFacingOneDirection;
	}
	bool notVisible(int width, int height);

	bool isCrazyMoves()  {
		return crazyMoves;
	}

	void setCrazyMoves(bool crazyMoves) {
		this->crazyMoves = crazyMoves;
	}

	int getSpeedFactor()  {
		return speedFactor;
	}

	void setSpeedFactor(int speedFactor) {
		this->speedFactor = speedFactor;
	}

	int collideWithClient(ClientList* clientList);

	bool isOnScreen(int width, int height);

	int getLastHitScore() const {
		return lastHitScore;
	}

	void setLastHitScore(int lastHitScore) {
		this->lastHitScore = lastHitScore;
	}

	bool isBigPlane() const {
		return bigPlane;
	}

	void setBigPlane(bool bigPlane) {
		this->bigPlane = bigPlane;
	}

	void setPwUpPath(string path){
		pwUpPath = path;
	}

	string getPwUpPath(){
		return pwUpPath;
	}

	void setPwUpHeight(int height){
		pwUpHeight = height;
	}

	int getPwUpHeiht(){
		return pwUpHeight;
	}

	void setPwUpWidth(int width){
		pwUpWidth = width;
	}

	int getPwUpWidth(){
		return pwUpWidth;
	}

	void setPwUpPoints(int points){
		pwUpPoints = points;
	}

	int getPwUpPoints(){
		return pwUpPoints;
	}

private:
	Formation* formation;
	int score;
	int lifes;
	string facingDirection;
	int timesFacingOneDirection;
	bool crazyMoves;
	int speedFactor;
	int lastHitScore;
	bool bigPlane;
	string pwUpPath;
	int pwUpWidth;
	int pwUpHeight;
	int pwUpPoints;
};

#endif /* ENEMYPLANE_H_ */
