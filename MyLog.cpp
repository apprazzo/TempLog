/*
 * MyLog.cpp
 *
 *  Created on: 10.06.2015
 *      Author: prazzo
 */

#include "MyLog.h"

using namespace std;

MyLog::MyLog(string fileName) {
	m_logFileName = fileName;

	m_logFile.open(m_logFileName.c_str(), ios::app);
	if (!m_logFile.is_open()) {
		cout << "open logfile: " << m_logFileName << " failed!" << endl;
		return;
	} else {
		logStart();
	}
}

MyLog::~MyLog() {
	logStopp();
	m_logFile.close();
}

void MyLog::logStart() {
}

void MyLog::logStopp() {
}

void MyLog::write(string text) {
	m_logFile << getDateTimeString(DE) << "\t" << text << endl;
}
