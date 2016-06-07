#include "tinyxml.h"
#include "Constants.h"
#include "DrawableObject.h"
#include "Avion.h"
#include "PowerUp.h"
#include "EnemyPlane.h"
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
	int getSprite(DrawableObject &object, int numSprite);
	int getElement(DrawableObject &element, int elementNum);
	void getPowerUp(PowerUp& powerUp, int powerUpNum);
	void getEnemyPlane(EnemyPlane* enemyPlane, int enemyPlaneNum, list<Formation*> formations);
	void getFormation(Formation* formation, int formationNum);
	int getNumberOfPowerUp();
	int getNumberOfElements();
	int getNumberOfEnemyPlanes();
	int getNumberOfFormations();
	int getPosXPortaAviones();
	int getPosYPortaAviones();
	int getFondoEscenario(DrawableObject &object);
	Avion* getAvion(int nroCliente);
	void reloadDoc();
	int getMaxNumberOfPlayerPerTeam();

private:
	TiXmlDocument doc;
	string fileName;
};

#endif /* XMLPARSER_H_ */
