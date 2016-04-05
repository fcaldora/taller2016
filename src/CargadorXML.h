#include "tinyxml.h"
#include <string>
#include <iostream>
#include <fstream>

#ifndef CARGADORXML_H_
#define CARGADORXML_H_

#include "ErrorLogWriter.h"
using namespace std;

class XMLLoader {

public:
	XMLLoader(ErrorLogWriter *errorLogWriter);
	bool serverXMLIsValid(const char* fileName);
	virtual ~XMLLoader();

private:
	ErrorLogWriter *errorLogWriter;
	TiXmlDocument xmlDocument;
	bool serverXMLHasValidValues(TiXmlDocument xmlFile);
	bool serverXMLHasValidElements(TiXmlDocument xmlFile);
};

#endif /* CARGADORXML_H_ */
