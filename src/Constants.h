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

struct clientMsj {
	char id[kLongChar];
	char type[kLongChar];
	char value[kLongChar];
};

struct actionMsj{
	char action[kLongChar];
};

struct updateMsj{
	int id;
	int posX;
	int posY;
	int actualPhotogram;
};

struct deleteMsj{
	int id;
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
