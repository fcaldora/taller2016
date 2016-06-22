/*
 * MessageBuilder.h
 *
 *  Created on: Apr 26, 2016
 *      Author: luciano
 */

#ifndef MESSAGEBUILDER_H_
#define MESSAGEBUILDER_H_

#include "Constants.h"
#include "Client.h"
#include "Object.h"
#include "Escenario.h"
#include "Explosion.h"
#include "EnemyPlane.h"
#include "Score.h"
#include "Team.h"

class MessageBuilder {
public:
	MessageBuilder();
	virtual ~MessageBuilder();

	clientMsj createSuccessfullyConnectedMessageForClient(Client *client, bool colaboration);
	clientMsj createSuccessfullyReconnectedMessageForClient(Client *client, bool colaboration);
	clientMsj createServerFullMessage();
	clientMsj createUserNameAlreadyInUseMessage();
	mensaje createInitialMessageForClient(Client *client);
	mensaje createInitialScoreMessage(Score* score, int posx, int posy, clientMsj mess);
	mensaje createPlaneMovementMessageForClient(Client *client);
	mensaje createInitBackgroundMessage(Escenario *escenario);
	mensaje createBulletMessage(Object*);
	mensaje createBackgroundUpdateMessage(Escenario *escenario);
	mensaje createBackgroundElementUpdateMessageForElement(DrawableObject *element);
	mensaje createBackgroundElementCreationMessageForElement(DrawableObject *element);
	mensaje createWindowInitMessage(int screenHeight, int screenWidth);
	mensaje createResetGameMessage();
	mensaje createReconnectionMessageForClient(Client* client);
	mensaje createDisconnectionMessageForClient(Client* client);
	mensaje createUpdatePhotogramMessageForPlane(Avion* plane);
	mensaje createExplosionMessage(Explosion* explosion);
	mensaje createEnemyPlaneCreationMessage(EnemyPlane* enemyPlane);
	mensaje createBulletSoundMessage();
	mensaje createEnemyBulletCreationMessage(EnemyPlane* enemyPlane, int id);
	mensaje createLifeMessage(int id, int height, int width);
	mensaje createExplosionSoundMessage();
	menuResponseMessage createMenuMessage(vector<Team *> *teams);
	StatsTypeMessage createStatsTypeMessageCollaborationType(bool isCollaborative);
	CollaborationStatsMessage createCollaborationStatsMessage(ScoreManager *scoreManager, ClientList *clientList);
	TeamsStatsMessage createTeamsStatsMessage(ScoreManager *scoreManager, vector<Team *> *teams,ClientList* clients);

private:
	mensaje createBackgroundElementBaseMessageForElement(DrawableObject *element);
};

#endif /* MESSAGEBUILDER_H_ */
