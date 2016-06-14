#include "XmlParser.h"

XmlParser::XmlParser(const char* filename) {
	TiXmlDocument file;
	if(file.LoadFile(filename)){
		this->doc = file;
		this->fileName = string(filename);
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
	return this->getMaxNumberOfPlayerPerTeam() * 2;
}

int XmlParser::getMaxNumberOfPlayerPerTeam() {
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* maxElem = docHandle.FirstChild("Servidor").FirstChild(kMaxNumberOfPlayersPerTeamTag).ToElement();

	if (maxElem)
		return atoi(maxElem->GetText());
	else {
		cout<<"Error al obtener el numero maximo de clientes";
		return 0;
	}
}

int XmlParser::getFirstBulletId() {
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement *firstBulletElement = docHandle.FirstChild(kServerTag).FirstChild(kFirstBulletIDTag).ToElement();

	if (firstBulletElement) {
 		return atoi(firstBulletElement->GetText());
	} else {
		return 100;
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

int XmlParser::getElement(DrawableObject &element, int elementNum){
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

	TiXmlElement* idElem = elementoElem->FirstChildElement(tagId);
	element.setId(atoi(idElem->GetText()));
	TiXmlElement* posxElem = elementoElem->FirstChildElement(tagPosX);
	element.setPosX(atoi(posxElem->GetText()));
	TiXmlElement* posyElem = elementoElem->FirstChildElement(tagPosY);
	element.setPosY(atoi(posyElem->GetText()));
	TiXmlElement* altoElem = elementoElem->FirstChildElement(tagAlto);
	element.setHeigth(atoi(altoElem->GetText()));
	TiXmlElement* anchoElem = elementoElem->FirstChildElement(tagAncho);
	element.setWidth(atoi(anchoElem->GetText()));
	TiXmlElement* pathElem = elementoElem->FirstChildElement(tagPath);
	element.setPath(pathElem->GetText());
	return 0;
}

int XmlParser::getFondoEscenario(DrawableObject &object){
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* escenarioElem = docHandle.FirstChild(tagEscenario).ToElement();
	if(escenarioElem == NULL){
		cout<<"Error al obtener el escenario"<<endl;
		return -1;
	}
	TiXmlElement* fondoElem = escenarioElem->FirstChildElement(tagFondo)->ToElement();
	if(fondoElem == NULL){
		cout<<"Error al obtener el fondo del escenario"<<endl;
		return -1;
	}
	TiXmlElement* idElem = fondoElem->FirstChildElement(tagSpriteId);
	object.setId(atoi(idElem->GetText()));
	TiXmlElement* anchoElem = fondoElem->FirstChildElement(tagAncho);
	object.setWidth(atoi(anchoElem->GetText()));
	TiXmlElement* altoElem = fondoElem->FirstChildElement(tagAlto);
	object.setHeigth(atoi(altoElem->GetText()));
	TiXmlElement* pathElem = fondoElem->FirstChildElement(tagPath);
	object.setPath(pathElem->GetText());
	return 0;
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
	TiXmlElement* lifes = avionElem->FirstChildElement(tagLifes);
	avion->setLifes(atoi(lifes->GetText()));
	return avion;
}

void XmlParser::reloadDoc(){
	if(!this->doc.LoadFile(this->fileName.c_str())){
		cout<<"Error al recargar el archivo.Archivo inexistente"<<endl;
	}
}

void XmlParser::getPowerUp(PowerUp& powerUp, int powerUpNum){
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* powerUpsElement = docHandle.FirstChild(tagPowerUps).ToElement();
	TiXmlElement* powerUpElement = powerUpsElement->FirstChildElement(tagPowerUp);
	for (int i = 0; i < powerUpNum ; i++){
		powerUpElement = powerUpElement->NextSiblingElement(tagPowerUp);
	}
	if ( powerUpElement == NULL){
		cout<<"Error al cargar el powerUp desde archivo"<<endl;
		return;
	}
	TiXmlElement* typeElement = powerUpElement->FirstChildElement(tagType);
	powerUp.setType(atoi(typeElement->GetText()));
	if (powerUp.getType() == ADDINGPOINTS){
		TiXmlElement* pointsElement = powerUpElement->FirstChildElement(tagPointsToAdd);
		powerUp.setPointsToAdd(atoi(pointsElement->GetText()));
	}
	TiXmlElement* idElement = powerUpElement->FirstChildElement(tagId);
	powerUp.setId(atoi(idElement->GetText()));
	TiXmlElement* altoElement = powerUpElement->FirstChildElement(tagAlto);
	powerUp.setHeigth(atoi(altoElement->GetText()));
	TiXmlElement* anchoElement = powerUpElement->FirstChildElement(tagAncho);
	powerUp.setWidth(atoi(anchoElement->GetText()));
	TiXmlElement* xElement = powerUpElement->FirstChildElement(tagPosX);
	powerUp.setPosX(atoi(xElement->GetText()));
	TiXmlElement* yElement = powerUpElement->FirstChildElement(tagPosY);
	powerUp.setPosY(atoi(yElement->GetText()));
	TiXmlElement* pathElement = powerUpElement->FirstChildElement(tagPath);
	powerUp.setPath(pathElement->GetText());
}

int XmlParser::getNumberOfPowerUp(){
	int quantity = 0;
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* powerUpsElement = docHandle.FirstChild(tagPowerUps).ToElement();
	TiXmlElement* powerUpElement = powerUpsElement->FirstChildElement(tagPowerUp);
	while(powerUpElement != NULL){
		quantity++;
		powerUpElement = powerUpElement->NextSiblingElement(tagPowerUp);
	}
	return quantity;
}

void XmlParser::getEnemyPlane(EnemyPlane* enemyPlane, int enemyPlaneNum, list<Formation*> formations){
	list<Formation*>::iterator it;
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* enemyPlanesElement = docHandle.FirstChild(tagEnemyPlanes).ToElement();
	TiXmlElement* enemyPlaneElement = enemyPlanesElement->FirstChildElement(tagEnemyPlane);
	for (int i = 0; i < enemyPlaneNum ; i++){
		enemyPlaneElement = enemyPlaneElement->NextSiblingElement(tagEnemyPlane);
	}
	if ( enemyPlaneElement == NULL){
		cout<<"Error al cargar el avion enemigo desde archivo"<<endl;
		return;
	}
	TiXmlElement* typeElement = enemyPlaneElement->FirstChildElement(tagLifes);
	int lifes = atoi(typeElement->GetText());
	if(lifes <= 0){
		lifes = 1;
	}
	enemyPlane->setLifes(lifes);
	TiXmlElement* idElement = enemyPlaneElement->FirstChildElement(tagId);
	enemyPlane->setId(atoi(idElement->GetText()));
	TiXmlElement* altoElement = enemyPlaneElement->FirstChildElement(tagAlto);
	enemyPlane->setHeigth(atoi(altoElement->GetText()));
	TiXmlElement* anchoElement = enemyPlaneElement->FirstChildElement(tagAncho);
	enemyPlane->setWidth(atoi(anchoElement->GetText()));
	TiXmlElement* xElement = enemyPlaneElement->FirstChildElement(tagPosX);
	enemyPlane->setPosX(atoi(xElement->GetText()));
	TiXmlElement* yElement = enemyPlaneElement->FirstChildElement(tagPosY);
	enemyPlane->setPosY(atoi(yElement->GetText()));
	TiXmlElement* actualPhotogramElement = enemyPlaneElement->FirstChildElement(tagActualPhotogram);
	enemyPlane->setActualPhotogram(atoi(actualPhotogramElement->GetText()));
	TiXmlElement* photogramsElement = enemyPlaneElement->FirstChildElement(tagPhotograms);
	enemyPlane->setPhotograms(atoi(photogramsElement->GetText()));
	TiXmlElement* pathElement = enemyPlaneElement->FirstChildElement(tagPath);
	enemyPlane->setPath(pathElement->GetText());
	TiXmlElement* crazyMoves = enemyPlaneElement->FirstChildElement(tagCrazy);
	enemyPlane->setCrazyMoves(atoi(crazyMoves->GetText()) == 1);
	TiXmlElement* facingDirection = enemyPlaneElement->FirstChildElement(tagFacingDirection);
	enemyPlane->setFacingDirection(facingDirection->GetText());
	TiXmlElement* score = enemyPlaneElement->FirstChildElement(tagScore);
	enemyPlane->setScore(atoi(score->GetText()));
	TiXmlElement* lastHitScore = enemyPlaneElement->FirstChildElement(tagLastHitScore);
	enemyPlane->setLastHitScore(atoi(lastHitScore->GetText()));
	TiXmlElement* formationIdElement = enemyPlaneElement->FirstChildElement(tagFormationId);
	Formation* finalFormation;
	if(atoi(formationIdElement->GetText()) != -1){
		for(it = formations.begin(); it != formations.end(); it++){
			if((*it)->getId() == atoi(formationIdElement->GetText())){
				finalFormation = (*it);
			}
		}
	}else{
		finalFormation = NULL;
	}
	enemyPlane->setFormation(finalFormation);
	//Factor para que el avion no se desplace tan rapido
	enemyPlane->setSpeedFactor(0);
}


int XmlParser::getNumberOfEnemyPlanes(){
	int quantity = 0;
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* enemyPlanesElement = docHandle.FirstChild(tagEnemyPlanes).ToElement();
	TiXmlElement* enemyPlaneElement = enemyPlanesElement->FirstChildElement(tagEnemyPlane);
	while(enemyPlaneElement != NULL){
		quantity++;
		enemyPlaneElement = enemyPlaneElement->NextSiblingElement(tagEnemyPlane);
	}
	return quantity;
}

void XmlParser::getFormation(Formation* formation, int formationNum){
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* formationsElement = docHandle.FirstChild(tagFormations).ToElement();
	TiXmlElement* formationElement = formationsElement->FirstChildElement(tagFormation);
	for (int i = 0; i < formationNum ; i++){
		formationElement = formationElement->NextSiblingElement(tagFormation);
	}
	if ( formationElement == NULL){
		cout<<"Error al cargar la formacion desde archivo"<<endl;
		return;
	}
	TiXmlElement* extraPointsElement = formationElement->FirstChildElement(tagExtraPoints);
	formation->setExtraPoints(atoi(extraPointsElement->GetText()));
	TiXmlElement* quantityElement = formationElement->FirstChildElement(tagQuantity);
	formation->setQuantity(atoi(quantityElement->GetText()));
	TiXmlElement* idElement = formationElement->FirstChildElement(tagId);
	formation->setId(atoi(idElement->GetText()));
	//inicializo el bonus en verdadero y hit id en -1
	formation->setBonus(true);
	formation->setHitId(-1);
	formation->setDestroyed(false);
}

int XmlParser::getNumberOfFormations(){
	int quantity = 0;
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* formationsElement = docHandle.FirstChild(tagFormations).ToElement();
	TiXmlElement* formationElement = formationsElement->FirstChildElement(tagFormation);
	while(formationElement != NULL){
		quantity++;
		formationElement = formationElement->NextSiblingElement(tagFormation);
	}
	return quantity;
}
int XmlParser::getPosXPortaAviones(){
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* escenarioElem = docHandle.FirstChild(tagEscenario).ToElement();
	if(escenarioElem == NULL){
		cout<<"Error al obtener el escenario para obtener posicion de porta aviones"<<endl;
		return -1;
	}
	TiXmlElement* posX = escenarioElem->FirstChildElement("portaAvionesX");
	TiXmlElement* anchoPAviones = escenarioElem->FirstChildElement("anchoPortaAviones");
	return ((atoi(posX->GetText()) + atoi(anchoPAviones->GetText())/2));
}

int XmlParser::getPosYPortaAviones(){
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* escenarioElem = docHandle.FirstChild(tagEscenario).ToElement();
	if(escenarioElem == NULL){
		cout<<"Error al obtener el escenario para obtener posicion de porta aviones"<<endl;
		return -1;
	}
	TiXmlElement* posY = escenarioElem->FirstChildElement("portaAvionesY");
	return (atoi(posY->GetText()));
}

void XmlParser::getStagesPositions(list<int> &stagesPositions){
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* escenarioElem = docHandle.FirstChild(tagEscenario).ToElement();
	TiXmlElement* stagesPos = escenarioElem->FirstChildElement("stagesPositions");
	TiXmlElement* stagePos = stagesPos->FirstChildElement("stagePosition");
	while (stagePos != NULL){
		stagesPositions.push_back(atoi(stagePos->GetText()));
		stagePos = stagePos->NextSiblingElement("stagePosition");
	}
}

bool XmlParser::startWithPracticeMode(){
	TiXmlHandle docHandle(&this->doc);
	TiXmlElement* escenarioElem = docHandle.FirstChild(tagEscenario).ToElement();
	TiXmlElement* practiceElem = escenarioElem->FirstChildElement("practica");
	if(strcmp(practiceElem->GetText(), "on") == 0){
		return true;
	}
	return false;
}

XmlParser::~XmlParser() {
}
