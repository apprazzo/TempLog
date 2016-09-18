/*
 *	Class dth22:
 *
 *	Created on: 22.07.2015
 *	    Author: prazzo
 */

#ifndef CDHT22_H_
#define CDHT22_H_

#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

#include <wiringPi.h>

//#include <string.h>
//#include <dirent.h>
//#include <fcntl.h>
//#include <assert.h>
//#include <sys/mman.h>
//#include <sys/stat.h>
//#include <sys/time.h>
//#include <bcm2835.h>


#define MAXTIMINGS 85
#define DAT_SIZE 5
//#define BCM2708_PERI_BASE 0x20000000
//#define GPIO_BASE (BCM2708_PERI_BASE + 0x200000)

using namespace std;

class Cdht22 {
private:
	int m_PIN;
	int m_data[DAT_SIZE];
	float m_temperature, m_humidity;

	int measure();
	void sensorReadOut();
	void sensorReadOut(const unsigned int number);
	void stdInit();
	uint8_t sizecvt(const int read);
	bool checkTH(float const t, float const h);
//	static int read_dht22_dat();

public:
//	virtual ~Cdht22() {};
	Cdht22(int pin = 7);

	float getTemperature();
	float getHumidity();
	void getTH(float &t,int &h);
	void getTHavg(float &t, int &h, const unsigned int number = 20);
	void setPin(int pin);
	int getPin();
};

#endif /* CDHT22_H_ */
