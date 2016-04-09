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

	void writeLogInFile(string error);
	void writeValueErrorForElementInXML(string element);
	void writeNotFoundFileForNameError(string fileName);
	void writeNotFoundElementInXML(string element);
	void writeUserDidnotEnterFileName();

private:
	LogLevelType logLevel;
	ofstream logFile;
};

#endif /* ERRORLOGWRITER_H_ */
