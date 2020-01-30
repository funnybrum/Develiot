#pragma once

#include "Arduino.h"


// Note: further optimization is possible as we have a few spare bits below. But for sake of
// clarity I'm skipping this for now.
struct DataSample {
    uint8_t sampleTimestamp;    // In minutes of current hour. I.e. for 13:23 the ts is 23. This
                                // will be used by the server to find the correct timestamp.
    uint8_t humidity;           // resolution is 0.5%
    uint8_t temperature;        // resolution is 0.5C, 0 is -40C, 255 is 88C.
    int8_t ac_voltage;          // AC voltage offset from 220V. -128 is special case and means
                                // that there is no AC.
    uint8_t battery_voltage;    // The battery voltage. In 0.02V. Range is from 0V to 5.12V.
};

class DataCollector {
    public:
        // Add a sample to the data collector. Return false iff the buffer is full. The buffer can
        // accomodate up to 243 bytes (this is the maximum LoraWAN payload).
        bool addSample(uint8_t current_minute,
                       uint16_t temp,               // In 0.1C
                       uint16_t humidity,           // In 0.1% RH
                       uint16_t ac_voltage,         // In 1V
                       uint16_t battery_voltage);   // In 0.01V

        // Get buffer size in bytes.
        uint8_t getBufferSize(); 

        // Get the buffer.
        uint8_t* getBuffer();

        // Get how many more samples can be collected in the current buffer.
        uint8_t getSpareCapacity();

        // Reset the buffer.
        void reset();

    private:
        uint8_t buffer[243];
        uint8_t pointer = 0;
};
