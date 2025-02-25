#ifndef _SPI_H
#define _SPI_H

#include <stdbool.h>
#include <stdint.h>
#include <xc.h>

#include "../../commands.h"

/// @brief SPI chip select multiplexer.
typedef enum
{
    SPI_CH1      = 0b0000, ///< CH1 PGA
    SPI_CH2      = 0b0001, ///< CH2 PGA
    SPI_PS       = 0b0010, ///< Power source
    SPI_HD       = 0b0100, ///< Header pin
    SPI_SD       = 0b0101, ///< SD-card
    SPI_DESELECT = 0b1111, ///< Deassert all
    #ifdef V5_HW
    // Backwards compatibility
    _V5_CH1      = 0b1110,
    _V5_CH2      = 0b1101,
    _V5_HD       = 0b0111,
    _V5_SD       = 0b1011,
    #endif // V5_HW
} tSPI_CS;

/**
 * @brief Clock polarity.
 * @details Clock polarity determines whether the SCLK line is high
 *          (SPI_IDLE_HIGH) or low (SPI_IDLE_LOW) at the start and end of data
 *          transmission.
*/
typedef enum
{
    SPI_IDLE_LOW,
    SPI_IDLE_HIGH,
} tSPIClockPolarity;


/**
 * @brief Clock edge.
 * @details Clock edge determines whether bits are shifted out of SPI1BUF to
 *          the SDO line when SCLK transissions from idle to active
 *          (SPI_SHIFT_LEADING) or active to idle (SPI_SHIFT_TRAILING). Note
 *          that "leading" and "trailing" in this context is not the same as
 *          "rising" and "falling"; depending on clock polarity, a leading
 *          edge can be either rising (SPI_IDLE_LOW) or falling
 *          (SPI_IDLE_HIGH), and vice versa for the trailing edge.
 *
 *          The combination of clock polarity and clock edge is commonly
 *          referred to as "SPI mode". There are four possible combinations,
 *          numbered 0-3. These modes are as follows:
 *
 *          SPI mode | Clock polarity | Clock edge
 *          ----------------------------------------------
 *                 0 | SPI_IDLE_LOW   | SPI_SHIFT_TRAILING
 *                 1 | SPI_IDLE_LOW   | SPI_SHIFT_LEADING
 *                 2 | SPI_IDLE_HIGH  | SPI_SHIFT_TRAILING
 *                 3 | SPI_IDLE_HIGH  | SPI_SHIFT_LEADING
*/
typedef enum
{
    SPI_SHIFT_LEADING,
    SPI_SHIFT_TRAILING,
} tSPIClockEdge;

/// @brief Prescalers for valid SPI clockrates.
typedef enum
{
    // Prescalers:    PPRE<<3 + SPRE
    // SPI_SCLK64000000 = 0b11000 + 0b111, // 1 * 1 = 1
    // SPI_SCLK32000000 = 0b11000 + 0b110, // 1 * 2 = 2
    // SPI_SCLK21333333 = 0b11000 + 0b101, // 1 * 3 = 3
    // SPI_SCLK16000000 = 0b11000 + 0b100, // 1 * 4 = 4
    // Maximum clock rate is 15 MHz.
    SPI_SCLK12800000 = 0b11000 + 0b011, // 1 * 5 = 5
    SPI_SCLK10666666 = 0b11000 + 0b010, // 1 * 6 = 6
    SPI_SCLK9142857 = 0b11000 + 0b001, // 1 * 7 = 7
    SPI_SCLK8000000 = 0b11000 + 0b000, // 1 * 8 = 8
    // SPI_SCLK1600000 = 0b10000 + 0b111, // 4 * 1 = 4
    // SPI_SCLK8000000 = 0b10000 + 0b110, // 4 * 2 = 8
    SPI_SCLK5333333 = 0b10000 + 0b101, // 4 * 3 = 12
    SPI_SCLK4000000 = 0b10000 + 0b100, // 4 * 4 = 16
    SPI_SCLK3200000 = 0b10000 + 0b011, // 4 * 5 = 20
    SPI_SCLK2666667 = 0b10000 + 0b010, // 4 * 6 = 24
    SPI_SCLK2285714 = 0b10000 + 0b001, // 4 * 7 = 28
    SPI_SCLK2000000 = 0b10000 + 0b000, // 4 * 8 = 32
    // SPI_SCLK4000000 = 0b01000 + 0b111, // 16 * 1 = 16
    // SPI_SCLK2000000 = 0b01000 + 0b110, // 16 * 2 = 32
    SPI_SCLK1333333 = 0b01000 + 0b101, // 16 * 3 = 48
    SPI_SCLK1000000 = 0b01000 + 0b100, // 16 * 4 = 64
    SPI_SCLK800000 = 0b01000 + 0b011, // 16 * 5 = 80
    SPI_SCLK666667 = 0b01000 + 0b010, // 16 * 6 = 96
    SPI_SCLK571429 = 0b01000 + 0b001, // 16 * 7 = 112
    SPI_SCLK500000 = 0b01000 + 0b000, // 16 * 8 = 128
    // SPI_SCLK1000000 = 0b00000 + 0b111, // 64 * 1 = 64
    // SPI_SCLK500000 = 0b00000 + 0b110, // 64 * 2 = 128
    SPI_SCLK333333 = 0b00000 + 0b101, // 64 * 3 = 192
    SPI_SCLK250000 = 0b00000 + 0b100, // 64 * 4 = 256
    SPI_SCLK200000 = 0b00000 + 0b011, // 64 * 5 = 320
    SPI_SCLK166667 = 0b00000 + 0b010, // 64 * 6 = 384
    SPI_SCLK142857 = 0b00000 + 0b001, // 64 * 7 = 448
    SPI_SCLK125000 = 0b00000 + 0b000, // 64 * 8 = 512
} tSPIClockrate;

