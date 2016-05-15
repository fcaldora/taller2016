/*
 * Object.h
 *
 *  Created on: Apr 24, 2016
 *      Author: facundo
 */

#ifndef OBJECT_H_
#define OBJECT_H_
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

class Object {
public:
	Object();
	virtual ~Object();

	int getHeigth() const {
		return heigth;
	}

	void setHeigth(int heigth) {
		this->heigth = heigth;
	}

	int getId() const {
		return id;
	}

	void setId(int id) {
		this->id = id;
	}

	const string& getPath() const {
		return imagePath;
	}

	void setPath(const string& path) {
		this->imagePath = path;
	}

	int getPhotograms() const {
		return photograms;
	}

	void setPhotograms(int photograms) {
		this->photograms = photograms;
	}

	int getPosX() const {
		return posX;
	}

	void setPosX(int posX) {
		this->posX = posX;
	}

	int getPosY() const {
		return posY;
	}

	void setPosY(int posY) {
		this->posY = posY;
	}

	int getWidth() const {
		return width;
	}

	void setWidth(int width) {
		this->width = width;
	}

	int getActualPhotogram() const {
			return actualPhotogram;
		}

	void setActualPhotogram(int photogram) {
		this->actualPhotogram = photogram;
	}

	bool isStatic(){
		return this->status;
	}

	void setStatus(bool status){
		this->status = status;
	}

	void move();

	bool notVisible(int width, int height);

	void setStep(int step){
		this->step = step;
	}

private:
	int id;
	string imagePath;
	int actualPhotogram;
	int photograms;
	int posX;
	int posY;
	int width;
	int heigth;
	int step;
	bool status;


};

#endif /* OBJECT_H_ */
