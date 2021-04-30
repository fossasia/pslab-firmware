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

// Configuration bits: selected in the GUI

// FICD
#pragma config ICS = PGD2    //ICD Communication Channel Select bits->Communicate on PGEC2 and PGED2
#pragma config JTAGEN = OFF    //JTAG Enable bit->JTAG is disabled

// FPOR
#pragma config ALTI2C1 = OFF    //Alternate I2C1 pins->I2C1 mapped to SDA1/SCL1 pins
#pragma config ALTI2C2 = OFF    //Alternate I2C2 pins->I2C2 mapped to SDA2/SCL2 pins
#pragma config WDTWIN = WIN25    //Watchdog Window Select bits->WDT Window is 25% of WDT period

// FWDT
#pragma config WDTPOST = PS32768    //Watchdog Timer Postscaler bits->1:32768
#pragma config WDTPRE = PR128    //Watchdog Timer Prescaler bit->1:128
#pragma config PLLKEN = ON    //PLL Lock Enable bit->Clock switch to PLL source will wait until the PLL lock signal is valid.
#pragma config WINDIS = OFF    //Watchdog Timer Window Enable bit->Watchdog Timer in Non-Window mode
#pragma config FWDTEN = OFF    //Watchdog Timer Enable bit->Watchdog timer enabled/disabled by user software

// FOSC
#pragma config POSCMD = HS    //Primary Oscillator Mode Select bits->HS Crystal Oscillator Mode
#pragma config OSCIOFNC = OFF    //OSC2 Pin Function bit->OSC2 is clock output
#pragma config IOL1WAY = OFF    //Peripheral pin select configuration->Allow multiple reconfigurations
#pragma config FCKSM = CSECMD    //Clock Switching Mode bits->Clock switching is enabled,Fail-safe Clock Monitor is disabled

// FOSCSEL
#pragma config FNOSC = FRC    //Oscillator Source Selection->FRC
#pragma config IESO = OFF    //Two-speed Oscillator Start-up Enable bit->Start up with user-selected oscillator source

// FGS
#pragma config GWRP = OFF    //General Segment Write-Protect bit->General Segment may be written
#pragma config GCP = OFF    //General Segment Code-Protect bit->General Segment Code protect is Disabled

#include "pin_manager.h"
#include "clock.h"
#include "system.h"
#include "system_types.h"
#include "ext_int.h"
#include "oc4.h"
#include "uart2.h"
#include "oc2.h"
#include "drivers/spi_master.h"
#include "i2c1.h"
#include "sd_spi/sd_spi.h"
#include "interrupt_manager.h"
#include "traps.h"
#include "dma.h"
#include "spi1_driver.h"
#include "ic2.h"
#include "ic4.h"
#include "tmr1.h"
#include "tmr3.h"
#include "tmr5.h"
#include "cmp2.h"
#include "cmp4.h"
#include "oc3.h"
#include "uart1.h"
#include "oc1.h"
#include "adc1.h"
#include "ic1.h"
#include "cvr.h"
#include "ic3.h"
#include "tmr2.h"
#include "cmp1.h"
#include "tmr4.h"
#include "fatfs/ff.h"
#include "cmp3.h"

void SYSTEM_Initialize(void)
{
    PIN_MANAGER_Initialize();
    INTERRUPT_Initialize();
    CLOCK_Initialize();
    OC4_Initialize();
    UART1_Initialize();
    CMP4_Initialize();
    OC1_Initialize();
    TMR4_Initialize();
    EXT_INT_Initialize();
    IC4_Initialize();
    UART2_Initialize();
    I2C1_Initialize();
    CMP1_Initialize();
    TMR3_Initialize();
    IC3_Initialize();
    IC2_Initialize();
    CMP2_Initialize();
    CVR_Initialize();
    OC3_Initialize();
    TMR2_Initialize();
    CMP3_Initialize();
    ADC1_Initialize();
    OC2_Initialize();
    TMR5_Initialize();
    IC1_Initialize();
    DMA_Initialize();
    TMR1_Initialize();
    SYSTEM_CORCONModeOperatingSet(CORCON_MODE_PORVALUES);
}

/**
 End of File
*/