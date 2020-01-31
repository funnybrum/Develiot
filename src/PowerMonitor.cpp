#include "PowerMonitor.h"
#include "Helpers.h"

BoschBME280 bme280 = BoschBME280();
MaximDS1338 ds1338 = MaximDS1338();
VoltMeter voltMeter = VoltMeter();
DataCollector dataCollector = DataCollector();

void setup() {
    // Needed for the DS1338 and the BME280
    Wire.begin();

    // Needed for the RN2483A serial communication (and for debugging purposes).
    Serial.begin(9600);
    while (!Serial);
      
    // For debugging purposes.
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    // The DS1338 is pinging the INT0 pin each second. Attach a function to the pin interrupt that
    // will be used to wake up from deep sleep.
    pinMode(WAKEUP_INTERRUPT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(WAKEUP_INTERRUPT_PIN), wakeup, FALLING);

    bme280.begin();
    voltMeter.begin();
    ds1338.enableSqOut1HZ();

    // To be invoked just once when the initial setup is being done.
    ds1338.setTime(20, 1, 30, 23, 5, 0);
}

void loop() {
    bme280.measure();
    ds1338.readTime();
    uint16_t acv = voltMeter.getACVoltage();
    uint16_t bv = voltMeter.getBatteryVoltage();

    Serial.print("Temp is ");
    Serial.print(bme280.getTemperature() / 10.0);
    Serial.print("C, humidity is ");
    Serial.println(bme280.getHumidity() / 10.0);


    dataCollector.addSample(
        ds1338.getMinute(),
        bme280.getTemperature(),
        bme280.getHumidity(),
        acv,
        bv
    );

    if (shouldPush(acv)) {
        // TODO - push the data over LoraWAN.
        dataCollector.reset();
    }

    dataCollector.dumpToSerial();

    sleep();
}