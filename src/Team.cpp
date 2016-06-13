/*
 * Team.cpp
 *
 *  Created on: May 29, 2016
 *      Author: pccasa
 */

#include "Team.h"

Team::Team() {
	this->maxNumberOfPlayers = 0;
	this->teamID = 0;
}

Team::Team(int teamID, string teamName, int maxNumberOfPlayers) {
	this->maxNumberOfPlayers = maxNumberOfPlayers;
	this->teamID = teamID;
	this->teamName = teamName;
}

bool Team::isFull() {
	return (this->maxNumberOfPlayers <= this->clients.size());
}

int Team::earnedPoints () {
	int earnedPoints = 0;

	for (Client *client : clients) {
		//earnedPoints += client->earnedPoints;
	}

	return earnedPoints;
}

Team::~Team() {

}
