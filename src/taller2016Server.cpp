#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <strings.h>
#include <stdio.h>
#include <sys/unistd.h>
#include <thread>
#include "XmlParser.h"
#include "CargadorXML.h"
#include "LogWriter.h"
#include "Procesador.h"
#include "Avion.h"
#include "Escenario.h"
#include "Sprite.h"
#include "Window.h"
#include <mutex>
#include "Client.h"
#include "Constants.h"

#define kMaxHostName 256
#define kServerTestFile "serverTest.txt"

using namespace std;
struct msjProcesado {
	clientMsj mensaje;
	int socket;
	bool procesado;
};

list<msjProcesado> messagesList;
list<Client*> clients;
std::mutex mutexColaMensajes;
std::mutex writingInLogFileMutex;
map<unsigned int, thread> clientEntranceMessages;
map<unsigned int, thread> clientExitMessages;
bool appShouldTerminate;
LogWriter *logWriter;
int numberOfCurrentAcceptedClients;
int screenHeight = 640;
int screenWidth = 480;

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

int readMsg(int socket, int bytesARecibir, mensaje* mensaje) {
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

void *procesarMensajes(list<msjProcesado> *msgList) {
	Procesador procesador;
	msjProcesado *auxiliar;
	mensaje msj;
	while (!appShouldTerminate) {
		std::list<Client*>::iterator it, it2;
		for (it = clients.begin(); it != clients.end(); ++it) {
			for(it2 = clients.begin(); it2 != clients.end(); ++it2){
				strcpy(msj.action, "draw");
				msj.id = (*it2)->getPlane()->getId();
				msj.posX = (*it2)->getPosX();
				msj.posY = (*it2)->getPosY();
				sendMsjInfo((*it)->getSocketMessages(), sizeof(msj), &msj);
			}
		}
	}
}

void broadcastMsj(mensaje msg) {
	std::list<Client*>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it) {
		sendMsjInfo((*it)->getSocketMessages(), sizeof(msg), &msg);
	}
}


bool checkNameAvailability(char value[]) {
	std::list<Client*>::iterator it;
	for (it = clients.begin(); it != clients.end(); ++it) {
		if (strcmp((*it)->getName().c_str(), value) == 0) {
			return false;
		}
	}
	return true;
}

Client* getClientByName(char name[]) {
	std::list<Client*>::iterator it;
	Client* response;
	for (it = clients.begin(); it != clients.end(); ++it) {
		if (strcmp((*it)->getName().c_str(), name) == 0) {
			response = (*it);
		}
	}
	return response;
}

Client* getClientBySocketMessages(int socket) {
	std::list<Client*>::iterator it;
	Client* response;
	for (it = clients.begin(); it != clients.end(); ++it) {
		if (socket == (*it)->getSocketMessages()) {
			response = (*it);
		}
	}
	return response;
}

void *clientReader(int socketConnection, list<msjProcesado>* messagesList) {
	int res = 0;
	bool clientHasDisconnected = false;
	while (!appShouldTerminate && !clientHasDisconnected) {
		clientMsj message;
		res = readMsj(socketConnection, sizeof(message), &message);
		if (res < 0) {

			shutdown(socketConnection, SHUT_RDWR);
			clientHasDisconnected = true;
			Client* clientDisconnect = getClientBySocketMessages(socketConnection);
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

		} else {
			mensaje respuesta;
			Client* client = getClientByName(message.id);
			respuesta.id = client->getPlane()->getId();
			strcpy(respuesta.action, "draw");
			if(strcmp(message.value, "DER") == 0){
				if((client->getPosX() + client -> getPlane()->getWidth()) < screenWidth){
					client->setPosX(client->getPosX() + 1);
				}
			}else if(strcmp(message.value, "IZQ") == 0){
				if(client->getPosX()>0){
					client->setPosX(client->getPosX() - 1);
				}
			}else if(strcmp(message.value, "ABJ") == 0){
				if((client->getPosY() + client -> getPlane()->getHeigth()) < screenHeight){
					client->setPosY(client->getPosY() + 1);
				}
			}else if(strcmp(message.value, "ARR") == 0){
				if(client->getPosY()>0){
					client->setPosY(client->getPosY() - 1);
				}
			}
			respuesta.posX = client->getPosX();
			respuesta.posY = client->getPosY();
			broadcastMsj(respuesta);
		}
	}
}

