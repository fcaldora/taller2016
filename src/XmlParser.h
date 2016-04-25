#include "tinyxml.h"
#include "Constants.h"
#include "Sprite.h"
#include <iostream>
#ifndef XMLPARSER_H_
#define XMLPARSER_H_

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
	int getSprite(Sprite &sprite, int numSprite);
	int getElement(struct elemento &element, int elementNum);
	int getFondoEscenario(Sprite &fondo);

private:
	TiXmlDocument doc;
};

#endif /* XMLPARSER_H_ */
