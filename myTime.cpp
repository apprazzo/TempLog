/*
 * getTime.cpp
 *
 *  Created on: 05.06.2015
 *      Author: prazzo
 */

#include "myTime.h"

#include <ctime>
#include <iostream>
#include <string>
#include <locale>
#include <stdio.h>


using namespace std;

string getDateTimeString(unsigned int wishedLocal) {
	string returnString;
	char localeDate[20];
	char localeTime[20];

	time_t rawTime = time(NULL);
	struct tm * timeInfo;

	time(&rawTime);
	timeInfo = localtime(&rawTime);

	strftime(localeTime, 20, "%T", timeInfo);

	switch (wishedLocal) {
	case DE:
		strftime(localeDate, 20, "%d.%m.%Y", timeInfo);
		break;
	case US:
		strftime(localeDate, 20, "%x", timeInfo);
		break;
	default:
		cout << "wrong locale code! Default is used!" << endl;
		break;
	}

	string dateString = localeDate;
	string timeString = localeTime;
	returnString = dateString + " " + timeString;

	return returnString;
}

string getDateTimeString() {
	string returnString;
	char localeDate[10];
	char localeTime[8];

	time_t rawTime = time(NULL);
	struct tm * timeInfo;

	time(&rawTime);
	timeInfo = localtime(&rawTime);

	strftime(localeTime, 20, "%X", timeInfo);
	strftime(localeDate, 20, "%x", timeInfo);
	setlocale(LC_TIME, "");

	string dateString = localeDate;
	string timeString = localeTime;
	returnString = dateString + " " + timeString;

	cout << returnString << endl;

	return returnString;
}

string getTimeString() {
	char returnString[20];

	time_t rawTime = time(NULL);
	struct tm * timeInfo;

	time(&rawTime);
	timeInfo = localtime(&rawTime);

	strftime(returnString, 20, "%X", timeInfo);

	return returnString;
}

string getDateString() {
	char returnString[20];

	time_t rawTime = time(NULL);
	struct tm * timeInfo;

	time(&rawTime);
	timeInfo = localtime(&rawTime);

	strftime(returnString, 20, "%d.%m.%Y", timeInfo);

	return returnString;
}
