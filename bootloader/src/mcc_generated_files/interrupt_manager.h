/**
  System Interrupts Generated Driver File 

  @Company:
    Microchip Technology Inc.

  @File Name:
    interrupt_manager.h

  @Summary:
    This is the generated driver implementation file for setting up the
    interrupts using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description:
    This source file provides implementations for PIC24 / dsPIC33 / PIC32MM MCUs interrupts.
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

#ifndef _INTERRUPT_MANAGER_H
#define _INTERRUPT_MANAGER_H

/**
  @Summary
    Initializes the interrupt priorities of the PIC24EP256GP204

  @Description
    This routine sets the interrupt priorities of the modules that have been configured
    for the PIC24EP256GP204

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    void SYSTEM_Initialize(void)
    {
        // Other initializers are called from this function
        INTERRUPT_Initialize ();
    }
    </code>

*/
void INTERRUPT_Initialize(void);

/**
  @Summary
    Enables global interrupts of the PIC24EP256GP204

  @Description
    This routine enables the global interrupt bit for the PIC24EP256GP204

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    void SYSTEM_Initialize(void)
    {
        // Other initializers are called from this function
        INTERRUPT_GlobalEnable ();
    }
    </code>

*/
inline static void INTERRUPT_GlobalEnable(void)
{
    __builtin_enable_interrupts();
}

/**
  @Summary
    Disables global interrupts of the PIC24EP256GP204

  @Description
    This routine disables the global interrupt bit for the PIC24EP256GP204

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    void SYSTEM_Initialize(void)
    {
        // Other initializers are called from this function
        INTERRUPT_GlobalDisable ();
    }
    </code>

*/
inline static void INTERRUPT_GlobalDisable(void)
{
    __builtin_disable_interrupts();
}

/**
  @Summary
    Returns the interrupt vector number of the interrupt which is pending.

  @Description
    This routine returns the interrupt vector number of the interrupt which is pending.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
        uint16_t ivrNum;
        ivrNum = INTERRUPT_VectorNumberGet();
    </code>

*/
inline static uint16_t INTERRUPT_VectorNumberGet(void)
{
    return _VECNUM; 
}

#endif