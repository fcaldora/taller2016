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

bool Team::isFull() {
	return (this->maxNumberOfPlayers <= this->clients.size());
}

int Team::earnedPoints () {
	int earnedPoints = 0;

	for (Client *client : clients) {
		earnedPoints += client->earnedPoints;
	}

	return earnedPoints;
}

Team::~Team() {

}
