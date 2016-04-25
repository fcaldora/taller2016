/*
 * Sprite.h
 *
 *  Created on: 24 de abr. de 2016
 *      Author: gusta
 */
#include <string>
#ifndef SPRITE_H_
#define SPRITE_H_

using namespace std;

class Sprite {
public:
	Sprite();
	virtual ~Sprite();
	void setCantidad(int cantidad);
	void setWidth(int width);
	void setHeight(int height);
	void setPath(string path);
	void setId(string id);
	int getCantidad();
	int getWidth();
	int getHeight();
	string getId();
	string getPath();

private:
	string path;
	string id;
	int cantidad;
	int width;
	int height;
};

#endif /* SPRITE_H_ */
