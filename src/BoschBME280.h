#pragma once

#include "bme280.h"

#define BME280_READ_INTERVAL 5000

class BoschBME280 {
    public:
        bool begin(uint8_t addr=0x77);
        bool measure();

        // temperature with the units 10 * °C, i.e. 232 is 23.2°C
        // prior using that method a successful measure should have completed 
        int16_t getTemperature();

        // humidity with the units 10 * % relative humidity, i.e. 532 is 53.2% RH.
        // prior using that method a successful measure should have completed 
        int16_t getHumidity();
    private:
        static int8_t I2CRead(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t cnt);
        static int8_t I2CWrite(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t cnt);

        uint8_t errors = 0;
        uint32_t req_delay;
        struct bme280_dev bme280;
        struct bme280_data comp_data;
};
