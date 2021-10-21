#ifndef POWER_SOURCE_H
#define	POWER_SOURCE_H

#include <xc.h> 

#define MCP4728_I2C_DEVICE_ADDRESS        0x60

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    /**
     * @brief This method setup power source individual channels with user set
     * values
     * 
     * @description
     * This command takes two argument over serial
     * 1. (uint8)  power channel
     *    This could be one of the four DAC channels 
     *      - PCS - 0 (0b00)
     *      - PV3 - 1 (0b01)
     *      - PV2 - 2 (0b10)
     *      - PV1 - 3 (0b11)
     * 2. (uint16) power level
     *    This is an integer value between 0 and 4095 correspond to the voltage 
     * ratio. The first bit determines if the source is internal (1) or external
     * (0).
     * 
     * It returns nothing over serial.
     * 
     * @return SUCCESS, FAILED
     */
    response_t POWER_SOURCE_SetPower(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* POWER_SOURCE_H */

