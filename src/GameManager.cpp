/*
 * GameManager.cpp

 *
 *  Created on: Apr 26, 2016
 *      Author: luciano
 */

#include "GameManager.h"

#include <strings.h>
#include <stdio.h>
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
#include <ctime>
#include <unistd.h>

#define kServerTestFile "serverTest.txt"

std::mutex mutexColaMensajes;
std::mutex writingInLogFileMutex;
map<unsigned int, thread> clientEntranceMessages;
map<unsigned int, thread> keepAliveThreads;
BulletList objects;
bool appShouldTerminate, gameInitiated, userWasConnected;
LogWriter *logWriter;
int numberOfCurrentAcceptedClients;
list<mensaje> messageList;
list<mensaje> drawableList; //lista que guarda todos los mensajes iniciales para levantar el juego :
							//datos de la ventana, escenario, aviones, elementos del escenario.


GameManager::GameManager() {
	this->appShouldTerminate = false;
	this->menuPresenter = NULL;
	this->parser = NULL;
	this->socketManager = NULL;
	this->xmlLoader = NULL;
	this->escenario = NULL;
	vector<Team *> *teams = new vector<Team *>;
	this->teams = teams;
}

void GameManager::reloadGameFromXml(){
	drawableList.clear();
	parser->reloadDoc();
	escenario->deleteElements();
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
	escenario->transformPositions();
	objects.setIdOfFirstBullet(parser->getMaxNumberOfClients() + escenario->getNumberElements());
	mensaje avionMsj;
	int i = 1;
	list<Client*>::iterator it = clientList->clients.begin();
	for (; it != clientList->clients.end(); it++){
		(*it)->plane = parser->getAvion(i);
		avionMsj = MessageBuilder().createInitialMessageForClient((*it));
		this->broadcastMessage(avionMsj);
		i++;
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


void broadcast(mensaje msg, ClientList* clientList){
	std::list<Client*>::iterator it;
	for (it = clientList->clients.begin(); it != clientList->clients.end(); ++it) {
		if((*it)->getConnnectionState()){
			sendMsjInfo((*it)->getSocketMessages(), sizeof(msg), &msg);
		}
	}
}

void broadcastMsj( ClientList *clientList, Procesador* processor, Escenario* escenario) {
	std::list<Client*>::iterator it;
	std::list<Object>::iterator objectIt;
	int contador = 0;
	while(!appShouldTerminate){
		usleep(1000);
		objects.moveBullets();
		for(int i = 0; i < objects.bulletQuantity(); i++){
			mensaje msg;
			objects.bulletMessage(i, msg, processor->getScreenWidth(), processor->getScreenHeight());
			if(strcmp(msg.action, "Bullet deleted") != 0)
				broadcast(msg, clientList);
		}
		mensaje msj;
		if(gameInitiated){
			usleep(1000);
			escenario->update();
			msj = MessageBuilder().createBackgroundUpdateMessage(escenario);
			broadcast(msj, clientList);
			for (int i = 0; i < escenario->getNumberElements(); i++){
				DrawableObject* element = escenario->getElement(i);
				msj = MessageBuilder().createBackgroundElementUpdateMessageForElement(element);
				broadcast(msj, clientList);
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
	}
}


void sendGameInfo(ClientList* clientList){
	list<mensaje>::iterator it;
	for (it = drawableList.begin(); it != drawableList.end(); it++){
		broadcast((*it), clientList);
	}
}

void disconnectClientForSocketConnection(unsigned int socketConnection, ClientList *clientList) {
	Client* clientDisconnect = clientList->getClientForSocket(socketConnection);
	clientDisconnect -> setConnected(false);
	map<unsigned int, thread>::iterator threadItr;

	for(threadItr = clientEntranceMessages.begin(); threadItr != clientEntranceMessages.end(); ++threadItr){
		if(threadItr->first == socketConnection)
			threadItr->second.detach();
	}

	for(threadItr = keepAliveThreads.begin(); threadItr != keepAliveThreads.end(); ++threadItr){
		if(threadItr->first == socketConnection)
			threadItr->second.detach();
	}
}

void sendClientMenuMessage (int socketConnection, vector<Team *> *teams ) {
	menuResponseMessage message = MessageBuilder().createMenuMessage(teams);
	sendMenuMessage(socketConnection, sizeof(message), &message);
}

void receiveClientMenuMessage (Client *client, Procesador *procesor) {
	menuRequestMessage message;
	readMenuMessage(client->getSocketMessages(), sizeof(message), &message);
	procesor->processMenuMessageForClient(message, client);
}

void *clientReader(Client *client, ClientList *clientList, Procesador *procesor, Escenario* escenario, XmlParser *parser, vector<Team *> *teams) {
	int res = 0;
	bool clientHasDisconnected = false;
	if (client->isFirstTimeLogin)
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
		} else {
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
				client->isFirstTimeLogin = true;
				client->currentNumberOfLifes = parser->getNumberOfLifesPerClient();

				clientList->addClient(client);

				message = MessageBuilder().createSuccessfullyConnectedMessageForClient(client);

				clientEntranceMessages[socketConnection] = std::thread(
						clientReader, client, clientList, procesor, escenario, parser, teams);
				mensaje mensajeAvion = MessageBuilder().createInitialMessageForClient(client);
				drawableList.push_back(mensajeAvion);
				vector<LifeObject *> lifeObjects = parser->getLifeObjects();

				vector<mensaje> messages = MessageBuilder().createLifeObjectMessagesForLifeObjects(lifeObjects);

				for (mensaje message : messages) {
					drawableList.push_back(message);
				}

				for (LifeObject *lifeObject : lifeObjects) {
					delete lifeObject;
				}

				sendMsj(socketConnection, sizeof(message), &(message));
				sendClientMenuMessage(socketConnection, teams);

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
				client->isFirstTimeLogin = false;

				message = MessageBuilder().createSuccessfullyReconnectedMessageForClient(client);
				escenarioMsj = MessageBuilder().createInitBackgroundMessage(escenario);
				clientEntranceMessages[socketConnection] = std::thread(clientReader, client, clientList, procesor, escenario, parser, teams);
				userWasConnected = true;
				sendMsj(socketConnection, sizeof(message), &(message));

			}
		}

		if(userWasConnected){
			mensaje ventanaMsj, mensajeObjeto;
			ventanaMsj.height = parser->getAltoVentana();
			ventanaMsj.width = parser->getAnchoVentana();
			sendMsjInfo(socketConnection, sizeof(mensaje), &ventanaMsj);
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
		}
	}
	pthread_exit(NULL);
}

int GameManager::initGameWithArguments(int argc, char* argv[]) {
	this->menuPresenter = new MenuPresenter(this->appShouldTerminate, this);
	this->clientList = new ClientList();

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

	int maxNumberOfClients = this->parser->getMaxNumberOfClients();
	int screenHeight = this->parser->getAltoVentana();
	int screenWidth = this->parser->getAnchoVentana();

	this->procesor = new Procesador(this->clientList, screenWidth, screenHeight, this);
	gameInitiated = false;
	int success = this->socketManager->createSocketConnection();
	if (success == EXIT_FAILURE)
		return EXIT_FAILURE;

	mensaje ventanaMsj, escenarioMsj;
	memset(&ventanaMsj, 0, sizeof(mensaje));
	memset(&escenarioMsj, 0, sizeof(mensaje));
	Escenario escenario;
	parser->getFondoEscenario(escenario);
	this->escenario = &escenario;
	ventanaMsj.height = parser->getAltoVentana();;
	ventanaMsj.width = parser->getAnchoVentana();
	escenario.setScrollingStep(1);
	escenario.setWindowHeight(ventanaMsj.height);
	escenario.setHeigth(parser->getAltoEscenario());
	escenarioMsj = MessageBuilder().createInitBackgroundMessage(&escenario);
	drawableList.push_back(ventanaMsj);
	drawableList.push_back(escenarioMsj);
	for(int i = 0; i < parser->getNumberOfElements(); i++){
		DrawableObject* object = new DrawableObject();
		mensaje elementMsg;
		memset(&elementMsg, 0, sizeof(mensaje));
		parser->getElement(*object, i);
		escenario.addElement(object);
		elementMsg = MessageBuilder().createBackgroundElementUpdateMessageForElement(object);
		strncpy(elementMsg.action, "create", 20);
		drawableList.push_back(elementMsg);
	}
	escenario.transformPositions();
	objects.setIdOfFirstBullet(maxNumberOfClients + escenario.getNumberElements());

	std::thread broadcastThread(broadcastMsj,clientList, this->procesor, this->escenario);
	std::thread clientConnectionWaiter(waitForClientConnection,
			maxNumberOfClients, this->socketManager->socketHandle, this->parser, this->clientList, this->procesor, this->escenario, this->teams);

	this->menuPresenter->presentMenu();

	clientConnectionWaiter.detach();
	broadcastThread.detach();
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
	if(numberOfCurrentAcceptedClients == this->parser->getMaxNumberOfClients() && !gameInitiated) {
		sendGameInfo(clientList);
		gameInitiated = true;
	}
}

void GameManager::addClientToTeamWithName(Client *client, string teamName) {
	for (Team *team : *this->teams) {
		if (strcmp(team->teamName.c_str(), teamName.c_str()) == 0) {
			team->clients.push_back(client);
		}
	}

	this->sendInitialGameInfo();
}

void GameManager::createTeamWithNameForClient(string teamName, Client *client) {
	int maxNumberOfPlayersPerTeam = this->parser->getMaxNumberOfPlayerPerTeam();
	Team *newTeam = new Team(this->teams->size(), teamName, maxNumberOfPlayersPerTeam);
	newTeam->clients.push_back(client);
	this->teams->push_back(newTeam);

	this->sendInitialGameInfo();
}

Object* GameManager::createBulletForClient(Client* client){
	Object bullet;
	bullet.setId(objects.getLastId() + 1);
	bullet.setPath("bullet.png");
	bullet.setPosX(client->plane->getPosX() + client->plane->getWidth()/2 - 15);
	bullet.setPosY(client->plane->getPosY() + 1);
	bullet.setWidth(30);
	bullet.setHeigth(30);
	bullet.setStatus(true);
	//La velocidad de disparo es relativa a la velocidad del avion.
	bullet.setStep(client->plane->getVelDisparo() + client->plane->getVelDesplazamiento());
	objects.addElement(bullet);
	return &bullet;
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
	for (Team *team : *this->teams) {
	    delete team;
	}
	delete this->teams;
	delete logWriter;
}

