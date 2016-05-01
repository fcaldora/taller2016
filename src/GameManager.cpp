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
#include <unistd.h>
#include "Procesador.h"


#define kServerTestFile "serverTest.txt"

std::mutex mutexColaMensajes;
std::mutex writingInLogFileMutex;
map<unsigned int, thread> clientEntranceMessages;
map<unsigned int, thread> clientExitMessages;
list<Bullet*> bullets;
int bulletsCount = 0;
bool appShouldTerminate, gameInitiated;
LogWriter *logWriter;
int numberOfCurrentAcceptedClients;
list<mensaje> messageList;
list<mensaje*> drawableList; //lista que guarda todos los mensajes iniciales para levantar el juego :
							//datos de la ventana, escenario, aviones, elementos del escenario.


GameManager::GameManager() {
	this->appShouldTerminate = false;
	this->menuPresenter = NULL;
	this->parser = NULL;
	this->socketManager = NULL;
	this->xmlLoader = NULL;
	this->scenery = NULL;
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
	return 1;
}


void broadcast(mensaje *msg, ClientList* clientList){
	std::list<Client*>::iterator it;
	for (it = clientList->clients.begin(); it != clientList->clients.end(); ++it) {
		sendMsjInfo((*it)->getSocketMessages(), sizeof(msg), msg);
	}
}

void broadcastMsj( ClientList *clientList, Procesador* processor, Escenario* escenario) {
	std::list<Client*>::iterator it;
	std::list<Bullet*>::iterator objectIt;
	while(!appShouldTerminate){
		usleep(1000);
		for(objectIt = bullets.begin(); objectIt != bullets.end(); objectIt++){
			if((*objectIt)->isStatic()){
				(*objectIt)->move();
				mensaje msg;
				if((*objectIt)->notVisible(processor->getScreenWidth(), processor->getScreenHeight())){
					strcpy(msg.action, "delete");
					msg.id = (*objectIt)->getId();
					bullets.erase(objectIt);
					objectIt--;
				}else{
					strcpy(msg.action, "draw");
					msg.id = (*objectIt)->getId();
					msg.posX = (*objectIt)->getPosX();
					msg.posY = (*objectIt)->getPosY();
				}
				for (it = clientList->clients.begin(); it != clientList->clients.end(); ++it) {
					sendMsjInfo((*it)->getSocketMessages(), sizeof(msg), &msg);
				}
			}
		}
		mensaje *msj = new mensaje;
		if(gameInitiated){
			usleep(1000);
			escenario->update();
			msj = MessageBuilder().createBackgroundUpdateMessage(escenario);
			broadcast(msj, clientList);
			for (int i = 0; i < escenario->getNumberElements(); i++){
				msj = MessageBuilder().createBackgroundElementUpdateMessage(escenario, i);
				broadcast(msj, clientList);
			}
		}		
	}
}

void sendGameInfo(ClientList* clientList){
	list<mensaje*>::iterator it;
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

	for(threadItr = clientExitMessages.begin(); threadItr != clientExitMessages.end(); ++threadItr){
		if(threadItr->first == socketConnection)
			threadItr->second.detach();
	}
}

void *clientReader(int socketConnection, ClientList *clientList, Procesador *procesor) {
	int res = 0;
	bool clientHasDisconnected = false;
	while (!appShouldTerminate && !clientHasDisconnected) {
		clientMsj message;
		res = readMsj(socketConnection, sizeof(message), &message);
		if (res < 0) {

			shutdown(socketConnection, SHUT_RDWR);
			clientHasDisconnected = true;
			disconnectClientForSocketConnection(socketConnection, clientList);

		} else {
			procesor->processMessage(message);
		}
	}
	pthread_exit(NULL);
}

