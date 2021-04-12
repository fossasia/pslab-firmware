/**
  @Generated 16-bit Bootloader Source File

  @Company:
    Microchip Technology Inc.

  @File Name: 
    boot_demo.c

  @Summary:
    This is the boot_demo.c file generated using 16-bit Bootloader

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
#include "../memory/flash.h"
#include "boot_process.h"

#include <stdbool.h>
#include <stdint.h>
#include "../clock.h"

#define FCY _XTAL_FREQ/4

#include <libpic30.h>
#include "boot_config.h"
#include "../pin_manager.h"

static bool inBootloadMode = false;

static bool EnterBootloadMode(void);

void BOOT_DEMO_Initialize(void)
{   
//    RGB_LED_SetHigh();
//    __delay_ms(1);
//    
//    unsigned char data[] = {0, 0, 0};
//    unsigned char location = 0;
//    unsigned char ca = 0;
//    unsigned char cb = 0;
//    
//    data[2] = 20;
//    data[1] = 50;
//    data[0] = 10;
//    
//    RGB_LED_SetLow();
//    
//    __delay_us(51);
//    
//    for (location = 0; location < 3; location++) {
//        cb = data[location];
//        for (ca = 0; ca < 8; ca++) {
//            // 64 -- 1us
//            if (cb & 0x80) {
//                // T1H = 0.9 us 0.7
//                RGB_LED_SetHigh();
//                __asm__ __volatile__ ("repeat #50"); __asm__ volatile ("nop");
//                // T1L = 0.35 us 0.6
//                RGB_LED_SetLow();
//                __asm__ __volatile__ ("repeat #20"); __asm__ volatile ("nop");
//            } else {
//                // T0H = 0.35 us 0.35 
//                RGB_LED_SetHigh();
//                __asm__ __volatile__ ("repeat #20"); __asm__ volatile ("nop");
//                // T0L = 0.9 us 0.8
//                RGB_LED_SetLow();
//                __asm__ __volatile__ ("repeat #50"); __asm__ volatile ("nop");
//            }
//            cb = cb << 1;
//        }
//    }
    

}

void BOOT_DEMO_Tasks(void)
{
    if(inBootloadMode == false)
    {
        if( (EnterBootloadMode() == true) || (BOOT_Verify() == false) )
        {
            inBootloadMode = true;
        }
        else
        {
            BOOT_StartApplication();
        }
    }
    
    BOOT_ProcessCommand();
}

static bool EnterBootloadMode(void)
{
    #warning "Update this function to return 'true' when you want to stay in the boot loader, and 'false' when you want to allow a release to the application code"
 
    /* NOTE: This might be a a push button status on power up, a command from a peripheral, 
     * or whatever is specific to your boot loader implementation */    

    return false;
}
