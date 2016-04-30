/*
 * Procesador.h
 *
 *  Created on: 7 de abr. de 2016
 *      Author: gusta
 */
#include <string>
#include "Constants.h"
#ifndef PROCESADOR_H_
#define PROCESADOR_H_

using namespace std;

#include "ClientList.h"

class Procesador {
public:
	Procesador();
	Procesador(ClientList *clientList, int screenWidth, int screenHeight);
	virtual ~Procesador();

	void processMessage(clientMsj message);

private:
	ClientList *clientList;
	int screenWidth;
	int screenHeight;
};

#endif /* PROCESADOR_H_ */
