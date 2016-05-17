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

int XmlParser::getSprite(DrawableObject &object, int numSprite){
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* spritesElem = docHandle.FirstChildElement(tagSprites).ToElement();
	TiXmlElement* aux = spritesElem->FirstChildElement(tagSprite);

	for (int i = 0; i < numSprite; i++){
		aux = aux->NextSiblingElement(tagSprite);
	}
	if(aux == NULL)
		return -1;

	TiXmlElement* pathElem = aux->FirstChildElement(tagPath);
	object.setPath(string(pathElem->GetText()));
	TiXmlElement* cantElem = aux->FirstChildElement(tagCantidad);
	object.setPhotograms(atoi(cantElem->GetText()));
	TiXmlElement* altoElem = aux->FirstChildElement(tagAlto);
	object.setHeigth(atoi(altoElem->GetText()));
	TiXmlElement* anchoElem = aux->FirstChildElement(tagAncho);
	object.setWidth(atoi(anchoElem->GetText()));
	TiXmlElement* idElem = aux->FirstChildElement(tagId);
	object.setId(atoi(idElem->GetText()));
	return 0;
}

int XmlParser::getNumberOfElements(){
	int result = 0;
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

	TiXmlElement* elemento = elementosElem->FirstChildElement(tagElemento);
	while(elemento != NULL){
		result++;
		elemento = elemento->NextSiblingElement(tagElemento);
	}
	return result;
}

DrawableObject* XmlParser::getElementAtIndex(int elementNum){
	DrawableObject *element = new DrawableObject();

	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* escenarioElem = docHandle.FirstChild(tagEscenario).ToElement();
	if(escenarioElem == NULL){
		cout<<"Error al obtener el escenario"<<endl;
		return NULL;
	}
	TiXmlElement* elementosElem = escenarioElem->FirstChild(tagElementos)->ToElement();
	if(elementosElem == NULL){
		cout<<"Error al obtener los elementos"<<endl;
		return NULL;
	}
	TiXmlElement* elementoElem = elementosElem->FirstChild(tagElemento)->ToElement();

	for(int i=0;i<elementNum;i++)
		elementoElem = elementoElem->NextSiblingElement(tagElemento);

	if(elementoElem == NULL){
		return NULL;
	}

	TiXmlElement* idElem = elementoElem->FirstChildElement(tagId);
	element->setId(atoi(idElem->GetText()));
	TiXmlElement* posxElem = elementoElem->FirstChildElement(tagPosX);
	element->setPosX(atoi(posxElem->GetText()));
	element->setPosXInit(atoi(posxElem->GetText()));

	TiXmlElement* posyElem = elementoElem->FirstChildElement(tagPosY);
	element->setPosY(atoi(posyElem->GetText()));
	element->setPosYInit(atoi(posyElem->GetText()));

	TiXmlElement* altoElem = elementoElem->FirstChildElement(tagAlto);
	element->setHeigth(atoi(altoElem->GetText()));
	TiXmlElement* anchoElem = elementoElem->FirstChildElement(tagAncho);
	element->setWidth(atoi(anchoElem->GetText()));
	TiXmlElement* pathElem = elementoElem->FirstChildElement(tagPath);
	element->setPath(pathElem->GetText());

	return element;
}

Escenario* XmlParser::getFondoEscenario(){
	Escenario *object = new Escenario();

	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* escenarioElem = docHandle.FirstChild(tagEscenario).ToElement();
	if(escenarioElem == NULL){
		cout<<"Error al obtener el escenario"<<endl;
		return NULL;
	}
	TiXmlElement* fondoElem = escenarioElem->FirstChildElement(tagFondo)->ToElement();
	if(fondoElem == NULL){
		cout<<"Error al obtener el fondo del escenario"<<endl;
		return NULL;
	}
	TiXmlElement* idElem = fondoElem->FirstChildElement(tagSpriteId);
	object->setId(atoi(idElem->GetText()));
	TiXmlElement* anchoElem = fondoElem->FirstChildElement(tagAncho);
	object->setWidth(atoi(anchoElem->GetText()));
	TiXmlElement* altoElem = fondoElem->FirstChildElement(tagAlto);
	object->setHeigth(atoi(altoElem->GetText()));
	TiXmlElement* pathElem = fondoElem->FirstChildElement(tagPath);
	object->setPath(pathElem->GetText());

	object->setScrollingStep(1);
	object->setHeigth(this->getAltoEscenario());
	object->setWindowHeight(this->getAltoVentana());

	return object;
}

Avion* XmlParser::getAvion(int nroCliente){
	Avion *avion = new Avion();

	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* avionesElem = docHandle.FirstChild(tagAviones).ToElement();
	if(avionesElem == NULL){
		cout<<"Error al obtener los aviones"<<endl;
		return NULL;
	}
	TiXmlElement* avionElem = avionesElem->FirstChildElement(tagAvion);
	for(int i = 1; i < nroCliente; i++){
		avionElem = avionElem->NextSiblingElement(tagAvion);
	}

	TiXmlElement* pathElem = avionElem->FirstChildElement(tagPath);
	avion->setPath(pathElem->GetText());
	TiXmlElement* posXElem = avionElem->FirstChildElement(tagPosX);
	avion->setPosX(atoi(posXElem->GetText()));
	TiXmlElement* posYElem = avionElem->FirstChildElement(tagPosY);
	avion->setPosY(atoi(posYElem->GetText()));
	TiXmlElement* anchoElem = avionElem->FirstChildElement(tagAncho);
	avion->setWidth(atoi(anchoElem->GetText()));
	TiXmlElement* altoElem = avionElem->FirstChildElement(tagAlto);
	avion->setHeigth(atoi(altoElem->GetText()));
	TiXmlElement* idElem = avionElem->FirstChildElement(tagId);
	avion->setId(atoi(idElem->GetText()));
	TiXmlElement* velDisparoElem = avionElem->FirstChildElement(tagVelDisparo);
	avion->setVelDisparo(atoi(velDisparoElem->GetText()));
	TiXmlElement* velDesplElem = avionElem->FirstChildElement(tagVelDesplazamiento);
	avion->setVelDesplazamiento(atoi(velDesplElem->GetText()));
	TiXmlElement* cantElem = avionElem->FirstChildElement(tagCantidad);
	avion->setPhotograms(atoi(cantElem->GetText()));

	return avion;
}

XmlParser::~XmlParser() {
	// TODO Auto-generated destructor stub
}
