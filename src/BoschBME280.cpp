#include "Arduino.h"
#include "BoschBME280.h"
#include "Wire.h"

bool BoschBME280::begin(uint8_t addr) {

    bme280.intf = BME280_I2C_INTF;
    bme280.write = BoschBME280::I2CWrite;
    bme280.read = BoschBME280::I2CRead;
    bme280.delay_ms = delay;
    bme280.dev_id = addr;

    bme280.settings.osr_h = BME280_OVERSAMPLING_1X;
    bme280.settings.osr_p = BME280_NO_OVERSAMPLING;
    bme280.settings.osr_t = BME280_OVERSAMPLING_2X;

    Wire.begin();

    errors += bme280_init(&bme280) != BME280_OK;
    errors += (bme280_set_sensor_settings(BME280_OSR_TEMP_SEL|BME280_OSR_HUM_SEL, &bme280) != BME280_OK) << 1;
    errors += (bme280_set_sensor_mode(BME280_FORCED_MODE, &bme280) != BME280_OK) << 2;
    req_delay = bme280_cal_meas_delay(&bme280.settings);

    if (errors > 0) {
        Serial.print("Failed to initialize BME280:");
        Serial.println(errors);
    }

    return errors == 0;
}

bool BoschBME280::measure() {
    if (errors != 0) {
        return BME280_E_DEV_NOT_FOUND;
    }

    errors += (bme280_set_sensor_mode(BME280_FORCED_MODE, &bme280) != BME280_OK) << 3;
    bme280.delay_ms(req_delay);
    errors += (bme280_get_sensor_data(BME280_ALL, &comp_data, &bme280) != BME280_OK) << 4;

    if (errors > 0) {
        Serial.print("Failed to read BME280:");
        Serial.println(errors);
    }

    return errors == 0;
}

int16_t BoschBME280::getTemperature() {
    return comp_data.temperature / 10;
}

int16_t BoschBME280::getHumidity() {
    return (10 * comp_data.humidity) >> 8;
}

int8_t BoschBME280::I2CWrite(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint16_t cnt)
{  
    int8_t ret = BME280_OK;

    Wire.beginTransmission(dev_addr);
    Wire.write(reg_addr);
    Wire.write(reg_data, cnt);
    Wire.endTransmission();
    
    return ret;
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
