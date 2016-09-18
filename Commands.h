/*
 * Commands.h
 *
 *  Created on: 15.06.2015
 *      Author: prazzo
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <map>
#include <string>

using namespace std;

enum CMDs{REBOOT, POWEROFF, TEMP};

class Commands {
private:
	map<string, int> m_map_CMDs;
	map<int, string> m_map_linux_CMD;

public:
	Commands();
	virtual ~Commands();

	int getIntFromCMD(string cmd);
	string getCMDFromInt(int i_cmd);
};

#endif /* COMMANDS_H_ */
