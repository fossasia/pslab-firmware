#ifndef MULTIMETER_H
#define	MULTIMETER_H

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * @brief Read a single voltage instance using ADC
     * @return SUCCESS
     */
    response_t MULTIMETER_GetVoltage(void);
    
    /**
     * @brief Read multiple (16) voltage instances using ADC
     * @return SUCCESS
     */
    response_t MULTIMETER_GetVoltageSummed(void);
    
    /**
     * @brief Charge or discharge capacitor through a resistor
     * @return SUCCESS
     */
    response_t MULTIMETER_ChargeCapacitor(void);
    
    /**
     * @brief Use CTMU to measure capacitance value and read it
     * @return SUCCESS
     */
    response_t MULTIMETER_GetCapacitance(void);

    /**
     * @brief Count the frequency (occurrence) of a signal over a
     * 100 ms time period using only timer registers
     *
     * @description
     * This command function takes one argument over serial:
     * 1. (uint8)  Configuration byte:
     *             | 7  | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
     *             |   PRE-SCALER   |      PIN      |
     *             PIN: Pin from PIN_MANAGER_DIGITAL_PINS
     *             PRE-SCALER: Scaling factor from TIMER_PARAMS_PRESCALER
     *
     * This method will output the following data over serial
     * 1. (char) scaling factor
     * 2. (int) TIMER 2 (Least significant potion)
     * 3. (int) TIMER 3 (Most significant potion)
     * Combine 3. and 2. to structure the full timing data
     *
     * @return SUCCESS
     */
    response_t MULTIMETER_HighFrequency(void);

    /**
     * @brief Count the frequency (occurrence) of a signal over a
     * 100 ms time period using input capture registers. Unlike
     * `MULTIMETER_HighFrequency`, this method will down-sample the
     * input signal frequency by a factor of 16 supporting higher
     * frequency measurements.
     *
     * @description
     * This command function takes one argument over serial:
     * 1. (uint8)  Configuration byte:
     *             | 7  | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
     *             |   PRE-SCALER   |      PIN      |
     *             PIN: Pin from PIN_MANAGER_DIGITAL_PINS
     *             PRE-SCALER: Scaling factor from TIMER_PARAMS_PRESCALER
     *
     * This method will output the following data over serial
     * 1. (char) scaling factor
     * 2. (int) IC1TMR (Least significant potion)
     * 3. (int) IC2TMR (Most significant potion)
     * Combine 3. and 2. to structure the full timing data
     *
     * @return SUCCESS
     */
    response_t MULTIMETER_HighFrequencyAlt(void);

    /**
    * @brief Count the frequency (occurrence) of a signal. This method
    * is to be used for low frequency measurements as it only measures
    * the time interval for 32 rising edges. This will have much finer
    * granularity than high frequency measurement methods.
    *
    * @description
    * This command function takes one argument over serial:
    * 1. (uint16) Timeout: period of wait until the operation is aborted
    * 1. (uint8)  Configuration byte:
    *             | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
    *             | X | X | X | X |      PIN      |
    *             PIN: Pin from PIN_MANAGER_DIGITAL_PINS
    *
    * This method will output the following data over serial
    * 1. (char) overflow flag
    * 2. (int) IC1BUF (Least significant potion)
    * 3. (int) IC2BUF (Most significant potion)
    * Combine 3. and 2. to structure the full timing data
    *
    * @return SUCCESS
    */
    response_t MULTIMETER_LowFrequency(void);

    /**
     * @brief Get an estimate of the capacitor range
     *
     * @description
     * This function can be used to get an estimate of how large a 
     * capacitor is. One use case could be to compare two different
     * capacitors without having to evaluate the exact capacitance.
     * This command takes only one argument over serial:
     * 1. (uint16) Charge time in micro seconds
     *
     * It returns the range value as uint16.
     *
     * @return SUCCESS
     */
    response_t MULTIMETER_GetCapRange(void);

    /**
     * @brief Measurements using Charge Time Measurement Unit
     *
     * @description
     * This command function takes one argument over serial:
     * 1. (uint8)  Configuration byte:
     *             | 7  | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
     *             | DG |  CSO  |      CHANNEL      |
     *             DG: Delay generator enable/disable (1/0)
     *             CSO: Current source output:
     *                    00: Base * 1000
     *                    01: Base = 0.53 uA
     *                    10: Base * 10
     *                    11: Base * 100
     *             CHANNEL: CTMU channel
     * 
     * It returns nothing over serial.
     * It sends an acknowledge byte (SUCCESS). This will be available only after
     * no less than 2.5 milliseconds.
     * 
     * @return SUCCESS
     */
    response_t MULTIMETER_GetCTMUVolts(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MULTIMETER_H */