void* waitForClientConnection(int maxNumberOfClients, int socketHandle, XmlParser *parser, ClientList *clientList, Procesador *procesor) {
	while (!appShouldTerminate) {
		logWriter->writeWaitingForClientConnection();
		int socketConnection = accept(socketHandle, NULL, NULL);
		logWriter->writeClientConnectionReceived();

		clientMsj message;
		//Leo el mensaje de conexion
		readMsj(socketConnection, sizeof(message), &message);
		mensaje* mensajeInicial;
		if (clientList->checkIfUserNameIsAlreadyInUse(message.value)) {

			if (numberOfCurrentAcceptedClients >= maxNumberOfClients) {
				message = MessageBuilder().createServerFullMessage();
			} else {
				numberOfCurrentAcceptedClients++;
				//Creo el cliente con el nombre del mensaje y lo agrego a la lista
				string name(message.value);
				Avion *clientPlane = new Avion();
				parser->getAvion(clientPlane, numberOfCurrentAcceptedClients);
				Client* client = new Client(name, socketConnection, 0, clientPlane);

				clientList->addClient(client);

				message = MessageBuilder().createSuccessfullyConnectedMessage();

				clientEntranceMessages[socketConnection] = std::thread(
						clientReader, socketConnection, clientList, procesor);

				mensajeInicial =  MessageBuilder().createInitialMessageForClient(client);
			}
		} else {
			Client* client = clientList->getClientForName(message.value);
			if (client->getConnnectionState()) {
				logWriter->writeUserNameAlreadyInUse(message.value);
				message = MessageBuilder().createUserNameAlreadyInUseMessage();
			} else {
				logWriter->writeResumeGameForUserName(message.value);
				client->setSocketMessages(socketConnection);
				client->setConnected(true);
				message = MessageBuilder().createSuccessfullyConnectedMessage();
				clientEntranceMessages[socketConnection] = std::thread(clientReader, socketConnection, clientList, procesor);
			}
		}

		sendMsj(socketConnection, sizeof(message), &(message));
		drawableList.push_back(mensajeInicial);
		//Se envia la info para levantar el juego recien cuando todos se conectaron.
		if(numberOfCurrentAcceptedClients == maxNumberOfClients && !gameInitiated){
			sendGameInfo(clientList);
			gameInitiated = true;
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

	mensaje *ventanaMsj = new mensaje;
	this->scenery = parser->getFondoEscenario();
	ventanaMsj->height = parser->getAltoVentana();
	ventanaMsj->width = parser->getAnchoVentana();
	this->scenery->setScrollingStep(1);
	this->scenery->setWindowHeight(ventanaMsj->height);
	mensaje * escenarioMsj = MessageBuilder().createInitBackgroundMessageForScenery(this->scenery);
	drawableList.push_back(ventanaMsj);
	drawableList.push_back(escenarioMsj);
	for(int i = 0; i < parser->getNumberOfElements(); i++){
		DrawableObject* object = new DrawableObject();
		parser->getElement(*object, i);
		this->scenery->addElement(object);
		//mensaje* elementMsg = (mensaje*) malloc(sizeof(mensaje));
		mensaje *elementMsg = MessageBuilder().createBackgroundElementUpdateMessage(this->scenery, i);
		strncpy(elementMsg->action, "create", 20);
		drawableList.push_back(elementMsg);
	}
	this->scenery->transformPositions();
	bulletsCount = maxNumberOfClients + this->scenery->getNumberElements();
	std::thread broadcastThread(broadcastMsj,clientList, this->procesor, this->scenery);
	std::thread clientConnectionWaiter(waitForClientConnection,
			maxNumberOfClients, this->socketManager->socketHandle, this->parser, this->clientList, this->procesor);

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

void GameManager::broadcastMessage(mensaje *message) {
	broadcast(message, this->clientList);
}

Bullet* GameManager::createBulletForClient(Client *client) {
	bulletsCount++;

	Bullet* bullet = new Bullet(bulletsCount, client->plane->getPosX(), client->plane->getPosY() + 1);

	bullets.push_back(bullet);
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
	for (threadItr = clientExitMessages.begin();
			threadItr != clientExitMessages.end(); ++threadItr) {
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
	delete logWriter;
	delete this->scenery;
	for (unsigned int i = 0; i < drawableList.size(); i++){
		delete drawableList.front();
		drawableList.pop_front();
	}
}

