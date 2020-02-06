#include "Arduino.h"
#include "Wire.h"
#include "BoschBME280.h"
#include "MaximDS1338.h"
#include "DataCollector.h"
#include "VoltMeter.h"
#include "TTN.h"

#define WAKEUP_INTERRUPT_PIN 2
#define AC_VOLTAGE_PIN A1
#define BATTERY_VOLTAGE_PIN A0
#define RN2483A_RX_PIN 4
#define RN2483A_TX_PIN 3
#define TTN_EUI "70B3D57ED0029044"
#define TTN_KEY "7B1BF6D72AFE411BA2F8FC6BCC08977E"


extern BoschBME280 bme280;
extern MaximDS1338 ds1338;
extern VoltMeter voltMeter;
extern DataCollector dataCollector;