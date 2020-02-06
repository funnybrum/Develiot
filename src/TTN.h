#pragma once

#include "PowerMonitor.h"
#include "TheThingsNetwork.h"
#include "SoftwareSerial.h"

class TTN {
    public:
        bool begin();
        bool send(uint8_t* buffer, uint8_t size);
        void sleep();
        void loop();
    private:
        static void onMessage(const uint8_t *payload, size_t size, port_t port);

        SoftwareSerial rn2483aSerial = SoftwareSerial(RN2483A_TX_PIN, RN2483A_RX_PIN);
        TheThingsNetwork ttn = TheThingsNetwork(rn2483aSerial, Serial, TTN_FP_EU868, 7);
        bool sleeping = false;
        bool connected = false;
};