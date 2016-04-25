XmlLibrary = tinyxml.cpp tinystr.cpp tinyxmlerror.cpp tinyxmlparser.cpp

all:
	g++ -Wall CargadorXML.cpp -c
	g++ -Wall XmlParser.cpp -c
	g++ -Wall LogWriter.cpp -c	
	g++ -Wall Procesador.cpp -c
	g++ -Wall Avion.cpp -c
	g++ -Wall Escenario.cpp -c
	g++ -Wall Window.cpp -c
	g++ -Wall Sprite.cpp -c
	g++ -Wall Client.cpp -c

	g++ -Wall taller2016Server.cpp CargadorXML.o XmlParser.o Procesador.o LogWriter.o Avion.o Escenario.o Window.o Sprite.o Client.o $(XmlLibrary) -o server -lpthread -std=c++11
