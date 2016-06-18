/*
 * GameManager.cpp

 *
 *  Created on: Apr 26, 2016
 *      Author: luciano
 */

#include "GameManager.h"

#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <thread>
#include "LogWriter.h"
#include "Avion.h"
#include "Escenario.h"
#include <mutex>
#include "Client.h"
#include "Object.h"
#include "Constants.h"
#include "MenuPresenter.h"
#include <sys/socket.h>
#include "MessageBuilder.h"
#include <chrono>
#include "DrawableObject.h"
#include "PowerUp.h"
#include <ctime>
#include <unistd.h>
#include "EnemyPlane.h"
#include "Explosion.h"
#include "Formation.h"
#include "Score.h"
#include "ScoreManager.h"

#define kServerTestFile "serverTest.txt"

std::mutex mutexColaMensajes;
std::mutex writingInLogFileMutex;
std::mutex enemiesMutex;
std::mutex enemyBulletMutex;
std::mutex formationsMutex;
map<unsigned int, thread> clientEntranceMessages;
map<unsigned int, thread> keepAliveThreads;
BulletList objects;
list<EnemyPlane*> enemyPlanes;
list<Explosion*> explosions;
list<Formation*> formations;
list<Score*> scores;
ScoreManager* scoreManager;
bool appShouldTerminate, gameInitiated, userWasConnected;
bool aterrizaje, colaboration;
LogWriter *logWriter;
int numberOfCurrentAcceptedClients;
list<mensaje> drawableList; //lista que guarda todos los mensajes iniciales para levantar el juego :
							//datos de la ventana, escenario, aviones, elementos del escenario.


GameManager::GameManager() {
	this->appShouldTerminate = false;
	this->menuPresenter = NULL;
	this->parser = NULL;
	this->socketManager = NULL;
	this->xmlLoader = NULL;
	this->escenario = NULL;
	this->procesor = NULL;
	vector<Team *> *teams = new vector<Team *>;
	this->teams = teams;
}

Escenario* GameManager::getScenary(){
	return escenario;
}

void GameManager::reloadGameFromXml(){
	drawableList.clear();
	parser->reloadDoc();
	escenario->deleteElements();
	enemyPlanes.clear();
	formations.clear();
	explosions.clear();
	escenario->deletePowerUps();
	escenario->resetScrollingOffset();
	escenario->setStagesPositions(parser);
	mensaje ventanaMsj, escenarioMsj;
	memset(&ventanaMsj, 0, sizeof(mensaje));
	memset(&escenarioMsj, 0, sizeof(mensaje));
	parser->getFondoEscenario(*escenario);
	ventanaMsj.height = parser->getAltoVentana();
	ventanaMsj.width = parser->getAnchoVentana();
	escenario->setScrollingStep(1);
	escenario->setWindowHeight(ventanaMsj.height);
	escenario->setHeigth(parser->getAltoEscenario());
	this->procesor->setScreenHeight(parser->getAltoVentana());
	this->procesor->setScreenWidth(parser->getAnchoVentana());
	escenarioMsj = MessageBuilder().createInitBackgroundMessage(escenario);
	strcpy(ventanaMsj.action,"windowSize");
	this->broadcastMessage(ventanaMsj);
	this->broadcastMessage(escenarioMsj);
	drawableList.push_back(escenarioMsj);
	for(int i = 0; i < parser->getNumberOfElements(); i++){
		DrawableObject* object = new DrawableObject();
		mensaje elementMsg;
		memset(&elementMsg, 0, sizeof(mensaje));
		parser->getElement(*object, i);
		escenario->addElement(object);
		elementMsg = MessageBuilder().createBackgroundElementUpdateMessageForElement(object);
		strncpy(elementMsg.action, "create", 20);
		drawableList.push_back(elementMsg);
		this->broadcastMessage(elementMsg);
	}
	for(int i = 0; i < parser->getNumberOfFormations(); i++){
		Formation* formation = new Formation();
		parser->getFormation(formation, i);
		formations.push_back(formation);
	}
	for(int i = 0; i < parser->getNumberOfEnemyPlanes(); i++){
		EnemyPlane* enemyPlane = new EnemyPlane();
		parser->getEnemyPlane(enemyPlane, i, formations, parser->getAnchoEscenario());
		enemyPlanes.push_back(enemyPlane);
		mensaje enemyPlanesMsj = MessageBuilder().createEnemyPlaneCreationMessage(enemyPlane);
		drawableList.push_back(enemyPlanesMsj);
		this->broadcastMessage(enemyPlanesMsj);
	}
	for(int i = 0; i < parser->getNumberOfPowerUp(); i++){
		PowerUp* powerUp = new PowerUp();
		parser->getPowerUp(*powerUp, i);
		escenario->addPowerUp(powerUp);
		mensaje powerUpMsj = MessageBuilder().createBackgroundElementCreationMessageForElement(powerUp);
		drawableList.push_back(powerUpMsj);
		this->broadcastMessage(powerUpMsj);
	}
	scoreManager->resetScores();
	escenario->transformPositions();
	objects.setIdOfFirstBullet(this->parser->getFirstBulletId());
	mensaje avionMsj;
	int i = 1;
	list<Client*>::iterator it = clientList->clients.begin();
	for (; it != clientList->clients.end(); it++){
		(*it)->plane = parser->getAvion(i);
		(*it)->setAlive(true);
		(*it)->plane->setLifes(3);
		avionMsj = MessageBuilder().createInitialMessageForClient((*it));
		this->broadcastMessage(avionMsj);
		for(int i=1; i<4; i++){
			mensaje corazon = MessageBuilder().createLifeMessage(5000*i + (*it)->plane->getId(), procesor->getScreenHeight(), procesor->getScreenWidth());
			this->broadcastMessage(corazon);
		}
		i++;
	}
	mensaje sortPlaneMsg;
	strncpy(sortPlaneMsg.action, "sortPlane", kLongChar);
	this->broadcastMessage(sortPlaneMsg);
}

