#include "Arduino.h"
#include "BoschBME280.h"
#include "Wire.h"

bool BoschBME280::begin(uint8_t addr) {
    // I2C configuration.
    bme280.intf = BME280_I2C_INTF;
    bme280.write = BoschBME280::I2CWrite;
    bme280.read = BoschBME280::I2CRead;
    bme280.delay_ms = delay;
    bme280.dev_id = addr;

    // Oversampling configuration.
    bme280.settings.osr_h = BME280_OVERSAMPLING_1X;
    bme280.settings.osr_p = BME280_NO_OVERSAMPLING;
    bme280.settings.osr_t = BME280_OVERSAMPLING_1X;
    bme280.settings.filter = BME280_FILTER_COEFF_OFF;

    // Initialize the sensor, apply the configuration and set the sensor to FORCED mode.
    errors += bme280_init(&bme280) != BME280_OK;
    errors += (bme280_set_sensor_settings(BME280_OSR_TEMP_SEL|BME280_OSR_HUM_SEL|BME280_FILTER_SEL, &bme280) != BME280_OK) << 1;
    errors += (bme280_set_sensor_mode(BME280_FORCED_MODE, &bme280) != BME280_OK) << 2;
    req_delay = bme280_cal_meas_delay(&bme280.settings);

    return errors == 0;
}

bool BoschBME280::measure() {
    if (errors != 0) {
        return false;
    }

    errors += (bme280_set_sensor_mode(BME280_FORCED_MODE, &bme280) != BME280_OK) << 3;
    bme280.delay_ms(req_delay);
    errors += (bme280_get_sensor_data(BME280_ALL, &comp_data, &bme280) != BME280_OK) << 4;

    return errors == 0;
}

int16_t BoschBME280::getTemperature() {
    return comp_data.temperature / 10;
}

int16_t BoschBME280::getHumidity() {
    return (10 * comp_data.humidity) / 1024;
}

int8_t BoschBME280::I2CWrite(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t cnt)
{  
    Wire.beginTransmission(dev_addr);
    Wire.write(reg_addr);
    Wire.write(reg_data, cnt);
    Wire.endTransmission();
    
    return BME280_OK;
}

int8_t BoschBME280::I2CRead(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t cnt)
{
    int8_t ret = BME280_OK;

    Wire.beginTransmission(dev_addr);
    
    Wire.write(reg_addr);
    Wire.endTransmission();
    
    Wire.requestFrom((int)dev_addr, (int)cnt);
    
    uint8_t available = Wire.available();
    if(available != cnt) {
        ret = BME280_E_COMM_FAIL;
    }
    
    for(uint8_t i = 0; i < available; i++) {
        if(i < cnt) {
            *(reg_data + i) = Wire.read();
        }
        else {
            Wire.read();
        }
    }

    return ret;
}
