XmlLibrary = tinyxml.cpp tinystr.cpp tinyxmlerror.cpp tinyxmlparser.cpp

all:
	g++ -Wall CargadorXML.cpp -c
	g++ -Wall XmlParser.cpp -c
	g++ -Wall LogWriter.cpp -c	
	g++ -Wall Procesador.cpp -c

	g++ -Wall taller2016Server.cpp CargadorXML.o XmlParser.o Procesador.o LogWriter.o ClientAttendant.o ClientMessage.o $(XmlLibrary) -o server -lpthread -std=c++11