int sendMenuMessage(int socket, int bytesToSend, menuResponseMessage *message) {
	int bytesSent = 0;
	int res = 0;

	while (bytesSent < bytesToSend) {
		res = send(socket, &(message)[bytesSent], bytesToSend - bytesSent,
		MSG_WAITALL);
		if (res == 0) { //Se cerró la conexion. Escribir en log de errores de conexion.
			return 0;
		} else if (res < 0) { //Error en el envio del mensaje. Escribir en el log.
			return -1;
		} else {
			bytesSent += res;
		}
	}
	return bytesSent;
}

void GameManager::setTeamIdScores(int teamId, int planeId){
	list<Score*>::iterator it;
	for(it = scores.begin(); it != scores.end(); it++){
		if((*it)->getId() == planeId){
			if(!colaboration)
				(*it)->setClientTeamId(teamId);
			else
				(*it)->setClientTeamId(0);
		}
	}
}

int sendMsj(int socket, int bytesAEnviar, clientMsj* mensaje) {
	int enviados = 0;
	int res = 0;

	while (enviados < bytesAEnviar) {
		res = send(socket, &(mensaje)[enviados], bytesAEnviar - enviados,
		MSG_WAITALL);
		if (res == 0) { //Se cerró la conexion. Escribir en log de errores de conexion.
			return 0;
		} else if (res < 0) { //Error en el envio del mensaje. Escribir en el log.
			return -1;
		} else {
			enviados += res;
		}
	}

	return enviados;
}

int sendMsjInfo(int socket, int bytesAEnviar, mensaje* mensaje) {
	int enviados = 0;
	int res = 0;

	while (enviados < bytesAEnviar) {
		res = send(socket, &(mensaje)[enviados], bytesAEnviar - enviados,
		MSG_WAITALL);
		if (res == 0) { //Se cerró la conexion. Escribir en log de errores de conexion.
			return 0;
		} else if (res < 0) { //Error en el envio del mensaje. Escribir en el log.
			return -1;
		} else {
			enviados += res;
		}
	}
	return enviados;
}

int sendStatsTypeMessage(int socket, int bytesToSend, StatsTypeMessage *message) {
	int sentBytes = 0;
	int res = 0;

	while (sentBytes < bytesToSend) {
		res = send(socket, &(message)[sentBytes], bytesToSend - sentBytes,
		MSG_WAITALL);
		if (res == 0) { //Se cerró la conexion. Escribir en log de errores de conexion.
			return 0;
		} else if (res < 0) { //Error en el envio del mensaje. Escribir en el log.
			return -1;
		} else {
			sentBytes += res;
		}
	}
	return sentBytes;
}

int sendCollaborationStatsMessage(int socket, int bytesToSend, CollaborationStatsMessage *message) {
	int sentBytes = 0;
	int res = 0;

	while (sentBytes < bytesToSend) {
		res = send(socket, &(message)[sentBytes], bytesToSend - sentBytes,
				MSG_WAITALL);
		if (res == 0) { //Se cerró la conexion. Escribir en log de errores de conexion.
			return 0;
		} else if (res < 0) { //Error en el envio del mensaje. Escribir en el log.
			return -1;
		} else {
			sentBytes += res;
		}
	}
	return sentBytes;
}

int sendTeamStatsMessage(int socket, int bytesToSend, TeamsStatsMessage *message) {
	int sentBytes = 0;
	int res = 0;

	while (sentBytes < bytesToSend) {
		res = send(socket, &(message)[sentBytes], bytesToSend - sentBytes,
				MSG_WAITALL);
		if (res == 0) { //Se cerró la conexion. Escribir en log de errores de conexion.
			return 0;
		} else if (res < 0) { //Error en el envio del mensaje. Escribir en el log.
			return -1;
		} else {
			sentBytes += res;
		}
	}
	return sentBytes;
}

int readMsj(int socket, int bytesARecibir, clientMsj* mensaje) {
	int recibidos = 0;
	int totalBytesRecibidos = 0;
	while (totalBytesRecibidos < bytesARecibir) {
		recibidos = recv(socket, &mensaje[totalBytesRecibidos],
				bytesARecibir - totalBytesRecibidos, MSG_WAITALL);
		if (recibidos < 0) {
			shutdown(socket, SHUT_RDWR);
			return -1;
		} else if (recibidos == 0) { //se corto la conexion desde el lado del servidor.
			shutdown(socket, SHUT_RDWR);
			return -1;
		} else {
			totalBytesRecibidos += recibidos;
		}
	}
	return recibidos;
}

void readMenuMessage(int socket, int bytesToReceive, menuRequestMessage* message) {
	int currentBytesReceived = 0;
	int totalBytesReceived = 0;
	while (totalBytesReceived < bytesToReceive) {
		currentBytesReceived = recv(socket, &message[totalBytesReceived],
				bytesToReceive - totalBytesReceived, MSG_WAITALL);
		if (currentBytesReceived < 0) {
			shutdown(socket, SHUT_RDWR);
		} else if (currentBytesReceived == 0) { //se corto la conexion desde el lado del servidor.
			shutdown(socket, SHUT_RDWR);
		} else {
			totalBytesReceived += currentBytesReceived;
		}
	}
}

