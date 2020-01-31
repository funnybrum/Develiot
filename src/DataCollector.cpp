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
    pointer += sizeof(ds);
    return true;
}

uint8_t DataCollector::getBufferSize() {
    return pointer;
} 

uint8_t* DataCollector::getBuffer() {
    return buffer;
}

uint8_t DataCollector::getSamplesCount() {
    return pointer / sizeof(DataSample);
}

uint8_t DataCollector::getSpareCapacity() {
    return (sizeof(buffer) - pointer) / sizeof(DataSample);
}

void DataCollector::reset() {
    pointer = 0;
}

void DataCollector::dumpToSerial() {
    for (int i = 0; i < pointer; i+=sizeof(DataSample)) {
        Serial.print(i);
        Serial.print(" - M: ");
        Serial.print(buffer[i]);
        Serial.print(", H: ");
        Serial.print(buffer[i+1] / 2.0);
        Serial.print(", T: ");
        Serial.print(buffer[i+2] / 2.0 - 40);
        Serial.print(", V: ");
        Serial.print(buffer[i+3] + 220);
        Serial.print(", v: ");
        Serial.println(buffer[i+4] / 100.0);
        Serial.flush();
    }
}