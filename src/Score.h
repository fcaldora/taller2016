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

	void setClientSocket(int socket){
		clientSocket = socket;
	}

	int getClientSocket(){
		return clientSocket;
	}

	int getScore() const {
		return score;
	}

	void setScore(int score) {
		this->score = score;
	}

	bool hasChanged();
	void updateLastScore();
	void setClientTeamId(int teamId);
	int getClientTeamId();
	int getScoreXPosition(int width);
	int getScoreYPosition(int height);
	int getLastScoreDifference();
private:
	int score;
	int lastScore;
	int id;
	int clientSocket;
	int clientTeamId;
};

#endif /* SCORE_H_ */
