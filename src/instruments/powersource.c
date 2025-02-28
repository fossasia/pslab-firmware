#include <stdbool.h>
#include "../commands.h"
#include "../bus/uart/uart.h"
#include "../bus/i2c/i2c.h"
#include "../bus/spi/spi.h"
#include "powersource.h"

/*********/
/* Types */
/*********/

/** @brief Channel selection.
 *  @details
 *      V5 hardware has four independent channels:
 *          0: PCS
 *          1: PV3
 *          2: PV2
 *          3: PV1
 *      V6 hardware has two pairs of paired channels:
 *          0: PCS & PVS2
 *          1: PVS1 & PVS3
 *      Paired channels share relative output levels, i.e. if PV1 outputs 5 V
 *      then PV3 outputs 3.3 V.
 */
enum Channel {
    PCS,
    PV3,
    PV2,
    PV1,
    NUM_V5_CHANNELS,
    PVS2_PCS = 0,
    PVS1_PVS3,
    NUM_V6_CHANNELS,
};

/** @brief Output gain selection bit.
*   @details
*       The gain values are reversed between the MCP4822 (v6) and the
*       MCP4728 (v5). Thanks, Microchip!
*/
enum Gain {
    #ifndef V5_HW
    GAIN_X2,
    GAIN_X1,
    #else
    GAIN_X1,
    GAIN_X2,
    #endif // V5_HW
    GAINS
};
int GAINVAL[GAINS] = {[GAIN_X1] = 1, [GAIN_X2] = 2};

#ifndef V5_HW

/// @brief Output shutdown control bit.
enum Output {
    OUTPUT_OFF,
    OUTPUT_ON
};

/// @brief MCP4822 write command, 4 configuration bits + 12 data bits.
union MCP4822Command {
    struct {
        uint16_t DATA : 12;
        uint16_t SHDN : 1;
        uint16_t GA : 1;
        uint16_t : 1;
        uint16_t AB : 1;
    };
    uint16_t reg;
};

#else // V5_HW

/** @brief Output shutdown control bit.
 *  @details
 *      Also reversed compared to MCP4822. When not on, DAC output is pulled to
 *      ground via a 1K, 100K, or 500K resistor.
 */
enum Output {
    OUTPUT_ON,
    OUTPUT_OFF_1K,
    OUTPUT_OFF_100K,
    OUTPUT_OFF, // 500K
};

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

#endif // V5_HW

/********************/
/* Static functions */
/********************/

#ifndef V5_HW

static enum Status initialize(void)
{
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

/**
 * @brief Convert a V5 pin number to the corresponding pin number for the V6.
 * @details See documentation for Channel.
 * @param channel
 * @return enum Channel
 */
static enum Channel v5_to_v6_channel(enum Channel const channel)
{
    return (channel + 1) % 2;
}

/************************/
/* Command functions */
/************************/

enum Status POWER_SOURCE_SetPower(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__((unused)) uint8_t *rets[],
    __attribute__((unused)) uint16_t *rets_size
) {
    union Input {
        struct {
            uint8_t channel;
            uint16_t setpoint;
        };
        uint8_t const *buffer;
    } input = {{0}};

    if (args_size != sizeof(input)) {
        return E_BAD_ARGSIZE;
    }

    input.buffer = args;
    enum Channel const channel =  v5_to_v6_channel(input.channel & 0x03);
    uint16_t const setpoint = input.setpoint & 0xFFF;

    union MCP4822Command cmd = {{
        .DATA = setpoint,
        .SHDN = OUTPUT_ON,
        .GA = GAIN_X2,
        .AB = channel,
    }};

    enum Status status = E_OK;

    if((status = initialize())) {return status;}
    return SPI_exchange_int(SPI_PS, &cmd.reg);
}

#else // V5_HW

enum Status POWER_SOURCE_SetPower(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__((unused)) uint8_t *rets[],
    __attribute__((unused)) uint16_t *rets_size
) {
    union Input {
        struct {
            uint8_t channel;
            uint16_t setpoint;
        };
        uint8_t const *buffer;
    } input = {{0}};

    if (args_size != sizeof(input)) {
        return E_BAD_ARGSIZE;
    }

    input.buffer = args;
    enum Channel const channel =  input.channel & 0x03;
    uint16_t const setpoint = input.setpoint & 0xFFF;

    enum VRef {
        VREF_EXTERNAL,
        VREF_INTERNAL,
    };
    enum Command {
        SINGLE_WRITE = 0b01011,
    };
    union MCP4728Command cmd = {{
        .CMD = SINGLE_WRITE,
        .DAC = channel,
        .VREF = VREF_INTERNAL,
        .PDSEL = OUTPUT_ON,
        .GX = GAIN_X2,
        .DATA_L = setpoint & 0xFF,
        .DATA_H = setpoint >> 8
    }};
    I2C_InitializeIfNot(I2C_BAUD_RATE_400KHZ, I2C_ENABLE_INTERRUPTS);

    enum MCP4728Address {
        ADDRESS = 0x60,
    };

    return I2C_BulkWrite(
        cmd.buffer,
        sizeof(cmd.buffer),
        ADDRESS
    );
}

#endif // V5_HW
