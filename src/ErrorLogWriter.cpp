/*
 * ErrorLogWriter.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: luciano
 */

#include "ErrorLogWriter.h"

#define kErrorsLogName "xmlErrorsLog.txt"

ErrorLogWriter::ErrorLogWriter() {
	this->errorLogFile.open(kErrorsLogName);
}

ErrorLogWriter::~ErrorLogWriter() {
	this->errorLogFile.close();
}

void ErrorLogWriter::writeValueErrorForElementInXML(string element){
	string error = "El valor del elemento '" + element + "' es invalido. Se usara un XML por defecto.";
	writeErrorInFile(error);
}

void ErrorLogWriter::writeNotFoundElementInXML(string element) {
	string error = "El elemento '" + element + "' no existe. Se usara un XML por defecto.";
	writeErrorInFile(error);
}

void ErrorLogWriter::writeNotFoundFileForNameError(string fileName) {
	string error = "El archivo '" + fileName + "' no existe o es invalido. Se usara archivo pot defecto.";
	writeErrorInFile(error);
}

void ErrorLogWriter::writeErrorInFile(string error) {
	cout << error << endl;
	this->errorLogFile << error << endl;
}

