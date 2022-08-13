#ifndef _XTAL_FREQ
#define _XTAL_FREQ  128000000UL
#endif

#include "xc.h"
#include "stdint.h"
#include "system_types.h"

#ifndef SYSTEM_H
#define	SYSTEM_H

/**
 * Initializes the CPU core control register.
 * @example
 * <code>
 * SYSTEM_CORCONInitialize();
 * </code>
 */
inline static void SYSTEM_CORCONInitialize() {
    CORCON = (CORCON & 0x00F2) | CORCON_MODE_PORVALUES; // POR value
}

/**
 * Sets the CPU core control register operating mode to a value that is decided by the
 * SYSTEM_CORCON_MODES argument.
 * @param modeValue SYSTEM_CORCON_MODES initialization mode specifier
 * @example
 * <code>
 * SYSTEM_CORCONModeOperatingSet(CORCON_MODE_ENABLEALLSATNORMAL_ROUNDUNBIASED);
 * </code>
 */
inline static void SYSTEM_CORCONModeOperatingSet(SYSTEM_CORCON_MODES modeValue) {
    CORCON = (CORCON & 0x00F2) | modeValue;
}

/**
 * Sets the value of CPU core control register.
 * @param value value that needs to be written to the CPU core control register
 * @example
 * <code>
 *  SYSTEM_CORCONRegisterValueSet(0x00E2);
 * </code>
 */
inline static void SYSTEM_CORCONRegisterValueSet(uint16_t value) {
    CORCON = value;
}

/**
 * Gets the value of CPU core control register.
 * @return value of the CPU core control register
 * @example
 * <code>
 * corconSave = SYSTEM_CORCONRegisterValueGet();
 * </code>
 */
inline static uint16_t SYSTEM_CORCONRegisterValueGet(void) {
    return CORCON;
}

/**
 * Gets the base address of the DEVID register for the currently selected device
 * @return base address of the DEVID register
 * @example
 * <code>
 * uint32_t devIdAddress;
 * devIdAddress = SYSTEM_DeviceIdRegisterAddressGet();
 * </code>
 */
inline static uint32_t SYSTEM_DeviceIdRegisterAddressGet(void) {
    return __DEVID_BASE;
}

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Initializes the device to the default states configured in the
 *                  MCC GUI
 * @Example
    SYSTEM_Initialize(void);
 */
void SYSTEM_Initialize(void);
#endif	/* SYSTEM_H */
