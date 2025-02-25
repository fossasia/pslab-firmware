#ifndef POWER_SOURCE_H
#define	POWER_SOURCE_H

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
     * @param output uint16
     *      Channel output level in mV relative to the VDD line (3300 mV). A
     *      value of 0 produces an output equal to the low end of the channel's
     *      range; a value of 3300 produces an output equal to the high end of
     *      the channel's range:
     *          channel     output==0    output==3300
     *          PV(S)1      -5 V        5 V
     *          PV(S)2      -3.3 V      3.3 V
     *          PV(S)3      0 V         3.3 V
     *          PCS         3.3 mA      0 A
     *      Note that the direction of the output range for the PCS channel is
     *      inverted compared to the other channels.
     *
     * @return SUCCESS, FAILED
     */
    enum Status POWER_SOURCE_SetPower(
        uint8_t const *args,
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

#ifdef	__cplusplus
}
#endif

#endif	/* POWER_SOURCE_H */
