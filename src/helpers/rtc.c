#include <stdint.h>
#include <time.h>

#include "../bus/i2c/i2c.h"
#include "../bus/uart/uart.h"
#include "../commands.h"
#include "../registers/system/pin_manager.h"
#include "delay.h"
#include "light.h"
#include "rtc.h"

// This bit is used to enable the oscillator. When negated, the oscillator is disabled.
uint8_t oscillator_enable = 0x7F

uint8_t data_to_bcd(uint8_t data){
    uint8_t bcd = data;

    if(data >= 10){
        uint8_t left = data / 10;
        uint8_t right = data % 10;
        bcd = (left << 4) | right;
    }
    return bcd;
}

uint8_t bcd_to_data(uint8_t bcd){
    uint8_t right = (0xF & bcd);
    uint8_t left = (bcd >> 4) & 0xF;

    left *= 10;
    return left + right;
}

response_t RTC_SetTime(uint32_t const * const unix_timestamp) {

    time_t timestamp = (time_t) *unix_timestamp;
    struct tm *tm_info;

    tm_info = gmtime(&timestamp);
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
    buffer[1] = data_to_bcd(sec) & oscillator_enable;                      // seconds
    buffer[2] = data_to_bcd(min);                                // minutes
    buffer[3] = (data_to_bcd(hours) & (1<<5));                   // hours (hrs format)
    buffer[4] = data_to_bcd(day);                                // day
    buffer[5] = data_to_bcd(date);                               // date
    buffer[6] = data_to_bcd(month);                              // month
    buffer[7] = data_to_bcd(year);                               // year
    buffer[8] = 0;                                       // control

    I2C_InitializeIfNot(I2C_BAUD_RATE_100KHZ, I2C_ENABLE_INTERRUPTS);

    return I2C_BulkWrite(buffer, 9, DS1307_I2C_DEVICE_ADDRESS);
}

response_t RTC_CmdSetTime(void) {
    uint32_t unix_timestamp = UART1_read_u32();

    response_t res = RTC_SetTime(&unix_timestamp);
    return res;
}

response_t RTC_SetDigit(void) {

    uint8_t buffer[2];
    buffer[0] = UART1_Read();  // register address
    buffer[1] = UART1_Read();  // data

    I2C_InitializeIfNot(I2C_BAUD_RATE_100KHZ, I2C_ENABLE_INTERRUPTS);

    return I2C_BulkWrite(buffer, 2, DS1307_I2C_DEVICE_ADDRESS);
}

response_t RTC_GetTime(uint32_t* unix_timestamp) {

    uint8_t buffer[7];
    struct tm tm_info;

    I2C_InitializeIfNot(I2C_BAUD_RATE_100KHZ, I2C_ENABLE_INTERRUPTS);

    if(I2C_BulkRead(DS1307_DATA_REG_SECONDS, DS1307_I2C_DEVICE_ADDRESS, buffer, 7) == SUCCESS) {

        // Need to convert from bcd to int.
        tm_info.tm_sec = bcd_to_data(buffer[0]);
        tm_info.tm_min = bcd_to_data(buffer[1]);
        tm_info.tm_hour = bcd_to_data(buffer[2]);
        tm_info.tm_wday = bcd_to_data(buffer[3] - 1);
        tm_info.tm_mday = bcd_to_data(buffer[4]);
        tm_info.tm_mon = bcd_to_data(buffer[5] - 1);
        tm_info.tm_year = bcd_to_data(2000 + buffer[6]);

        tm_info.tm_sec = tm_info.tm_sec & oscillator_enable;

        uint32_t timestamp = (uint32_t) mktime(&tm_info);
        *unix_timestamp = timestamp;
    } else return FAILED;

    return SUCCESS;
}

response_t RTC_CmdGetTime(void){
    uint32_t unix_timestamp;
    response_t res = RTC_GetTime(&unix_timestamp);

    // What if error occurs here, Returns fail.
    UART1_write_u32(unix_timestamp);
    return res;
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
