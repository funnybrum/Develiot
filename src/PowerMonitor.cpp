#include "PowerMonitor.h"
#include "Helpers.h"

BoschBME280 bme280 = BoschBME280();
MaximDS1338 ds1338 = MaximDS1338();
VoltMeter voltMeter = VoltMeter();
DataCollector dataCollector = DataCollector();
TTN ttn = TTN();

void setup() {
    // Needed for the DS1338 and the BME280
    Wire.begin();

    // Needed for the RN2483A serial communication (and for debugging purposes).
    Serial.begin(9600);
    while (!Serial);

    // The DS1338 is pinging the INT0 pin each second. Attach a function to the pin interrupt that
    // will be used to wake up from deep sleep.
    pinMode(WAKEUP_INTERRUPT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(WAKEUP_INTERRUPT_PIN), wakeup, FALLING);

    bme280.begin();
    voltMeter.begin();
    ds1338.enableSqOut1HZ();
    bool ttnConnected = ttn.begin();

    if (!ttnConnected) {
        Serial.print("Failed to connect to TTN over LoRaWAN!");
    }

    // For testing purposes on the prototype board with no RTC battery.
    ds1338.setTime(20, 1, 30, 23, 5, 0);
}

void loop() {
    ttn.loop();
    bme280.measure();
    ds1338.readTime();
    uint16_t acv = voltMeter.getACVoltage();
    uint16_t bv = voltMeter.getBatteryVoltage();

    dataCollector.addSample(
        ds1338.getMinute(),
        bme280.getTemperature(),
        bme280.getHumidity(),
        acv,
        bv
    );

    if (shouldPush(acv)) {
        if (ttn.send(dataCollector.getBuffer(), dataCollector.getBufferSize())) {
            dataCollector.reset();
        }
    }

    if (acv == 0) {
        // If there is no AC - keep the LoRaWAN module in sleep.
        ttn.sleep();
    }

    sleep();
}