void broadcast(mensaje msg, ClientList* clientList){
	std::list<Client*>::iterator it;
	for (it = clientList->clients.begin(); it != clientList->clients.end(); ++it) {
		if((*it)->getConnnectionState()){
			sendMsjInfo((*it)->getSocketMessages(), sizeof(msg), &msg);
		}
	}
}

void broadcastStatsTypeMessage(StatsTypeMessage message, ClientList *clientList) {
	std::list<Client*>::iterator it;
	for (it = clientList->clients.begin(); it != clientList->clients.end(); ++it) {
		if((*it)->getConnnectionState()){
			sendStatsTypeMessage((*it)->getSocketMessages(), sizeof(message), &message);
		}
	}
}

void broadcastCollaborationStatsMessage(CollaborationStatsMessage message, ClientList *clientList) {
	for (Client *client : clientList->clients) {
		if(client->getConnnectionState()){
			sendCollaborationStatsMessage(client->getSocketMessages(), sizeof(message), &message);
		}
	}
}

void broadcastTeamStatsMessage(TeamsStatsMessage message, ClientList *clientList) {
	for (Client *client : clientList->clients) {
		if(client->getConnnectionState()){
			sendTeamStatsMessage(client->getSocketMessages(), sizeof(message), &message);
		}
	}
}

