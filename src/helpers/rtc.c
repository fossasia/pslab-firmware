#include <stdint.h>
#include <time.h>

#include "../bus/i2c/i2c.h"
#include "../commands.h"
#include "../registers/system/pin_manager.h"
#include "delay.h"
#include "light.h"
#include "rtc.h"

#define DS1307_I2C_DEVICE_ADDRESS 0x68

typedef enum {
    DS1307_DATA_REG_SECONDS, // Clock Halt (1) | 10 Seconds (3) | Seconds (4)
    DS1307_DATA_REG_MINUTES, // XX (1) | 10 Minutes (3) | Minutes (4)
    DS1307_DATA_REG_HOURS,   // XX (1) | 12/24 | AM/PM | 10h | Hours (4)
    DS1307_DATA_REG_DAYS,    // XX (5) | Day (3)
    DS1307_DATA_REG_DATES,   // XX (2) | 10 Dates (2) | Dates (4)
    DS1307_DATA_REG_MONTHS,  // XX (3) | 10 Months (1) | Month (4)
    DS1307_DATA_REG_YEARS,   // 10 Years (4) | Year (4) [00 - 99]
    DS1307_DATA_REG_CONTROL  // OUT (1) | XX (2) | SQWE (1) | XX (2) | RS1:0 (2)
} DS1307_DATA_REG;

// This bit is used to enable the oscillator. When negated, the oscillator is disabled.
static uint8_t const oscillator_enable = 0x7F;

static uint8_t data_to_bcd(uint8_t data){
    uint8_t bcd = data;

    if(data >= 10){
        uint8_t left = data / 10;
        uint8_t right = data % 10;
        bcd = (left << 4) | right;
    }
    return bcd;
}

static uint8_t bcd_to_data(uint8_t bcd){
    uint8_t right = (0xF & bcd);
    uint8_t left = (bcd >> 4) & 0xF;

    left *= 10;
    return left + right;
}

enum Status RTC_set_time(time_t const *const unix_timestamp) {
    struct tm *const tm_info = gmtime(unix_timestamp);
    uint8_t sec = tm_info->tm_sec % 60;
    uint8_t min = tm_info->tm_min;
    uint8_t hours = tm_info->tm_hour;
    uint8_t day = tm_info->tm_wday + 1;
    uint8_t date = tm_info->tm_mday;
    uint8_t month = tm_info->tm_mon + 1;
    uint8_t year = tm_info->tm_year % 100; // Tm_year starts from 00 which means(1900).
                                           // ds1307 only stores the last 2 digits.

    // Default 24 hrs format.
    uint8_t buffer[9];
    buffer[0] = DS1307_DATA_REG_SECONDS;
    buffer[1] = data_to_bcd(sec) & oscillator_enable;            // seconds
    buffer[2] = data_to_bcd(min);                                // minutes
    buffer[3] = data_to_bcd(hours);                             // hours (hrs format)
    buffer[4] = data_to_bcd(day);                                // day
    buffer[5] = data_to_bcd(date);                               // date
    buffer[6] = data_to_bcd(month);                              // month
    buffer[7] = data_to_bcd(year);                               // year
    buffer[8] = 0;                                               // control

    I2C_InitializeIfNot(I2C_BAUD_RATE_100KHZ, I2C_ENABLE_INTERRUPTS);

    return I2C_BulkWrite(buffer, 9, DS1307_I2C_DEVICE_ADDRESS);
}

enum Status RTC_cmd_set_time(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {
    time_t unix_timestamp = 0;

    if (args_size != sizeof(unix_timestamp)) {
        return E_BAD_ARGSIZE;
    }

    unix_timestamp = *(time_t *)args;
    enum Status res = RTC_set_time(&unix_timestamp);
    return res;
}

enum Status RTC_get_time(time_t *const unix_timestamp) {
    uint8_t buffer[7] = {0};
    struct tm tm_info = {0};
    enum Status status = E_OK;

    I2C_InitializeIfNot(I2C_BAUD_RATE_100KHZ, I2C_ENABLE_INTERRUPTS);
    status = I2C_BulkRead(
        DS1307_DATA_REG_SECONDS,
        DS1307_I2C_DEVICE_ADDRESS,
        buffer,
        sizeof(buffer)
    );

    // Need to convert from bcd to int.
    tm_info.tm_sec = bcd_to_data(buffer[0]);
    tm_info.tm_min = bcd_to_data(buffer[1]);
    tm_info.tm_hour = bcd_to_data(buffer[2]);
    tm_info.tm_wday = bcd_to_data(buffer[3]) - 1;
    tm_info.tm_mday = bcd_to_data(buffer[4]);
    tm_info.tm_mon = bcd_to_data(buffer[5]) - 1;
    tm_info.tm_year = 100 + bcd_to_data(buffer[6]); // 100 means for year 2000 (2000 - 1900)
    tm_info.tm_sec = tm_info.tm_sec & oscillator_enable;

    *unix_timestamp = mktime(&tm_info);
    return status;
}

enum Status RTC_cmd_get_time(
    uint8_t args[],
    __attribute__ ((unused)) uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *rets_size
) {
    time_t unix_timestamp = 0;
    enum Status status = RTC_get_time(&unix_timestamp);
    *(time_t *)args = unix_timestamp;
    *rets = args;
    *rets_size = sizeof(unix_timestamp);
    return status;
}
