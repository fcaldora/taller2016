/*
 * SocketManager.h
 *
 *  Created on: Apr 26, 2016
 *      Author: luciano
 */

#ifndef SOCKETMANAGER_H_
#define SOCKETMANAGER_H_
#include "Constants.h"
#include "XmlParser.h"
#include "LogWriter.h"

class SocketManager {
public:
	SocketManager(LogWriter *logWriter, XmlParser *parser);
	virtual ~SocketManager();
	int createSocketConnection();

	int socketHandle;

private:
	LogWriter *logWriter;
	XmlParser *parser;
};

#endif /* SOCKETMANAGER_H_ */
