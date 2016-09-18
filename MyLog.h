/*
 * MyLog.h
 *
 *  Created on: 10.06.2015
 *      Author: prazzo
 */

#ifndef MYLOG_H_
#define MYLOG_H_

#include <stdio.h>			// std C I/O
#include <iostream>			// std Cpp I/O
#include <fstream>			// std Cpp file-I/O
#include <string>

#include "myTime.h"

using namespace std;

class MyLog {
private:
	ofstream m_logFile;
//	static const char m_logFileName[16] = "PIserverLOG.txt";
	string m_logFileName;

public:
	MyLog(string fileName = "PIserverLOG.txt");
	virtual ~MyLog();

	void logStart();
	void logStopp();
	void write(string text);
};

#endif /* MYLOG_H_ */
