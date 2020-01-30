#include "PowerMonitor.h"
#include <avr/sleep.h>

BoschBME280 bme280 = BoschBME280();
MaximDS1338 ds1338 = MaximDS1338();

// TODO Check how this can be replaced with the EMPTY_INTERRUPT macro.
void wakeup() {}

// Sleep until the next minute has started. This will sleep untill the next minute has started.
void sleep() {
    Serial.flush();  // TODO check if this is necessary once the LoraWAN module is attached.
    sleep_enable();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    int8_t seconds = ds1338.getSecond();
    while (seconds < 60) {
        sleep_cpu();
        seconds++;
    }
    sleep_disable();
}

void setup() {
    // Needed for the DS1338 and the BME280
    Wire.begin();

    // Needed for the RN2483A serial communication and for debugging purposes.
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
    ds1338.enableSqOut1HZ();

    // To be invoked just once when the initial setup is being done.
    ds1338.setTime(20, 1, 30, 23, 5, 0);
}

void loop() {
    bme280.measure();
    ds1338.readTime();

    Serial.print("m:");
    Serial.print(ds1338.getMinute());
    Serial.print(":");
    Serial.print(ds1338.getSecond());
    Serial.print(",t=");
    Serial.print(bme280.getTemperature() / 10.0, 1);
    Serial.print(",h=");
    Serial.println(bme280.getHumidity() / 10.0, 1);

    sleep();
}