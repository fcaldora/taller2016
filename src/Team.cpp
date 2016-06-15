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
	points = 0;
}

Team::Team(int teamID, string teamName, int maxNumberOfPlayers) {
	this->maxNumberOfPlayers = maxNumberOfPlayers;
	this->teamID = teamID;
	this->teamName = teamName;
	points = 0;
}

bool Team::isFull() {
	return (this->maxNumberOfPlayers <= this->clients.size());
}

bool Team::isClientOfThisTeam(int planeId){
	list<Client*>::iterator it;
	for(it = clients.begin(); it != clients.end(); it++){
		if((*it)->getPlane()->getId() == planeId)
			return true;
	}
	return false;
}

void Team::addPoints(int points){
	this->points += points;
}

int Team::getPoints(){
	return points;
}

int Team::getTeamId(){
	return teamID;
}

Team::~Team() {

}
