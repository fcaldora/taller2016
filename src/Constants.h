/*
 * Constants.h
 *
 *  Created on: Apr 9, 2016
 *      Author: luciano
 */
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <algorithm>

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

using namespace std;

#define kLongChar 20
#define LONG_ID 20
#define LONG_PATH 20
#define kServerTag "Servidor"
#define kConfigurationTag "configuracion"
#define kLogLevelTag "nivelDeLog"
#define kMaxNumberOfClientsTag "CantidadMaximaClientes"
#define kPortTag "Puerto"
#define tagVentana "ventana"
#define tagAlto "alto"
#define tagAncho "ancho"
#define tagSprites "sprites"
#define tagSprite "sprite"
#define tagId "id"
#define tagPath "path"
#define tagCantidad "cantidad"
#define tagEscenario "escenario"
#define tagFondo "fondo"
#define tagPosX "x"
#define tagPosY "y"
#define tagElementos "elementos"
#define tagElemento "elemento"
#define tagPosicion "posicion"
#define tagSpriteId "spriteId"
#define tagAvion "avion"
#define tagVelDesplazamiento "velocidadDesplazamiento"
#define tagVelDisparo "velocidadDisparos"
#define tagAvionSpriteId "avionSpriteId"
#define tagVueltaSpriteID "vueltaSpriteId"
#define tagDisparosSpriteId "disparosSpriteId"
#define tagAviones "aviones"
#define tagPowerUps "powerUps"
#define tagEnemyPlanes "enemyPlanes"
#define tagEnemyPlane "enemyPlane"
#define tagPowerUp "powerUp"
#define tagPointsToAdd "points"
#define tagType "type"
#define tagLifes "lifes"
#define tagCrazy "crazyMoves"
#define tagScore "score"
#define tagLastHitScore "lastHitScore"
#define tagFormation "formation"
#define tagFormations "formations"
#define tagExtraPoints "extraPoints"
#define tagFacingDirection "facingDirection"
#define tagQuantity "quantity"
#define tagFormationId "formationId"
#define tagActualPhotogram "actualPhotogram"
#define tagPhotograms "photograms"
#define DOUBLESHOOTING 1
#define KILLENEMIES 2
#define ADDINGPOINTS 3

struct sprite{
	char id[LONG_ID];
	char path[LONG_PATH];
	int cantidad;
	int alto;
	int ancho;
};
struct elemento{
	int id;
	int x;
	int y;
};
struct fondoEscenario{
	char id[LONG_ID];
	int ancho;
	int alto;
};
struct escenario{
	int ancho;
	int algo;
	struct fondoEscenario fondo;
	list<struct elemento> elementos;
};

struct clientMsj {
	char id[kLongChar];
	char type[kLongChar];
	char value[kLongChar];
};

struct mensaje {
	char action[kLongChar];
	int id;
	char imagePath[kLongChar];
	int posX;
	int posY;
	int width;
	int height;
	bool activeState;
	int actualPhotogram;
	int photograms;
	char errorMsj[kLongChar];
};

enum LogLevelType {
	LogLevelTypeOnlyErrors = 1,
	LogLevelTypeEverything = 2
};

#endif /* CONSTANTS_H_ */