void broadcastMsj( ClientList *clientList, Procesador* processor, Escenario* escenario, vector<Team *> *teams, XmlParser *parser) {
	std::list<Client*>::iterator it;
	std::list<Object>::iterator objectIt;
	std::list<EnemyPlane*>::iterator enemyPlanesIt;
	std::list<Explosion*>::iterator explosionsIt;
	std::list<Score*>::iterator scoreIt;
	std::list<Formation*>::iterator formationIt;
	int contador = 0;
	int disparos = 0;
	int hit = -1;
	int bulletId = -1;
	bool alreadyCreatedScores = false;
	while(!appShouldTerminate){
		usleep(1000);
		objects.moveBullets();
		for(int i = 0; i < objects.bulletQuantity(); i++){
			mensaje msg;
			hit = objects.bulletMessage(i, msg, processor->getScreenWidth(), processor->getScreenHeight(), enemyPlanes, clientList);
			if(hit != -1){
				if(hit > clientList->clients.size()){
					bulletId = objects.getObject(i)->getClientId();
					for(enemyPlanesIt = enemyPlanes.begin(); enemyPlanesIt != enemyPlanes.end(); enemyPlanesIt++){
						if(hit == (*enemyPlanesIt)->getId()){
							if((*enemyPlanesIt)->getLifes() > 0){
								scoreManager->increaseScoreForHit(bulletId, (*enemyPlanesIt));
							}else{
								scoreManager->increaseDestroyScore(bulletId, (*enemyPlanesIt));
							}
						}
					}
				}else if(hit != -2){
					mensaje clientHit;
					//Aviso a los clientes que borren al avion-
					Client* client = clientList->getClientForPlaneId(hit);
					//Explosion* explosion = new Explosion(client->getPlane()->getPosX(), client->getPlane()->getPosY(), true, 15000 + explosions.size());
					//msj = MessageBuilder().createExplosionMessage(explosion);
					//broadcast(msj, clientList);
					cout << "USER HAS BEEN HIT" << endl;
					strcpy(clientHit.action, "path");
					clientHit.id = client->getPlane()->getId() + (5000*(client->getPlane()->getLifes() + 1));
					cout << "LIFE ID " << clientHit.id << endl;
					strcpy(clientHit.imagePath, "nheart.png");
					clientHit.width = 39;
					clientHit.height = 42;
					broadcast(clientHit, clientList);
					if(!client->isAlive()){
						strcpy(clientHit.action, "delete");
						clientHit.id = client->getPlane()->getId();
						broadcast(clientHit, clientList);
					}
				}else{

				}
			}
			if(strcmp(msg.action, "Bullet deleted") != 0)
				broadcast(msg, clientList);
		}
		objects.clearBullets();
		mensaje msj;
		if(gameInitiated){
			if(!colaboration && !alreadyCreatedScores){
				alreadyCreatedScores = true;
				if(!colaboration){
					mensaje teamScore;
					cout << "TEAM SCORE CREATION" << endl;
					for (unsigned int i = 0; i < teams->size(); i++) {
						strcpy(teamScore.action, "createTeamScore");
						teamScore.id = (*teams)[i]->teamID;
						strcpy(teamScore.imagePath, (*teams)[i]->teamName.c_str());
						broadcast(teamScore, clientList);
					}
				}

			}
			usleep(1000);
			escenario->update();
			aterrizaje = escenario->hayQueAterrizar();
			msj = MessageBuilder().createBackgroundUpdateMessage(escenario);
			broadcast(msj, clientList);
			for (int i = 0; i < escenario->getNumberElements(); i++){
				DrawableObject* element = escenario->getElement(i);
				msj = MessageBuilder().createBackgroundElementUpdateMessageForElement(element);
				broadcast(msj, clientList);
			}
			for(int i = 0; i < escenario->getNumberOfPowerUps(); i++){
				msj = MessageBuilder().createBackgroundElementUpdateMessageForElement(escenario->getPowerUp(i));
				broadcast(msj, clientList);
				for(it = clientList->clients.begin(); it != clientList->clients.end(); it++){
					if (escenario->getPowerUp(i) != NULL){
						if(escenario->getPowerUp(i)->haveCollision((*it)->getPlane())){
							escenario->getPowerUp(i)->applyPowerUp((*it)->getPlane(), scoreManager, enemyPlanes, processor);
							mensaje deletePowerUp;
							strncpy(deletePowerUp.action,"delete",20);
							deletePowerUp.id = escenario->getPowerUp(i)->getId();
							escenario->deletePowerUp(escenario->getPowerUp(i)->getId());
							broadcast(deletePowerUp, clientList);
						}
					}
				}
			}
			for(enemyPlanesIt = enemyPlanes.begin(); enemyPlanesIt != enemyPlanes.end(); enemyPlanesIt++){
				mensaje msj;
				(*enemyPlanesIt)->move();
				int clientId = (*enemyPlanesIt)->collideWithClient(clientList);
				if(clientId != -1){
					//Aviso a los clientes que borren al avion-
					Client* client = clientList->getClientForPlaneId(clientId);
					//Explosion* explosion = new Explosion(client->getPlane()->getPosX(), client->getPlane()->getPosY(), true, 15000 + explosions.size());
					//msj = MessageBuilder().createExplosionMessage(explosion);
					//broadcast(msj, clientList);
					client->setAlive(false);
					strcpy(msj.action, "delete");
					msj.id = clientId;
					broadcast(msj, clientList);
					(*enemyPlanesIt)->setLifes(0);
				}
				if((*enemyPlanesIt)->notVisible(processor->getScreenWidth(), processor->getScreenHeight())
						|| (*enemyPlanesIt)->getLifes() <= 0){
					//Aviso a los clientes que borren al avion enemigo y creo explosion
					mensaje msjExplosionSound;
					Explosion* explosion = new Explosion((*enemyPlanesIt)->getPosX(), (*enemyPlanesIt)->getPosY(), false,  objects.getLastId() + 1);
					objects.setLastId(objects.getLastId() + 1);
					msj = MessageBuilder().createExplosionMessage(explosion);
					msjExplosionSound = MessageBuilder().createExplosionSoundMessage();
					broadcast(msj, clientList);
					broadcast(msjExplosionSound, clientList);
					if((*enemyPlanesIt)->isBigPlane() && (*enemyPlanesIt)->getLifes() <= 0){
						PowerUp bonusPowerUp;
						bonusPowerUp.setPath((*enemyPlanesIt)->getPwUpPath());
						bonusPowerUp.setHeigth((*enemyPlanesIt)->getPwUpHeiht());
						bonusPowerUp.setWidth((*enemyPlanesIt)->getPwUpWidth());
						bonusPowerUp.setPointsToAdd((*enemyPlanesIt)->getPwUpPoints());
						bonusPowerUp.setId(objects.getLastId()+1);
						objects.setLastId(objects.getLastId()+1);
						bonusPowerUp.setPosX((*enemyPlanesIt)->getPosX());
						bonusPowerUp.setPosY((*enemyPlanesIt)->getPosY());
						bonusPowerUp.setType(3);//Tipo 3: suma puntos.
						mensaje pwUpCreation = MessageBuilder().createBackgroundElementCreationMessageForElement(&bonusPowerUp);
						broadcast(pwUpCreation, clientList);
						escenario->addPowerUp(&bonusPowerUp);
					}
					enemiesMutex.lock();
					explosions.push_back(explosion);
					strcpy(msj.action, "delete");
					msj.id = (*enemyPlanesIt)->getId();
					enemyPlanesIt = enemyPlanes.erase(enemyPlanesIt);
					enemiesMutex.unlock();
					//enemyPlanesIt--;
					hit = -1;
				}else{
					strcpy(msj.action, "draw");
					msj.id = (*enemyPlanesIt)->getId();
					msj.actualPhotogram = (*enemyPlanesIt)->getActualPhotogram();
					msj.posX = (*enemyPlanesIt)->getPosX();
					msj.posY = (*enemyPlanesIt)->getPosY();
					//Creo una bala si el avion esta visible y el random es 23
					if((*enemyPlanesIt)->isOnScreen(processor->getScreenWidth(), processor->getScreenHeight())){
						if(disparos == 1000 && !escenario->getPracticeMode()){
							int shoot = rand()%4;
							if(shoot == 0){
								if((*enemyPlanesIt)->isBigPlane()){
									for(int i=0; i<5; i++){
										cout << " CREO 5 BALAS " << endl;
										mensaje disparo;
										strcpy(disparo.action, "create");
										disparo = MessageBuilder().createEnemyBulletCreationMessage((*enemyPlanesIt), objects.getLastId() + 1);
										broadcast(disparo, clientList);
										processor->processBigPlaneBullet((*enemyPlanesIt),i);
									}
								}else{
									mensaje disparo;
									strcpy(disparo.action, "create");
									disparo = MessageBuilder().createEnemyBulletCreationMessage((*enemyPlanesIt), objects.getLastId() + 1);
									broadcast(disparo, clientList);
									processor->processEnemyBullet((*enemyPlanesIt));
								}
							}
						}
					}
				}

				broadcast(msj, clientList);
			}
			if(disparos == 1000){
				disparos = 0;
			}
			processor->gameManager->updateScoresTeam(scores);
			for(scoreIt = scores.begin(); scoreIt != scores.end(); scoreIt++){
				if((*scoreIt)->hasChanged()){
					strcpy(msj.action, "score");
					msj.id = (*scoreIt)->getClientTeamId();
					msj.actualPhotogram = (*scoreIt)->getId();
					//msj.posX = (*scoreIt)->getScoreXPosition(processor->getScreenWidth());
					//msj.posY = (*scoreIt)->getScoreYPosition(processor->getScreenHeight());
					//msj.photograms = (*scoreIt)->getScore();
					msj.activeState = colaboration;
					msj.width = (*scoreIt)->getScore();
					msj.photograms = processor->gameManager->getScoreTeamForClient((*scoreIt)->getClientTeamId());
					cout << "SCORE " << (*scoreIt)->getId() << ": " << (*scoreIt)->getScore() << endl;
					broadcast(msj, clientList);
					(*scoreIt)->updateLastScore();
				}
			}
			if(contador == 29){
				for(explosionsIt = explosions.begin(); explosionsIt != explosions.end(); explosionsIt++){
					if(!(*explosionsIt)->isFinished()){
						strcpy(msj.action, "draw");
						msj.id = (*explosionsIt)->getId();
						msj.actualPhotogram = (*explosionsIt)->getActualPhotogram();
						msj.posX = (*explosionsIt)->getPosX();
						msj.posY = (*explosionsIt)->getPosY();
						(*explosionsIt)->updateFrame();
					}else{
						strcpy(msj.action, "delete");
						msj.id = (*explosionsIt)->getId();
						explosions.erase(explosionsIt);
						explosionsIt--;
					}

					broadcast(msj, clientList);
				}
			}
			disparos++;
			for(formationIt = formations.begin(); formationIt != formations.end(); formationIt++){
				if((*formationIt)->isDestroyed()){
					formationsMutex.lock();
					formationIt = formations.erase(formationIt);
					formationsMutex.unlock();
					//formationIt--;
				}
			}
		}
		contador++;
		if (contador == 30){
			contador = 0;
			for(it = clientList->clients.begin(); it != clientList->clients.end(); it++){
				if((*it)->plane->updatePhotogram()){
					mensaje photogramMsg = MessageBuilder().createUpdatePhotogramMessageForPlane((*it)->plane);
					broadcast(photogramMsg, clientList);
				}
			}
		}
		if(aterrizaje){
			for(it = clientList->clients.begin(); it != clientList->clients.end(); it++){
				(*it)->plane->aterrizar(escenario->getPortaAvionesX());
				mensaje planeUpdate = MessageBuilder().createPlaneMovementMessageForClient((*it));
				broadcast(planeUpdate, clientList);
			}
		}
		if(escenario->stageHasChanged()){
			mensaje endStageMsj;
			strncpy(endStageMsj.action, "endStage", kLongChar);
			for(scoreIt = scores.begin(); scoreIt != scores.end(); scoreIt++){
				if((*scoreIt)->isConnected()){
					endStageMsj.photograms = (*scoreIt)->getScore(); //Envio puntaje
					endStageMsj.actualPhotogram = escenario->getCurrentStageNumber(); //Envio numero de etapa
					sendMsjInfo((*scoreIt)->getClientSocket(), sizeof(mensaje), &endStageMsj);
				}
			}
		}
		if(escenario->gameFinished()){
			mensaje finishMsj;
			strncpy(finishMsj.action,"close",kLongChar);
			broadcast(finishMsj,clientList);
			bool gameIsCollaborative = parser->gameIsColaborationType();
			StatsTypeMessage statsTypeMessage = MessageBuilder().createStatsTypeMessageCollaborationType(gameIsCollaborative);
			broadcastStatsTypeMessage(statsTypeMessage, clientList);
			if (gameIsCollaborative) {
				CollaborationStatsMessage collaborationStatsMessage = MessageBuilder().createCollaborationStatsMessage(scoreManager, clientList);
				broadcastCollaborationStatsMessage(collaborationStatsMessage, clientList);
			} else {
				TeamsStatsMessage teamsStatsMessage = MessageBuilder().createTeamsStatsMessage(scoreManager, teams);
				broadcastTeamStatsMessage(teamsStatsMessage, clientList);
			}
			appShouldTerminate = true;
		}
	}
}


