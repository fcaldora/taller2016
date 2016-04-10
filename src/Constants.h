/*
 * Constants.h
 *
 *  Created on: Apr 9, 2016
 *      Author: luciano
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

using namespace std;
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>

#define kLongChar 20

#define kServerTag "Servidor"
#define kConfigurationTag "configuracion"
#define kLogLevelTag "nivelDeLog"
#define kMaxNumberOfClientsTag "CantidadMaximaClientes"
#define kPortTag "Puerto"

struct clientMsj {
	char id[kLongChar];
	char type[kLongChar];
	char value[kLongChar];
};

enum LogLevelType {
	LogLevelTypeOnlyErrors = 1,
	LogLevelTypeEverything = 2
};

#endif /* CONSTANTS_H_ */
