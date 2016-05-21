/*
 * Messenger.cpp
 *
 *  Created on: 21 de may. de 2016
 *      Author: gusta
 */

#include "Messenger.h"

Messenger::Messenger() {}

int Messenger::readClientMsj(int socket, int bytesARecibir, clientMsj* mensaje){
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

int Messenger::sendActionMsj(int socket, int bytesAEnviar, actionMsj* mensaje){
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

int Messenger::sendMensaje(int socket, int bytesAEnviar, mensaje* mensaje){
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

int Messenger::sendUpdateMsj(int socket, int bytesAEnviar,updateMsj* mensaje){
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

int Messenger::sendClientMsj(int socket, int bytesAEnviar, clientMsj* mensaje){
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

int Messenger::sendDeleteMsj(int socket, int bytesAEnviar, deleteMsj* mensaje){
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

void Messenger::broadcastClientMsj(clientMsj msg, ClientList* clientList){
	std::list<Client*>::iterator it;
	for (it = clientList->clients.begin(); it != clientList->clients.end(); ++it) {
		if((*it)->getConnnectionState()){
			sendClientMsj((*it)->getSocketMessages(), sizeof(msg), &msg);
		}
	}
}

void Messenger::broadcastMensaje(mensaje msg, ClientList* clientList){
	std::list<Client*>::iterator it;
	for (it = clientList->clients.begin(); it != clientList->clients.end(); ++it) {
		if((*it)->getConnnectionState()){
			sendMensaje((*it)->getSocketMessages(), sizeof(msg), &msg);
		}
	}
}

void Messenger::broadcastUpdateMsj(updateMsj msg, ClientList* clientList){
	std::list<Client*>::iterator it;
	for (it = clientList->clients.begin(); it != clientList->clients.end(); ++it) {
		if((*it)->getConnnectionState()){
			sendUpdateMsj((*it)->getSocketMessages(), sizeof(msg), &msg);
		}
	}
}

void Messenger::broadcastActionMsj(actionMsj msg, ClientList* clientList){
	std::list<Client*>::iterator it;
	for (it = clientList->clients.begin(); it != clientList->clients.end(); ++it) {
		if((*it)->getConnnectionState()){
			sendActionMsj((*it)->getSocketMessages(), sizeof(msg), &msg);
		}
	}
}

void Messenger::broadcastDeleteMsj(deleteMsj msg, ClientList* clientList){
	std::list<Client*>::iterator it;
	for (it = clientList->clients.begin(); it != clientList->clients.end(); ++it) {
		if((*it)->getConnnectionState()){
			sendDeleteMsj((*it)->getSocketMessages(), sizeof(msg), &msg);
		}
	}
}

Messenger::~Messenger() {}

