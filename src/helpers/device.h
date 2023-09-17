#ifndef VERSION_H
#define	VERSION_H

#ifdef	__cplusplus
extern "C" {
#endif

    /**
     * @brief Get hardware version.
     * @return DO_NOT_BOTHER
     */
    response_t DEVICE_GetVersion(void);
    
    /**
     * @brief Get firmware version.
     * @return MAJOR (uint8_t), MINOR (uint8_t), PATCH (uint8_t)
     */
    response_t DEVICE_get_fw_version(void);

    /**
     * @brief Reset the device by moving program counter to 0x0000
     * @return DO_NOT_BOTHER
     */
    response_t DEVICE_Reset(void);
    
    /**
     * @brief Read content from Special Function Registers
     * @return SUCCESS
     */
    response_t DEVICE_ReadRegisterData(void);
    
    /**
     * @brief Write content to Special Function Registers given the register 
     * address and data as two integers
     * @return SUCCESS
     */
    response_t DEVICE_WriteRegisterData(void);

#ifdef	__cplusplus
}
#endif

#endif	/* VERSION_H */
