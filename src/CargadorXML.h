#include "tinyxml.h"
#include <string>
#include <iostream>
#include <fstream>

#ifndef CARGADORXML_H_
#define CARGADORXML_H_

using namespace std;

class XMLLoader {
public:
	XMLLoader();
	void cargarCliente(string nombreArchivo);
	bool serverXMLIsValid(const char* fileName);
	virtual ~XMLLoader();

private:
	TiXmlDocument xmlDocument;
	ofstream errorLogFile;
	void writeErrorInFile(string error);
	void writeNotFoundFileForNameError(string fileName);
	void writeNotFoundElementInXML(string element);
	bool serverXMLHasValidValues(TiXmlDocument xmlFile);
	bool serverXMLHasValidElements(TiXmlDocument xmlFile);
	void writeValueErrorForElementInXML(string element);
};

#endif /* CARGADORXML_H_ */
