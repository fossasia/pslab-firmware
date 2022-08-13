/**
  WATCHDOG Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    watchdog.h

  @Summary
    This is the generated driver implementation file for the WATCHDOG driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides implementations for driver APIs for WATCHDOG.
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

#ifndef WATCHDOG_H
#define	WATCHDOG_H

/**
  Section: Type defines
 */ 

/**
 * Enables Watch Dog Timer (WDT) using the software bit.
 * @example
 * <code>
 * WATCHDOG_TimerSoftwareEnable();
 * </code>
 */
inline static void WATCHDOG_TimerSoftwareEnable(void)
{
    RCONbits.SWDTEN = 1;
}

/**
 * Disables Watch Dog Timer (WDT) using the software bit.
 * @example
 * <code>
 * WATCHDOG_TimerSoftwareDisable();
 * </code>
 */
inline static void WATCHDOG_TimerSoftwareDisable(void)
{
    RCONbits.SWDTEN = 0;
}

/**
 * Clears the Watch Dog Timer (WDT).
 * @example
 * <code>
 * WATCHDOG_TimerClear();
 * </code>
 */
inline static void WATCHDOG_TimerClear(void)
{
    ClrWdt();
}

#endif	/* WATCHDOG_H */
/**
 End of File
*/