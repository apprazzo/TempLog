/*
 * main.cpp
 *
 *  Created on: 05.06.2015
 *      Author: prazzo
 */

#include <stdio.h>			// std C I/O
#include <iostream>			// std Cpp I/O
#include <fstream>			// std Cpp file-I/O

#include "myTime.h"
#include "MyLog.h"
#include "UDPServer.h"

using namespace std;

void readConfig(unsigned int & portNo, string & fileName) {
	ifstream conf;
	string bufferLine;
	string valueString;
	int lineOfFile = 0;
	string key;
	size_t pos=0;

	conf.open("PiServer.conf", ios_base::in);

	if (!conf) {
		cerr << "ERROR - cannot read conf!" << endl;
	}

	while (!conf.eof()) {
		getline(conf, bufferLine);
		lineOfFile++;

		if (bufferLine.length() > 1) {
			pos = bufferLine.find_first_of('\t');
			if (pos != 0) {
				key = bufferLine.substr(0, pos);
				valueString = bufferLine.substr(pos+1);

				pos = 0;
				pos = valueString.find_first_of("\t");
				if ((pos > 0) && (pos < 64)) {
					cerr << "ERROR: 2nd TAB found in line: " << lineOfFile
							<< endl;
				} else {
					if (key.compare(0,1,"#") == 0)
					{
						;
//						cout << "comment out: " << key << endl;
					}
					else if (key == "PORT")
						portNo = atoi(valueString.c_str());
					else if (key == "LOG")
						fileName = valueString;
					else
						cerr << "ERROR - unknown KEY: " << key << " in line: "
								<< lineOfFile << endl;
				}
			} else {
				cerr << "ERROR: no TAB found in line: " << lineOfFile << endl;
			}
		}
	}

//	cout << "conf-file has " << lineOfFile << " lines" << endl;

	return;
}

int main(int argc, char *argv[]) {
	unsigned int portNo = 7891;
	string filename = "PIserverLOG.txt";

	readConfig(portNo, filename);

	MyLog myLogFile(filename);

	if (argc == 2) {
		int tempArgv1 = atoi(argv[1]);

		if ((tempArgv1 >= 1024) && (tempArgv1 <= 49151))
			portNo = tempArgv1;
	}

	UDPServer myUDPServer(&myLogFile, portNo);

	cout << "server stopped!" << endl;

	return 0;
}
