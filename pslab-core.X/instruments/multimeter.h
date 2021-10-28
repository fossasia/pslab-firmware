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

#ifdef	__cplusplus
}
#endif

#endif	/* MULTIMETER_H */
