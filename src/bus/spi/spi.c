#include <stdint.h>
#include <stdlib.h>
#include <xc.h>

#include "../../commands.h"
#include "../../transport/packet_handler.h"

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

/********************/
/* Static functions */
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
static void chip_select(enum SPI_CS cs)
#else
static void chip_select(enum SPI_CS const cs)
#endif // V5_HW
{
    #ifdef V5_HW
    switch (cs) {
    case SPI_CS_CH1:
        cs = _SPI_CS_V5_CH1;
        break;
    case SPI_CS_CH2:
        cs = _SPI_CS_V5_CH2;
        break;
    case SPI_CS_HD:
        cs = _SPI_CS_V5_HD;
        break;
    case SPI_CS_SD:
        cs = _SPI_CS_V5_SD;
        break;
    default:
        cs = SPI_CS_NONE;
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

/********************/
/* Public functions */
/********************/

/**
 * @brief Enable SPI bus and select slave.
 * @pre An appropriate configuration must be set with SPI_configure() before
 *      using this function.
 * @post close() must be called to release the bus.
 * @param cs One of SPI_CS.
 * @return enum Status status
 */
enum Status SPI_open(enum SPI_CS const cs)
{
    if (SPI1STATbits.SPIEN) {
        return E_RESOURCE_BUSY;
    }
    SPI1STATbits.SPIEN = 1;
    chip_select(cs);
    return E_OK;
}

/// @brief Deselect slaves and disable SPI bus.
enum Status SPI_close(void)
{
    chip_select(SPI_CS_NONE);
    SPI1STATbits.SPIEN = 0;
    return E_OK;
}

enum Status SPI_set_mode(enum SPI_CPOL const cpol, enum SPI_CKE const cke)
{
    if (SPI1STATbits.SPIEN) { return E_RESOURCE_BUSY; }
    SPI1CON1bits.CKP = cpol;
    SPI1CON1bits.CKE = cke;
    return E_OK;
}

enum Status SPI_set_clock(enum SPI_SCLK const sclk)
{
    if (SPI1STATbits.SPIEN) { return E_RESOURCE_BUSY; }
    SPI1CON1bits.PPRE = (sclk >> 3) & 0b11;
    SPI1CON1bits.SPRE = sclk & 0b111;
    return E_OK;
}

enum Status SPI_set_word_size(enum SPI_WordSize const wsize)
{
    if (SPI1STATbits.SPIEN) { return E_RESOURCE_BUSY; }
    SPI1CON1bits.MODE16 = wsize & 0b1;
    return E_OK;
}

static enum Status _read8(uint8_t outdata[], uint16_t const size)
{
    for (uint16_t i = 0; i < size; ++i) {
        SPI1BUF = 0;
        while (!SPI1STATbits.SPIRBF) {;}
        outdata[i] = SPI1BUF;
    }

    return E_OK;
}

static enum Status _read16(uint16_t outdata[], uint16_t const size)
{
    // Size in bytes!
    for (uint16_t i = 0; i < size / 2; ++i) {
        SPI1BUF = 0;
        while (!SPI1STATbits.SPIRBF) {;}
        outdata[i] = SPI1BUF;
    }

    return E_OK;
}

static enum Status _write8(uint8_t indata[], uint16_t const size)
{
    for (uint16_t i = 0; i < size; ++i) {
        SPI1BUF = indata[i];
        while (!SPI1STATbits.SPIRBF) {;}
        SPI1BUF;
    }

    return E_OK;
}

static enum Status _write16(uint16_t indata[], uint16_t const size)
{
    // Size in bytes!
    for (uint16_t i = 0; i < size / 2; ++i) {
        SPI1BUF = indata[i];
        while (!SPI1STATbits.SPIRBF) {;}
        SPI1BUF;
    }

    return E_OK;
}

static enum Status _exchange8(
    uint8_t indata[],
    uint8_t outdata[],
    uint16_t const size
) {
    for (uint16_t i = 0; i < size; ++i) {
        SPI1BUF = indata[i];
        while (!SPI1STATbits.SPIRBF) {;}
        outdata[i] = SPI1BUF;
    }

    return E_OK;
}

static enum Status _exchange16(
    uint16_t indata[],
    uint16_t outdata[],
    uint16_t const size
) {
    // size in bytes!
    for (uint16_t i = 0; i < size / 2; ++i) {
        SPI1BUF = indata[i];
        while (!SPI1STATbits.SPIRBF) {;}
        outdata[i] = SPI1BUF;
    }

    return E_OK;
}

/**
 * @brief Exchange data over SPI.
 *
 * @pre The bus must be enabled with open() before calling this function.
 *
 * Data is exchanged in full duplex, i.e. for every byte written to bus, a
 * byte is also read back. Data is exchanged eiter 8 or 16 bits at a time,
 * depending on the value of SPI1CON1bits.MODE16.
 *
 * The parameters `indata` and `outdata` may point to the same memory area.
 *
 * Either `indata` or `outdata` may be NULL. Both may be NULL iff `size == 0`.
 *
 * @param[in]  indata  Memory area which will be written to bus.
 * @param[out] outdata Memory area into which data will be readback.
 * @param[in]  size    Number of bytes to exchange.
 *
 * @retval status      Status code.
*/
enum Status SPI_exchange(
    uint8_t indata[],
    uint8_t outdata[],
    uint16_t const size
) {
    if (!SPI1STATbits.SPIEN) { return E_RESOURCE_NOT_INITIALIZED; }
    enum Status status = E_OK;

    switch (SPI1CON1bits.MODE16) {
    case SPI_WORD_SIZE_8:
        if (!indata) {
            status = _read8(outdata, size);
        } else if (!outdata) {
            status = _write8(indata, size);
        } else {
            status = _exchange8(indata, outdata, size);
        }
    case SPI_WORD_SIZE_16:
        if (!indata) {
            status = _read16((uint16_t *)outdata, size);
        } else if (!outdata) {
            status = _write16((uint16_t *)indata, size);
        } else {
            status = _exchange16((uint16_t *)indata, (uint16_t *)outdata, size);
        }
    }

    return status;
}

/*********************/
/* Command functions */
/*********************/

enum Status SPI_cmd_open(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {
    uint8_t cs = SPI_CS_NONE;
    if (args_size != sizeof(cs)) { return E_BAD_ARGSIZE; }
    cs = *args;
    return SPI_open(cs);
}

enum Status SPI_cmd_close(
    __attribute__ ((unused)) uint8_t args[],
    __attribute__ ((unused)) uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {
    return SPI_close();
}

enum Status SPI_cmd_set_mode(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {
    union Input {
        struct {
            uint8_t cpol;
            uint8_t cke;
        };
        uint8_t *buffer;
    } input = {{0}};

    if (args_size != sizeof(input)) { return E_BAD_ARGSIZE; }
    input.buffer = args;
    return SPI_set_mode(input.cpol, input.cke);
}

enum Status SPI_cmd_set_clock(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {
    uint16_t sclk = SPI_SCLK_125000;
    if (args_size != sizeof(sclk)) { return E_BAD_ARGSIZE; }
    sclk = *args;
    return SPI_set_clock(sclk);
}

enum Status SPI_cmd_set_word_size(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {
    uint8_t wsize = SPI_WORD_SIZE_8;
    if (args_size != sizeof(wsize)) { return E_BAD_ARGSIZE; }
    wsize = *args;
    return SPI_set_word_size(wsize);
}

enum Status SPI_cmd_read(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *rets_size
) {
    uint16_t size = 0;
    if (args_size != sizeof(size)) { return E_BAD_ARGSIZE; }
    size = *(uint16_t *)args;
    *rets = args;
    *rets_size = size;
    return SPI_exchange(NULL, *rets, size);
}

enum Status SPI_cmd_write(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {
    union Input {
        struct {
            uint16_t size;
            uint8_t data[];
        };
        uint8_t *buffer;
    } input = {{0}};
    if (args_size < sizeof(input)) { return E_BAD_ARGSIZE; }
    input.buffer = args;
    return SPI_exchange(input.data, NULL, input.size);
}

enum Status SPI_cmd_exchange(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *rets_size
) {
    union Input {
        struct {
            uint16_t size;
            uint8_t data[];
        };
        uint8_t *buffer;
    } input = {{0}};
    if (args_size < sizeof(input)) { return E_BAD_ARGSIZE; }
    input.buffer = args;
    *rets = args;
    *rets_size = input.size;
    return SPI_exchange(input.data, *rets, input.size);
}
