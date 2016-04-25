/*
 * Avion.h
 *
 *  Created on: 24 de abr. de 2016
 *      Author: gusta
 */

#include <string>
#include "Sprite.h"
#ifndef AVION_H_
#define AVION_H_

using namespace std;

class Avion {
public:
	Avion();
	virtual ~Avion();
	void setVelDesplazamiento(int velDesplazamiento);
	int getVelDesplazamiento();
	void setVelDisparo(int velDisparo);
	int getVelDisparo();
	void moveVerical(int movement);
	void moveHorizontal(int movement);
	int getPosX();
	int getPosY();

private:
	int velDesplazamiento;
	int velDisparo;
	int posActualX;
	int posActualY;
	Sprite avionSprite;
	Sprite disparoSprite;
	Sprite vueltaSprite;

};

#endif /* AVION_H_ */
