#include <stdbool.h>
#include "../commands.h"
#include "../bus/uart/uart1.h"
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

    I2C_InitializeIfNot(I2C_BAUD_RATE_400KHZ, true);
    
    return I2C_BulkWrite(buffer, 3, MCP4728_I2C_DEVICE_ADDRESS);
}