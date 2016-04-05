#include "CargadorXML.h"
#include <iostream>
#include <sstream>

//Constantes para crear el archivo en caso de que no exista.
#define MAXCLIENTES "7"
#define IP "127.0.0.1"
#define PUERTO "8080"
#define kMaxNumberOfClients 10
#define kMaxNumberOfValidPort 65535
#define kErrorsLogName "xmlErrorsLog.txt"

XMLLoader::XMLLoader() {
	this->errorLogFile.open(kErrorsLogName);
}

bool XMLLoader::serverXMLHasValidElements(TiXmlDocument xmlFile) {
	TiXmlElement *server = xmlFile.FirstChildElement("Servidor");
	if (server == NULL){
		writeNotFoundElementInXML("Servidor");
		return false;
	}

	TiXmlElement *maxNumberOfClients = server->FirstChildElement("CantidadMaximaClientes");
	if (maxNumberOfClients == NULL) {
		writeNotFoundElementInXML("CantidadMaximaClientes");
		return false;
	}

	TiXmlElement *port = maxNumberOfClients->NextSiblingElement("Puerto");
	if (port == NULL) {
		writeNotFoundElementInXML("Puerto");
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
		writeValueErrorForElementInXML("CantidadMaximaClientes");
		return false;
	}

	const char* port = xmlFile.FirstChildElement("Servidor")->FirstChildElement("CantidadMaximaClientes")->NextSiblingElement("Puerto")->GetText();
	std::stringstream portStrValue;
	portStrValue << port;
	unsigned int portIntValue;
	portStrValue >> portIntValue;

	if (portIntValue <= 0 || portIntValue > kMaxNumberOfValidPort) {
		writeValueErrorForElementInXML("Puerto");
		return false;
	}

	return true;
}

bool XMLLoader::serverXMLIsValid(const char* fileName) {
	TiXmlDocument xmlFile(fileName);

	if(!xmlFile.LoadFile()) {
		writeNotFoundFileForNameError(fileName);
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

void XMLLoader::writeValueErrorForElementInXML(string element){
	string error = "El valor del elemento '" + element + "' es invalido. Se usara un XML por defecto.";
	writeErrorInFile(error);
}

void XMLLoader::writeNotFoundElementInXML(string element) {
	string error = "El elemento '" + element + "' no existe. Se usara un XML por defecto.";
	writeErrorInFile(error);
}

void XMLLoader::writeNotFoundFileForNameError(string fileName) {
	string error = "El archivo '" + fileName + "' no existe o es invalido. Se usara archivo pot defecto.";
	writeErrorInFile(error);
}

void XMLLoader::writeErrorInFile(string error) {
	cout << error << endl;
	this->errorLogFile << error << endl;
}

void XMLLoader::cargarCliente(string nombreArchivo){
	if(!this->xmlDocument.LoadFile(nombreArchivo.c_str()))
		{
			//Escribo en el file de errores.
			this->errorLogFile <<"El archivo '"<<nombreArchivo<< "' no existe. Se creó un archivo con ese nombre.\n";
			//abro un archivo nuevo con valores estander.
			TiXmlDocument archivoNuevo;
			TiXmlElement* cliente = new TiXmlElement("Cliente");
			archivoNuevo.LinkEndChild(cliente);
			TiXmlElement* conexion = new TiXmlElement("Conexion");
			TiXmlElement* ipElem = new TiXmlElement("Ip");
			TiXmlText* ip = new TiXmlText(IP);
			ipElem->LinkEndChild(ip);
			TiXmlElement* puertoElem = new TiXmlElement("Puerto");
			TiXmlText* puerto = new TiXmlText(PUERTO);
			puertoElem->LinkEndChild(puerto);
			conexion->LinkEndChild(ipElem);
			conexion->LinkEndChild(puertoElem);
			cliente->LinkEndChild(conexion);

			TiXmlElement* mensajes = new TiXmlElement("Mensajes");
			TiXmlElement* mensajeUno = new TiXmlElement("Mensaje");
			TiXmlElement* idElem = new TiXmlElement("Id");
			TiXmlElement* tipoElem = new TiXmlElement("Tipo");
			TiXmlElement* valorElem = new TiXmlElement("Valor");
			TiXmlText* id = new TiXmlText("mensaje1");
			TiXmlText* tipo = new TiXmlText("string");
			TiXmlText* valor = new TiXmlText("Hola mundo");
			idElem->LinkEndChild(id);
			tipoElem->LinkEndChild(tipo);
			valorElem->LinkEndChild(valor);
			mensajeUno->LinkEndChild(idElem);
			mensajeUno->LinkEndChild(tipoElem);
			mensajeUno->LinkEndChild(valorElem);
			mensajes->LinkEndChild(mensajeUno);
			cliente->LinkEndChild(mensajes);
			archivoNuevo.SaveFile(nombreArchivo.c_str());
			this->xmlDocument.LoadFile(nombreArchivo.c_str());
		}else{
			cout<<"carga exitosa\n";
	}
}

XMLLoader::~XMLLoader() {
	this->xmlDocument.Clear();
	this->errorLogFile.close();
}
