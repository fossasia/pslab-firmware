#include <stdbool.h>
#include "../commands.h"
#include "../bus/uart/uart.h"
#include "../bus/i2c/i2c.h"
#include "../helpers/delay.h"
#include "../registers/system/pin_manager.h"
#include "powersource.h"

response_t POWER_SOURCE_SetPower(void) {

    uint8_t buffer[3];
    buffer[0] = 0x58 | ((UART1_Read() & 0x03) << 1); // Channel
    uint16_t power = UART1_ReadInt();
    buffer[1] = (power >> 8) & 0x9F;
    buffer[2] = power & 0xFF;

    I2C_InitializeIfNot(I2C_BAUD_RATE_400KHZ, ENABLE_INTERRUPTS);
    
    return I2C_BulkWrite(buffer, 3, MCP4728_I2C_DEVICE_ADDRESS);
}

response_t POWER_SOURCE_SetDAC(void) {

    uint8_t address = UART1_Read();
    uint8_t channel = UART1_Read();
    uint8_t power = UART1_ReadInt();

    I2C_InitializeIfNot(I2C_BAUD_RATE_400KHZ, DISABLE_INTERRUPTS);

    I2C_StartSignal();
    I2C_Transmit(address);
    I2C_Transmit(64 | (channel << 1));
    I2C_Transmit(0xFF & (power >> 8));
    I2C_Transmit(0xFF & power);
    I2C_StopSignal();

    DELAY_us(6);

    return SUCCESS;
}