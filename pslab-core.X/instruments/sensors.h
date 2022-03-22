#ifndef SENSORS_H
#define	SENSORS_H

#include <xc.h>
#include "../registers/comparators/ic_params.h"

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * @brief This method will configure all 4 Input Capture modules to listen
     * to state changes in pin1 and pin2. The state changes are defined in 
     * IC_PARAMS_CAPTURE_MODE and each of the pin is fetched from the array 
     * PIN_MANAGER_DIGITAL_PINS. Once this function is called, the timer values
     * correspond to each state change on each pin will be stored in the Input
     * Capture timer registers. These timer values need to be fetched by the 
     * user using a different routine.
     * 
     * @param pin1 First pin connected to a Digital pin
     * @param pin1_edge Triggering transition on pin 1
     * @param pin2 Second pin connected to a Digital pin
     * @param pin2_edge Triggering transition on pin 2
     * 
     * @return None
     */
    void SENSORS_ConfigureInterval(uint8_t pin1, IC_PARAMS_CAPTURE_MODE pin1_edge,
        uint8_t pin2, IC_PARAMS_CAPTURE_MODE pin2_edge);
    
    /**
     * @brief This method will implement a counter using logic level variations
     * of a digital pin.
     * 
     * @description
     * This command takes one argument over serial
     * 1. (uint8)  digital pin
     *    This pin has to be one of the pins from PIN_MANAGER_DIGITAL_PINS array
     * 
     * It returns nothing over serial.
     * 
     * One of the use cases of this method could be when counting a set of 
     * pebbles. One can connect a light encoder output pin to the pin specified
     * as the `digital pin` and by passing the pebbles through the encoder will
     * keep on incrementing a counter. This value can be fetched from routine 
     * SENSORS_GetCounter();
     * 
     * @return SUCCESS
     */
    response_t SENSORS_StartCounter(void);
    
    /**
     * @brief Fetch the counter value in TMR2 register
     * 
     * @description
     * This method does not take any arguments over serial. It will return the
     * value stored in timer 2 counting register. This value is most likely 
     * captured from SENSORS_StartCounter method
     * 
     * @return SUCCESS
     */
    response_t SENSORS_GetCounter(void);

    /**
     * @brief Setup and measure timing from an HCSR04 ultrasonic sensor. This
     * sensor would usually support measurement distances from 2-400 cm. Working
     * principle of this sensor is as follows. When we keep the TRIG pin high
     * for a duration of 10 us, transmitter will send out a controlled sound
     * burst. As soon as this burst is over, ECHO pin will be pulled high. When
     * a reflection from the sound burst is received at the receiver, ECHO pin
     * will be pulled low. We measure the time interval between these two events
     * and count it as the time-of-flight (ToF) for the sound wave between the
     * sensor and an obstacle in front of it.
     *
     * @description
     * This command function takes one argument over serial:
     * 1. (uint16) timeout
     *    This value is used to limit the range of HCSR04. The minimum timeout
     *    this can support is 1.024 milliseconds correspond to ~17 cm (330 m/s)
     *    and the rest will be multiples of 1.024 milliseconds. Calculate the
     *    timeout for a required distance limit using the following equation.
     *    Note that the range limit will also be multiples of ~17 cm (330 m/s).
     *
     *    timeout = 2 * distance (in meters) / (0.001024 * 330)
     *
     * User needs to read five consecutive 16-bit integers over serial.
     * 1. reading_1
     * 2. reading_2
     *    Combine these two integers to form the start time of ECHO signal
     * 3. reading_3
     * 4. reading_4
     *    Combine these two integers to form the end time of ECHO signal
     * 5. IC2TMR
     *    For sanity checking
     * 
     * Pin Connection
     * ================================================
     > VCC  : +5V
     > TRIG : SQ1
     > ECHO : LA1
     > GND  : GND
     * 
     * Timing measurement diagram
     * ================================================
     > Trigger Pin (SQR1)
        _|------|______________________________________
          <10us>
        ________|-|_|-|_|-|_|-|_|-|____________________
                 <----- Burst ---->
     >  Echo Pin (LA1)
        __________________________|-------- ... ----|__
                                   <- Reflections ->
     * 
     * @return SUCCESS
     */
    response_t SENSORS_HCSR04(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SENSORS_H */
