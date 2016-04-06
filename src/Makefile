XmlLibrary = tinyxml.cpp tinystr.cpp tinyxmlerror.cpp tinyxmlparser.cpp

all:
	g++ -Wall CargadorXML.cpp -c
	g++ -Wall XmlParser.cpp -c
	g++ -Wall ClientAttendant.cpp -c
	g++ -Wall ClientMessage.cpp -c
	g++ -Wall ErrorLogWriter.cpp -c

	g++ -Wall taller2016Server.cpp CargadorXML.o XmlParser.o ErrorLogWriter.o ClientAttendant.o ClientMessage.o $(XmlLibrary) -o server -lpthread -std=c++11
