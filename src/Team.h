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
	virtual ~Team();

	bool isFull();
	int teamID;
	string teamName;

	unsigned int maxNumberOfPlayers;
	int earnedPoints();

	list<Client *> clients;
};

#endif /* TEAM_H_ */
