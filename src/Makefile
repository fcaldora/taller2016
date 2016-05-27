XmlLibrary = tinyxml.cpp tinystr.cpp tinyxmlerror.cpp tinyxmlparser.cpp

all:
	g++ -Wall CargadorXML.cpp -c
	g++ -Wall XmlParser.cpp -c
	g++ -Wall LogWriter.cpp -c	
	g++ -Wall Procesador.cpp -c -std=c++11
	g++ -Wall Avion.cpp -c
	g++ -Wall Escenario.cpp -c
	g++ -Wall Client.cpp -c
	g++ -Wall GameManager.cpp -c -std=c++11 -pthread
	g++ -Wall ClientList.cpp -c -std=c++11 
	g++ -Wall DrawableObject.cpp -c -std=c++11 
	g++ -Wall MenuPresenter.cpp -c -std=c++11 
	g++ -Wall MessageBuilder.cpp -c -std=c++11
	g++ -Wall SocketManager.cpp -c -std=c++11
	g++ -Wall Object.cpp -c -std=c++11
	g++ -Wall BulletList.cpp -c -std=c++11
	g++ -Wall PowerUp.cpp -c -std=c++11
	g++ -Wall PowerUpList.cpp -c -std=c++11
	

	g++ -Wall -g taller2016Server.cpp  BulletList.o PowerUp.o PowerUpList.o CargadorXML.o XmlParser.o Procesador.o LogWriter.o Avion.o Escenario.o Client.o GameManager.o ClientList.o DrawableObject.o MenuPresenter.o MessageBuilder.o Object.o SocketManager.o $(XmlLibrary) -o server -lpthread -std=c++11
