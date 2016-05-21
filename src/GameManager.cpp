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
#include "Messenger.h"

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
	actionMsj action;
	strcpy(action.action,"windowSize");
	Messenger().broadcastActionMsj(action, clientList);
	broadcastMessage(ventanaMsj);
	strcpy(action.action,"create");
	Messenger().broadcastActionMsj(action, clientList);
	this->broadcastMessage(escenarioMsj);
	drawableList.push_back(escenarioMsj);
	for(int i = 0; i < parser->getNumberOfElements(); i++){
		DrawableObject* object = new DrawableObject();
		mensaje elementMsg;
		parser->getElement(*object, i);
		escenario->addElement(object);
		elementMsg = MessageBuilder().createBackgroundElementCreationMessageForElement(object);
		drawableList.push_back(elementMsg);
		Messenger().broadcastActionMsj(action, clientList);
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
		Messenger().broadcastActionMsj(action, clientList);
		this->broadcastMessage(avionMsj);
		i++;
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
			updateMsj msg;
			deleteMsj delMsj;
			actionMsj action;
			if(objects.bulletDeleteMessage(i, delMsj, processor->getScreenWidth(), processor->getScreenHeight())){
				strncpy(action.action, "delete", kLongChar);
				mutexColaMensajes.lock();
				Messenger().broadcastActionMsj(action, clientList);
				Messenger().broadcastDeleteMsj(delMsj, clientList);
				mutexColaMensajes.unlock();
			}else if(objects.bulletDrawMessage(i, msg,processor->getScreenWidth(), processor->getScreenHeight() )){
				strncpy(action.action, "draw", kLongChar);
				mutexColaMensajes.lock();
				Messenger().broadcastActionMsj(action, clientList);
				Messenger().broadcastUpdateMsj(msg, clientList);
				mutexColaMensajes.unlock();
			}

		}
		updateMsj msj;
		if(gameInitiated){
			usleep(1000);
			escenario->update();
			actionMsj action;
			strncpy(action.action, "draw", kLongChar);
			Messenger().broadcastActionMsj(action, clientList);
			msj = MessageBuilder().createBackgroundUpdateMessage(escenario);
			Messenger().broadcastUpdateMsj(msj, clientList);
			for (int i = 0; i < escenario->getNumberElements(); i++){
				DrawableObject* element = escenario->getElement(i);
				Messenger().broadcastActionMsj(action, clientList);
				msj = MessageBuilder().createBackgroundElementUpdateMessageForElement(element);
				Messenger().broadcastUpdateMsj(msj, clientList);
			}
		}
		contador++;
		if (contador == 30){
			contador = 0;
			actionMsj action;
			strncpy(action.action, "draw", kLongChar);
			for(it = clientList->clients.begin(); it != clientList->clients.end(); it++){
				if((*it)->plane->updatePhotogram()){
					mutexColaMensajes.lock();
					Messenger().broadcastActionMsj(action, clientList);
					updateMsj photogramMsg = MessageBuilder().createUpdatePhotogramMessageForPlane((*it)->plane);
					Messenger().broadcastUpdateMsj(photogramMsg, clientList);
					mutexColaMensajes.unlock();
				}
			}
		}
	}
}


