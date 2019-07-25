/**
  @Generated PIC24 / dsPIC33 / PIC32MM MCUs Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    mcc.h

  @Summary:
    This is the mcc.h file generated using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.95-b-SNAPSHOT
        Device            :  PIC24EP256GP204
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36
        MPLAB             :  MPLAB X v5.10
 */

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
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

#ifndef MCC_H
#define	MCC_H
#include <xc.h>
#include "reset.h"
#include "system.h"
#include "system_types.h"
#include "clock.h"
#include "pin_manager.h"
#include <stdint.h>
#include <stdbool.h>
#include "ic4.h"
#include "uart1.h"
#include "ic2.h"
#include "tmr4.h"
#include "interrupt_manager.h"
#include "traps.h"
#include "i2c2_driver.h"
#include "tmr2.h"
#include "spi1_driver.h"
#include "cmp1.h"
#include "oc3.h"
#include "ptg.h"
#include "oc1.h"
#include "memory/flash.h"
#include "cmp3.h"
#include "adc1.h"
#include "sd_spi/sd_spi.h"
#include "ic3.h"
#include "ic1.h"
#include "uart2.h"
#include "tmr3.h"
#include "tmr5.h"
#include "fatfs/ff.h"
#include "tmr1.h"
#include "oc4.h"
#include "dma.h"
#include "oc2.h"
#include "watchdog.h"
#include "cmp4.h"
#include "cmp2.h"
#include "reset.h"

#ifndef _XTAL_FREQ
#define _XTAL_FREQ  120000000UL
#endif

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Initializes the oscillator to the default states configured in the
 *                  MCC GUI
 * @Example
    OSCILLATOR_Initialize(void);
 */
void OSCILLATOR_Initialize(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse CLOCK_Initialize (void) instead. ")));

/**
 * Checks reset cause, flashes UI with an error code as a result.
 * 
 * Note: this function should be called before any use of CLRWDT
 * since it has a side-effect of clearing the appropriate bits in the
 * register showing reset cause (see DS70602B page 8-10)
 */
uint16_t SYSTEM_GetResetCause(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse RESET_GetCause(void) (void) instead. ")));


/**
 * Enables Watch Dog Timer (WDT) using the software bit.
 * @example
 * <code>
 * WDT_WatchdogtimerSoftwareEnable();
 * </code>
 */
__attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse WATCHDOG_TimerSoftwareEnable (void) instead. ")))
inline static void WDT_WatchdogtimerSoftwareEnable(void)
{
    RCONbits.SWDTEN = 1;
}

/**
 * Disables Watch Dog Timer (WDT) using the software bit.
 * @example
 * <code>
 * WDT_WatchdogtimerSoftwareDisable();
 * </code>
 */
__attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse WATCHDOG_TimerSoftwareDisable (void) instead. ")))
inline static void WDT_WatchdogtimerSoftwareDisable(void)
{
    RCONbits.SWDTEN = 0;
}

/**
 * Clears the Watch Dog Timer (WDT).
 * @example
 * <code>
 * WDT_WatchdogTimerClear();
 * </code>
 */
__attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse WATCHDOG_TimerClear (void) instead. ")))
inline static void WDT_WatchdogTimerClear(void)
{
    ClrWdt();
}

/**
 * Gets the base address of the DEVID register for the currently selected device
 * @return base address of the DEVID register
 * @example
 * <code>
 * uint32_t devIdAddress;
 * devIdAddress = DEVICE_DeviceIdRegisterAddressGet();
 * </code>
 */
__attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse SYSTEM_DeviceIdRegisterAddressGet (void) instead. ")))
inline static uint32_t DEVICE_DeviceIdRegisterAddressGet(void)
{
    return __DEVID_BASE;
}

/**
 * Initializes the CPU core control register.
 * @example
 * <code>
 * CORCON_Initialize();
 * </code>
 */
__attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse SYSTEM_CORCONInitialize() instead. ")))
inline static void CORCON_Initialize()
{
    CORCON = (CORCON & 0x00F2) | CORCON_MODE_PORVALUES;    // POR value
}

/**
 * Sets the CPU core control register operating mode to a value that is decided by the
 * SYSTEM_CORCON_MODES argument.
 * @param modeValue SYSTEM_CORCON_MODES initialization mode specifier
 * @example
 * <code>
 * CORCON_ModeOperatingSet(CORCON_MODE_ENABLEALLSATNORMAL_ROUNDUNBIASED);
 * </code>
 */
__attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse SYSTEM_CORCONModeOperatingSet(SYSTEM_CORCON_MODES modeValue) instead. ")))
inline static void CORCON_ModeOperatingSet(SYSTEM_CORCON_MODES modeValue)
{
    CORCON = (CORCON & 0x00F2) | modeValue;
}

/**
 * Sets the value of CPU core control register.
 * @param value value that needs to be written to the CPU core control register
 * @example
 * <code>
 * CORCON_RegisterValueSet(0x00E2);
 * </code>
 */
__attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse SYSTEM_CORCONRegisterValueSet(uint16_t value) instead. ")))
inline static void CORCON_RegisterValueSet(uint16_t value)
{
    CORCON = value;
}

/**
 * Gets the value of CPU core control register.
 * @return value of the CPU core control register
 * @example
 * <code>
 * corconSave = CORCON_RegisterValueGet();
 * </code>
 */
__attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse SYSTEM_CORCONRegisterValueGet (void) instead. ")))
inline static uint16_t CORCON_RegisterValueGet(void)
{    
    return CORCON;
}

/**
 * It handles the reset cause by clearing the cause register values.
 * Its a weak function user can override this function.
 * @return None
 * @example
 * <code>
 * SYSTEM_ResetCauseHandler();
 * </code>
 */
void __attribute__ ((weak)) SYSTEM_ResetCauseHandler(void) __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse RESET_CauseHandler(void) (void) instead. ")));

/**
 * This function resets the reset cause register.
 * @return None
 * @example
 * <code>
 * SYSTEM_ResetCauseClearAll();
 * </code>
 */
void SYSTEM_ResetCauseClearAll() __attribute__((deprecated ("\nThis will be removed in future MCC releases. \nUse RESET_CauseClearAll(void) (void) instead. ")));

#endif	/* MCC_H */
/**
 End of File
 */