#include "Arduino.h"
#include "Wire.h"
#include "BoschBME280.h"
#include "MaximDS1338.h"
#include "DataCollector.h"
#include "VoltMeter.h"

#define WAKEUP_INTERRUPT_PIN 2
#define AC_VOLTAGE_PIN A1
#define BATTERY_VOLTAGE_PIN A0

extern BoschBME280 bme280;
extern MaximDS1338 ds1338;
extern VoltMeter voltMeter;
extern DataCollector dataCollector;
