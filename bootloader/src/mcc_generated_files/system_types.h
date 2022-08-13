/**
  @Generated PIC24 / dsPIC33 / PIC32MM MCUs Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    system_types.h

  @Summary:
    This is the system_types.h file generated using PIC24 / dsPIC33 / PIC32MM MCUs

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

#ifndef SYSTEM_TYPES_H
#define	SYSTEM_TYPES_H

/**
  Section: Type defines
 */ 
    
/** 
 * CORCON initialization type enumerator. Supported types:
 * CORCON_MODE_PORVALUES
 * CORCON_MODE_ENABLEALLSATNORMAL_ROUNDBIASED
 * CORCON_MODE_ENABLEALLSATNORMAL_ROUNDUNBIASED
 * CORCON_MODE_DISABLEALLSAT_ROUNDBIASED
 * CORCON_MODE_DISABLEALLSAT_ROUNDUNBIASED
 * CORCON_MODE_ENABLEALLSATSUPER_ROUNDBIASED
 * CORCON_MODE_ENABLEALLSATSUPER_ROUNDUNBIASED
 */
typedef enum tagCORCON_MODE_TYPE
{ 
    CORCON_MODE_PORVALUES   = 0x0020,                       /** Use POR values of CORCON */
    CORCON_MODE_ENABLEALLSATNORMAL_ROUNDBIASED = 0x00E2,    /** Enable saturation for ACCA, ACCB
                                                             *  and Dataspace write, enable normal
                                                             *  ACCA/ACCB saturation mode and set
                                                             *  rounding to Biased (conventional)
                                                             *  mode. Rest of CORCON settings are
                                                             *  set to the default POR values.
                                                             *  */
    CORCON_MODE_ENABLEALLSATNORMAL_ROUNDUNBIASED = 0x00E0,  /** Enable saturation for ACCA, ACCB
                                                             *  and Dataspace write, enable normal
                                                             *  ACCA/ACCB saturation mode and set
                                                             *  rounding to Unbiased (convergent)
                                                             *  mode. Rest of CORCON settings are
                                                             *  set to the default POR values.
                                                             *  */
    CORCON_MODE_DISABLEALLSAT_ROUNDBIASED = 0x0022,         /** Disable saturation for ACCA, ACCB
                                                             *  and Dataspace write and set
                                                             *  rounding to Biased (conventional)
                                                             *  mode. Rest of CORCON settings are
                                                             *  set to the default POR values.
                                                             *  */
    CORCON_MODE_DISABLEALLSAT_ROUNDUNBIASED = 0x0020,       /** Disable saturation for ACCA, ACCB
                                                             *  and Dataspace write and set
                                                             *  rounding to Unbiased (convergent)
                                                             *  mode. Rest of CORCON settings are
                                                             *  set to the default POR values.
                                                             *  */
    CORCON_MODE_ENABLEALLSATSUPER_ROUNDBIASED = 0x00F2,    /** Enable saturation for ACCA, ACCB
                                                             *  and Dataspace write, enable super
                                                             *  ACCA/ACCB saturation mode and set
                                                             *  rounding to Biased (conventional)
                                                             *  mode. Rest of CORCON settings are
                                                             *  set to the default POR values.
                                                             *  */
    CORCON_MODE_ENABLEALLSATSUPER_ROUNDUNBIASED = 0x00F0,  /** Enable saturation for ACCA, ACCB
                                                             *  and Dataspace write, enable super
                                                             *  ACCA/ACCB saturation mode and set
                                                             *  rounding to Unbiased (convergent)
                                                             *  mode. Rest of CORCON settings are
                                                             *  set to the default POR values.
                                                             *  */
} SYSTEM_CORCON_MODES;

#endif	/* SYSTEM_TYPES_H */
/**
 End of File
*/