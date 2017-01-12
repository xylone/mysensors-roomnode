/*
  Roomnode.ino - Sketch for MySensors temperature and humidity node
*/

#include <SPI.h>
#include <Wire.h>
#include <MySensor.h>
#include <BH1750.h>
#include "Si7021.h"
#include "Config.h"

#define CHILD_ID_TEMP 0
#define CHILD_ID_HUM 1
#define CHILD_ID_VOLT 2
#define CHILD_ID_MQ_135 3
#define CHILD_ID_MOTION 4
#define CHILD_ID_LIGHT 5

MySensor gw;

Si7021 tempHumiditySensor;

#ifdef ROOMNODE_HAS_SI7021
	MyMessage msgHum(CHILD_ID_HUM, V_HUM);
	MyMessage msgTemp(CHILD_ID_TEMP, V_TEMP);
#endif
#ifdef ROOMNODE_BATTERY_POWERED
	MyMessage msgVolt(CHILD_ID_VOLT, V_VOLTAGE);
#endif
#ifdef ROOMNODE_HAS_MQ_135
	MyMessage msgMq135(CHILD_ID_MQ_135, V_LEVEL);
#endif
#ifdef ROOMNODE_HAS_PIR
	#define MOTION_DIGITAL_INPUT_SENSOR 3			 // The digital input you attached your motion sensor.  (Only 2 and 3 generates interrupt!)
	#define INTERRUPT MOTION_DIGITAL_INPUT_SENSOR-2  // Usually the interrupt = pin -2 (on uno/nano anyway)
	MyMessage msgMotion(CHILD_ID_MOTION, V_TRIPPED);
#endif
#ifdef ROOMNODE_HAS_LUX
	BH1750 lightSensor;
	MyMessage msgLight(CHILD_ID_LIGHT, V_LIGHT_LEVEL);
#endif

int RAW_VOLT_PIN = A0;

unsigned long lastSendTime = 0;

void setup()
{
	Serial.println("Sketch setup");

	// Blink
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);
	delay(500);
	digitalWrite(13, LOW);

	// Use the 1.1 V internal reference
	analogReference(INTERNAL);

	#ifdef ROOMNODE_SENSORS_POWER_PIN
		// Power up Si7021
		pinMode(ROOMNODE_SENSORS_POWER_PIN, OUTPUT);
		digitalWrite(ROOMNODE_SENSORS_POWER_PIN, HIGH);
	#endif

	#ifdef ROOMNODE_IS_REPEATER
		gw.begin(NULL, AUTO, true);
	#else
		gw.begin();
	#endif

	// Send the Sketch Version Information to the Gateway
	gw.sendSketchInfo(ROOMNODE_NAME, ROOMNODE_VERSION);

	// Register all sensors to gw (they will be created as child devices)
	#ifdef ROOMNODE_HAS_SI7021
		gw.present(CHILD_ID_HUM, S_HUM, "Si7021");
		gw.present(CHILD_ID_TEMP, S_TEMP, "Si7021");
		
		// Start  Si7021
		tempHumiditySensor.begin();
	#endif
	
	#ifdef ROOMNODE_HAS_MQ_135
		gw.present(CHILD_ID_MQ_135, S_AIR_QUALITY, "MQ-135");
	#endif
	
	#ifdef ROOMNODE_HAS_PIR
		gw.present(CHILD_ID_MOTION, S_MOTION);

		pinMode(MOTION_DIGITAL_INPUT_SENSOR, INPUT);      // sets the motion sensor digital pin as input
	#endif
	
	#ifdef ROOMNODE_HAS_LUX
	  	gw.present(CHILD_ID_LIGHT,  S_LIGHT_LEVEL);
	
	  	lightSensor.begin();
	#endif

	
	#ifdef ROOMNODE_BATTERY_POWERED
		gw.present(CHILD_ID_VOLT, S_MULTIMETER, "Battery");
	#endif
	
	Serial.println("Sketch setup done");
}

