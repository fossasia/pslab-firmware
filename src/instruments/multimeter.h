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
