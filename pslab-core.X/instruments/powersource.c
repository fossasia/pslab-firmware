#include <stdbool.h>
#include "../commands.h"
#include "../bus/uart/uart1.h"
#include "../bus/i2c/i2c.h"
#include "../helpers/delay.h"
#include "../registers/system/pin_manager.h"

static bool has_previous_power_source_config = false;

response_t POWER_SOURCE_SetPower(void) {
    
    uint8_t address = UART1_Read();
    uint8_t device = UART1_Read();
    uint16_t data = UART1_ReadInt();
    
    LED_SetLow();
    // If I2C interface is configured previously
    if (!has_previous_power_source_config) {
        has_previous_power_source_config = true;
        I2C_SetBaudRate(0x90);
        I2C_Initialize();
    }
    
    I2C_MESSAGE_STATUS status = I2C_MESSAGE_PENDING;
    uint8_t buffer[3];
    
    buffer[0] = address;
    buffer[1] = (data >> 8) & 0xFF;
    buffer[2] = data & 0xFF;

    // Retry transmission
    uint16_t timeOut = 0;
    uint16_t slaveTimeOut = 0;

    while(status != I2C_MESSAGE_FAIL) {
        I2C_MasterWrite(buffer, 3, device, &status);
        while(status == I2C_MESSAGE_PENDING) {
            DELAY_us(10); // TODO: Tune this delay
            if (slaveTimeOut == SLAVE_I2C_GENERIC_DEVICE_TIMEOUT) break;
            else slaveTimeOut++;
        } 
        if ((slaveTimeOut == SLAVE_I2C_GENERIC_DEVICE_TIMEOUT) || 
            (status == I2C_MESSAGE_COMPLETE))
            break;
        if (timeOut == SLAVE_I2C_GENERIC_RETRY_MAX) break;
        else timeOut++;
    }
    LED_SetHigh();
    
    return SUCCESS;
}