void sendGameInfo(ClientList* clientList){
	list<mensaje>::iterator it;
	list<Client*>::iterator clientIt;
	for (it = drawableList.begin(); it != drawableList.end(); it++){
		broadcast((*it), clientList);
	}
	mensaje sortPlaneMsg;
	strncpy(sortPlaneMsg.action, "sortPlane", kLongChar);
	broadcast(sortPlaneMsg, clientList);
	mensaje teamIdMsg;
	strncpy(teamIdMsg.action, "teamId", kLongChar);
	for(clientIt = clientList->clients.begin(); clientIt != clientList->clients.end(); clientIt++){
		teamIdMsg.id = (*clientIt)->getTeamId();
		sendMsjInfo((*clientIt)->getSocketMessages(), sizeof(mensaje), &teamIdMsg);
	}

}

void disconnectClientForSocketConnection(unsigned int socketConnection, ClientList *clientList) {
	Client* clientDisconnect = clientList->getClientForSocket(socketConnection);
	clientDisconnect -> setConnected(false);
	list<Score*>::iterator scoreIt;
	map<unsigned int, thread>::iterator threadItr;

	for(threadItr = clientEntranceMessages.begin(); threadItr != clientEntranceMessages.end(); ++threadItr){
		if(threadItr->first == socketConnection)
			threadItr->second.detach();
	}

	for(threadItr = keepAliveThreads.begin(); threadItr != keepAliveThreads.end(); ++threadItr){
		if(threadItr->first == socketConnection)
			threadItr->second.detach();
	}
	for(scoreIt = scores.begin(); scoreIt != scores.end(); scoreIt++){
		if((*scoreIt)->getClientSocket() == socketConnection){
			(*scoreIt)->setConnected(false);
		}
	}
}

void receiveClientMenuMessage (Client *client, Procesador *procesor) {
	menuRequestMessage message;
	readMenuMessage(client->getSocketMessages(), sizeof(message), &message);
	procesor->processMenuMessageForClient(message, client);
}

