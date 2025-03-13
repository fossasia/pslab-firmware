#ifndef _SPI_H
#define _SPI_H

#include <stdbool.h>
#include <stdint.h>
#include <xc.h>

#include "../../commands.h"

/// @brief SPI chip select multiplexer.
enum SPI_CS
{
    SPI_CS_CH1      = 0b0000, ///< CH1 PGA
    SPI_CS_CH2      = 0b0001, ///< CH2 PGA
    SPI_CS_PS       = 0b0010, ///< Power source
    SPI_CS_HD       = 0b0100, ///< Header pin
    SPI_CS_SD       = 0b0101, ///< SD-card
    SPI_CS_NONE     = 0b1111, ///< Deassert all
    #ifdef V5_HW
    // Backwards compatibility
    _SPI_CS_V5_CH1  = 0b1110,
    _SPI_CS_V5_CH2  = 0b1101,
    _SPI_CS_V5_HD   = 0b0111,
    _SPI_CS_V5_SD   = 0b1011,
    #endif // V5_HW
};

/**
 * @brief Clock polarity.
 * @details Clock polarity determines whether the SCLK line is high
 *          (SPI_CPOL_HIGH) or low (SPI_CPOL_LOW) at the start and end
 *          of data transmission.
*/
enum SPI_CPOL
{
    SPI_CPOL_LOW,
    SPI_CPOL_HIGH,
};


/**
 * @brief Clock edge.
 * @details Clock edge determines whether bits are shifted out of SPI1BUF to
 *          the SDO line when SCLK transissions from idle to active
 *          (SPI_CKE_LEADING) or active to idle (SPI_CKE_TRAILING). Note
 *          that "leading" and "trailing" in this context is not the same as
 *          "rising" and "falling"; depending on clock polarity, a leading
 *          edge can be either rising (SPI_CPOL_LOW) or falling
 *          (SPI_CPOL_HIGH), and vice versa for the trailing edge.
 *
 *          Another common name for this feature is clock phase, CPHA. CKE is
 *          the inversion of CPHA, i.e. CKE=0 == CPHA=1, CKE=1 == CPHA=0.
 *
 *          The combination of clock polarity and clock edge is commonly
 *          referred to as "SPI mode". There are four possible combinations,
 *          numbered 0-3. These modes are as follows:
 *
 *          SPI mode | Clock polarity | Clock edge
 *          ----------------------------------------------
 *                 0 | SPI_CPOL_LOW   | SPI_CKE_TRAILING
 *                 1 | SPI_CPOL_LOW   | SPI_CKE_LEADING
 *                 2 | SPI_CPOL_HIGH  | SPI_CKE_TRAILING
 *                 3 | SPI_CPOL_HIGH  | SPI_CKE_LEADING
*/
enum SPI_CKE
{
    SPI_CKE_LEADING,
    SPI_CKE_TRAILING,
};

