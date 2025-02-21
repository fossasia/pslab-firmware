#include <stdint.h>
#include <stdlib.h>
#include <xc.h>

#include "../../commands.h"
#include "../../transport/host.h"
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
 * @brief Wrapper to make HOST_read_u8 type-compatible with HOST_read_u16.
 * @details When reading or writing to host, we want to use either
 *          HOST_read/write_u8 or HOST_read/write_u16 depending on the
 *          value of MODE16. A pair of function pointers are set to either
 *          pair, but they must be type compatible for this to work.
*/
static enum Status host_read_wrapper(uint16_t *const u16)
{
    return HOST_read_u8((uint8_t *const)u16);
}

/**
 * @brief Wrapper to make HOST_write_u8 type-compatible with HOST_write_u16.
 */
static enum Status host_write_wrapper(uint16_t const u16)
{
    return HOST_write_u8((uint8_t const)u16);
}

/**
 * @brief Read and/or write data from HOST to SPI.
 * @param dir One of READ, WRITE, EXCHANGE.
 * @param count Number of bytes/words to read and/or write.
*/
static enum Status host_exchange(
    const enum Direction dir,
    const uint16_t count
) {
     enum Status (*host_read)(uint16_t *const u16);
     enum Status (*host_write)(uint16_t const u16);
     enum Status status = E_OK;

    switch (SPI1CON1bits.MODE16) {
    case BYTE:
        host_read = host_read_wrapper;
        host_write = host_write_wrapper;
        break;
    case WORD:
        host_read = HOST_read_u16;
        host_write = HOST_write_u16;
        break;
    default:
        status = E_BAD_ARGUMENT;
        break;
    }

    for (size_t i = 0; i < count; ++i) {
        uint16_t data = 0;

        switch (dir) {
        case READ:
            status |= host_write(exchange(0));
            break;
        case WRITE:
            status |= host_read(&data);
            exchange(data);
            break;
        case EXCHANGE:
            status |= host_read(&data);
            status |= host_write(exchange(data));
            break;
        default:
            status = E_BAD_ARGUMENT;
            break;
        }
    }

    return status;
}

/**
 * @brief Do task common to several command functions.
 * @details All read/write/exchange command functions first read the chip
 *          select pin, followed by the number of bytes/words to trancieve.
 *          They also do some checks to make sure the bus is configured
 *          appropriately. Since these tasks are the same, they are handled in
 *          this function. It returns a Status which the calling function
 *          can return in turn.
 * @return FAILED if MODE16 is not the same as width.
 *         FAILED if the bus is already open.
 *         SUCCESS otherwise.
*/
static enum Status command(
    uint8_t const *const args,
    uint16_t const args_size,
    enum Direction const dir,
    enum Width const width
) {
    if (args_size != 3) {
        return E_BAD_ARGSIZE;
    }

    tSPI_CS cs = args[0];
    uint16_t count = *(uint16_t *)(args + 1);

    if (SPI1CON1bits.MODE16 != width)
    {
        return E_BAD_ARGUMENT;
    }

    if (!open(cs))
    {
        return E_FAILED;
    }

    enum Status const status = host_exchange(dir, count);
    close();
    return status;
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

enum Status SPI_conf(
    uint8_t const *args,
    uint16_t args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    union {
        SPI1CON1BITS con1bits;
        uint16_t con1;
    } conf;

    if (args_size != sizeof(conf)) {
        return E_BAD_ARGSIZE;
    }

    conf.con1 = *(uint16_t *)args;
    bool conf_ok = SPI_configure(conf.con1bits);

    if (conf_ok)
    {
        return E_OK;
    }

    return E_FAILED;
}

enum Status SPI_read_bytes(
    uint8_t const *const args,
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    return command(args, args_size, READ, BYTE);
}

enum Status SPI_write_bytes(
    uint8_t const *const args,
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    return command(args, args_size, WRITE, BYTE);
}

enum Status SPI_exchange_bytes(
    uint8_t const *const args,
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    return command(args, args_size, EXCHANGE, BYTE);
}

enum Status SPI_read_ints(
    uint8_t const *const args,
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    return command(args, args_size, READ, WORD);
}

enum Status SPI_write_ints(
    uint8_t const *const args,
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    return command(args, args_size, WRITE, WORD);
}

enum Status SPI_exchange_ints(
    uint8_t const *const args,
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    return command(args, args_size, EXCHANGE, WORD);
}
