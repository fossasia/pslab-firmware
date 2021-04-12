/**
  @Generated 16-bit Bootloader Source File

  @Company:
    Microchip Technology Inc.

  @File Name: 
    boot_private.h

  @Summary:
    This is the boot_private.h file generated using 16-bit Bootloader

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
#ifndef BOOT_PRIVATE_H
#define BOOT_PRIVATE_H

#include "boot_config.h" 
#define MINIMUM_WRITE_BLOCK_SIZE 8

#define SIZE_OF_CMD_STRUCT_0 11
struct __attribute__((__packed__)) CMD_STRUCT_0{
    uint8_t cmd;
    uint16_t dataLength;
    uint32_t unlockSequence;
    uint32_t address;
};

struct __attribute__((__packed__)) CMD_STRUCT_0_WITH_PAYLOAD{
    uint8_t cmd;
    uint16_t dataLength;
    uint32_t unlockSequence;
    uint32_t address;
    uint8_t data[BOOT_CONFIG_MAX_PACKET_SIZE - SIZE_OF_CMD_STRUCT_0];
};

struct __attribute__((__packed__)) GET_VERSION_RESPONSE{
    uint8_t cmd;
    uint16_t dataLength;
    uint32_t unlockSequence;
    uint32_t address;
    //---
    uint16_t version;
    uint16_t maxPacketLength;
    uint16_t unused1;
    uint16_t deviceId;
    uint16_t unused2;
    uint16_t eraseSize;
    uint16_t writeSize;
    uint32_t unused3;
    uint32_t userRsvdStartSddress;
    uint32_t userRsvdEndSddress;
};   
    
struct __attribute__((__packed__)) RESPONSE_TYPE_0{
    uint8_t cmd;
    uint16_t dataLength;
    uint32_t unlockSequence;
    uint32_t address;
    //---
    uint8_t success;
};
struct __attribute__((__packed__)) RESPONSE_TYPE_0_2_PAYLOAD{
    uint8_t cmd;
    uint16_t dataLength;
    uint32_t unlockSequence;
    uint32_t address;
    //---
    uint8_t success;
    uint16_t data;
};
struct __attribute__((__packed__)) RESPONSE_TYPE_0_WITH_PAYLOAD{
    uint8_t cmd;
    uint16_t dataLength;
    uint32_t unlockSequence;
    uint32_t address;
    //---
    uint8_t success;    
    uint8_t data[BOOT_CONFIG_MAX_PACKET_SIZE - SIZE_OF_CMD_STRUCT_0 - 1];
}; 
struct __attribute__((__packed__)) GET_MEMORY_ADDRESS_RANGE_RESPONSE{
    uint8_t  cmd;
    uint16_t dataLength;  
    uint32_t unlockSequence; 
    uint32_t address; 
     uint8_t success; 
    //---
    uint32_t programFlashStart; 
    uint32_t programFlashEnd; 
};

#endif
