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
		strncpy(this->imagePath, path.c_str(),20);
	}

	int getPhotograms() const {
		return numberOfPhotograms;
	}

	void setPhotograms(int photograms) {
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

	int getActualPhotogram() const {
			return actualPhotogram;
		}

	void setActualPhotogram(int photogram) {
		this->actualPhotogram = photogram;
	}

	void setPosXInit(int posXInit){
		this->posXInit = posXInit;
	}

	void setPosYInit(int posYInit){
		this->posYInit = posYInit;
	}

	int getPosXInit(){
		return posXInit;
	}

	int getPosYInit(){
		return posYInit;
	}

protected:
	int id;
	char imagePath[20];
	int actualPhotogram;
	int numberOfPhotograms;
	int posX;
	int posY;
	int posXInit;
	int posYInit;
	int width;
	int heigth;
};

#endif /* OBJECT_H_ */
