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

#ifdef	__cplusplus
}
#endif

#endif	/* MULTIMETER_H */

