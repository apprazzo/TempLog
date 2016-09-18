/*
 * UDPServer.h
 *
 *  Created on: 11.06.2015
 *      Author: prazzo
 */

#ifndef UDPSERVER_H_
#define UDPSERVER_H_

#include "MyLog.h"
#include "Commands.h"
#include "Cdht22.h"

#include <stdlib.h>			// std strtol()
#include <unistd.h>			// system call, etc
#include <sys/types.h>			// system data type definitions
#include <sys/socket.h>			// socket specific definitions
#include <netinet/in.h>			// INET specific definitions
#include <arpa/inet.h>			// IP address conversion
#include <netdb.h>			// gethostbyname
#include <signal.h>
#include <vector>

class UDPServer {
private:
	bool m_server_stop;
	int m_sockfd;
	int m_newsockfd;
	int m_portno;

	char m_buffer[256];
	struct sockaddr_in m_serverAddr;

	struct sockaddr_in m_clientAddr;
	socklen_t m_clientLength;
	char m_client[64];
	char m_client_msg[256];
	string m_client_msg_string;

	float m_temperature;
	int m_humidity;
	MyLog* m_log;
	Commands m_cmds;
	Cdht22 m_dht22;
	void killPIDs();

public:
	virtual ~UDPServer();
	UDPServer(MyLog *logFile);
	UDPServer(MyLog *logFile, int portNo);

	void stdInit();
	void start();
	void menu();
	void doService();
	void readSensor();
	string recvMSG();
	string recvMSGfrom();
	void MSG2cmd(string recvedMSG);
};

#endif /* UDPSERVER_H_ */
