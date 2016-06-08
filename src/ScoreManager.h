/*
 * ScoreManager.h
 *
 *  Created on: May 31, 2016
 *      Author: facundo
 */

#ifndef SCOREMANAGER_H_
#define SCOREMANAGER_H_
#include "Score.h"
#include "EnemyPlane.h"

using namespace std;

class ScoreManager {
public:
	ScoreManager();
	virtual ~ScoreManager();
	void setScores(list<Score*> scores);
	list<Score*> getScores();
	void increaseScore(int id,int hit, int score, int extraScore);
	void increaseDestroyScore(int id, EnemyPlane* enemyPlane);
	void increaseScoreForHit(int id, EnemyPlane* enemyPlane);
	void increaseScoreForPowerUp(int id, int powerUpBonus);
private:
	list<Score*> scores;
};

#endif /* SCOREMANAGER_H_ */
