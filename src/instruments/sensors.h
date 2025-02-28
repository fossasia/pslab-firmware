#ifndef SENSORS_H
#define	SENSORS_H

#include <xc.h>
#include "../registers/comparators/ic_params.h"

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
     * @param pin2 Second pin connected to a Digital pin
     * @param pin1_edge Triggering transition on pin 1
     * @param pin2_edge Triggering transition on pin 2
     *
     * @return None
     */
    void SENSORS_ConfigureInterval(uint8_t pin1, uint8_t pin2,
            IC_PARAMS_CAPTURE_MODE pin1_edge, IC_PARAMS_CAPTURE_MODE pin2_edge);

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
    enum Status SENSORS_start_counter(
        uint8_t args[],
        uint16_t args_size,
        uint8_t *rets[],
        uint16_t *rets_size
    );

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
    enum Status SENSORS_get_counter(
        uint8_t args[],
        uint16_t args_size,
        uint8_t *rets[],
        uint16_t *rets_size
    );

#endif	/* SENSORS_H */
