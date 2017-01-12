/*
  Si7021.h - Library for Si7021 Temp humidity sensor
  Created by Peter Bootsma, February 12, 2016.
  License: GPLv3
*/

#ifndef Si7021_h
#define Si7021_h

#include <Arduino.h>
#include <Wire.h>

#define SI7021_ADDR 0x40
#define SI7021_CMD_READ_HUMIDITY 0xE5
#define SI7021_CMD_READ_TEMP 0xE0 // Read temp from the humidity measurement, avoids a measurement

class Si7021
{
  public:
  	void begin();
  	bool read();
  	
  	float temperature;
  	float humidity;
  private:
};

#endif // Si7021_h