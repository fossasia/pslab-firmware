#ifndef LIGHT_H
#define	LIGHT_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include "types.h"

    /**
     * @brief Control the color of RGB LED
     *
     * @description
     * Lights up the RGB LED. This method takes around 61.5 us to finish.
     *
     * @param
     * red: red color level (0-255)
     * green: green color level (0-255)
     * blue: blue color level (0-255)
     *
     * @example
     * Light_RGB(255, 0, 0); will light up red color
     */
    void LIGHT_RGB(uint8_t red, uint8_t green, uint8_t blue);

    /**
     * @brief Controls the on-board RGB LED
     *
     * @description
     * This routine takes three types of arguments over serial.
     * 1. (uint8) count
     *    Number of color levels. This has to be a multiple of 3 since each LED
     *    has three colors to control.
     * 2. (uint8 [count]) color levels
     *    This a sequence of bytes with the amount determined by count variable.
     *    These will define red, green and blue colors of each of the LEDs.
     * 3. (uint8) pin
     *    The pin to which the RGB LED's DIN is connected.
     *    0: Onboard RGB,
     *    1: SQ1,
     *    2: SQ2,
     *    3: SQ3,
     *    4: SQ4
     *
     * It will not return anything over serial. An acknowledgment will be passed.
     *
     * @return none
     */
    enum Status LIGHT_rgb_pin(
        uint8_t **args,
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

#ifdef	__cplusplus
}
#endif

#endif	/* LIGHT_H */
