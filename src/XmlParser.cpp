#include "XmlParser.h"

XmlParser::XmlParser(const char* fileName) {
	TiXmlDocument xmlFile(fileName);
	if (xmlFile.LoadFile()) {
		this->doc = xmlFile;
	}
}

int XmlParser::getServerPort(){
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* puertoElem = docHandle.FirstChild("Servidor").FirstChild("Puerto").ToElement();

	if(puertoElem)
		return atoi(puertoElem->GetText());
	else {
		cout<<"Error al obtener el puerto";
		return 8080;
	}
}

int XmlParser::getMaxNumberOfClients(){
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* maxElem = docHandle.FirstChild("Servidor").FirstChild("CantidadMaximaClientes").ToElement();

	if (maxElem)
		return atoi(maxElem->GetText());
	else {
		cout<<"Error al obtener el numero maximo de clientes";
		return 0;
	}
}

LogLevelType XmlParser::getLogLevel() {
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement *configurationElement = docHandle.FirstChild(kServerTag).FirstChild(kConfigurationTag).ToElement();
	if (configurationElement == NULL)
		return LogLevelTypeOnlyErrors;
	TiXmlElement *logLevelElement = configurationElement->FirstChild(kLogLevelTag)->ToElement();
	if(logLevelElement == NULL)
		return LogLevelTypeOnlyErrors;

	const char* logLevelChar = logLevelElement->GetText();
	std::stringstream logLevelStrValue;
	logLevelStrValue << logLevelChar;
	unsigned int logLevelValue;
	logLevelStrValue >> logLevelValue;

	switch(logLevelValue){
	case 1:
		return LogLevelTypeOnlyErrors;
	case 2:
		return LogLevelTypeEverything;
	default :
		return LogLevelTypeOnlyErrors;
	}
}

XmlParser::~XmlParser() {
	// TODO Auto-generated destructor stub
}

