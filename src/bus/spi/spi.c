#include <stdint.h>
#include <stdlib.h>
#include <xc.h>

#include "../../commands.h"
#include "../uart/uart.h"
#include "spi.h"

/*********/
/* Types */
/*********/

/// @brief Aggregate of values to write to mux control pins.
struct MuxInput
{
    uint16_t RA10; ///< Mux pin S0.
    uint16_t RA7;  ///< Mux pin S1.
    uint16_t RB9;  ///< Mux pin S2. Not used in V5.
    uint16_t RC4;  ///< Only used in V5.5.
};

/// @brief Bus transaction type.
enum Direction
{
    READ,
    WRITE,
    EXCHANGE,
};

/// @brief Values for SPI1CON1bits.MODE16.
enum Width
{
    BYTE,
    WORD,
};

/********************/
/* Static functions */
/********************/

/**
 * @brief Enable SPI bus and select slave.
 * @pre An appropriate configuration must be set with SPI_configure() before
 *      using this function.
 * @post close() must be called to release the bus.
 * @param cs One of tSPI_CS.
 * @return false if bus is already open.
 *         true otherwise.
 */
static bool open(const tSPI_CS cs)
{
    if (SPI1STATbits.SPIEN)
    {
        return false;
    }
    SPI1STATbits.SPIEN = 1;
    SPI_chip_select(cs);
    return true;
}

/// @brief Deselect slaves and disable SPI bus.
static void close(void)
{
    SPI_chip_select(SPI_DESELECT);
    SPI1STATbits.SPIEN = 0;
}

/**
 * @brief Exchange a single byte or word.
 * @pre The bus must be enabled with open() before calling this function.
 * @param data Data to be written to the bus.
 * @return Data read from the bus.
*/
static uint16_t exchange(const uint16_t data)
{
    SPI1BUF = data;
    while (!SPI1STATbits.SPIRBF) {;}
    return SPI1BUF;
}

/**
 * @brief Exchange a single byte or word.
 * @note This function takes care of opening and closing the bus.
 * @pre An appropriate configuration must be set with SPI_configure() before
 *      using this function.
 * @param cs One of tSPI_CS.
 * @param data A pointer to either a uint8_t (if MODE16 == 0) or a uint16_t
 *             (if MODE16 == 1).
 * @return false if the bus is already open.
 *         true otherwise.
*/
static bool exchange_single(const tSPI_CS cs, void* const data)
{
    if (!open(cs))
    {
        return false;
    }

    if (SPI1CON1bits.MODE16 == BYTE)
    {
        *((uint8_t*)data) = (uint8_t)exchange((uint16_t)(*(uint8_t*)data));
    }
    else
    {
        *((uint16_t*)data) = exchange(*(uint16_t*)data);
    }

    close();
    return true;
}

/**
 * @brief Wrapper to make UART_Read() type-compatible with UART_ReadInt.
 * @details When reading or writing to serial, we want to use either
 *          UART_Read/Write() or UART_ReadInt/WriteInt() depending on the
 *          value of MODE16. A pair of function pointers are set to either
 *          pair, but they must be type compatible for this to work.
*/
static uint16_t uart_read_wrapper(const EUxSelect usel)
{
    return (uint16_t)UART_Read(usel);
}

/**
 * @brief Wrapper to make UART_Write() type-compatible with UART_WriteInt.
 */
static void uart_write_wrapper(const EUxSelect usel, const uint16_t data)
{
    UART_Write(usel, (uint8_t)data);
}

/**
 * @brief Read and/or write data from UART to SPI.
 * @param usel One of U1SELECT or U2SELECT.
 * @param dir One of READ, WRITE, EXCHANGE.
 * @param count Number of bytes/words to read and/or write.
*/
static void uart_exchange(
    const EUxSelect usel,
    const enum Direction dir,
    const uint16_t count
) {
     uint16_t (*uart_read)(EUxSelect usel);
     void (*uart_write)(EUxSelect usel, uint16_t data);

    if (SPI1CON1bits.MODE16 == BYTE)
    {
        uart_read = uart_read_wrapper;
        uart_write = uart_write_wrapper;
    }
    else // WORD
    {
        uart_read = UART_ReadInt;
        uart_write = UART_WriteInt;
    }

    for (size_t i; i < count; i++)
    {
        if (dir == READ)
        {
            uart_write(usel, exchange(0));
        }
        else if (dir == WRITE)
        {
            exchange(uart_read(usel));
        }
        else // EXCHANGE
        {
            uart_write(usel, exchange(uart_read(usel)));
        }
    }
}

/**
 * @brief Do task common to several command functions.
 * @details All read/write/exchange command functions first read the chip
 *          select pin, followed by the number of bytes/words to trancieve.
 *          They also do some checks to make sure the bus is configured
 *          appropriately. Since these tasks are the same, they are handled in
 *          this function. It return a response_t which the calling function
 *          can return in turn.
 * @return FAILED if MODE16 is not the same as width.
 *         FAILED if the bus is already open.
 *         SUCCESS otherwise.
*/
static response_t command(const enum Direction dir, const enum Width width)
{
    tSPI_CS cs = UART1_Read();
    uint16_t count = UART1_ReadInt();

    if (SPI1CON1bits.MODE16 != width)
    {
        return FAILED;
    }

    if (!open(cs))
    {
        return FAILED;
    }

    uart_exchange(U1SELECT, dir, count);
    close();
    return SUCCESS;
}