void *responderCliente(int socket, list<msjProcesado> *msgList) {
	msjProcesado auxiliar;
	int respuesta;
	bool clientHasDisconnected = false;

	while (!appShouldTerminate && !clientHasDisconnected) {
		mutexColaMensajes.lock();
		auxiliar = msgList->front();
		if ((auxiliar.socket == socket) && (auxiliar.procesado == true)) {
			msgList->pop_front();
			respuesta = sendMsj(socket, sizeof(auxiliar.mensaje),
					&(auxiliar.mensaje));
			if (respuesta <= 0) {
				clientHasDisconnected = true;
			}
		}
		mutexColaMensajes.unlock();
	}

}

void* waitForClientConnection(int maxNumberOfClients, int socketHandle) {
	while (!appShouldTerminate) {
		logWriter->writeWaitingForClientConnection();
		int socketConnection = accept(socketHandle, NULL, NULL);
		logWriter->writeClientConnectionReceived();

		clientMsj message;
		//Leo el mensaje de conexion
		readMsj(socketConnection, sizeof(message), &message);
		mensaje mensajeInicial;

		if (checkNameAvailability(message.value)) {

			if (numberOfCurrentAcceptedClients >= maxNumberOfClients) {
				strncpy(message.id, "0", 20);
				strncpy(message.type, "server_full", 20);
				strncpy(message.value, "Try again later", 20);
			} else {
				//Creo el cliente con el nombre del mensaje y lo agrego a la lista
				string name(message.value);
				Client* client = new Client(name, socketConnection, 0);
				Object* planeClient = new Object();
				planeClient->setHeigth(81);
				planeClient->setWidth(81);
				planeClient->setId(clients.size()+1);
				cout << clients.size()+1 << endl;
				planeClient->setPath("avionPrueba.png");
				planeClient->setPhotograms(1);
				planeClient->setActualPhotogram(1);
				planeClient->setPosX(200);
				planeClient->setPosY(200);
				client->setPlane(planeClient);
				client->setPosX(200);
				client->setPosY(200);

				clients.push_back(client);

				strncpy(message.id, "0", 20);
				strncpy(message.type, "connection_ok", 20);
				strncpy(message.value, "Client connected", 20);
				clientEntranceMessages[socketConnection] = std::thread(
						clientReader, socketConnection, &messagesList);
				//clientExitMessages[socketConnection] = std::thread(
					//	responderCliente, socketConnection, &messagesList);
				numberOfCurrentAcceptedClients++;
				strcpy(mensajeInicial.action, "create");
				strcpy(mensajeInicial.imagePath, client->getPlane()->getPath().c_str());
				mensajeInicial.id = client->getPlane()->getId();
				mensajeInicial.photograms = client->getPlane()->getPhotograms();
				mensajeInicial.actualPhotogram = client->getPlane()->getActualPhotogram();
				mensajeInicial.height = client->getPlane()->getHeigth();
				mensajeInicial.width = client->getPlane()->getWidth();
				mensajeInicial.posX = client->getPlane()->getPosX();
				mensajeInicial.posY = client->getPlane()->getPosY();
				mensajeInicial.activeState = true;

			}
		} else {
			Client* client = getClientByName(message.value);
			if (client->getConnnectionState()) {
				cout
						<< "El nombre de cliente no esta disponible, ya que hay un cliente jugando con ese nombre. "
						<< message.value << endl;
				strncpy(message.id, "0", 20);
				strncpy(message.type, "error", 20);
				strncpy(message.value, "Name already used", 20);
			} else {
				cout << "Nombre ya utilizado, reaunudando partida de: "
						<< message.value << endl;
				client->setSocketMessages(socketConnection);
				client->setConnected(true);
				strncpy(message.id, "0", 20);
				strncpy(message.type, "connection_ok", 20);
				strncpy(message.value, "Client connected", 20);
				clientEntranceMessages[socketConnection] = std::thread(clientReader, socketConnection, &messagesList);
				//clientExitMessages[socketConnection] = std::thread(responderCliente, socketConnection, &messagesList);
			}
		}

		int response = sendMsj(socketConnection, sizeof(message), &(message));
		broadcastMsj(mensajeInicial);

	}
}

