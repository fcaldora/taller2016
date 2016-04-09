#include "tinyxml.h"
#include <string>
#include <iostream>
#include <fstream>

#ifndef CARGADORXML_H_
#define CARGADORXML_H_

#include "LogWriter.h"
#include "Constants.h"

class XMLLoader {

public:
	XMLLoader(LogWriter *errorLogWriter);
	bool serverXMLIsValid(const char* fileName);
	virtual ~XMLLoader();

private:
	LogWriter *errorLogWriter;
	TiXmlDocument xmlDocument;
	bool serverXMLHasValidValues(TiXmlDocument xmlFile);
	bool serverXMLHasValidElements(TiXmlDocument xmlFile);
};

#endif /* CARGADORXML_H_ */