/********************/
/* Public functions */
/********************/

/**
 * @details A multiplexer controls the chip select (CS) pins of the PSLab's
 * built-in SPI devices. The multiplexer is in turn controlled by MCU pins
 * RA10, RA7, and RB9. Settings these pins either high or low results in one
 * of the mux's seven outputs being driven low.
 *
 * However, in earlier versions of the hardware, the mux was not present. In
 * the V5, only RA7 and RA10 are used, with each controlling the CS of one of
 * the oscilloscope PGAs directly. The V5.5 added the SD-card slot and an
 * external CS pin available on the SPI header. These are directly controlled
 * by RB9 and RC4. In the V6, the RC4 pin is unconnected.
 *
 * The following table summarizes the above information:
 *
 * Pin states       | Asserted chip select(s)
 * RC4 RB9 RA7 RA10 | V6    V5.5               V5
 * 0   0   0   0      CH1   CH1, CH2, HD, SD   CH1, CH2
 * 0   0   0   1      CH2   CH2, HD, SD        CH2
 * 0   0   1   0      PS    CH1, HD, SD        CH1
 * 0   0   1   1      NC    HD, SD             NC
 * 0   1   0   0      HD    CH1, CH2, HD       CH1, CH2
 * 0   1   0   1      SD    CH2, HD            CH2
 * 0   1   1   0      NC    CH1, HD            CH1
 * 0   1   1   1      NC    HD                 NC
 * 1   0   0   0      CH1   CH1, CH2, SD       CH1, CH2
 * 1   0   0   1      CH2   CH2, SD            CH2
 * 1   0   1   0      PS    CH1, SD            CH1
 * 1   0   1   1      NC    SD                 NC
 * 1   1   0   0      HD    CH1, CH2           CH1, CH2
 * 1   1   0   1      SD    CH2                CH2
 * 1   1   1   0      NC    CH1                CH1
 * 1   1   1   1      NC    NC                 NC
 *
 * CH1: Oscilloscope channel 1 programmable gain amplifier
 * CH2: Oscilloscope channel 2 programmable gain amplifier
 * PS:  Programmable current- and voltage sources
 * HD:  External header pin
 * SD:  SD-card
 * NC:  Not connected / All CS pins deselected.
 */
#ifdef V5_HW
void SPI_chip_select(tSPI_CS cs)
#else
void SPI_chip_select(const tSPI_CS cs)
#endif // V5_HW
{
    #ifdef V5_HW
    switch (cs) {
    case SPI_CH1:
        cs = _V5_CH1;
        break;
    case SPI_CH2:
        cs = _V5_CH2;
        break;
    case SPI_HD:
        cs = _V5_HD;
        break;
    case SPI_SD:
        cs = _V5_SD;
        break;
    default:
        cs = SPI_DESELECT;
    }
    #endif // V5_HW
    struct MuxInput pins = {
        .RA10 = cs & 1,
        .RA7 = (cs >> 1) & 1,
        .RB9 = (cs >> 2) & 1,
        .RC4 = (cs >> 3) & 1,
    };

    _LATA10 = pins.RA10;
    _LATA7  = pins.RA7;
    _LATB9  = pins.RB9;
    _LATC4  = pins.RC4;
}

bool SPI_configure(const SPI_Config conf)
{
    if (SPI1STATbits.SPIEN)
    {
        return false;
    }
    SPI1CON1bits = conf;
    SPI1CON2 = 0;
    SPI1STAT = 0;
    return true;
}

bool SPI_exchange_byte(const tSPI_CS cs, uint8_t* const data)
{
    if (SPI1CON1bits.MODE16 == WORD)
    {
        return false;
    }
    return exchange_single(cs, data);
}

bool SPI_exchange_int(const tSPI_CS cs, uint16_t* const data)
{
    if (SPI1CON1bits.MODE16 == BYTE)
    {
        return false;
    }
    return exchange_single(cs, data);
}

/*********************/
/* Command functions */
/*********************/

response_t SPI_conf(void)
{
    union
    {
        SPI1CON1BITS con1bits;
        uint16_t con1;
    } conf;
    conf.con1 = UART1_ReadInt();
    bool conf_ok = SPI_configure(conf.con1bits);

    if (conf_ok)
    {
        return SUCCESS;
    }
    return FAILED;
}

response_t SPI_read_bytes(void)
{
    return command(READ, BYTE);
}

response_t SPI_write_bytes(void)
{
    return command(WRITE, BYTE);
}

response_t SPI_exchange_bytes(void)
{
    return command(EXCHANGE, BYTE);
}

response_t SPI_read_ints(void)
{
    return command(READ, WORD);
}

response_t SPI_write_ints(void)
{
    return command(WRITE, WORD);
}

response_t SPI_exchange_ints(void)
{
    return command(EXCHANGE, WORD);
}
