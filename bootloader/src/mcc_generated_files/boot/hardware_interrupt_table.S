/**
  @Generated 16-bit Bootloader Source File

  @Company:
    Microchip Technology Inc.

  @File Name: 
    hardware_interrupt_table.S

  @Summary:
    This is the hardware_interrupt_table.S file generated using 16-bit Bootloader

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  16-bit Bootloader - 1.18.4
        Device            :  PIC24EP256GP204
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36B
        MPLAB             :  MPLAB X v5.15
*/
/*
Copyright (c) [2012-2019] Microchip Technology Inc.  

    All rights reserved.

    You are permitted to use the accompanying software and its derivatives 
    with Microchip products. See the Microchip license agreement accompanying 
    this software, if any, for additional info regarding your rights and 
    obligations.
    
    MICROCHIP SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT 
    WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT 
    LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT 
    AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP OR ITS
    LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT 
    LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE 
    THEORY FOR ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT 
    LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES, 
    OR OTHER SIMILAR COSTS. 
    
    To the fullest extend allowed by law, Microchip and its licensors 
    liability will not exceed the amount of fees, if any, that you paid 
    directly to Microchip to use this software. 
    
    THIRD PARTY SOFTWARE:  Notwithstanding anything to the contrary, any 
    third party software accompanying this software is subject to the terms 
    and conditions of the third party's license agreement.  To the extent 
    required by third party licenses covering such third party software, 
    the terms of such license will apply in lieu of the terms provided in 
    this notice or applicable license.  To the extent the terms of such 
    third party licenses prohibit any of the restrictions described here, 
    such restrictions will not apply to such third party software.
*/
/* This file fills the hardware IVT table with the address of the corresponding
 * user remapped interrupt table.  Thus when a hardware interrupt occurs, 
 * the hardware interrupt vector will jump to the appropriate location in the 
 * user/application interrupt table.  That table will have a "goto _handler"
 * call that will call the appropriate handler function for the interrupt.
 */

#include "boot_config.h"
    	     
__bootloader_DefaultInterrupt:
    goto __DefaultInterrupt
    
    .equ remap_offset, 4
    
    .macro remap_to_application NAME
    .pword BOOT_CONFIG_USER_IVT_TABLE_ADDRESS+remap_offset
    .equ remap_offset, remap_offset+4
    .endm
    
    .macro keep_in_bootloader NAME
    .weak __\NAME
    .global __\NAME
    .equiv __\NAME, __bootloader_DefaultInterrupt
    .pword __\NAME
    .endm
    
    .macro remap_to_application_default NAME
    .pword BOOT_CONFIG_USER_IVT_TABLE_ADDRESS
    .endm
    
/*
 * IVT section information.
 */
    .section .ivt, code, address(0x4), keep
    
    #define REMAP_TABLE
    .include "mcc_generated_files/boot/interrupts.S"
