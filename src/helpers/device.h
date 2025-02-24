#ifndef VERSION_H
#define	VERSION_H

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * @brief Get hardware version.
     * @return Hardware version string
     */
    enum Status DEVICE_get_hw_version(
        uint8_t const *args,
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

    /**
     * @brief Get firmware version.
     * @return MAJOR (uint8_t), MINOR (uint8_t), PATCH (uint8_t)
     */
    enum Status DEVICE_get_fw_version(
        uint8_t const *args,
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

    /**
     * @brief Reset the device by moving program counter to 0x0000
     * @return No return
     */
    enum Status DEVICE_reset(
        uint8_t const *args,
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

    /**
     * @brief Read content from Special Function Registers
     * @return Register contents
     */
    enum Status DEVICE_read_register(
        uint8_t const *args,
        uint16_t args_size,
        uint8_t volatile **rets,
        uint16_t *rets_size
    );

    /**
     * @brief Write content to Special Function Registers given the register
     * address and data as two integers
     * @return
     */
    enum Status DEVICE_write_register(
        uint8_t const *args,
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

#ifdef	__cplusplus
}
#endif

#endif	/* VERSION_H */
