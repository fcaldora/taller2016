#include <string>
#include <iostream>
#include "tinyxml.h"

#ifndef XMLPARSER_H_
#define XMLPARSER_H_

#include "Constants.h"

class XmlParser {
public:
	XmlParser(const char* fileName);
	virtual ~XmlParser();
	int getServerPort();
	int getMaxNumberOfClients();
	LogLevelType getLogLevel();
private:
	TiXmlDocument doc;
};

#endif /* XMLPARSER_H_ */
