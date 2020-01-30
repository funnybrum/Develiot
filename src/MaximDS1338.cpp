#include "MaximDS1338.h"
#include "Wire.h"

#define DS1338_ADDR 0x68
#define DS1338_REG_SECONDS 0x00
#define DS1338_REG_MINUTES 0x01
#define DS1338_REG_HOURS 0x02
#define DS1338_REG_DAY 0x03
#define DS1338_REG_DATE 0x04
#define DS1338_REG_MONTH 0x05
#define DS1338_REG_YEAR 0x06
#define DS1338_REG_CONTROL 0x07
#define DS1338_ONE_HZ_OUT 0b00110000

#define decode_bcd(x) ((x >> 4) * 10 + (x & 0x0F))
#define encode_bcd(x) ((((x / 10) & 0x0F) << 4) + (x % 10))

bool MaximDS1338::enableSqOut1HZ() {
    uint8_t buf[2];
	buf[0] = DS1338_REG_CONTROL;
	buf[1] = DS1338_ONE_HZ_OUT;
	return I2CWrite(DS1338_ADDR, buf, 2) == 0;
}

bool MaximDS1338::setTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
    uint8_t buf[8];

	buf[0] = DS1338_REG_SECONDS;
	buf[1] = encode_bcd(second);
	buf[2] = encode_bcd(minute);
	buf[3] = encode_bcd(hour);
	buf[4] = 1;		                // Not used?
	buf[5] = encode_bcd(day);
	buf[6] = encode_bcd(month);
	buf[7] = encode_bcd(year);

	uint8_t res = I2CWrite(DS1338_ADDR, buf, 8);

	if (res) {
		Serial.print("Failed to set time: ");
        Serial.println(res);
	}

	return res == 0;
}

bool MaximDS1338::readTime() {
    uint8_t buf[7];
    buf[0] = DS1338_REG_SECONDS;
    uint8_t res = I2CWrite(DS1338_ADDR, buf, 1);

    if (res) {
		Serial.print("Failed to send read time CMD: ");
        Serial.println(res);
        return false;
	}

    res = I2CRead(DS1338_ADDR, buf, 7);

    if (res) {
		Serial.print("Failed to read time: ");
        Serial.println(res);
        return false;
	}

    time.second = decode_bcd(buf[0]);
	time.minute = decode_bcd(buf[1]);
    time.hour = decode_bcd(buf[2]);
	time.day = decode_bcd(buf[4]);
	time.month = decode_bcd(buf[5]);
	time.year = decode_bcd(buf[6]);
    return true;
}

uint8_t MaximDS1338::getMinute() {
    return time.minute;
}

uint8_t MaximDS1338::getSecond() {
	return time.second;
}

int8_t MaximDS1338::I2CWrite(uint8_t addr, uint8_t *buf, uint8_t num) {
	Wire.beginTransmission(addr);
	for (uint8_t i = 0; i < num; i++) {
		Wire.write(buf[i]);
	}
	return Wire.endTransmission();
}

int8_t MaximDS1338::I2CRead(uint8_t addr, uint8_t *buf, uint8_t num) {
    Wire.requestFrom(addr, num);

	if (Wire.available() < num) {
		return 1;
	}

	for (uint8_t i = 0; i < num; i++) {
		buf[i] = Wire.read();
	}

	return 0;
}