void sendGameInfo(ClientList* clientList){
	list<mensaje>::iterator it;
	actionMsj action;
	strncpy(action.action, "create", kLongChar);
	for (it = drawableList.begin(); it != drawableList.end(); it++){
		Messenger().broadcastActionMsj(action, clientList);
		Messenger().broadcastMensaje((*it), clientList);
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

void *clientReader(int socketConnection, ClientList *clientList, Procesador *procesor, Escenario* escenario) {
	int res = 0;
	bool clientHasDisconnected = false;
	while (!appShouldTerminate && !clientHasDisconnected) {
		clientMsj message;
		res = Messenger().readClientMsj(socketConnection, sizeof(message), &message);
		if (res < 0) {
			shutdown(socketConnection, SHUT_RDWR);
			clientHasDisconnected = true;
			disconnectClientForSocketConnection(socketConnection, clientList);
			Client *client = clientList->getClientForSocket(socketConnection);
			actionMsj action;
			strncpy(action.action, "path", kLongChar);
			Messenger().broadcastActionMsj(action, clientList);
			mensaje disconnection = MessageBuilder().createDisconnectionMessageForClient(client);
			Messenger().broadcastMensaje(disconnection, clientList);
		} else {
			procesor->processMessage(message);
		}
	}
	pthread_exit(NULL);
}

void* waitForClientConnection(int maxNumberOfClients, int socketHandle, XmlParser *parser, ClientList *clientList, Procesador *procesor, Escenario* escenario) {
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
		Messenger().readClientMsj(socketConnection, sizeof(message), &message);
		userWasConnected = false;
		if (clientList->checkIfUserNameIsAlreadyInUse(message.value)) {

			if (numberOfCurrentAcceptedClients >= maxNumberOfClients) {
				message = MessageBuilder().createServerFullMessage();
			} else {
				numberOfCurrentAcceptedClients++;
				//Creo el cliente con el nombre del mensaje y lo agrego a la lista
				string name(message.value);
				Avion *clientPlane = parser->getAvion(numberOfCurrentAcceptedClients);
				Client* client = new Client(name, socketConnection, 0, clientPlane);

				clientList->addClient(client);

				message = MessageBuilder().createSuccessfullyConnectedMessage();

				clientEntranceMessages[socketConnection] = std::thread(
						clientReader, socketConnection, clientList, procesor, escenario);
				mensaje mensajeAvion = MessageBuilder().createInitialMessageForClient(client);
				drawableList.push_back(mensajeAvion);
			}
		} else {
			Client* client = clientList->getClientForName(message.value);
			if (client->getConnnectionState()) {
				logWriter->writeUserNameAlreadyInUse(message.value);
				message = MessageBuilder().createUserNameAlreadyInUseMessage();
			} else {
				actionMsj action;
				strncpy(action.action, "path", kLongChar);
				Messenger().broadcastActionMsj(action, clientList);
				mensaje reconnection = MessageBuilder().createReconnectionMessageForClient(client);
				Messenger().broadcastMensaje(reconnection, clientList);

				logWriter->writeResumeGameForUserName(message.value);
				client->setSocketMessages(socketConnection);
				client->setConnected(true);

				message = MessageBuilder().createSuccessfullyConnectedMessage();
				escenarioMsj = MessageBuilder().createInitBackgroundMessage(escenario);
				clientEntranceMessages[socketConnection] = std::thread(clientReader, socketConnection, clientList, procesor, escenario);
				userWasConnected = true;
			}
		}

		Messenger().sendClientMsj(socketConnection, sizeof(message), &(message));
		//Se envia la info para levantar el juego recien cuando todos se conectaron.
		if(numberOfCurrentAcceptedClients == maxNumberOfClients && !gameInitiated){
			sendGameInfo(clientList);
			gameInitiated = true;
		}
		if(userWasConnected){
			mensaje ventanaMsj, mensajeObjeto;
			actionMsj action;
			strncpy(action.action, "create", kLongChar);
			ventanaMsj.height = parser->getAltoVentana();
			ventanaMsj.width = parser->getAnchoVentana();
			Messenger().sendActionMsj(socketConnection, sizeof(actionMsj), &action);
			Messenger().sendMensaje(socketConnection, sizeof(mensaje), &ventanaMsj);
			Messenger().sendActionMsj(socketConnection, sizeof(actionMsj), &action);
			Messenger().sendMensaje(socketConnection, sizeof(mensaje), &escenarioMsj);
			std::list<Client*>::iterator it;
			for (it = clientList->clients.begin(); it != clientList->clients.end(); ++it) {
				mensajeObjeto = MessageBuilder().createInitialMessageForClient((*it));
				Messenger().sendActionMsj(socketConnection,sizeof(actionMsj), &action);
				Messenger().sendMensaje(socketConnection, sizeof(mensaje), &mensajeObjeto);
			}
			for(int i = 0; i < parser->getNumberOfElements(); i++){
				DrawableObject object;
				mensaje elementMsg;
				parser->getElement(object, i);
				elementMsg = MessageBuilder().createBackgroundElementBaseMessageForElement(&object);
				Messenger().sendActionMsj(socketConnection, sizeof(actionMsj), &action);
				Messenger().sendMensaje(socketConnection, sizeof(mensaje), &elementMsg);
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

	this->procesor = new Procesador(this->clientList, screenWidth, screenHeight, this, &mutexColaMensajes);
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
		elementMsg = MessageBuilder().createBackgroundElementBaseMessageForElement(object);
		drawableList.push_back(elementMsg);
	}
	escenario.transformPositions();
	objects.setIdOfFirstBullet(maxNumberOfClients + escenario.getNumberElements());
	std::thread broadcastThread(broadcastMsj,clientList, this->procesor, this->escenario);
	std::thread clientConnectionWaiter(waitForClientConnection,
			maxNumberOfClients, this->socketManager->socketHandle, this->parser, this->clientList, this->procesor, this->escenario);

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
	Messenger().broadcastMensaje(message, clientList);
}

void GameManager::restartGame() {
	escenario->restart();
}

Object GameManager::createBulletForClient(Client* client){
	Object bullet;
	bullet.setId(objects.getLastId() + 1);
	bullet.setPath("bullet.png");
	bullet.setPosX(client->plane->getPosX() + client->plane->getWidth()/2 - 15);
	bullet.setPosY(client->plane->getPosY() + 1);
	bullet.setWidth(30);
	bullet.setHeigth(30);
	bullet.setStatus(true);
	bullet.setStep((client->plane->getVelDisparo() + client->plane->getVelDesplazamiento())/5);
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

