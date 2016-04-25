#include "XmlParser.h"

XmlParser::XmlParser(const char* filename) {
	TiXmlDocument file;
	if(file.LoadFile(filename)){
		this->doc = file;
	}
	else cout<<"El archivo no existe"<<endl;
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

int XmlParser::getAnchoVentana(){
	int anchoValue = -1;
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* ventana = docHandle.FirstChild(tagVentana).ToElement();
	if(ventana == NULL){
		cout<<"Error al obtener la ventana"<<endl;
		return -1;
	}
	TiXmlElement* ancho = ventana->FirstChild(tagAncho)->ToElement();
	if(ancho == NULL){
		cout<<"Error al obtener el ancho de la ventana"<<endl;
		return -1;
	}
	anchoValue = atoi(ancho->GetText());
	return(anchoValue);
}

int XmlParser::getAltoVentana(){
	int altoValue = -1;
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* ventana = docHandle.FirstChild(tagVentana).ToElement();
	if(ventana == NULL){
		cout<<"Error al obtener la ventana"<<endl;
		return -1;
	}
	TiXmlElement* alto = ventana->FirstChild(tagAlto)->ToElement();
	if(alto == NULL){
		cout<<"Error al obtener el alto de la ventana"<<endl;
		return -1;
	}
	altoValue = atoi(alto->GetText());
	return(altoValue);
}

int XmlParser::getAltoEscenario(){
	int altoValue = -1;
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* escenario = docHandle.FirstChild(tagEscenario).ToElement();
	if(escenario == NULL){
		cout<<"Error al obtener el escenario"<<endl;
		return -1;
	}
	TiXmlElement* alto = escenario->FirstChild(tagAlto)->ToElement();
	if(alto == NULL){
		cout<<"Error al obtener el alto del escenario"<<endl;
		return -1;
	}
	altoValue = atoi(alto->GetText());
	return altoValue;
}

int XmlParser::getAnchoEscenario(){
	int anchoValue = -1;
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* escenario = docHandle.FirstChild(tagEscenario).ToElement();
	if(escenario == NULL){
		cout<<"Error al obtener el escenario"<<endl;
		return -1;
	}
	TiXmlElement* ancho = escenario->FirstChild(tagAncho)->ToElement();
	if(ancho == NULL){
		cout<<"Error al obtener el ancho del escenario"<<endl;
		return -1;
	}
	anchoValue = atoi(ancho->GetText());
	return(anchoValue);
}

int XmlParser::getVelocidadDespAvion(){
	int velocidad = -1;
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* avion = docHandle.FirstChild(tagAvion).ToElement();
	if(avion == NULL){
			cout<<"Error al obtener el avion"<<endl;
			return -1;
		}
	TiXmlElement* velocidadElem = avion->FirstChild(tagVelDesplazamiento)->ToElement();
	if(velocidadElem == NULL){
			cout<<"Error al obtener la velocidad de desplazamiento"<<endl;
			return -1;
		}
	velocidad = atoi(velocidadElem->GetText());
	return velocidad;
}

int XmlParser::getVelocidadDispAvion(){
	int velocidad = -1;
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* avion = docHandle.FirstChild(tagAvion).ToElement();
	if(avion == NULL){
			cout<<"Error al obtener el avion"<<endl;
			return -1;
		}
	TiXmlElement* velocidadDisp = avion->FirstChild(tagVelDisparo)->ToElement();
	if(velocidadDisp == NULL){
			cout<<"Error al obtener la velocidad de disparo"<<endl;
			return -1;
		}
	velocidad = atoi(velocidadDisp->GetText());
	return velocidad;
}

int XmlParser::getSprite(Sprite &sprite, int numSprite){
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* spritesElem = docHandle.FirstChildElement(tagSprites).ToElement();
	TiXmlElement* aux = spritesElem->FirstChildElement(tagSprite);

	for (int i = 0; i < numSprite; i++){
		aux = aux->NextSiblingElement(tagSprite);
	}
	if(aux == NULL)
		return -1;

	TiXmlElement* pathElem = aux->FirstChildElement(tagPath);
	sprite.setPath(string(pathElem->GetText()));
	TiXmlElement* cantElem = aux->FirstChildElement(tagCantidad);
	sprite.setCantidad(atoi(cantElem->GetText()));
	TiXmlElement* altoElem = aux->FirstChildElement(tagAlto);
	sprite.setHeight(atoi(altoElem->GetText()));
	TiXmlElement* anchoElem = aux->FirstChildElement(tagAncho);
	sprite.setWidth(atoi(anchoElem->GetText()));
	TiXmlElement* idElem = aux->FirstChildElement(tagId);
	sprite.setId(string(idElem->GetText()));
	return 0;
}

int XmlParser::getElement(struct elemento &element, int elementNum){
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* escenarioElem = docHandle.FirstChild(tagEscenario).ToElement();
	if(escenarioElem == NULL){
		cout<<"Error al obtener el escenario"<<endl;
		return -1;
	}
	TiXmlElement* elementosElem = escenarioElem->FirstChild(tagElementos)->ToElement();
	if(elementosElem == NULL){
		cout<<"Error al obtener los elementos"<<endl;
		return -1;
	}
	TiXmlElement* elementoElem = elementosElem->FirstChild(tagElemento)->ToElement();

	for(int i=0;i<elementNum;i++)
		elementoElem = elementoElem->NextSiblingElement(tagElemento);

	if(elementoElem == NULL){
		return -1;
	}

	TiXmlElement* idElem = elementoElem->FirstChildElement(tagSpriteId);
	strncpy(element.id, idElem->GetText(),20);
	TiXmlElement* posicionElem = elementoElem->FirstChildElement(tagPosicion);
	TiXmlElement* posxElem = posicionElem->FirstChildElement(tagPosX);
	element.x = atoi(posxElem->GetText());
	TiXmlElement* posyElem = posicionElem->FirstChildElement(tagPosY);
	element.y = atoi(posyElem->GetText());
	return 0;
}

int XmlParser::getFondoEscenario(Sprite &fondo){
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* escenarioElem = docHandle.FirstChild(tagEscenario).ToElement();
	if(escenarioElem == NULL){
		cout<<"Error al obtener el escenario"<<endl;
		return -1;
	}
	TiXmlElement* fondoElem = docHandle.FirstChildElement(tagFondo).ToElement();
	if(fondoElem == NULL){
		cout<<"Error al obtener el fondo del escenario"<<endl;
		return -1;
	}
	TiXmlElement* idElem = fondoElem->FirstChildElement(tagSpriteId);
	fondo.setId(idElem->GetText());
	TiXmlElement* anchoElem = fondoElem->FirstChildElement(tagAncho);
	fondo.setWidth(atoi(anchoElem->GetText()));
	TiXmlElement* altoElem = fondoElem->FirstChildElement(tagAlto);
	fondo.setHeight(atoi(altoElem->GetText()));
	return 0;
}
XmlParser::~XmlParser() {
	// TODO Auto-generated destructor stub
}