void *clientReader(Client *client, ClientList *clientList, Procesador *procesor, Escenario* escenario) {
	int res = 0;
	bool clientHasDisconnected = false;

	if (client->isFirstTimeConnected && !colaboration)
		receiveClientMenuMessage(client, procesor);

	while (!appShouldTerminate && !clientHasDisconnected) {
		clientMsj message;
		res = readMsj(client->getSocketMessages(), sizeof(message), &message);
		if (res < 0) {
			shutdown(client->getSocketMessages(), SHUT_RDWR);
			clientHasDisconnected = true;
			disconnectClientForSocketConnection(client->getSocketMessages(), clientList);
			mensaje disconnection = MessageBuilder().createDisconnectionMessageForClient(client);
			broadcast(disconnection, clientList);
		} else if(!aterrizaje) {
			procesor->processMessage(message);
		}
	}
	pthread_exit(NULL);
}

void* waitForClientConnection(int maxNumberOfClients, int socketHandle, XmlParser *parser, ClientList *clientList, Procesador *procesor, Escenario* escenario, vector<Team *> *teams) {
	while (!appShouldTerminate) {
		logWriter->writeWaitingForClientConnection();
		int socketConnection = accept(socketHandle, NULL, NULL);
		logWriter->writeClientConnectionReceived();
		mensaje ventanaMsj;
		ventanaMsj.height = parser->getAltoVentana();
		ventanaMsj.width = parser->getAnchoVentana();
		sendMsjInfo(socketConnection, sizeof(mensaje), &ventanaMsj);

		struct timeval timeOut;
		timeOut.tv_sec = 100;
		timeOut.tv_usec = 0;
		setsockopt(socketHandle, SOL_SOCKET, SO_RCVTIMEO, &timeOut, sizeof(struct timeval));

		clientMsj message;
		mensaje escenarioMsj;
		//Leo el mensaje de conexion
		readMsj(socketConnection, sizeof(message), &message);
		userWasConnected = false;
		if (clientList->checkIfUserNameIsAlreadyInUse(message.value)) {

			if (numberOfCurrentAcceptedClients >= maxNumberOfClients) {
				message = MessageBuilder().createServerFullMessage();
				sendMsj(socketConnection, sizeof(message), &(message));

			} else {
				numberOfCurrentAcceptedClients++;
				//Creo el cliente con el nombre del mensaje y lo agrego a la lista
				string name(message.value);
				Avion *clientPlane = parser->getAvion(numberOfCurrentAcceptedClients);
				Client* client = new Client(name, socketConnection, 0, clientPlane);
				client->isFirstTimeConnected = true;

				clientList->addClient(client);


				clientEntranceMessages[socketConnection] = std::thread(
						clientReader, client, clientList, procesor, escenario);
				mensaje mensajeAvion = MessageBuilder().createInitialMessageForClient(client);
				drawableList.push_back(mensajeAvion);

				//Creo puntaje para el cliente
				Score* playerScore = new Score();
				playerScore->setId(client->getPlane()->getId());
				playerScore->setScore(0);
				playerScore->setClientSocket(socketConnection);
				scores.push_back(playerScore);

				for(int i=1; i<4; i++){
					mensaje corazon = MessageBuilder().createLifeMessage(5000*i + clientPlane->getId(), procesor->getScreenHeight(), procesor->getScreenWidth());
					drawableList.push_back(corazon);
				}
				if(colaboration){
					mensaje mensajeScore = MessageBuilder().createInitialScoreMessage(playerScore, playerScore->getScoreXPosition(procesor->getScreenWidth()), playerScore->getScoreYPosition(procesor->getScreenHeight()), message);
					drawableList.push_back(mensajeScore);
				}

				message = MessageBuilder().createSuccessfullyConnectedMessageForClient(client, colaboration);

				sendMsj(socketConnection, sizeof(message), &(message));

				if(!colaboration){
					menuResponseMessage menuMessage = MessageBuilder().createMenuMessage(teams);
					sendMenuMessage(socketConnection, sizeof(menuMessage), &menuMessage);

				}else{
					procesor->addClientToColaborationTeam(client);
					client->setTeamId(0);//id del equipo de colaboracion.
					procesor->gameManager->setTeamIdScores(0, client->plane->getId());
				}
			}
		} else {
			Client* client = clientList->getClientForName(message.value);
			if (client->getConnnectionState()) {
				logWriter->writeUserNameAlreadyInUse(message.value);
				message = MessageBuilder().createUserNameAlreadyInUseMessage();
				sendMsj(socketConnection, sizeof(message), &(message));

			} else {
				mensaje reconnection = MessageBuilder().createReconnectionMessageForClient(client);
				broadcast(reconnection, clientList);

				logWriter->writeResumeGameForUserName(message.value);
				client->setSocketMessages(socketConnection);
				client->setConnected(true);
				client->isFirstTimeConnected = false;

				message = MessageBuilder().createSuccessfullyReconnectedMessageForClient(client, colaboration);
				escenarioMsj = MessageBuilder().createInitBackgroundMessage(escenario);
				clientEntranceMessages[socketConnection] = std::thread(clientReader, client, clientList, procesor, escenario);
				userWasConnected = true;
				sendMsj(socketConnection, sizeof(message), &(message));

			}
		}

		if(userWasConnected){
			mensaje mensajeObjeto;
			sendMsjInfo(socketConnection, sizeof(mensaje), &escenarioMsj);
			std::list<Client*>::iterator it;
			for (it = clientList->clients.begin(); it != clientList->clients.end(); ++it) {
				mensajeObjeto = MessageBuilder().createInitialMessageForClient((*it));
				sendMsjInfo(socketConnection, sizeof(mensaje), &mensajeObjeto);
			}
			for(int i = 0; i < parser->getNumberOfElements(); i++){
				DrawableObject object;
				mensaje elementMsg;
				parser->getElement(object, i);
				elementMsg = MessageBuilder().createBackgroundElementUpdateMessageForElement(&object);
				strncpy(elementMsg.action, "create", 20);
				sendMsjInfo(socketConnection, sizeof(mensaje), &elementMsg);
			}
			for (int i = 0; i < parser->getNumberOfPowerUp(); i++){
				PowerUp powerUp;
				mensaje powerUpMsg;
				parser->getPowerUp(powerUp, i);
				powerUpMsg = MessageBuilder().createBackgroundElementCreationMessageForElement(&powerUp);
				sendMsjInfo(socketConnection, sizeof(mensaje), &powerUpMsg);
			}
			mensaje sortPlaneMsg;
			strncpy(sortPlaneMsg.action, "sortPlane", kLongChar);
			sendMsjInfo(socketConnection, sizeof(mensaje), &sortPlaneMsg);
		}
	}
	pthread_exit(NULL);
}

