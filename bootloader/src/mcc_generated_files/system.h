/**
  @Generated PIC24 / dsPIC33 / PIC32MM MCUs Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    system.h

  @Summary:
    This is the system.h file generated using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC24EP256GP204
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB             :  MPLAB X v5.45
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#ifndef _XTAL_FREQ
#define _XTAL_FREQ  64000000UL
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
inline static void SYSTEM_CORCONInitialize()
{
    CORCON = (CORCON & 0x00F2) | CORCON_MODE_PORVALUES;    // POR value
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
inline static void SYSTEM_CORCONModeOperatingSet(SYSTEM_CORCON_MODES modeValue)
{
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
inline static void SYSTEM_CORCONRegisterValueSet(uint16_t value)
{
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
inline static uint16_t SYSTEM_CORCONRegisterValueGet(void)
{    
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
inline static uint32_t SYSTEM_DeviceIdRegisterAddressGet(void)
{
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
/**
 End of File
*/