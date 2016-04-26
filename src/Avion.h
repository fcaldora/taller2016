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

#include "DrawableObject.h"

using namespace std;

class Avion: public DrawableObject {
public:
	Avion(int velDesplazamiento, int velDisparo, int numberOfPhotograms,
			int actualPhotogram, int postionX, int positionY, int height, int width, string imagePath);
	virtual ~Avion();

	void setVelDesplazamiento(int velDesplazamiento);
	int getVelDesplazamiento();
	void setVelDisparo(int velDisparo);
	int getVelDisparo();
	void moveVerical(int movement);
	void moveHorizontal(int movement);

	void moveOneStepRight();
	void moveOneStepLeft();
	void moveOneStepUp();
	void moveOneStepDown();

private:
	int velDesplazamiento;
	int velDisparo;
	Sprite *avionSprite;
	Sprite *disparoSprite;
	Sprite *vueltaSprite;
};

#endif /* AVION_H_ */
