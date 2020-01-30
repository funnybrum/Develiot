#pragma once

#include "Arduino.h"

struct DateTime
{
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t day;
  uint8_t month;
  uint8_t year;
};

class MaximDS1338 {
    public:
        // Enable the output with 1hz frequency.
        bool enableSqOut1HZ();

        // Read the clock time.
        bool readTime();

        // Set the time. Year is the year - 2000. I.e. for 2020 -> year = 20.
        bool setTime(uint8_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);

        // Get the current minute
        uint8_t getMinute();

        // Get the current second
        uint8_t getSecond();

    private:
        int8_t I2CRead(uint8_t addr, uint8_t *buf, uint8_t num);
        int8_t I2CWrite(uint8_t addr, uint8_t *buf, uint8_t num);

        DateTime time;
        uint8_t errors = 0;
};
