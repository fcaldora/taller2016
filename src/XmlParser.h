#include "tinyxml.h"
#include "Constants.h"
#include "DrawableObject.h"
#include "Avion.h"
#include <iostream>
#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include "Escenario.h"

using namespace std;

class XmlParser {
public:
	XmlParser(const char* filename);
	virtual ~XmlParser();
	int getServerPort();
	int getMaxNumberOfClients();
	LogLevelType getLogLevel();
	int getAnchoVentana();
	int getAltoVentana();
	int getAnchoEscenario();
	int getAltoEscenario();
	int getVelocidadDespAvion();
	int getVelocidadDispAvion();
	int getSprite(DrawableObject &object, int numSprite);
	int getElement(DrawableObject &element, int elementNum);
	int getNumberOfElements();
	Escenario* getFondoEscenario();
	int getAvion(Avion* avion, int nroCliente);

private:
	TiXmlDocument doc;
};

#endif /* XMLPARSER_H_ */
