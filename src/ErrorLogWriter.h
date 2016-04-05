/*
 * ErrorLogWriter.h
 *
 *  Created on: Apr 5, 2016
 *      Author: luciano
 */

#ifndef ERRORLOGWRITER_H_
#define ERRORLOGWRITER_H_

#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class ErrorLogWriter {
public:
	ErrorLogWriter();
	virtual ~ErrorLogWriter();
	void writeErrorInFile(string error);
	void writeValueErrorForElementInXML(string element);
	void writeNotFoundFileForNameError(string fileName);
	void writeNotFoundElementInXML(string element);

private:
	ofstream errorLogFile;
};

#endif /* ERRORLOGWRITER_H_ */
