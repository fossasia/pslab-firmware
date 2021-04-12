/**
  @Generated 16-bit Bootloader Source File

  @Company:
    Microchip Technology Inc.

  @File Name: 
    boot_config.h

  @Summary:
    This is the boot_config.h file generated using 16-bit Bootloader

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
#ifndef BOOT_CONFIG_H
#define BOOT_CONFIG_H

#define BOOT_CONFIG_PROGRAMMABLE_ADDRESS_LOW 0x1800
#define BOOT_CONFIG_PROGRAMMABLE_ADDRESS_HIGH 0x2A7FE

#define BOOT_CONFIG_VERIFICATION_APPLICATION_HEADER_SIZE 0

#define BOOT_CONFIG_APPLICATION_IMAGE_APPLICATION_HEADER_ADDRESS (BOOT_CONFIG_PROGRAMMABLE_ADDRESS_LOW)

#define BOOT_CONFIG_APPLICATION_RESET_ADDRESS (BOOT_CONFIG_PROGRAMMABLE_ADDRESS_LOW + BOOT_CONFIG_VERIFICATION_APPLICATION_HEADER_SIZE)

#define BOOT_CONFIG_USER_IVT_TABLE_ADDRESS 0x1A00


#define BOOT_CONFIG_VERSION 0x0102

#define BOOT_CONFIG_MAX_PACKET_SIZE 0x100

#endif