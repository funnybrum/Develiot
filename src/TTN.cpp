#include "TTN.h"
#include "stddef.h"

bool TTN::begin() {
    rn2483aSerial.begin(4800);

    // Custom implementation for the autobaud operation. The one provided by the TTN library does
    // not work.
    digitalWrite(RN2483A_RX_PIN, 0);
    delay(100);
    rn2483aSerial.write(0x55);
    rn2483aSerial.flush();

    Serial.println("-- STATUS");
    ttn.showStatus();

    Serial.println("-- JOIN");
    connected = ttn.join(TTN_EUI, TTN_KEY);
    return connected;
}

void TTN::sleep() {
    if (!sleeping) {
        // Put the device in sleep forever. Sending message will wake up the device.
        ttn.sleep(4294967295L);
        sleeping = true;
        rn2483aSerial.flush();
    }
}

void TTN::loop() {
    if (!sleeping) {
        // Poll incoming messages only if the module is not sleeping.
        ttn.poll();
    }
}

bool TTN::send(uint8_t* buff, uint8_t size) {
    if (!connected) {
        return false;
    }

    if (sleeping) {
        ttn.wake();
        sleeping = false;
    }

    return ttn.sendBytes(buff, size) == TTN_SUCCESSFUL_TRANSMISSION;
}

void TTN::onMessage(const uint8_t *payload, size_t size, port_t port) {
    // Set time command
    Serial.println("Received command!");
    if (size == 7 && payload[0]==0x01) {
        Serial.println("Setting the time.");
        ds1338.setTime(payload[1], payload[2], payload[3], payload[4], payload[5], payload[6]);
    }
}