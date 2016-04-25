/*
 * Window.cpp
 *
 *  Created on: 24 de abr. de 2016
 *      Author: gusta
 */

#include "Window.h"

Window::Window() {
	height = 0;
	width = 0;

}

int Window::getHeight(){
	return height;
}

int Window::getWidth(){
	return width;
}

void Window::setHeight(int height){
	this->height = height;
}

void Window::setWidth(int width){
	this->width = width;
}

Window::~Window() {
	// TODO Auto-generated destructor stub
}