void prepareForExit(XMLLoader *xmlLoader, XmlParser *xmlParser,
		LogWriter *logWriter) {
	delete logWriter;
	delete xmlLoader;
	delete xmlParser;
}

int main(int argc, char* argv[]) {
	const char* fileName;
	logWriter = new LogWriter;
	XMLLoader *xmlLoader = new XMLLoader(logWriter);
	numberOfCurrentAcceptedClients = 0;
	if (argc != 2) {
		fileName = kServerTestFile;
		logWriter->writeUserDidnotEnterFileName();
	} else {
		fileName = argv[1];
		if (!xmlLoader->serverXMLIsValid(fileName)) {
			fileName = kServerTestFile;
		}
	}

	XmlParser *parser = new XmlParser(fileName);
	logWriter->setLogLevel(parser->getLogLevel());
	struct sockaddr_in socketInfo;
	char sysHost[kMaxHostName + 1]; // Hostname of this computer we are running on
	struct hostent *hPtr;
	int socketHandle;
	int portNumber = parser->getServerPort();
	int maxNumberOfClients = parser->getMaxNumberOfClients();
	bzero(&socketInfo, sizeof(sockaddr_in));  // Clear structure memory

	// Get system information

	gethostname(sysHost, kMaxHostName); // Get the name of this computer we are running on
	if ((hPtr = gethostbyname(sysHost)) == NULL) {
		cerr << "System hostname misconfigured." << endl;
		logWriter->writeError("Error al obtener el hostname");
		prepareForExit(xmlLoader, parser, logWriter);
		exit(EXIT_FAILURE);
	}

	// create socket

	if ((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		close(socketHandle);
		logWriter->writeError("Error al crear el socket");
		prepareForExit(xmlLoader, parser, logWriter);
		exit(EXIT_FAILURE);
	}

	// Load system information into socket data structures

	socketInfo.sin_family = AF_INET;
	socketInfo.sin_addr.s_addr = htonl(INADDR_ANY); // Use any address available to the system
	socketInfo.sin_port = htons(portNumber);  // Set port number

	// Bind the socket to a local socket address
	if (bind(socketHandle, (struct sockaddr *) &socketInfo, sizeof(socketInfo))
			< 0) {
		close(socketHandle);
		perror("bind");
		logWriter->writeError("Error en el bind");
		prepareForExit(xmlLoader, parser, logWriter);
		exit(EXIT_FAILURE);
	}
	if (listen(socketHandle, 5) == -1) {
		prepareForExit(xmlLoader, parser, logWriter);
		logWriter->writeError("Error en el listen");
		return EXIT_FAILURE;
	}

	appShouldTerminate = false;
	//std::thread procesadorMensajes(procesarMensajes, &messagesList);
	std::thread clientConnectionWaiter(waitForClientConnection,
			maxNumberOfClients, socketHandle);
	while (!appShouldTerminate) {
		cout << "Menu:" << endl;
		cout << "1. Salir" << endl;
		cout << "Ingresar opcion:" << endl;
		unsigned int userDidChooseOption;
		cin >> userDidChooseOption;
		if (userDidChooseOption == 1) {
			appShouldTerminate = true;
		} else {
			cout << "Opcion incorrecta" << endl;
		}
	}

	//procesadorMensajes.join();
	clientConnectionWaiter.detach();
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

	logWriter->writeUserDidFinishTheApp();
	prepareForExit(xmlLoader, parser, logWriter);
	return EXIT_SUCCESS;
}
