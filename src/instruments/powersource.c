#include <stdbool.h>
#include "../commands.h"
#include "../bus/uart/uart.h"
#include "../bus/i2c/i2c.h"
#include "../bus/spi/spi.h"
#include "../helpers/delay.h"
#include "../registers/system/pin_manager.h"
#include "powersource.h"


#ifndef V5_HW

/*********/
/* Types */
/*********/

/// @brief DAC selection bit.
enum DAC
{
    DAC_A,
    DAC_B
};

/// @brief Output gain selection bit.
enum Gain
{
    X2,
    X1
};

/// @brief Output shutdown control bit.
enum Output
{
    SHUTDOWN,
    ACTIVE
};

/// @brief MCP4822 write command, 4 configuration bits + 12 data bits.
union MCP4822Command
{
    struct
    {
        uint16_t DATA : 12;
        enum Output SHDN : 1;
        enum Gain GA : 1;
        uint16_t : 1;
        enum DAC AB : 1;
    };
    uint16_t reg;
};

/********************/
/* Static functions */
/********************/

static bool initialize(void)
{
    const SPI_Config conf = {{{
        .PPRE = SPI_SCLK125000 >> 3,
        .SPRE = SPI_SCLK125000 & 7,
        .MSTEN = 1,
        .CKP = 0,
        .SSEN = 0,
        .CKE = 1,
        .SMP = 1,
        .MODE16 = 1,
        .DISSDO = 0,
        .DISSCK = 0
    }}};
    return SPI_configure(conf);
}

/*********************/
/* Command functions */
/*********************/

response_t POWER_SOURCE_SetPower(void)
{
    const enum DAC dac = (UART1_Read() + 1) % 2;
    const uint16_t power = UART1_ReadInt() & 0xFFF;
    union MCP4822Command cmd = {{
        .DATA = power,
        .SHDN = ACTIVE,
        .GA = X2,
        .AB = dac
    }};

    if(initialize())
    {
        return SPI_exchange_int(SPI_PS, &cmd.reg) ? SUCCESS : FAILED;
    }
    return FAILED;
}

#else

response_t POWER_SOURCE_SetPower(void) {

    uint8_t buffer[3];
    buffer[0] = 0x58 | ((UART1_Read() & 0x03) << 1); // Channel
    uint16_t power = UART1_ReadInt();
    buffer[1] = (power >> 8) & 0x9F;
    buffer[2] = power & 0xFF;

    I2C_InitializeIfNot(I2C_BAUD_RATE_400KHZ, I2C_ENABLE_INTERRUPTS);

    return I2C_BulkWrite(buffer, 3, MCP4728_I2C_DEVICE_ADDRESS);
}

#endif // V5_HW
