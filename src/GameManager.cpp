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

#define kServerTestFile "serverTest.txt"

std::mutex mutexColaMensajes;
std::mutex writingInLogFileMutex;
map<unsigned int, thread> clientEntranceMessages;
map<unsigned int, thread> keepAliveThreads;
list<Object*> objects;
int objectsCount = 0;
bool appShouldTerminate, gameInitiated, userWasConnected;
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
	this->escenario = NULL;
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
	std::list<Object*>::iterator objectIt;
	while(!appShouldTerminate){
		usleep(1000);
		for(objectIt = objects.begin(); objectIt != objects.end(); objectIt++){
			if((*objectIt)->isStatic()){
				(*objectIt)->move();
			}
			mensaje msg;
			if((*objectIt)->notVisible(processor->getScreenWidth(), processor->getScreenHeight())){
				strcpy(msg.action, "delete");
				msg.id = (*objectIt)->getId();
				objects.erase(objectIt);
				objectIt--;
			}else{
				strcpy(msg.action, "draw");
				msg.id = (*objectIt)->getId();
				msg.posX = (*objectIt)->getPosX();
				msg.posY = (*objectIt)->getPosY();
			}
			broadcast(msg, clientList);
		}
		mensaje msj;
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
		cout << "OBJECTS : " << objects.size() << endl;
	}
}


void sendGameInfo(ClientList* clientList){
	list<mensaje*>::iterator it;
	for (it = drawableList.begin(); it != drawableList.end(); it++){
		broadcast(*(*it), clientList);
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

Object* createBullet(Client* client){
	objectsCount++;
	Object* bullet = new Object();
	bullet->setId(objectsCount);
	bullet->setPath("bullet.png");
	bullet->setPosX(client->plane->getPosX() + client->plane->getWidth()/2 - 15);
	bullet->setPosY(client->plane->getPosY() + 1);
	bullet->setWidth(30);
	bullet->setHeigth(30);
	bullet->setStatus(true);
	objects.push_back(bullet);
	return bullet;
}

void *clientReader(int socketConnection, ClientList *clientList, Procesador *procesor) {
	int res = 0, option;
	bool clientHasDisconnected = false;
	while (!appShouldTerminate && !clientHasDisconnected) {
		clientMsj message;
		res = readMsj(socketConnection, sizeof(message), &message);
		if (res < 0) {

			shutdown(socketConnection, SHUT_RDWR);
			clientHasDisconnected = true;
			disconnectClientForSocketConnection(socketConnection, clientList);
			mensaje disconnection;
			strcpy(disconnection.action, "path");
			disconnection.id = clientList->getClientForSocket(socketConnection)->plane->getId();
			strcpy(disconnection.imagePath, "disconnected.png");
			broadcast(disconnection, clientList);

		} else {
			//cout << message.value << endl;
			option = procesor->processMessage(message);
			Client* client = clientList->getClientForName(message.id);
			mensaje respuesta;
			switch(option){
				case 1:
					respuesta = MessageBuilder().createPlaneMovementMessageForClient(client);
					break;
				case 2:
					//cout << "BALA CREADA" << endl;
					respuesta = MessageBuilder().createBulletMessage(createBullet(client));
					break;
				case 3:
					//keep alive
					break;
			}
			if(option != 3){
				broadcast(respuesta, clientList);
			}
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
		//cout<<"Error al settear el timeout"<<endl;
		//cout<<strerror(errno)<<endl;

		clientMsj message;
		mensaje escenarioMsj;
		//Leo el mensaje de conexion
		readMsj(socketConnection, sizeof(message), &message);
		mensaje* mensajeInicial = (mensaje*) malloc(sizeof(mensaje));
		userWasConnected = false;
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
				MessageBuilder().createInitialMessageForClient(client, mensajeInicial);
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
				escenarioMsj = MessageBuilder().createInitBackgroundMessage(escenario);
				clientEntranceMessages[socketConnection] = std::thread(clientReader, socketConnection, clientList, procesor);
				userWasConnected = true;
			}
		}

		sendMsj(socketConnection, sizeof(message), &(message));
		drawableList.push_back(mensajeInicial);
		//Se envia la info para levantar el juego recien cuando todos se conectaron.
		if(numberOfCurrentAcceptedClients == maxNumberOfClients && !gameInitiated){
			sendGameInfo(clientList);
			gameInitiated = true;
		}
		if(userWasConnected){
			mensaje ventanaMsj;
			ventanaMsj.height = parser->getAltoVentana();
			ventanaMsj.width = parser->getAnchoVentana();
			sendMsjInfo(socketConnection, sizeof(mensaje), &ventanaMsj);
			sendMsjInfo(socketConnection, sizeof(mensaje), &escenarioMsj);
			std::list<Client*>::iterator it;
			for (it = clientList->clients.begin(); it != clientList->clients.end(); ++it) {
				MessageBuilder().createInitialMessageForClient((*it), mensajeInicial);
				sendMsjInfo((*it)->getSocketMessages(), sizeof(mensaje), mensajeInicial);
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

	this->procesor = new Procesador(this->clientList, screenWidth, screenHeight);
	gameInitiated = false;
	int success = this->socketManager->createSocketConnection();
	if (success == EXIT_FAILURE)
		return EXIT_FAILURE;

	mensaje ventanaMsj, escenarioMsj;
	Escenario escenario;
	parser->getFondoEscenario(escenario);
	this->escenario = &escenario;
	ventanaMsj.height = parser->getAltoVentana();;
	ventanaMsj.width = parser->getAnchoVentana();
	escenario.setScrollingStep(1);
	escenario.setWindowHeight(ventanaMsj.height);
	escenarioMsj = MessageBuilder().createInitBackgroundMessage(&escenario);
	drawableList.push_back(&ventanaMsj);
	drawableList.push_back(&escenarioMsj);
	for(int i = 0; i < parser->getNumberOfElements(); i++){
		DrawableObject* object = new DrawableObject();
		parser->getElement(*object, i);
		escenario.addElement(object);
		mensaje* elementMsg = (mensaje*) malloc(sizeof(mensaje));
		*elementMsg = MessageBuilder().createBackgroundElementUpdateMessage(&escenario, i);
		strncpy(elementMsg->action, "create", 20);
		drawableList.push_back(elementMsg);
	}
	escenario.transformPositions();
	objectsCount = maxNumberOfClients + escenario.getNumberElements();
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
	for (unsigned int i = 0; i<drawableList.size(); i++){
		free(drawableList.front());
		drawableList.pop_front();
	}
}

