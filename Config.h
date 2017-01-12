/*
  Config.h - Config file for Roomnode sketch
*/

#ifndef RoomnodeConfig_h
#define RoomnodeConfig_h

// Default settings
#define ROOMNODE_VERSION "1.3"
#define ROOMNODE_SEND_INTERVAL 300000 // Default 5 minutes
#define VOLTAGE_DIVIDER_HIGH 1000
#define VOLTAGE_DIVIDER_LOW 330

//#define ROOMNODE_POWER_DOWN_I2C
//#define ROOMNODE_SENSORS_POWER_PIN A2
//#define ROOMNODE_IS_REPEATER

//#define ROOMNODE_HAS_SI7021
//#define ROOMNODE_HAS_MQ_135
//#define ROOMNODE_HAS_PIR
//#define ROOMNODE_HAS_LUX

#define ROOMNODE_MQ_135_MAX_PPM 10000
#define ROOMNODE_MQ_135_PIN A0
#define ROOMNODE_MQ_135_FACTOR 0.6

//#define ROOMNODE_BATTERY_POWERED
#define ROOMNODE_BATTERY_SAMPLE_COUNT 5
#define ROOMNODE_BATTERY_FULL_VOLT 2.8
#define ROOMNODE_BATTERY_EMPTY_VOLT 1.8

// Define which roomnode config to use
//#define ROOMNODE_LIVINGROOM
//#define ROOMNODE_BATHROOM
#define ROOMNODE_REFRIGERATOR
//#define ROOMNODE_FREEZER
//#define ROOMNODE_AIR_QUALITY
//#define ROOMNODE_MASTER_BEDROOM
//#define ROOMNODE_YARA_BEDROOM
//#define ROOMNODE_TIMO_BEDROOM

// Roomnode specific settings

// Livingroom
#ifdef ROOMNODE_LIVINGROOM
	#define ROOMNODE_NAME "Woonkamer"
	#define ROOMNODE_HAS_SI7021
//	#define ROOMNODE_HAS_PIR
	#define ROOMNODE_HAS_LUX
	#define ROOMNODE_HAS_MQ_135
	#define ROOMNODE_IS_REPEATER
#endif

//#ifdef ROOMNODE_AIR_QUALITY
//	#define ROOMNODE_NAME "Air quality"
//	#define ROOMNODE_SEND_INTERVAL 60000
//	#define ROOMNODE_HAS_MQ_135
//#endif

// Refrigerator
#ifdef ROOMNODE_REFRIGERATOR
	#define ROOMNODE_NAME "Koelkast"
	#define VOLTAGE_DIVIDER_HIGH 998
	#define VOLTAGE_DIVIDER_LOW 330
	#define ROOMNODE_SENSORS_POWER_PIN 7
	#define ROOMNODE_POWER_DOWN_I2C
	#define ROOMNODE_BATTERY_POWERED
#endif

// Freezer
#ifdef ROOMNODE_FREEZER
	#define ROOMNODE_NAME "Diepvries"
	#define ROOMNODE_BATTERY_POWERED
	#define ROOMNODE_SENSORS_POWER_PIN A2
	#define ROOMNODE_POWER_DOWN_I2C
	#define ROOMNODE_HAS_SI7021
	#define ROOMNODE_BATTERY_FULL_VOLT 1.4
	#define ROOMNODE_BATTERY_EMPTY_VOLT 1.2
#endif

// Bathroom
//#ifdef ROOMNODE_BATHROOM
//	#define ROOMNODE_NAME "Badkamer"
//	#define ROOMNODE_BATTERY_POWERED
//	#define ROOMNODE_SENSORS_POWER_PIN A2
//	#define ROOMNODE_HAS_SI7021
//	#define ROOMNODE_BATTERY_POWERED
//#endif

// Master Bedroom
#ifdef ROOMNODE_MASTER_BEDROOM
	#define ROOMNODE_NAME "Slaapkamer"
	#define ROOMNODE_HAS_SI7021
	#define ROOMNODE_POWER_DOWN_I2C
	#define ROOMNODE_SENSORS_POWER_PIN A2
	#define ROOMNODE_BATTERY_POWERED
#endif

// Yara Bedroom
#ifdef ROOMNODE_YARA_BEDROOM
	#define ROOMNODE_NAME "Slaapkamer Yara"
	#define ROOMNODE_HAS_SI7021
	#define ROOMNODE_POWER_DOWN_I2C
	#define ROOMNODE_SENSORS_POWER_PIN A2
	#define ROOMNODE_BATTERY_POWERED
	#define ROOMNODE_BATTERY_FULL_VOLT 1.4
	#define ROOMNODE_BATTERY_EMPTY_VOLT 1.2
#endif

// Timo Bedroom
#ifdef ROOMNODE_TIMO_BEDROOM
	#define ROOMNODE_NAME "Slaapkamer Timo"
	#define ROOMNODE_HAS_SI7021
	#define ROOMNODE_POWER_DOWN_I2C
	#define ROOMNODE_SENSORS_POWER_PIN A2
	#define ROOMNODE_BATTERY_POWERED
#endif

#define VOLTAGE_DIVIDER_FACTOR ((VOLTAGE_DIVIDER_HIGH + VOLTAGE_DIVIDER_LOW)/(VOLTAGE_DIVIDER_LOW * 1.0))

#endif // RoomnodeConfig_h