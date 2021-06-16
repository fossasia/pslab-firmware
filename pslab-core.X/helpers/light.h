#ifndef LIGHT_H
#define	LIGHT_H

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * @Param
     * red, green, blue in that order
     *
     * @Returns
     * none
     * 
     * @Description
     * Lights up the RGB LED. This method takes around 61.5 us to finish.
     * 
     * @Example
     * Light_RGB(255, 0, 0); will light up red color
     */
    void LIGHT_RGB(uint8_t red, uint8_t green, uint8_t blue);


#ifdef	__cplusplus
}
#endif

#endif	/* LIGHT_H */
