/*
 * Procesador.h
 *
 *  Created on: 7 de abr. de 2016
 *      Author: gusta
 */
#include <string>
#ifndef PROCESADOR_H_
#define PROCESADOR_H_

using namespace std;

class Procesador {
public:
	Procesador();
	bool isMsgValid(string type, string string);
	virtual ~Procesador();
private:
	bool isAnInt(string string);
	bool isAChar(string string);
	bool isADouble(string string);
};

#endif /* PROCESADOR_H_ */
