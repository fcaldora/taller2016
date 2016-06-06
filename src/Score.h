/*
 * Score.h
 *
 *  Created on: May 31, 2016
 *      Author: facundo
 */

#ifndef SCORE_H_
#define SCORE_H_

class Score {
public:
	Score();
	virtual ~Score();

	int getId() const {
		return id;
	}

	void setId(int id) {
		this->id = id;
	}

	int getScore() const {
		return score;
	}

	void setScore(int score) {
		this->score = score;
	}

	bool hasChanged();

	int getScoreXPosition(int width);
	int getScoreYPosition(int height);
private:
	int score;
	int lastScore;
	int id;
};

#endif /* SCORE_H_ */
