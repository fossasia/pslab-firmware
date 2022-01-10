#ifndef INTERVAL_H
#define	INTERVAL_H

#include <xc.h>
#include "../commands.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
    /**
     * @brief Setup input capture on one channel
     * 
     * @description
     * This routing will configure IC1,2 and DMA0,1 modules to listen to logic
     * level changes on a pin defined by the channel byte and log timer values
     * in BUFFER 1st and 2nd quarters. If trigger is set by the trig byte, this
     * will use IC4 module to listen to the trigger observation and time log will
     * then start.
     * It will combine both IC1 and IC2 modules to form a 32-bit timer which will
     * have support for more data points than `CaptureThree` and `CaptureFour`
     * routines.
     * 
     * @param count : number of data points to capture
     * @param channel : the pin that needs to be monitored
     * @param mode : logic level variation. Refer to `IC_PARAMS_CAPTURE_MODE`
     * @param trig : en/disable trigger using IC4
     *              | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
     *              |    TRIG PIN   | X | TRIG MODE |
     * 
     * @return None
     */
    void INTERVAL_CaptureOne(uint16_t count, uint8_t channel, uint8_t mode, uint8_t trig);

    /**
     * @brief Setup input capture on two channels
     * 
     * @description
     * This routing will configure IC1,2,3,4 and DMA0,1,2,3 modules to listen to 
     * logic level changes on pins defined by the channel byte and log timer 
     * values in BUFFER.
     * It will combine both IC1, IC2 and IC3, IC4 modules to form a 32-bit timer 
     * which will have support for more data points than `CaptureThree` and 
     * `CaptureFour` routines.
     * 
     * @param count : number of data points to capture
     * @param mode : logic level variation. Refer to `IC_PARAMS_CAPTURE_MODE`
     * @param channel : the pin that needs to be monitored
     * 
     * @return None
     */
    void INTERVAL_CaptureTwo(uint16_t count, uint8_t mode, uint8_t channel);
    
    /**
     * @brief Setup input capture on three channels
     * 
     * @description
     * This routing will use IC1,2,3 modules with DMA0,1,2 channels to capture
     * logic level variations in LA1, LA2 and LA3 pins. These pins are fixed.
     * If trigger is set by the trig byte, this will use IC4 module to listen to
     * the trigger observation and time log will then start.
     * 
     * @param count : number of data points to capture
     * @param mode : logic level variation. Refer to `IC_PARAMS_CAPTURE_MODE`
     * @param trig : en/disable trigger using IC4
     *              | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
     *              |    TRIG PIN   | X | TRIG MODE |
     * 
     * @return None
     */
    void INTERVAL_CaptureThree(uint16_t count, uint16_t mode, uint8_t trig);
    
    /**
     * @brief Setup input capture on four channels
     * 
     * @description
     * This routing will use IC1,2,3,4 modules with DMA0,1,2,3 channels to capture
     * logic level variations in LA1, LA2, LA3 and LA4 pins. These pins are fixed.
     * Timer 2 will be used as the timer source unlike the other three capture
     * methods that use peripheral clock as the timer source.
     * 
     * @param count : number of data points to capture
     * @param mode : logic level variation. Refer to `IC_PARAMS_CAPTURE_MODE`
     * @param prescaler : pre-scaler value for timer 2 module
     * 
     * @return None
     */
    void INTERVAL_CaptureFour(uint16_t count, uint16_t mode, uint8_t prescaler);
    
    response_t INTERVAL_GetState(void);
    
    // Getters and setters
    
    void SetDIGITAL_STATES(uint8_t);
    void SetDefaultDIGITAL_STATES(void);
    uint8_t GetDIGITAL_STATES(void);
    
    void SetDefaultDIGITAL_STATES_ERROR(void);
    uint8_t GetDIGITAL_STATES_ERROR(void);

#ifdef	__cplusplus
}
#endif

#endif	/* INTERVAL_H */