int GameManager::initGameWithArguments(int argc, char* argv[]) {
	this->menuPresenter = new MenuPresenter(this->appShouldTerminate, this);
	this->clientList = new ClientList();
	colaboration = false;
	const char* fileName;
	logWriter = new LogWriter;
	this->xmlLoader = new XMLLoader(logWriter);
	numberOfCurrentAcceptedClients = 0;
	if (argc != 2) {
		fileName = kServerTestFile;
		logWriter->writeUserDidnotEnterFileName();
	} else {
		fileName = argv[1];
		if (!this->xmlLoader->serverXMLIsValid(fileName)) {
			fileName = kServerTestFile;
		}
	}

	this->parser = new XmlParser(fileName);
	logWriter->setLogLevel(this->parser->getLogLevel());
	this->socketManager = new SocketManager(logWriter, this->parser);
	if(parser->gameIsColaborationType()){
		colaboration = true;
		this->createColaborationTeam("team 1");
	}
	int maxNumberOfClients = this->parser->getMaxNumberOfClients(colaboration);
	int screenHeight = this->parser->getAltoVentana();
	int screenWidth = this->parser->getAnchoVentana();

	this->procesor = new Procesador(this->clientList, screenWidth, screenHeight, this);
	gameInitiated = false;
	int success = this->socketManager->createSocketConnection();
	if (success == EXIT_FAILURE)
		return EXIT_FAILURE;

	mensaje escenarioMsj;
	memset(&escenarioMsj, 0, sizeof(mensaje));
	Escenario escenario;
	parser->getFondoEscenario(escenario);
	this->escenario = &escenario;

	escenario.setScrollingStep(1);
	escenario.setWindowHeight(parser->getAnchoVentana());
	escenario.setHeigth(parser->getAltoEscenario());
	escenarioMsj = MessageBuilder().createInitBackgroundMessage(&escenario);
	drawableList.push_back(escenarioMsj);
	for(int i = 0; i < parser->getNumberOfElements(); i++){
		DrawableObject* object = new DrawableObject();
		mensaje elementMsg;
		memset(&elementMsg, 0, sizeof(mensaje));
		parser->getElement(*object, i);
		escenario.addElement(object);
		elementMsg = MessageBuilder().createBackgroundElementCreationMessageForElement(object);
		drawableList.push_back(elementMsg);
	}
	for(int i = 0; i < parser->getNumberOfPowerUp(); i++){
		PowerUp* powerUp = new PowerUp();
		parser->getPowerUp(*powerUp, i);
		escenario.addPowerUp(powerUp);
		mensaje powerUpMsj = MessageBuilder().createBackgroundElementCreationMessageForElement(powerUp);
		drawableList.push_back(powerUpMsj);
	}
	for(int i = 0; i < parser->getNumberOfFormations(); i++){
		Formation* formation = new Formation();
		parser->getFormation(formation, i);
		formations.push_back(formation);
	}
	for(int i = 0; i < parser->getNumberOfEnemyPlanes(); i++){
		EnemyPlane* enemyPlane = new EnemyPlane();
		parser->getEnemyPlane(enemyPlane, i, formations, parser->getAnchoEscenario());
		enemyPlanes.push_back(enemyPlane);
		mensaje enemyPlanesMsj = MessageBuilder().createEnemyPlaneCreationMessage(enemyPlane);
		drawableList.push_back(enemyPlanesMsj);
	}
	escenario.setPracticeMode(parser->startWithPracticeMode());
	escenario.transformPositions();
	aterrizaje = false;
	escenario.setPosPortaAviones(parser->getPosXPortaAviones(),parser->getPosYPortaAviones());
	escenario.setStagesPositions(parser);
	objects.setIdOfFirstBullet(this->parser->getFirstBulletId());
	std::thread broadcastThread(broadcastMsj,clientList, this->procesor, this->escenario, this->teams, this->parser);
	std::thread clientConnectionWaiter(waitForClientConnection,
			maxNumberOfClients, this->socketManager->socketHandle, this->parser, this->clientList, this->procesor, this->escenario, this->teams);
	this->menuPresenter->presentMenu();

	clientConnectionWaiter.detach();
	broadcastThread.detach();
	//enemyPlanesThread.detach();
	return EXIT_SUCCESS;
}

void GameManager::userDidChooseExitoption() {
	logWriter->writeUserDidFinishTheApp();
	this->appShouldTerminate = true;
	appShouldTerminate = true;
}

void GameManager::broadcastMessage(mensaje message) {
	broadcast(message, clientList);
}

void GameManager::restartGame() {
	escenario->restart();
}

