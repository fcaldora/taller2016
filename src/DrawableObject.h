/*
 * Object.h
 *
 *  Created on: Apr 24, 2016
 *      Author: facundo
 */

#ifndef DrawableObject_H_
#define DrawableObject_H_
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>


using namespace std;

class DrawableObject {
public:
	DrawableObject();
	virtual ~DrawableObject();

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

	string getPath() {
		return string(imagePath);
	}

	void setPath(string path) {
		this->imagePath = path;
	}

	int getNumberOfPhotograms() const {
		return numberOfPhotograms;
	}

	void setNumberOfPhotograms(int photograms) {
		this->numberOfPhotograms = photograms;
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

	int getActualPhotogramNumber() const {
			return actualPhotogramNumber;
		}

	void setActualPhotogramNumber(int photogramNumber) {
		this->actualPhotogramNumber = photogramNumber;
	}

	bool isStatic(){
		return this->status;
	}

	void setStatus(bool status){
		this->status = status;
	}

protected:
	int id;
	string imagePath;
	int actualPhotogramNumber;
	int numberOfPhotograms;
	int posX;
	int posY;
	int width;
	int heigth;
	bool status;

};

#endif /* OBJECT_H_ */
