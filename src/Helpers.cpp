#include "PowerMonitor.h"
#include "avr/sleep.h"

/**
 * A function checking if the data should be send. This will be done if one of the following is
 * true:
 *   1) AC voltage is zero, but previous time it wasn't zero. This will allow an alert to be
 *      triggered.
 *   2) AC was zero, but now it is not zero. This will be used as indication that power is
 *      restored.
 *   3) If the buffer used for storing data is full.
 *   4) If there is AC and the buffer has at least 10 mintues of data. 
 */
uint16_t prevACVoltage = 9999;
bool shouldPush(uint16_t acVoltage) {
    // If AC has dropped - send the data to trigger an alert.
    if ((prevACVoltage > 0 && acVoltage == 0) ||
        (prevACVoltage == 0 && acVoltage > 0)) {
        prevACVoltage = acVoltage;
        return true;
    }
    prevACVoltage = acVoltage;

    // If the buffer is full - send the data.
    if (dataCollector.getSpareCapacity() == 1) {
        return true;
    }

    // If there is AC and we have collecte 10 samples - send the data.
    if (acVoltage > 0 && dataCollector.getSamplesCount() >= 10) {
        return true;
    }

    return false;
}

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