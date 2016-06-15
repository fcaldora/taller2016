/*
 * Team.h
 *
 *  Created on: May 29, 2016
 *      Author: pccasa
 */

#ifndef TEAM_H_
#define TEAM_H_

#include "Constants.h"
#include "Client.h"

class Team {
public:
	Team();
	Team(int teamID, string teamName, int maxNumberOfPlayers);
	bool isClientOfThisTeam(int planeId);
	void addPoints(int points);
	int getPoints();
	virtual ~Team();
	int getTeamId();
	bool isFull();
	int teamID;
	string teamName;

	unsigned int maxNumberOfPlayers;
	list<Client *> clients;
private:
	int points;
};

#endif /* TEAM_H_ */
