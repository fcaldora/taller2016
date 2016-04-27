/*
 * GameManager.cpp

 *
 *  Created on: Apr 26, 2016
 *      Author: luciano
 */

#include "GameManager.h"

#include <strings.h>
#include <stdio.h>
#include <thread>
#include "LogWriter.h"
#include "Procesador.h"
#include "Avion.h"
#include "Escenario.h"
#include "Sprite.h"
#include "Window.h"
#include <mutex>
#include "Client.h"
#include "Constants.h"
#include "MenuPresenter.h"
#include <sys/socket.h>
#include "MessageBuilder.h"

#define kServerTestFile "serverTest.txt"

std::mutex mutexColaMensajes;
std::mutex writingInLogFileMutex;
map<unsigned int, thread> clientEntranceMessages;
map<unsigned int, thread> clientExitMessages;
bool appShouldTerminate;
LogWriter *logWriter;
int numberOfCurrentAcceptedClients;
int screenHeight;
int screenWidth;

GameManager::GameManager() {
	this->appShouldTerminate = false;
	this->menuPresenter = NULL;
	this->parser = NULL;
	this->socketManager = NULL;
	this->xmlLoader = NULL;
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

void broadcastMsj(mensaje msg, ClientList *clientList) {
	std::list<Client*>::iterator it;
	for (it = clientList->clients.begin(); it != clientList->clients.end(); ++it) {
		sendMsjInfo((*it)->getSocketMessages(), sizeof(msg), &msg);
	}
}

void disconnectClientForSocketConnection(int socketConnection, ClientList *clientList) {
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

void *clientReader(int socketConnection, ClientList *clientList) {
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
			Client* client = clientList->getClientForName(message.id);
			if(strcmp(message.value, "DER") == 0){
				if((client->plane->getPosX() + client -> getPlane()->getWidth()) < screenWidth){
					client->plane->moveOneStepRight();
				}
			}else if(strcmp(message.value, "IZQ") == 0){
				if(client->plane->getPosX() > 0){
					client->plane->moveOneStepLeft();
				}
			}else if(strcmp(message.value, "ABJ") == 0){
				if((client->plane->getPosY() + client -> getPlane()->getHeigth()) < screenHeight){
					client->plane->moveOneStepDown();
				}
			}else if(strcmp(message.value, "ARR") == 0){
				if(client->plane->getPosY() > 0){
					client->plane->moveOneStepUp();
				}
			}

			mensaje respuesta = MessageBuilder().createPlaneMovementMessageForClient(client);

			broadcastMsj(respuesta, clientList);
		}
	}
	pthread_exit(NULL);
}

Avion* clientPlaneBuilder(XmlParser *parser) {
	int travelingSpeed = parser->getVelocidadDespAvion();
	int shootingSpeed = parser->getVelocidadDispAvion();
	int numberOfPhotograms = 1;
	int actualPhotogram = 1;
	int positionX = 200;
	int positionY = 200;
	int height = 81;
	int width = 81;
	string imagePath = "avionPrueba.png";
	Avion* clientPlane = new Avion(travelingSpeed,shootingSpeed, numberOfPhotograms,
			actualPhotogram, positionX, positionY, height, width,imagePath);

	//cout << clients.size()+1 << endl;
	return clientPlane;
}

void* waitForClientConnection(int maxNumberOfClients, int socketHandle, XmlParser *parser, ClientList *clientList) {
	while (!appShouldTerminate) {
		logWriter->writeWaitingForClientConnection();
		int socketConnection = accept(socketHandle, NULL, NULL);
		logWriter->writeClientConnectionReceived();

		clientMsj message;
		//Leo el mensaje de conexion
		readMsj(socketConnection, sizeof(message), &message);
		mensaje mensajeInicial;

		if (clientList->checkIfUserNameIsAlreadyInUse(message.value)) {

			if (numberOfCurrentAcceptedClients >= maxNumberOfClients) {
				message = MessageBuilder().createServerFullMessage();
			} else {
				//Creo el cliente con el nombre del mensaje y lo agrego a la lista
				string name(message.value);
				Avion *clientPlane = clientPlaneBuilder(parser);
				Client* client = new Client(name, socketConnection, 0, clientPlane);

				clientList->addClient(client);

				message = MessageBuilder().createSuccessfullyConnectedMessage();

				clientEntranceMessages[socketConnection] = std::thread(
						clientReader, socketConnection, clientList);

				numberOfCurrentAcceptedClients++;
				mensajeInicial = MessageBuilder().createInitialMessageForClient(client);
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
				clientEntranceMessages[socketConnection] = std::thread(clientReader, socketConnection, clientList);
			}
		}

		sendMsj(socketConnection, sizeof(message), &(message));
		broadcastMsj(mensajeInicial, clientList);
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
	screenHeight = this->parser->getAltoVentana();
	screenWidth = this->parser->getAnchoVentana();

	int success = this->socketManager->createSocketConnection();
	if (success == EXIT_FAILURE)
		return EXIT_FAILURE;

	std::thread clientConnectionWaiter(waitForClientConnection,
			maxNumberOfClients, this->socketManager->socketHandle, this->parser, this->clientList);

	this->menuPresenter->presentMenu();

	clientConnectionWaiter.detach();

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
	for (threadItr = clientExitMessages.begin();
			threadItr != clientExitMessages.end(); ++threadItr) {
		threadItr->second.detach();
	}
}

GameManager::~GameManager() {
	delete this->clientList;
	detachClientMessagesThreads();
	delete this->menuPresenter;
	delete this->parser;
	delete this->xmlLoader;
	delete this->socketManager;
	delete logWriter;
}

