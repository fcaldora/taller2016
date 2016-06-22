/*
 * Avion.h
 *
 *  Created on: 24 de abr. de 2016
 *      Author: gusta
 */

#include <string>
#ifndef AVION_H_
#define AVION_H_

#include "DrawableObject.h"

using namespace std;

class Avion: public DrawableObject {
public:
	Avion();
	virtual ~Avion();

	void setVelDesplazamiento(int velDesplazamiento);
	int getVelDesplazamiento();
	void setVelDisparo(int velDisparo);
	int getVelDisparo();
	void moveVerical(int movement);
	void moveHorizontal(int movement);
	void setPhotogram();
	void moveOneStepRight();
	void moveOneStepLeft();
	void moveOneStepUp();
	void moveOneStepDown();
	bool updatePhotogram(bool aterrizaje, bool gameInitiated);
	bool doubleShooting();
	void setDoubleShooting(bool);
	void aterrizar(int finishX);
	bool isLooping;
	void setIfIsStarting(bool starting);
	int getLifes() const {
		return lifes;
	}
	void resetLandingCounter();
	void setLifes(int lifes) {
		this->lifes = lifes;
	}

private:
	int velDesplazamiento;
	int velDisparo;
	bool hasDoubleShooting;
	int lifes;
	int lastRollPhotogram;
	int landingCounter;
	bool isStarting;
	int startingCounter;
};

#endif /* AVION_H_ */
