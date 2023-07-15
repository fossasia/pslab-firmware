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
     *        3 (0b11) - PV3
     * @param power uint16
     *        Integer value between 0 and 4095, corresponding to the voltage
     *        ratio.
     *        In the PSLab V5, the MSb determines if the voltage reference is
     *        internal (1) or external (0). With internal reference, the
     *        output range is 0 - 4.096 V with a resolution of 1 mV / bit;
     *        with external reference, the range is 0 - 3.3 V with a resolution
     *        of 806 µV / bit.
     *        In the PSLab V6, only internal reference is available.
     *
     * @return SUCCESS, FAILED
     */
    response_t POWER_SOURCE_SetPower(void);

#ifdef	__cplusplus
}
#endif

#endif	/* POWER_SOURCE_H */

