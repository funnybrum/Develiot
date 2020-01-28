#include "PowerMonitor.h"

BoschBME280 bme280 = BoschBME280();

void setup() {
  bme280.begin();
}

void loop() {
    if (bme280.measure()) {
        Serial.print("Temp=");
        Serial.println(bme280.getTemperature() / 10.0);
        Serial.print("Humidity=");
        Serial.println(bme280.getHumidity() / 10.0);
    }
}