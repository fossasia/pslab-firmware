#include <stdint.h>

#include "../bus/i2c/i2c.h"
#include "../bus/uart/uart.h"
#include "../commands.h"
#include "../registers/system/pin_manager.h"
#include "delay.h"
#include "light.h"
#include "rtc.h"

response_t RTC_SetTime(void) {
    
    uint8_t buffer[9];
    buffer[0] = DS1307_DATA_REG_SECONDS;
    buffer[1] = UART1_Read() & 0x7F;  // seconds
    buffer[2] = UART1_Read();         // minutes
    buffer[3] = UART1_Read();         // hours
    buffer[4] = UART1_Read();         // day
    buffer[5] = UART1_Read();         // date
    buffer[6] = UART1_Read();         // month
    buffer[7] = UART1_Read();         // year
    buffer[8] = UART1_Read();         // control

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

    I2C_InitializeIfNot(I2C_BAUD_RATE_100KHZ, I2C_ENABLE_INTERRUPTS);

    if(I2C_BulkRead(DS1307_DATA_REG_SECONDS, DS1307_I2C_DEVICE_ADDRESS, buffer, 7) == SUCCESS) {
        uint8_t i;
        for (i = 0; i < sizeof(buffer); i++) UART1_Write(buffer[i]);
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
