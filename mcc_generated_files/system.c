/**
  @Generated PIC24 / dsPIC33 / PIC32MM MCUs Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    system.h

  @Summary:
    This is the sysetm.h file generated using PIC24 / dsPIC33 / PIC32MM MCUs

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

#include "pin_manager.h"
#include "clock.h"
#include "system.h"
#include "system_types.h"
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
#include "cmp4.h"
#include "cmp2.h"

void SYSTEM_Initialize(void)
{
    PIN_MANAGER_Initialize();
    INTERRUPT_Initialize();
    CLOCK_Initialize();
    UART1_Initialize();
    UART2_Initialize();
    PTG_Initialize();
    OC4_Initialize();
    CMP4_Initialize();
    TMR4_Initialize();
    OC1_Initialize();
    IC4_Initialize();
    CMP1_Initialize();
    TMR3_Initialize();
    IC3_Initialize();
    IC2_Initialize();
    CMP2_Initialize();
    OC3_Initialize();
    TMR2_Initialize();
    CMP3_Initialize();
    ADC1_Initialize();
    OC2_Initialize();
    TMR5_Initialize();
    IC1_Initialize();
    TMR1_Initialize();
    DMA_Initialize();
    SYSTEM_CORCONModeOperatingSet(CORCON_MODE_PORVALUES);
}

/**
 End of File
 */