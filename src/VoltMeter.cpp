#include "VoltMeter.h"
#include "PowerMonitor.h"

void VoltMeter::begin() {
    // Use the internal 1.1V as reference voltage for the ADC.
    analogReference(INTERNAL);
}

uint16_t VoltMeter::getACVoltage() {
    // Measuring AC voltage requires several samples to catch the peak voltage. There is a voltage
    // peak every 0.01 seconds. To make sure we get a peak we'll take measurments for 0.05 seconds.
    // The arduino on ATmega 328P can do ~10 000 analog reads per second, so for 0.02 seconds these
    // are ~200 measurments.

    int val = 0;
    for (int i = 0; i < 200; i++) {
        val = max(val, analogRead(AC_VOLTAGE_PIN));
    }

    // Based on calculation the voltage at A1 will be 1.1V if the AC voltage is 286.4V.
    val = map(val, 0, 1023, 0, 286);
    
    // Consider anything below 5V AC as no AC.
    if (val < 5) {
        val = 5;
    }

    return val;
}

uint16_t VoltMeter::getBatteryVoltage() {
    int val = analogRead(BATTERY_VOLTAGE_PIN);
    // Based on calculation the voltage at A0 will be 1.1V if the battery voltage is 4.73V.
    return map(val, 0, 1023, 0, 473);
}

