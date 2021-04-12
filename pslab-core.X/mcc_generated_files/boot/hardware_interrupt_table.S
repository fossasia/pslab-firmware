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
/* This file makes a GOTO table for every remapped interrupt.  The GOTO
 * table will jump to the specified interrupt handler if it is defined in the
 * user code and will go to the DefaultInterrupt() handler otherwise.  Just like
 * the compiler normally would.
 */
    
#include "boot_config.h"
   
/*
 * .macro: defines a macro
 * .weak:  define \NAME i.e InterruptName as weak
 * .equiv: Assign the Interrupt to DefaultInterrupt if Interrupt definition does
 *         not exist
 * goto:   goto the Interrupt defined in the application if defined.
 */
    .equ remap_offset, 4
    
    .macro remap_to_application NAME
    .weak __\NAME
    .global __\NAME
    .equiv __\NAME, __remapped_DefaultInterrupt
    .section remapped\NAME, code, address(BOOT_CONFIG_USER_IVT_TABLE_ADDRESS+remap_offset), keep
    goto __\NAME
    .equ remap_offset, remap_offset + 4
    .endm
    
    .macro keep_in_bootloader NAME
    .endm
    
    .macro remap_to_application_default NAME
    .endm
    
    /* First entry in the remap table is the default interrupt, and it is
     * required and not implemented in hardware vectors, so we will implement
     * it here. */
    .weak __remapped_DefaultInterrupt
    .global __remapped_DefaultInterrupt
    .section __remapped_DefaultInterrupt, code, address(BOOT_CONFIG_USER_IVT_TABLE_ADDRESS), keep
    goto __DefaultInterrupt
    
    /* All other entries are hardware vectors and can be selectively forwarded
     * by the user.  Pull them in via the interrupt configuration file. */
    #define REMAP_TABLE
    .include "mcc_generated_files/boot/interrupts.S"
