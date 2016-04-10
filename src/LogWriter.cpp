/*
 * ErrorLogWriter.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: luciano
 */

#include "LogWriter.h"

#define kErrorsLogName "LogFile.txt"

LogWriter::LogWriter() {
	this->logFile.open(kErrorsLogName);
	this->logLevel = LogLevelTypeOnlyErrors;
}

LogWriter::~LogWriter() {
	this->logFile.close();
}

void LogWriter::setLogLevel(LogLevelType logLevel) {
	this->logLevel = logLevel;
}

void LogWriter::writeValueErrorForElementInXML(string element){
	string error = "El valor del elemento '" + element + "' es invalido. Se usara un XML por defecto.";
	writeLogInFile(error);
}

void LogWriter::writeNotFoundElementInXML(string element) {
	string error = "El elemento '" + element + "' no existe. Se usara un XML por defecto.";
	writeLogInFile(error);
}

void LogWriter::writeNotFoundFileForNameError(string fileName) {
	string error = "El archivo '" + fileName + "' no existe o es invalido. Se usara archivo pot defecto.";
	writeLogInFile(error);
}

void LogWriter::writeUserDidnotEnterFileName() {
	writeLogInFile("Falta escribir el nombre del archivo del servidor, se usara uno por defecto.");
}

void LogWriter::writeError(string error) {
	writeLogInFile(error);
}

void LogWriter::writeWaitingForClientConnection() {
	if (this->logLevel == LogLevelTypeOnlyErrors)
		return;
	string log = "Esperando conexion de cliente";
	writeLogInFile(log);
}

void LogWriter::writeClientConnectionReceived() {
	if (this->logLevel == LogLevelTypeOnlyErrors)
		return;
	string log = "Conexion de cliente recibida";
	writeLogInFile(log);
}

void LogWriter::writeClientMessageReceviedFromSocketConnection(clientMsj message, int socketConnection) {
	if (this->logLevel == LogLevelTypeOnlyErrors)
		return;

	stringstream strs;
	strs << socketConnection;
	string temp_str = strs.str();
	char* socketConnectionString = (char*) temp_str.c_str();
	stringstream converter;
	converter << socketConnection;
	char* str1 = "Socket:";
	char* str2 = " recibio el mensaje con ID:";

	char* logHeader = (char *) malloc(1 + strlen(str1)+ strlen(socketConnectionString)+ strlen(str2));
	strcpy(logHeader, str1);
	strcat(logHeader, socketConnectionString);
	strcat(logHeader, str2);
	char *log = commentForMessage(logHeader, message);
	free(logHeader);

	writeLogInFile(log);
	free(log);
}

void LogWriter::writeMessageWasprossed(clientMsj message) {
	if (this->logLevel == LogLevelTypeOnlyErrors)
		return;

	char *logHeader = "Se proceso el mensaje con ID:";
	char *log = commentForMessage(logHeader, message);

	writeLogInFile(log);
	free(log);
}

void LogWriter::writeUserDidFinishTheApp() {
	if (this->logLevel == LogLevelTypeOnlyErrors)
		return;

	string log = "El usuario cerro la aplicacion";
	writeLogInFile(log);
}

void LogWriter::writeLogInFile(string error) {
	cout << error << endl;
	this->logFile << error << endl;
}

char * LogWriter::commentForMessage(char *headerText, clientMsj message) {
	char* str1 = headerText;
	char* messageID = message.id;
	char *str2 = " tipo:";
	char *messageType = message.type;
	char *str3 = " valor:";
	char *messageValue = message.value;

	char * str4 = (char *) malloc(1 + strlen(str1)+ strlen(messageID)+ strlen(str2)+ strlen(messageType)+ strlen(str3)+ strlen(messageValue));
	strcpy(str4, str1);
	strcat(str4, messageID);
	strcat(str4, str2);
	strcat(str4, messageType);
	strcat(str4, str3);
	strcat(str4, messageValue);
	return str4;
}

