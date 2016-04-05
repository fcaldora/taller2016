#include <string>
#include <iostream>
#include "tinyxml.h"

#define LONGCHAR 20

struct clientMsj {
	char id[LONGCHAR];
	char type[LONGCHAR];
	char value[LONGCHAR];
};

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

using namespace std;

class XmlParser {
public:
	XmlParser(const char* fileName);
	virtual ~XmlParser();
	void obtenerMensaje(clientMsj &mensaje, int nroMensaje);
	int getServerPort();
	void obtenerPuertoCl(int &puerto);
	void obtenerIp(string &ip);
	int getMaxNumberOfClients();
private:
	TiXmlDocument doc;
};

#endif /* XMLPARSER_H_ */
