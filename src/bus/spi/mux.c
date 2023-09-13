/* mux.c
 *
 * A multiplexer controls the chip select (CS) pins of the PSLab's built-in
 * SPI devices. The multiplexer is in turn controlled by MCU pins RA10,
 * RA7, and RB9. Settings these pins either high or low results in one of the
 * mux's seven outputs being driven low.
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
 * NC:  Not connected / All CS pins deasserted
 *
 * This file provides an abstraction layer to control these pins in a way that
 * is convenient and backwards compatible with older versions of the PSLab
 * harware.
 *
 * Note to future developers: The V5.5 hardware was produced in very limited
 * numbers and was only distributed to developers. Backwards compatibility
 * with the V5.5 can therefore be safely broken if necessary.
 */

#include <xc.h>
#include <stdint.h>

#include "../../commands.h"
#include "mux.h"

/* Private definitions */

struct MuxInput
{
    uint16_t RA10; ///< Mux pin S0.
    uint16_t RA7;  ///< Mux pin S1.
    uint16_t RB9;  ///< Mux pin S2. Not used in V5.
    uint16_t RC4;  ///< Only used in V5.5.
};

void ChipSelect(struct MuxInput pins)
{
    _LATA10 = pins.RA10;
    _LATA7  = pins.RA7;
    _LATB9  = pins.RB9;
    _LATC4  = pins.RC4;
}

/* Public definitions */

void MUX_ChipSelect(tMUX_CS cs)
{
    #ifdef V5_HW
    switch cs {
    case MUX_CH1:
        cs = _V5_CH1;
        break;
    case MUX_CH2:
        cs = _V5_CH2;
        break;
    case MUX_HD:
        cs = _V5_HD;
        break;
    case MUX_SD:
        cs = _V5_SD;
        break;
    default:
        cs = MUX_DEASSERT;
    }
    #endif // V5_HW
    struct MuxInput pins = {
        .RA10 = cs & 1,
        .RA7 = (cs >> 1) & 1,
        .RB9 = (cs >> 2) & 1,
        .RC4 = (cs >> 3) & 1,
    };
    ChipSelect(pins);
}
