#include "PowerMonitor.h"

BoschBME280 bme280 = BoschBME280();

void setup() {
  Wire.begin();
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  
  delay(500);

  bme280.begin();
  Serial.begin(9600);
}

void loop() {
  delay(2000);
  Serial.println(bme280.measure());
  Serial.print("Temp=");
  Serial.println(bme280.getTemperature() / 10.0);
  Serial.print("Humidity=");
  Serial.println(bme280.getHumidity() / 10.0);
}