/// @brief Prescalers for valid SPI clockrates.
enum SPI_SCLK
{
    // Prescalers:    PPRE<<3 + SPRE
    // SPI_SCLK_64000000 = 0b11000 + 0b111, // 1 * 1 = 1
    // SPI_SCLK_32000000 = 0b11000 + 0b110, // 1 * 2 = 2
    // SPI_SCLK_21333333 = 0b11000 + 0b101, // 1 * 3 = 3
    // SPI_SCLK_16000000 = 0b11000 + 0b100, // 1 * 4 = 4
    // Maximum clock rate is 15 MHz.
    SPI_SCLK_12800000 = 0b11000 + 0b011, // 1 * 5 = 5
    SPI_SCLK_10666666 = 0b11000 + 0b010, // 1 * 6 = 6
    SPI_SCLK_9142857 = 0b11000 + 0b001, // 1 * 7 = 7
    SPI_SCLK_8000000 = 0b11000 + 0b000, // 1 * 8 = 8
    // SPI_SCLK_1600000 = 0b10000 + 0b111, // 4 * 1 = 4
    // SPI_SCLK_8000000 = 0b10000 + 0b110, // 4 * 2 = 8
    SPI_SCLK_5333333 = 0b10000 + 0b101, // 4 * 3 = 12
    SPI_SCLK_4000000 = 0b10000 + 0b100, // 4 * 4 = 16
    SPI_SCLK_3200000 = 0b10000 + 0b011, // 4 * 5 = 20
    SPI_SCLK_2666667 = 0b10000 + 0b010, // 4 * 6 = 24
    SPI_SCLK_2285714 = 0b10000 + 0b001, // 4 * 7 = 28
    SPI_SCLK_2000000 = 0b10000 + 0b000, // 4 * 8 = 32
    // SPI_SCLK_4000000 = 0b01000 + 0b111, // 16 * 1 = 16
    // SPI_SCLK_2000000 = 0b01000 + 0b110, // 16 * 2 = 32
    SPI_SCLK_1333333 = 0b01000 + 0b101, // 16 * 3 = 48
    SPI_SCLK_1000000 = 0b01000 + 0b100, // 16 * 4 = 64
    SPI_SCLK_800000 = 0b01000 + 0b011, // 16 * 5 = 80
    SPI_SCLK_666667 = 0b01000 + 0b010, // 16 * 6 = 96
    SPI_SCLK_571429 = 0b01000 + 0b001, // 16 * 7 = 112
    SPI_SCLK_500000 = 0b01000 + 0b000, // 16 * 8 = 128
    // SPI_SCLK_1000000 = 0b00000 + 0b111, // 64 * 1 = 64
    // SPI_SCLK_500000 = 0b00000 + 0b110, // 64 * 2 = 128
    SPI_SCLK_333333 = 0b00000 + 0b101, // 64 * 3 = 192
    SPI_SCLK_250000 = 0b00000 + 0b100, // 64 * 4 = 256
    SPI_SCLK_200000 = 0b00000 + 0b011, // 64 * 5 = 320
    SPI_SCLK_166667 = 0b00000 + 0b010, // 64 * 6 = 384
    SPI_SCLK_142857 = 0b00000 + 0b001, // 64 * 7 = 448
    SPI_SCLK_125000 = 0b00000 + 0b000, // 64 * 8 = 512
};

/// @brief Number of bits exchanged at a time.
enum SPI_WordSize
{
    SPI_WORD_SIZE_8,
    SPI_WORD_SIZE_16,
};

enum Status SPI_open(enum SPI_CS cs);
enum Status SPI_close(void);
enum Status SPI_set_mode(enum SPI_CPOL cpol, enum SPI_CKE cke);
enum Status SPI_set_clock(enum SPI_SCLK sclk);
enum Status SPI_set_word_size(enum SPI_WordSize wsize);
enum Status SPI_exchange(uint8_t indata[], uint8_t outdata[], uint16_t size);

enum Status SPI_cmd_open(
    uint8_t args[],
    uint16_t args_size,
    uint8_t *rets[],
    uint16_t *rets_size
);
enum Status SPI_cmd_close(
    uint8_t args[],
    uint16_t args_size,
    uint8_t *rets[],
    uint16_t *rets_size
);
enum Status SPI_cmd_set_mode(
    uint8_t args[],
    uint16_t args_size,
    uint8_t *rets[],
    uint16_t *rets_size
);
enum Status SPI_cmd_set_clock(
    uint8_t args[],
    uint16_t args_size,
    uint8_t *rets[],
    uint16_t *rets_size
);
enum Status SPI_cmd_set_word_size(
    uint8_t args[],
    uint16_t args_size,
    uint8_t *rets[],
    uint16_t *rets_size
);
enum Status SPI_cmd_read(
    uint8_t args[],
    uint16_t args_size,
    uint8_t *rets[],
    uint16_t *rets_size
);
enum Status SPI_cmd_write(
    uint8_t args[],
    uint16_t args_size,
    uint8_t *rets[],
    uint16_t *rets_size
);
enum Status SPI_cmd_exchange(
    uint8_t args[],
    uint16_t args_size,
    uint8_t *rets[],
    uint16_t *rets_size
);

#endif // _SPI_H
