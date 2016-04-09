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

void LogWriter::writeLogInFile(string error) {
	cout << error << endl;
	this->logFile << error << endl;
}

