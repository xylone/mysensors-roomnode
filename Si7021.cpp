/*
  Si7021.cpp - Library for Si7021 Temp humidity sensor
  Created by Peter Bootsma, February 12, 2016.
  License: GPLv3
*/

#include "Si7021.h"

void Si7021::begin() 
{
	// Only begin i2c if not done yet
	if (TWCR == 0)  
	{
   		Wire.begin();
	}
}

bool Si7021::read()
{
	bool succes = false;
	temperature = 0;
	humidity = 0;
	
	// Initiating humidity and temperature measurement
	Wire.beginTransmission(SI7021_ADDR);
	Wire.write(SI7021_CMD_READ_HUMIDITY);
	Wire.endTransmission();

	// Read and calculate humidity data
	Wire.requestFrom(SI7021_ADDR, 2);
	if(Wire.available() >= 2) {
		long rhCode = (Wire.read() << 8 | Wire.read()) & 0x0000FFFF;
		humidity = (125 * rhCode) / 65536.0 - 6;
	}
	
	// Initiating temperature measurement
	Wire.beginTransmission(SI7021_ADDR);
	Wire.write(SI7021_CMD_READ_TEMP);
	Wire.endTransmission();

	// Read and calculate temperature
	Wire.requestFrom(SI7021_ADDR, 2);
	if (Wire.available() >= 2) {
		long tempCode = (Wire.read() << 8 | Wire.read()) & 0x0000FFFF;
		temperature = (175.72 * tempCode) / 65536 - 46.85;
		succes = true;
	}
	
	return succes;
}