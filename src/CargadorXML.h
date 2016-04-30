#include "tinyxml.h"
#include <string>
#include <iostream>
#include <fstream>

#ifndef CARGADORXML_H_
#define CARGADORXML_H_

#include "LogWriter.h"
#include "Constants.h"
#include "Escenario.h"
#include "XmlParser.h"
#include "Avion.h"

class XMLLoader {

public:
	XMLLoader(LogWriter *errorLogWriter);
	bool serverXMLIsValid(const char* fileName);
	virtual ~XMLLoader();
	bool cargarEscenario(Escenario &escenario, string pathDoc);
	bool cargarAvion(Avion &avion, string pathDoc);
	bool obtenerSprite(DrawableObject &sprite, int numSprite, string pathDoc);
	bool cargarElemento(elemento &elemento, int numElemento, string pathDoc);

private:
	LogWriter *errorLogWriter;
	TiXmlDocument xmlDocument;
	bool serverXMLHasValidValues(TiXmlDocument xmlFile);
	bool serverXMLHasValidElements(TiXmlDocument xmlFile);
};

#endif /* CARGADORXML_H_ */