typedef SPI1CON1BITS SPI_Config;

/**
 * @brief Select or deselect SPI chip select pins.
 * @note This function should not be called in most cases. All
 *       read/write/exchange functions which are part of this API handle chip
 *       selection internally. This function is made available to enable
 *       alternative SPI implementations to use the chip select multiplexer.
 * @param cs One of tSPI_CS.
*/
#ifdef V5_HW
void SPI_chip_select(tSPI_CS cs);
#else
void SPI_chip_select(const tSPI_CS cs);
#endif // V5_HW

/**
 * @brief Set SPI1CON1 register.
 * @param conf An SPI1CON1BITS instance, see datasheet for member descriptions.
 * @return E_OK if bus is available, else E_RESOURCE_BUSY.
 */
enum Status SPI_configure(const SPI_Config conf);

/**
 * @brief Exchange a single byte.
 * @pre An appropriate configuration must be set with SPI_configure before
 *      using this function.
 * @param cs One of tSPI_CS.
 * @param data Pointer to a uint8_t containing data to be written to the bus.
 *             Data read from the bus will be written through the same pointer.
 * @return E_BAD_ARGUMENT if SPI1CON1bits.MODE16 == 1.
 *         E_RESOURCE_BUSY if bus is already open.
 *         E_OK otherwise.
 */
enum Status SPI_exchange_byte(const tSPI_CS cs, uint8_t* const data);

/**
 * @brief Exchange a single word.
 * @pre An appropriate configuration must be set with SPI_configure before
 *      using this function.
 * @param cs One of tSPI_CS.
 * @param data Pointer to a uint16_t containing data to be written to the bus.
 *             Data read from the bus will be written through the same pointer.
 * @return E_BAD_ARGUMENT if SPI1CON1bits.MODE16 == 0.
 *         E_RESOURCE_BUSY if bus is already open.
 *         E_OK otherwise.
 */
enum Status SPI_exchange_int(const tSPI_CS cs, uint16_t* const data);

/**
 * @brief Set SPI1CON1 register.
 * @param conf uint16_t corresponding to SPI1CON1, see datasheet.
 * @return E_OK if bus is available, else E_RESOURCE_BUSY.
 */
enum Status SPI_conf(
    uint8_t const *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Read bytes from the SPI bus.
 * @pre An appropriate configuration must be set with SPI_conf before using
 *      this function.
 * @param cs uint8_t corresponding to one of tSPI_CS.
 * @param count uint16_t, number of bytes to read.
 * @return E_BAD_ARGUMENT if SPI1CON1bits.MODE16 == 1.
 *         E_RESOURCE_BUSY if bus is already open.
 *         E_SUCCESS otherwise.
 */
enum Status SPI_read_bytes(
    uint8_t const *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Write bytes to the SPI bus.
 * @pre An appropriate configuration must be set with SPI_conf before using
 *      this function.
 * @param cs uint8_t corresponding to one of tSPI_CS.
 * @param count uint16_t, number of bytes to write.
 * @param data <count> bytes of data.
 * @return E_BAD_ARGUMENT if SPI1CON1bits.MODE16 == 1.
 *         E_RESOURCE_BUSY if bus is already open.
 *         E_OK otherwise.
 */
enum Status SPI_write_bytes(
    uint8_t const *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Read and write bytes from the SPI bus.
 * @pre An appropriate configuration must be set with SPI_conf before using
 *      this function.
 * @param cs uint8_t corresponding to one of tSPI_CS.
 * @param count uint16_t, number of bytes to exchange.
 * @param data <count> bytes of data.
 * @return E_BAD_ARGUMENT if SPI1CON1bits.MODE16 == 1.
 *         E_RESOURCE_BUSY if bus is already open.
 *         E_OK bytes followed by SUCCESS otherwise.
 */
enum Status SPI_exchange_bytes(
    uint8_t const *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Read ints from the SPI bus.
 * @pre An appropriate configuration must be set with SPI_conf before using
 *      this function.
 * @param cs uint8_t corresponding to one of tSPI_CS.
 * @param count uint16_t, number of ints to read.
 * @return E_BAD_ARGUMENT if SPI1CON1bits.MODE16 == 0.
 *         E_RESOURCE_BUSY if bus is already open.
 *         E_OK otherwise.
 */
enum Status SPI_read_ints(
    uint8_t const *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Write ints to the SPI bus.
 * @pre An appropriate configuration must be set with SPI_conf before using
 *      this function.
 * @param cs uint8_t corresponding to one of tSPI_CS.
 * @param count uint16_t, number of ints to write.
 * @param data <2*count> bytes of data.
 * @return E_BAD_ARGUMENT if SPI1CON1bits.MODE16 == 0.
 *         E_RESOURCE_BUSY if bus is already open.
 *         E_OK otherwise.
 */
enum Status SPI_write_ints(
    uint8_t const *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Read and write ints from the SPI bus.
 * @pre An appropriate configuration must be set with SPI_conf before using
 *      this function.
 * @param cs uint8_t corresponding to one of tSPI_CS.
 * @param count uint16_t, number of ints to exchange.
 * @param data <2*count> bytes of data.
 * @return E_BAD_ARGUMENT if SPI1CON1bits.MODE16 == 0.
 *         E_RESOURCE_BUSY if bus is already open.
 *         E_OK otherwise.
 */
enum Status SPI_exchange_ints(
    uint8_t const *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

#endif // _SPI_H
