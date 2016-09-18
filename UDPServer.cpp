/*
 * prazzo.ddns.net	UDPServer.cpp
 *
 *  Created on: 11.06.2015
 *      Author: prazzo
 */

#include "UDPServer.h"
#include "MyLog.h"
#include "Cdht22.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>

#define SLEEP_SENSORREAD 41
#define AVG_TH 10

using namespace std;

// Destructor
UDPServer::~UDPServer() {
	m_log->logStopp();
}

// Constructor and using std port

UDPServer::UDPServer(MyLog *logFile) {
//	cout << "std constructor" << endl;
	m_portno = 12345;
	m_log = logFile;
	stdInit();
}

//Constructor with a specific port
UDPServer::UDPServer(MyLog *logFile, int portNo) {
//	cout << "portNo constructor" << endl;
	m_portno = portNo;
	m_log = logFile;
	stdInit();
}

void UDPServer::stdInit() {
	m_server_stop = false;
//	cout << "Weclome to: UDP-Server stdInit()" << endl;
	m_humidity = -100;
	m_temperature = -300;
	m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	stringstream textBinding;

	if (m_sockfd < 0) {
		cerr << "ERROR opening socket" << endl;
		textBinding << "!!!ERROR - Socket creation FAILED on port:\t" << m_portno;
		m_log->write(textBinding.str());
		return;
	}

	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_addr.s_addr = INADDR_ANY;
	m_serverAddr.sin_port = htons(m_portno);

	start();
}

void UDPServer::start() {
	stringstream textBinding;
	if (bind(m_sockfd, (struct sockaddr *) &m_serverAddr, sizeof(m_serverAddr))
			< 0) {
		cerr << "ERROR on binding!" << endl;
		textBinding << "ERROR - Socket binding FAILED on port: " << m_portno;
		m_log->write(textBinding.str());
		return;
	}

	textBinding << "Socket binding successful on port:\t" << m_portno;
	m_log->write(textBinding.str());

	char myHostNameBuffer[16];
	gethostname(myHostNameBuffer, sizeof(myHostNameBuffer));

	cout << "server (" << myHostNameBuffer << ": " << ") started ... " << endl;
	menu();
}

void UDPServer::menu() {
	cout << "Server is wating for incoming MSG" << endl;

	int pid = fork();
	if (pid == 0) {
		cout << "doService ==> my dad is: " << getppid() << " and I am: " << getpid() << endl;
		doService();
	} else {
		while (true) {
			cout << "for shutdown press \"0\"" << endl;
			string input;
			cin >> input;
			if (strcmp(input.c_str(), "0") == 0) {
//				cout << "I am killing: " << pid << endl;
//				kill(pid, SIGUSR1);
				return;
			}
		}
	}
}

void UDPServer::doService() {
	pid_t pid = fork();
	if (pid == 0)
	{
		cout << "readSensor ==> my dad is: " << getppid() << " and I am: " << getpid() << endl;
		while (1)
		{
	                readSensor();
        	        sleep(SLEEP_SENSORREAD);
                }
	} else
	{
		while (1)
		{
//			cout << "MSGrecv ==> my dad is: " << getppid() << " and I am: " << getpid() << endl;
                       	string recvedMSG = recvMSGfrom();
			signal(SIGCHLD, SIG_IGN);

			if(fork() == 0) {
//		                cout << "PID: "<< getpid() << " - recvedMSG = " << recvedMSG << endl;
	       		        MSG2cmd(recvedMSG);
				exit(EXIT_SUCCESS);
			}
                }
	}

}

string UDPServer::recvMSGfrom() {
	memset(m_client_msg, NULL, sizeof(m_client_msg));
	memset(m_client, NULL, sizeof(m_client));

	recvfrom(m_sockfd, m_client_msg, sizeof(m_client_msg), 0,
			(struct sockaddr*) &m_clientAddr, &m_clientLength);

	m_client_msg_string = m_client_msg;
	m_client_msg_string.erase(m_client_msg_string.end()-1);

	getnameinfo((struct sockaddr*) &m_clientAddr, m_clientLength, m_client, sizeof(m_client), NULL,
			0, 0);

	m_log->write(("recvMSG\t" + m_client_msg_string + "\t" + "from " + (string) m_client));

	return m_client_msg_string;
}

void UDPServer::MSG2cmd(string recvedMSG) {
	int cmd = m_cmds.getIntFromCMD(recvedMSG);
	string returnString = "";

	switch (cmd) {
	case REBOOT:
//		cout << "reboot ==> " << m_cmds.getCMDFromInt(cmd) << endl;
		returnString = "return reboot\n";
		break;
	case POWEROFF:
//		cout << "poweroff ==> " << m_cmds.getCMDFromInt(cmd) << endl;
		returnString = "return poweroff\n";
		break;
	case TEMP:
//		cout << "temp ==> " << m_cmds.getCMDFromInt(cmd) << endl;
		returnString = "return temperature\n";
		break;
	default:
		;
//		cout << "cmd \"" << recvedMSG << "\" not found!" << endl;
	}
	
	if(returnString.size() > 255) {
		returnString = "original return-MSG was too long!";
	}
	sendto(m_sockfd, returnString.c_str(), returnString.size(), 0, (struct sockaddr
			*)&m_clientAddr, m_clientLength);
}

void UDPServer::readSensor()
{
	char tempBufferTemp[8];
	char tempBufferHum[8];

//	cout << "try reading sensor" << endl;
	m_dht22.getTHavg(m_temperature, m_humidity, AVG_TH);
//	cout << "Temperatur = " << m_temperature << " °C" < <endl;
//	cout << "Humidity = " << m_humidity << " %" << endl;

	sprintf(tempBufferTemp, "%0.2f", m_temperature);
	sprintf(tempBufferHum, "%d", m_humidity);

	string string2log = "TH\t" + (string)tempBufferTemp + "\t" + (string)tempBufferHum;

	string display = "T=" + (string)tempBufferTemp + "C    H=" + (string)tempBufferHum + "%";

	m_log->write(string2log);

//	m_log->write(("TH\t" + to_string(m_temperature) + "\t" + to_string( m_humidity)));
}
