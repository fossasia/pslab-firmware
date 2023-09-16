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
enum DAC {
    DAC_A,
    DAC_B
};

/// @brief Output gain selection bit.
enum Gain {
    GAIN_X2,
    GAIN_X1
};

/// @brief Output shutdown control bit.
enum Output {
    OUTPUT_OFF,
    OUTPUT_ON
};

/// @brief MCP4822 write command, 4 configuration bits + 12 data bits.
union MCP4822Command {
    struct {
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

static bool initialize(void) {
    const SPI_Config conf = {{{
        .PPRE = SPI_SCLK125000 >> 3,
        .SPRE = SPI_SCLK125000 & 7,
        .MSTEN = 1,
        .CKP = SPI_IDLE_LOW,
        .SSEN = 0,
        .CKE = SPI_SHIFT_TRAILING,
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

response_t POWER_SOURCE_SetPower(void) {
    enum DAC const dac = (UART1_Read() + 1) % 2;
    uint16_t const voltage = UART1_ReadInt() & 0xFFF;
    union MCP4822Command cmd = {{
        .DATA = voltage,
        .SHDN = OUTPUT_ON,
        .GA = GAIN_X2,
        .AB = dac
    }};

    if(initialize()) {
        return (
            SPI_exchange_int(SPI_PS, &cmd.reg) ? SUCCESS : FAILED
        );
    }

    return FAILED;
}

#else

union MCP4728Command {
    struct {
        uint16_t : 1; // UDAC
        uint16_t DAC : 2;
        uint16_t CMD : 5;

        uint16_t DATA_H : 4;
        uint16_t GX : 1;
        uint16_t PDSEL : 2;
        uint16_t VREF : 1;

        uint16_t DATA_L : 8;
    };
    uint8_t buffer[3];
};

response_t POWER_SOURCE_SetPower(void) {
    uint8_t const dac = UART1_Read() & 0x03;
    uint16_t const voltage = UART1_ReadInt() & 0xFFF;
    union MCP4728Command cmd = {{
        .CMD = 0b01011, // Single write
        .DAC = dac,
        .VREF = 1, // Internal
        .PDSEL = 0, // Normal mode
        .GX = 1, // Gain x2
        .DATA_L = voltage & 0xFF,
        .DATA_H = (voltage >> 8) & 0xF
    }};
    I2C_InitializeIfNot(I2C_BAUD_RATE_400KHZ, I2C_ENABLE_INTERRUPTS);

    return I2C_BulkWrite(
        cmd.buffer,
        sizeof(cmd.buffer),
        MCP4728_I2C_DEVICE_ADDRESS
    );
}

#endif // V5_HW