void loop()
{
	unsigned long loopStart = millis();
	
	#ifdef ROOMNODE_IS_REPEATER
		if ((loopStart - lastSendTime) > ROOMNODE_SEND_INTERVAL) {
			readSensorsAndSendData();
			lastSendTime = loopStart;
		}
		gw.process();
	#else
		readSensorsAndSendData();
		#ifdef ROOMNODE_HAS_PIR
		  	gw.sleep(INTERRUPT, CHANGE, ROOMNODE_SEND_INTERVAL);
		#else
			gw.sleep(ROOMNODE_SEND_INTERVAL);
		#endif
	#endif
}

void readSensorsAndSendData() {
	
	// Power up i2c
	#ifdef ROOMNODE_POWER_DOWN_I2C	
		powerUpI2C();
	#endif
	
	#ifdef ROOMNODE_HAS_SI7021
		// Read temp and humidity
		bool tempHumidityRead = tempHumiditySensor.read();
	
		if (tempHumidityRead) {
			// Send temperature through RF
			gw.send(msgTemp.set(tempHumiditySensor.temperature, 1));
		
			// Send Humidity through RF
			gw.send(msgHum.set(tempHumiditySensor.humidity, 1));
		}
	#endif

	// Power down i2c
	#ifdef ROOMNODE_POWER_DOWN_I2C
		powerDownI2C();
	#endif
	
	#ifdef ROOMNODE_HAS_MQ_135
		int mq135Value = analogRead(ROOMNODE_MQ_135_PIN);
		int mq135Ppm = (mq135Value/1023.0) * ROOMNODE_MQ_135_MAX_PPM;
		gw.send(msgMq135.set(mq135Ppm));
	#endif

	#ifdef ROOMNODE_HAS_PIR
		boolean tripped = digitalRead(MOTION_DIGITAL_INPUT_SENSOR) == HIGH;
		//Serial.println(tripped);
		gw.send(msgMotion.set(tripped ? "1" : "0")); // Send tripped value to gw
	#endif
	
	#ifdef ROOMNODE_HAS_LUX
		uint16_t lux = lightSensor.readLightLevel(); // Get Lux value
	    gw.send(msgLight.set(lux));
	#endif

	#ifdef ROOMNODE_BATTERY_POWERED
		// Battery voltage
		float voltValue = 0;
		for (int i=0; i<ROOMNODE_BATTERY_SAMPLE_COUNT; i++) {
			voltValue += analogRead(RAW_VOLT_PIN)/(ROOMNODE_BATTERY_SAMPLE_COUNT * 1.0);
		}
		float volt = voltValue/1023.0 * 1.1 * VOLTAGE_DIVIDER_FACTOR;
		gw.send(msgVolt.set(volt, 3));
		
		// Battery percentage
		int batteryPcnt = 100*(volt - ROOMNODE_BATTERY_EMPTY_VOLT)/(ROOMNODE_BATTERY_FULL_VOLT-ROOMNODE_BATTERY_EMPTY_VOLT);
		if (batteryPcnt > 100) batteryPcnt = 100;
		gw.sendBatteryLevel(batteryPcnt);
	#endif
}

void powerDownI2C()
{
	power_twi_disable();
	
	//Power down 7021
	#ifdef ROOMNODE_SENSORS_POWER_PIN
		digitalWrite(ROOMNODE_SENSORS_POWER_PIN, LOW);
	#endif
	
	// Disable digital input buffers on A0 to A5 pins
    //DIDR0 = 0x3F; 
    
    pinMode(SDA, INPUT);
    pinMode(SCL, INPUT);
    digitalWrite(SDA, LOW);
    digitalWrite(SCL, LOW);
}

void powerUpI2C()
{
	//Power up I2C SDA SCL
	digitalWrite(SDA, HIGH);
	digitalWrite(SCL, HIGH);

	//Power up Si7021 and wait for stable state
	#ifdef ROOMNODE_SENSORS_POWER_PIN
		digitalWrite(ROOMNODE_SENSORS_POWER_PIN, HIGH);
	#endif

	power_twi_enable();
	delay(100);
}