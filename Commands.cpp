/*
 * Commands.cpp
 *
 *  Created on: 15.06.2015
 *      Author: prazzo
 */

#include "Commands.h"

#include <map>
#include <string>

using namespace std;

Commands::Commands() {
	m_map_CMDs.insert(pair<string, int>("reboot", REBOOT));
	m_map_CMDs.insert(pair<string, int>("poweroff", POWEROFF));
	m_map_CMDs.insert(pair<string, int>("temp", TEMP));

	m_map_linux_CMD.insert(pair<int, string>(REBOOT, "sudo reboot"));
	m_map_linux_CMD.insert(pair<int, string>(POWEROFF, "sudo poweroff"));
	m_map_linux_CMD.insert(pair<int, string>(TEMP, "What is the temperature?"));
}

Commands::~Commands() {
	m_map_CMDs.clear();
}

int Commands::getIntFromCMD(string cmd) {
	map<string, int>::iterator it;
	it = m_map_CMDs.find(cmd);

	if (it != m_map_CMDs.end())
		return m_map_CMDs.find(cmd)->second;
	else
		return -1;
}

string Commands::getCMDFromInt(int i_cmd) {
	string returnString = "";

	returnString = m_map_linux_CMD.find(i_cmd)->second;

	return returnString;
}
