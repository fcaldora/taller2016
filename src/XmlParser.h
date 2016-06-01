#include "tinyxml.h"
#include "Constants.h"
#include "DrawableObject.h"
#include "Avion.h"
#include <iostream>
#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include "LifeObject.h"

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
	int getFondoEscenario(DrawableObject &object);
	Avion* getAvion(int nroCliente);
	void reloadDoc();
	int getMaxNumberOfPlayerPerTeam();
	int getNumberOfLifesPerClient();
	vector<LifeObject*> getLifeObjects();

private:
	TiXmlDocument doc;
	string fileName;
};

#endif /* XMLPARSER_H_ */
