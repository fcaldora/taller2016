/*
 * Procesador.cpp
 *
 *  Created on: 7 de abr. de 2016
 *      Author: gusta
 */

#include "Procesador.h"

Procesador::Procesador() {
	// TODO Auto-generated constructor stub

}



//Si el string tiene un solo caracter, entonces es un char.
bool Procesador::isAChar(string string){
	return (string.size()==1);
}

bool Procesador::isAnInt(string string){
	string::const_iterator it = string.begin();
	while(it != string.end()){
		if(!std::isdigit(*it))
			return false;
		it++;
	}
	return true;
}

bool Procesador::isADouble(string string){
	//Si no contiene ningun punto, no es un double.
	if(string.find(".")==string::npos){
		return false;
	}
	string::const_iterator it = string.begin();
	while(it != string.end()){
		if(!std::isdigit(*it) && *it!= '.' ){
			return false;
		}
		it++;
	}
	return true;
}

bool Procesador::isMsgValid(string type, string valor){
	if(type.compare("INT"))
		return(isAnInt(valor));
	if(type.compare("CHAR"))
		return(isAChar(valor));
	if(type.compare("DOUBLE"))
		return(isADouble(valor));
	return false;
}
Procesador::~Procesador() {
	// TODO Auto-generated destructor stub
}

