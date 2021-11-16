#ifndef LIGHT_H
#define	LIGHT_H

#ifdef	__cplusplus
extern "C" {
#endif
    
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
     * This routine takes two types of arguments over serial.
     * 1. (uint8) count
     *    Number of color levels. This always has to be 3. It is implemented this
     * way to have backward compatibility with old firmware and python backend.
     * 2. (uint8 [count]) color levels
     *    This a sequence of bytes with the amount determined by count variable.
     * In this case it will be three bytes in series. These will define red, green
     * and blue colors of the LED.
     * 
     * It will not return anything over serial. An acknowledgment will be passed.
     * 
     * @return none
     */
    response_t LIGHT_Onboard(void);
    
    /**
     * @brief Controls an RGB LED stripe from SQR1 pin
     * 
     * @description
     * This routine takes two types of arguments over serial.
     * 1. (uint8) count
     *    Number of color levels. This has to be a multiple of 3 since each LED 
     * has three colors to control.
     * 2. (uint8 [count]) color levels
     *    This a sequence of bytes with the amount determined by count variable.
     * These will define red, green and blue colors of each of the LEDs.
     * 
     * It will not return anything over serial. An acknowledgment will be passed.
     * 
     * @return none
     */
    response_t LIGHT_One(void);
    
    /**
     * @brief Controls an RGB LED stripe from SQR2 pin
     * 
     * @description
     * This routine takes two types of arguments over serial.
     * 1. (uint8) count
     *    Number of color levels. This has to be a multiple of 3 since each LED 
     * has three colors to control.
     * 2. (uint8 [count]) color levels
     *    This a sequence of bytes with the amount determined by count variable.
     * These will define red, green and blue colors of each of the LEDs.
     * 
     * It will not return anything over serial. An acknowledgment will be passed.
     * 
     * @return none
     */
    response_t LIGHT_Two(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* LIGHT_H */
