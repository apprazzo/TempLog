/*
 * 	Class dht22:
 *
 *	Created on: 22.07.2015
 *	    Author: prazzo
 */

#include "Cdht22.h"

#include <iostream>
#include <math.h>

#define DIGITS 2

using namespace std;

Cdht22::Cdht22(int pin) {
	stdInit();
	setPin(pin);
}

void Cdht22::stdInit() {
	for (int i = 0; i < DAT_SIZE; i++)
		m_data[i] = 0;

	m_temperature = -300;
	m_humidity = -100;

	if (wiringPiSetup() == -1)
		exit(EXIT_FAILURE);

	if (setuid(getuid()) < 0)
	{
		perror("Droppingprivileges failed\n");
		exit(EXIT_FAILURE);
	}
}

void Cdht22::setPin(int pin) {
	if (pin > 1 && pin < 40)
	{
		m_PIN = pin;
//		cout << "DHT PIN set: " << m_PIN << endl;
	}
	else
		cerr << "PIN No. out of range: " << pin << endl;
}

int Cdht22::getPin() {
	return m_PIN;
}

float Cdht22::getTemperature() {
	sensorReadOut(2);
	return m_temperature;
}

float Cdht22::getHumidity() {
	sensorReadOut(2);
	return m_humidity;
}

uint8_t Cdht22::sizecvt(const int read) {
	if (read > 255 || read < 0)
	{
		cerr << "Invalid data from wiringPi library!" << endl;
		exit(EXIT_FAILURE);
	}

	return (uint8_t)read;
}

int Cdht22::measure() {
	uint8_t laststate = HIGH;
	uint8_t counter = 0;
	uint8_t j = 0, i;

	for(int k = 0; k < DAT_SIZE; k++)
	{
		m_data[k] = 0;
	}


	// pull pin down for 18 ms
	pinMode(m_PIN, OUTPUT);
	digitalWrite(m_PIN, HIGH);
	delay(10);
	digitalWrite(m_PIN, LOW);
	delay(18);
	// pull pin up for 40 us
	digitalWrite(m_PIN, HIGH);
	delayMicroseconds(40);
	// prepare to read pin
	pinMode(m_PIN, INPUT);

	// detect change and read data
	for (i=0; i<MAXTIMINGS; i++)
	{
		counter = 0;
		while(sizecvt(digitalRead(m_PIN)) == laststate)
		{
			counter++;
			delayMicroseconds(1);
			if (counter == 255)
				break;
		}

		laststate = sizecvt(digitalRead(m_PIN));

		if (counter == 255)
			break;

		// ignore first 3 transitions
		if((i >= 4) && (i%2 == 0))
		{
			// shove each bit into the storage bytes
			m_data[j/8] <<= 1;
			if (counter > 16)
				m_data[j/8] |= 1;
			j++;
		}
	}

	// check we read 40 bits (8bits x 5) + verify checksum in the last byte
	// print it out if data is good
	if (( j>= 40) &&
	  (m_data[4] == ((m_data[0] + m_data[1] + m_data[2] + m_data[3]) & 0xFF)) )
	{
		m_humidity = (float)m_data[0]*256 + (float)m_data[1];
		m_humidity /= 10;
		m_temperature = (float)(m_data[2] & 0x7F) * 256 + (float)m_data[3];
		m_temperature /= 10.0;
		if((m_data[2] & 0x80) != 0)
			m_temperature *= -1;

		return 1;
	}
	else
	{
//		cerr << "Data not good!" << endl;
		return 0;
	}
}

void Cdht22::getTH(float &t, int &h)
{
	sensorReadOut(3);

	t = m_temperature;
	h = (int)m_humidity;
}

void Cdht22::sensorReadOut()
{
	while(measure() == 0)
		delay(1000);
}

void Cdht22::sensorReadOut(const unsigned int number)
{
	if ((number>1) && (number<=10))
		for (int i = 0; i < number; i++)
		{
			while(measure() == 0)
				delay(1000);
		}
	else
		cerr << "number for sensorReadOut out of range(" << number << ")" << endl <<
		  "it should be between 2 and 10" << endl;
}

void Cdht22::getTHavg(float &t, int &h, const unsigned int number)
{
	if ((number>0) && (number<256))
	{
		float Tsum = 0;
		float Hsum = 0;

		// getting rid of old measured values
		sensorReadOut(2);

		// summing up
		for (int i = 0; i < number; i++)
		{
//			cout << "sensorReadOut " << i+1 << " of " << number << endl;
			sensorReadOut();
			Tsum += m_temperature;
			Hsum += m_humidity;
		}

		// calc avg
		t = Tsum / number;
		h = Hsum / number;
//		cout << "correct numbers (T/H) (" << t << "/" << h << ")" << endl;
		// round values
		if (DIGITS >= 1)
			t = roundf(t*pow(10,DIGITS))/pow(10,DIGITS);
		else
			t = roundf(t);
		h = (int)roundf(h);
//		cout << "rounded numbers (T/H) (" << t << "/" << h << ")" << endl;
	}
	else
	{
		cerr << "invalid number for avg-value (" << number << ")" << endl;
		t=-300;
		h=-100;
		return;
	}
}
