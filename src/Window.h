/*
 * Window.h
 *
 *  Created on: 24 de abr. de 2016
 *      Author: gusta
 */

#ifndef WINDOW_H_
#define WINDOW_H_

class Window {
public:
	Window();
	int getWidth();
	int getHeight();
	void setWidth(int width);
	void setHeight(int height);
	virtual ~Window();
private:
	int width;
	int height;
};

#endif /* WINDOW_H_ */
