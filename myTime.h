/*
 * getTime.h
 *
 *  Created on: 05.06.2015
 *      Author: prazzo
 */

#ifndef MYTIME_H_
#define MYTIME_H_

//#include "myTime.cpp"

#include <string>

using namespace std;

enum LOCALES{US, DE, ES};

string getDateTimeString();
string getDateTimeString(unsigned int wishedLocal);
string getTimeString();
string getDateString();


#endif /* MYTIME_H_ */
