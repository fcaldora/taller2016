/*
 * Sprite.cpp
 *
 *  Created on: 24 de abr. de 2016
 *      Author: gusta
 */

#include "Sprite.h"

Sprite::Sprite() {
	cantidad = 0;
	height = 0;
	width = 0;
}

void Sprite::setCantidad(int cantidad){
	this->cantidad = cantidad;
}

void Sprite::setHeight(int height){
	this->height = height;
}

void Sprite::setId(string id){
	this->id = id;
}

void Sprite::setPath(string path){
	this->path = path;
}

void Sprite::setWidth(int width){
	this->width = width;
}

int Sprite::getCantidad(){
	return cantidad;
}

int Sprite::getHeight(){
	return height;
}

int Sprite::getWidth(){
	return width;
}

string Sprite::getId(){
	return id;
}

string Sprite::getPath(){
	return path;
}

Sprite::~Sprite() {
	// TODO Auto-generated destructor stub
}

