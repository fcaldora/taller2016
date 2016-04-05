#include "tinyxml.h"
#include <string>
#include <iostream>
#include <fstream>

#ifndef CARGADORXML_H_
#define CARGADORXML_H_

using namespace std;

class CargadorXML {
public:
	CargadorXML();
	void cargarServidor(string nombreArchivo);
	void cargarCliente(string nombreArchivo);
	TiXmlDocument* getDocumento();
	bool serverXMLIsValid(const char* fileName);
	virtual ~CargadorXML();

private:
	TiXmlDocument archivo;
	ofstream archivoErrores;
	void writeErrorInFile(string error);
	void writeNotFoundFileForNameError(string fileName);
	void writeNotFoundElementInXML(string element);
	bool serverXMLHasValidValues(TiXmlDocument xmlFile);
	bool serverXMLHasValidElements(TiXmlDocument xmlFile);
	void writeValueErrorForElementInXML(string element);
};

#endif /* CARGADORXML_H_ */
