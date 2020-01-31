#pragma once

#include "Arduino.h"

class VoltMeter {
    public:
        void begin();
        uint16_t getBatteryVoltage();
        uint16_t getACVoltage();
};
