#ifndef _MUX_H
#define _MUX_H

/// @brief SPI chip select multiplexer.
typedef enum
{
    MUX_CH1      = 0b0000, ///< CH1 PGA
    MUX_CH2      = 0b0001, ///< CH2 PGA
    MUX_PS       = 0b0010, ///< Power source
    MUX_HD       = 0b0100, ///< Header pin
    MUX_SD       = 0b0101, ///< SD-card
    MUX_DEASSERT = 0b1111, ///< Deassert all
    #ifdef V5_HW
    // Backwards compatibility
    _V5_CH1      = 0b1110,
    _V5_CH2      = 0b1101,
    _V5_HD       = 0b0111,
    _V5_SD       = 0b1011,
    #endif // V5_HW
} tMUX_CS;

/**
 * @brief Assert a chip select pin.
 * @param cs A tMUX_CS specifying the CS to assert.
 */
void MUX_ChipSelect(tMUX_CS cs);

#endif // _MUX_H
