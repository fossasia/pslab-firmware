#include <stdint.h>
#include <time.h>

#include "../bus/i2c/i2c.h"
#include "../bus/uart/uart.h"
#include "../commands.h"
#include "../registers/system/pin_manager.h"
#include "delay.h"
#include "light.h"
#include "rtc.h"

uint8_t bcd(uint8_t data){
    uint8_t left,right;
    uint8_t bcd = data;

    if(data >= 10){
        left = left / 10;
        right = right % 10;
        bcd = (left << 4) | right;
    }
    return bcd;
}

response_t RTC_SetTime(void) {

    time_t  unix_timestamp = (time_t) UART1_read_u32();
    struct tm *tm_info;

    tm_info = gmtime(&unix_timestamp);
    uint8_t sec = tm_info->tm_sec;
    uint8_t min = tm_info->tm_min;
    uint8_t hours = tm_info->tm_hour;
    uint8_t day = tm_info->tm_wday + 1;
    uint8_t date = tm_info->tm_mday;
    uint8_t month = tm_info->tm_mon + 1;

    if(tm_info->tm_year < 2000 || tm_info->tm_year > 2099)
        return ARGUMENT_ERROR;

    uint8_t year = (tm_info->tm_year + 1900) % 100;

    if(sec == 60)
        sec = 0;

    // Default 24 hrs format.
    uint8_t buffer[9];
    buffer[0] = DS1307_DATA_REG_SECONDS;
    buffer[1] = bcd(sec) & ~(1<<7);                      // seconds
    buffer[2] = bcd(min);                                // minutes
    buffer[3] = (bcd(hours) & (1<<5));                   // hours (hrs format)
    buffer[4] = bcd(day);                                // day
    buffer[5] = bcd(date);                               // date
    buffer[6] = bcd(month);                              // month
    buffer[7] = bcd(year);                               // year
    buffer[8] = 0;                                       // control

    I2C_InitializeIfNot(I2C_BAUD_RATE_100KHZ, I2C_ENABLE_INTERRUPTS);

    return I2C_BulkWrite(buffer, 9, DS1307_I2C_DEVICE_ADDRESS);
}

response_t RTC_SetDigit(void) {

    uint8_t buffer[2];
    buffer[0] = UART1_Read();  // register address
    buffer[1] = UART1_Read();  // data

    I2C_InitializeIfNot(I2C_BAUD_RATE_100KHZ, I2C_ENABLE_INTERRUPTS);

    return I2C_BulkWrite(buffer, 2, DS1307_I2C_DEVICE_ADDRESS);
}

response_t RTC_GetTime(void) {

    uint8_t buffer[7];
    struct tm tm_info;

    I2C_InitializeIfNot(I2C_BAUD_RATE_100KHZ, I2C_ENABLE_INTERRUPTS);

    if(I2C_BulkRead(DS1307_DATA_REG_SECONDS, DS1307_I2C_DEVICE_ADDRESS, buffer, 7) == SUCCESS) {
        tm_info.tm_sec = buffer[0];
        tm_info.tm_min = buffer[1];
        tm_info.tm_hour = buffer[2];
        tm_info.tm_wday = buffer[3] - 1;
        tm_info.tm_mday = buffer[4];
        tm_info.tm_mon = buffer[5] - 1;
        tm_info.tm_year = 2000 + buffer[6];

        tm_info.tm_sec = tm_info.tm_sec & ~(1 << 7);

        uint32_t unix_timestamp = (uint32_t) mktime(&tm_info);
        UART1_write_u32(unix_timestamp);
    } else return FAILED;

    return SUCCESS;
}

response_t RTC_GetDigit(void) {

    uint8_t buffer[1];
    uint8_t reg = UART1_Read();  // register address
    uint8_t *pR = &reg;

    I2C_InitializeIfNot(I2C_BAUD_RATE_100KHZ, I2C_ENABLE_INTERRUPTS);

    if(I2C_BulkRead(pR, DS1307_I2C_DEVICE_ADDRESS, buffer, 1) == SUCCESS) {
        UART1_Write(buffer[0]);
    } else return FAILED;

    return SUCCESS;
}
