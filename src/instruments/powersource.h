#ifndef POWER_SOURCE_H
#define	POWER_SOURCE_H

#include <xc.h>

#define MCP4728_I2C_DEVICE_ADDRESS        0x60

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    /**
     * @brief Set the voltage output of the power source's individual channels.
     *
     * @param channel uint8_t
     *        One of the DAC channels:
     *        PSLab V6:
     *        0 (0b00) - PCS & PVS2
     *        1 (0b01) - PVS1 & PVS3
     *        PSLab V5:
     *        0 (0b00) - PCS
     *        1 (0b01) - PV3
     *        2 (0b10) - PV2
     *        3 (0b11) - PV1
     * @param voltage uint16
     *        Integer value between 0 and 3300, corresponding to the channel's
     *        output voltage with 0 corresponding to the lowest voltage and
     *        3300 to the highest.
     *
     * @return SUCCESS, FAILED
     */
    response_t POWER_SOURCE_SetPower(void);

#ifdef	__cplusplus
}
#endif

#endif	/* POWER_SOURCE_H */

