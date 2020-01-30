#include "DataCollector.h"

bool DataCollector::addSample(uint8_t current_minute,
                              uint16_t temp,
                              uint16_t humidity,
                              uint16_t ac_voltage,
                              uint16_t battery_voltage) {
    if (getSpareCapacity() < 1) {
        return false;
    }    

    DataSample ds = DataSample();
    ds.sampleTimestamp = current_minute;
    ds.temperature = (temp + 400) / 5;
    ds.humidity = humidity / 5;
    if (ac_voltage == 0) {
        ds.ac_voltage = -128;
    } else {
        ds.ac_voltage = ac_voltage - 220;
    }
    ds.battery_voltage = battery_voltage / 2;
    memcpy(buffer + pointer, &ds, sizeof(ds));
    return true;
}

uint8_t DataCollector::getBufferSize() {
    return pointer;
} 

uint8_t* DataCollector::getBuffer() {
    return buffer;
}

uint8_t DataCollector::getSpareCapacity() {
    return (sizeof(buffer) - pointer) / sizeof(DataSample);
}

void DataCollector::reset() {
    pointer = 0;
}