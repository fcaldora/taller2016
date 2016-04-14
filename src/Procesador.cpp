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
		if(!std::isdigit(*it) && *it!='+' && *it!='-')
			return false;
		it++;
	}
	return true;
}

bool Procesador::isADouble(string string){
	//Si no contiene ningun punto, no es un double.
	//if(string.find(".")==string::npos){
	//	return false;
	//}
	string::const_iterator it = string.begin();
	while(it != string.end()){
		if(!std::isdigit(*it) && *it!= '.' && *it!='e' && *it!='+' && *it!='-' ){
			return false;
		}
		it++;
	}
	return true;
}

bool Procesador::isMsgValid(string type, string valor){
	std::transform(type.begin(), type.end(), type.begin(), ::toupper);
	if(type.compare("INT")==0)
		return(isAnInt(valor));
	if(type.compare("CHAR")==0)
		return(isAChar(valor));
	if(type.compare("DOUBLE")==0)
		return(isADouble(valor));
	if(type.compare("STRING")==0)
		return true;
	return false;
}
Procesador::~Procesador() {
	// TODO Auto-generated destructor stub
}