void GameManager::sendInitialGameInfo() {
	//Se envia la info para levantar el juego recien cuando todos se conectaron.
	if(numberOfCurrentAcceptedClients == this->parser->getMaxNumberOfClients(colaboration) && !gameInitiated){
		sendGameInfo(this->clientList);
		gameInitiated = true;
		scoreManager = new ScoreManager();
		scoreManager->setScores(scores);
	}
}

void GameManager::addClientToTeamWithName(Client *client, string teamName) {
	for (unsigned int i = 0; i < this->teams->size(); i++) {
		if (strcmp((*this->teams)[i]->teamName.c_str(), teamName.c_str()) == 0) {
			(*this->teams)[i]->clients.push_back(client);
			client->setTeamId((*this->teams)[i]->getTeamId());
			this->setTeamIdScores((*this->teams)[i]->getTeamId(), client->plane->getId());
		}
	}

	this->sendInitialGameInfo();
}

void GameManager::createTeamWithNameForClient(string teamName, Client *client) {
	int maxNumberOfPlayersPerTeam = this->parser->getMaxNumberOfPlayerPerTeam();
	Team *newTeam = new Team(this->teams->size(), teamName, maxNumberOfPlayersPerTeam);
	client->setTeamId(this->teams->size());
	this->setTeamIdScores(this->teams->size(), client->plane->getId());
	newTeam->clients.push_back(client);
	this->teams->push_back(newTeam);

	this->sendInitialGameInfo();
}

void GameManager::createColaborationTeam(string teamName){
	int maxNumberOfPlayersPerTeam = this->parser->getMaxNumberOfPlayerPerTeam();
	Team *newTeam = new Team(this->teams->size(), teamName, maxNumberOfPlayersPerTeam);
	this->teams->push_back(newTeam);
}

void GameManager::updateScoresTeam(list<Score*> scores){
	list<Score*>::iterator it;
	for(it = scores.begin(); it != scores.end(); it++){
		if((*it)->hasChanged()){
			for (unsigned int i = 0; i < this->teams->size(); i++){
				if((*this->teams)[i]->isClientOfThisTeam((*it)->getClientTeamId())){
					(*this->teams)[i]->addPoints((*it)->getLastScoreDifference());
				}
			}
		}
	}
}

int GameManager::getScoreTeamForClient(int clientTeamId){
	for (unsigned int i = 0; i < this->teams->size(); i++){
		if((*this->teams)[i]->isClientOfThisTeam(clientTeamId)){
			return((*this->teams)[i]->getPoints());
		}
	}
	return 0;
}

void GameManager::createEnemyBullet(EnemyPlane* enemyPlane){
	Object bullet;
	bullet.setId(objects.getLastId() + 1);
	bullet.setPosX(enemyPlane->getPosX() + enemyPlane->getWidth()/2);
	bullet.setPosY(enemyPlane->getPosY() + enemyPlane->getWidth());
	bullet.setWidth(12);
	bullet.setHeigth(12);
	bullet.setActualPhotogram(1);
	bullet.setPath("enemybullet.png");
	bullet.setStatus(true);
	bullet.setStep(1);
	bullet.setFacingDirection(enemyPlane->getFacingDirection());
	bullet.setEnemyBullet(true);
	objects.addElement(bullet);
}

void GameManager::createBigPlaneBullet(EnemyPlane* enemyPlane, int i){
	Object bullet;
	bullet.setId(objects.getLastId() + 1);
	bullet.setPosX(enemyPlane->getPosX() + enemyPlane->getWidth()/2);
	bullet.setPosY(enemyPlane->getPosY() + enemyPlane->getWidth());
	bullet.setWidth(12);
	bullet.setHeigth(12);
	bullet.setActualPhotogram(1);
	bullet.setPath("enemybullet.png");
	bullet.setStatus(true);
	bullet.setStep(1);
	bullet.setFD(i+2);
	bullet.setEnemyBullet(true);
	objects.addElement(bullet);
}

Object GameManager::createBulletForClient(Client* client, int posX){
	//Aca hay que setear. Si el cliente tiene disparo doble, se crean dos balas.
	Object bullet;
	bullet.setId(objects.getLastId() + 1);
	bullet.setClientId(client->getPlane()->getId());
	bullet.setPath("playerbullet.png");
	//bullet.setPosX(client->plane->getPosX() + client->plane->getWidth()/2 - 15);
	bullet.setPosX(posX);
	bullet.setPosY(client->plane->getPosY() + 1);
	bullet.setWidth(9);
	bullet.setHeigth(20);
	bullet.setStatus(true);
	bullet.setStep((client->plane->getVelDisparo() + client->plane->getVelDesplazamiento())/5);
	bullet.setEnemyBullet(false);
	objects.addElement(bullet);
	return bullet;
}

void GameManager::detachClientMessagesThreads() {
	//Creo un iterador para hacer detach en los threads abiertos antes de cerrar el server
	map<unsigned int, thread>::iterator threadItr;

	//Recorro ambos mapas y hago detach.
	for (threadItr = clientEntranceMessages.begin();
			threadItr != clientEntranceMessages.end(); ++threadItr) {
		threadItr->second.detach();
	}
	for (threadItr = keepAliveThreads.begin();
			threadItr != keepAliveThreads.end(); ++threadItr) {
		threadItr->second.detach();
	}
}

GameManager::~GameManager() {
	delete this->procesor;
	delete this->clientList;
	detachClientMessagesThreads();
	delete this->menuPresenter;
	delete this->parser;
	delete this->xmlLoader;
	delete this->socketManager;
	delete this->escenario;
	delete logWriter;
}

