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

    /**
     * @brief Reads DMA status registry data
     *
     * @description
     * This method will sequentially read register addresses at BUFFER pointer
     * and all four DMA channel pointers. It will also read digital state parameters.
     *
     * There are no input parameters to this method. The output of this method should
     * be read over serial in the following order.
     * 1. (int) BUFFER pointer
     * 2. (int) DMA Channel 0 pointer
     * 3. (int) DMA Channel 1 pointer
     * 4. (int) DMA Channel 2 pointer
     * 5. (int) DMA Channel 3 pointer
     * 6. (char) Digital states
     * 7. (char) Digital states error
     *
     * @return SUCCESS
     */
    response_t INTERVAL_GetState(void);

    /**
     * @brief Measures the time interval between two pin state change events
     *
     * @description
     * This method will count the time difference between two pin change events
     * attached to two pins.
     * The events can be any event defined at the list of events in `IC_PARAMS_CAPTURE_MODE`.
     * The pins should be any pin in the list of `PIN_MANAGER_DIGITAL_PINS`.
     *
     * @param timeout : period of wait until the operation is aborted
     * @param pins  : input pins defined at `PIN_MANAGER_DIGITAL_PINS`
     *              | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
     *              |  PIN EVENT 2  |  PIN EVENT 1  |
     * @param modes : pin change event defined at `IC_PARAMS_CAPTURE_MODE`
     *              | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
     *              | X | X |  EVENT 2  |  EVENT 1  |
     *
     * The output of this method should be read over serial in the following order.
     * 1. (int) IC1BUF - LSW
     * 2. (int) IC2BUF - MSW
     * Combine 1. and 2. to get the trigger time of the event 1
     * 3. (int) IC3BUF - LSW
     * 4. (int) IC4BUF - MSW
     * Combine 3. and 4. to get the trigger time of the event 2
     * 5. (int) IC2TMR
     *
     * @return SUCCESS
     */
    response_t INTERVAL_IntervalMeasure(void);

    /**
     * @brief Measures the time between multiple pin state change events
     *
     * @description
     * This method will log time units for multiple changes occurred on
     * defined digital pins. Unlike `INTERVAL_IntervalMeasure` where it
     * measure only a single change of states, this method will measure
     * upto 4 change of pin states.
     * The events can be any event defined at the list of events in `IC_PARAMS_CAPTURE_MODE`.
     * The pins should be any pin in the list of `PIN_MANAGER_DIGITAL_PINS`.
     *
     * @param timeout : period of wait until the operation is aborted
     * @param pins : input pins defined at `PIN_MANAGER_DIGITAL_PINS`
     *              | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
     *              |  PIN EVENT 2  |  PIN EVENT 1  |
     * @param modes : pin change event defined at `IC_PARAMS_CAPTURE_MODE`
     *              | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
     *              | X | X |  EVENT 2  |  EVENT 1  |
     * @param intrpts  : input pins defined at `PIN_MANAGER_DIGITAL_PINS`
     *              | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
     *              |  INTERRUPT 2  |  INTERRUPT 1  |
     *
     * The output of this method should be read over serial in the following order.
     * 1. (int) IC1BUF - LSW
     * 2. (int) IC2BUF - MSW
     * Combine 1. and 2. to get the trigger time of the change event. Depending on the
     * intrpts, the two registers (1. and 2.) may need to be read repeatedly to capture
     * timing data for each event occurrence.
     * 3. (int) IC3BUF - LSW
     * 4. (int) IC4BUF - MSW
     * Combine 3. and 4. to get the trigger time of the change event. Depending on the
     * intrpts, the two registers (3. and 4.) may need to be read repeatedly to capture
     * timing data for each event occurrence.
     * Note: ICxBUF is a 4-level buffer that can store time log for four change events.
     * 5. (int) IC2TMR
     *
     * @return SUCCESS
     */
    response_t INTERVAL_TimeMeasure(void);

    /**
     * @brief Measures the time until a pin state change event occurs
     *
     * @description
     * This method will stop counting time when the defined pin change event occurred.
     * The event can be any event defined at the list of events in `IC_PARAMS_CAPTURE_MODE`.
     * The pin should be any pin in the list of `PIN_MANAGER_DIGITAL_PINS`.
     *
     * @param timeout : period of wait until the operation is aborted
     * @param mode : pin change event defined at `IC_PARAMS_CAPTURE_MODE`
     *              | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
     *              | X | X | X | X | X |   EVENT   |
     * @param pin  : input pin defined at `PIN_MANAGER_DIGITAL_PINS`
     *              | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
     *              | X | X | X | X |   PIN EVENT   |
     *
     * The output of this method should be read over serial in the following order.
     * 1. (int) IC2TMR
     * 2. (int) IC1BUF - LSW
     * 3. (int) IC2BUF - MSW
     * Combine 2. and 3. to get the trigger time of the change event. Depending on the mode,
     * the last two registers (2. and 3.) may need to be read repeatedly to capture timing
     * data for each event occurrence.
     * Note: ICxBUF is a 4-level buffer that can store time log for four change events.
     *
     * @return SUCCESS
     */
    response_t INTERVAL_UntilEvent(void);
    
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

