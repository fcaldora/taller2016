#include "CargadorXML.h"
#include <iostream>
#include <sstream>

#define kMaxNumberOfClients 10
#define kMaxNumberOfValidPort 65535

XMLLoader::XMLLoader(LogWriter *errorLogWriter) {
	this->errorLogWriter = errorLogWriter;
}

bool XMLLoader::serverXMLHasValidElements(TiXmlDocument xmlFile) {
	TiXmlElement *server = xmlFile.FirstChildElement(kServerTag);
	if (server == NULL){
		this->errorLogWriter->writeNotFoundElementInXML(kServerTag);
		return false;
	}

	TiXmlElement *maxNumberOfClients = server->FirstChildElement("CantidadMaximaClientes");
	if (maxNumberOfClients == NULL) {
		this->errorLogWriter->writeNotFoundElementInXML("CantidadMaximaClientes");
		return false;
	}

	TiXmlElement *port = maxNumberOfClients->NextSiblingElement("Puerto");
	if (port == NULL) {
		this->errorLogWriter->writeNotFoundElementInXML("Puerto");
		return false;
	}

	return true;
}

bool XMLLoader::serverXMLHasValidValues(TiXmlDocument xmlFile){
	const char* maxNumberOfClients = xmlFile.FirstChildElement("Servidor")->FirstChildElement("CantidadMaximaClientes")->GetText();
	std::stringstream strValue;
	strValue << maxNumberOfClients;
	unsigned int intValue;
	strValue >> intValue;

	if (intValue <= 0 || intValue >= kMaxNumberOfClients) {
		this->errorLogWriter->writeValueErrorForElementInXML("CantidadMaximaClientes");
		return false;
	}

	const char* port = xmlFile.FirstChildElement("Servidor")->FirstChildElement("CantidadMaximaClientes")->NextSiblingElement("Puerto")->GetText();
	std::stringstream portStrValue;
	portStrValue << port;
	unsigned int portIntValue;
	portStrValue >> portIntValue;

	if (portIntValue <= 0 || portIntValue > kMaxNumberOfValidPort) {
		this->errorLogWriter->writeValueErrorForElementInXML("Puerto");
		return false;
	}

	return true;
}

bool XMLLoader::serverXMLIsValid(const char* fileName) {
	TiXmlDocument xmlFile(fileName);

	if(!xmlFile.LoadFile()) {
		this->errorLogWriter->writeNotFoundFileForNameError(fileName);
		xmlFile.Clear();
		return false;
	}

	if (!serverXMLHasValidElements(xmlFile) || !serverXMLHasValidValues(xmlFile)) {
		xmlFile.Clear();
		return false;
	}

	xmlFile.Clear();
	return true;
}

XMLLoader::~XMLLoader() {
	this->xmlDocument.Clear();
}
