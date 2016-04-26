/*
 * ErrorLogWriter.h
 *
 *  Created on: Apr 5, 2016
 *      Author: luciano
 */

#ifndef ERRORLOGWRITER_H_
#define ERRORLOGWRITER_H_

#include "Constants.h"

class LogWriter {
public:
	LogWriter();
	virtual ~LogWriter();

	void setLogLevel(LogLevelType logLevel);

	void writeValueErrorForElementInXML(string element);
	void writeNotFoundFileForNameError(string fileName);
	void writeNotFoundElementInXML(string element);
	void writeUserDidnotEnterFileName();
	void writeError(string error);
	void writeUserDidFinishTheApp();
	void writeWaitingForClientConnection();
	void writeClientConnectionReceived();
	void writeClientMessageReceviedFromSocketConnection(clientMsj message, int socketConnection);
	void writeMessageWasprossed(clientMsj message);
	void writeUserNameAlreadyInUse(char* userName);
	void writeResumeGameForUserName(char *userName);

private:
	LogLevelType logLevel;
	ofstream logFile;
	char* commentForMessage(char *headerText, clientMsj message);
	void writeLogInFile(string error);
};

#endif /* ERRORLOGWRITER_H_ */
