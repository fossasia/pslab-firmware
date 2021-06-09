
/**
  OC4 Generated Driver API Source File

  @Company
    Microchip Technology Inc.

  @File Name
    oc4.c

  @Summary
    This is the generated source file for the OC4 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for driver for OC4.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC24EP256GP204
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.45
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

/**
  Section: Included Files
 */

#include "oc4.h"

/** OC Mode.

  @Summary
    Defines the OC Mode.

  @Description
    This data type defines the OC Mode of operation.

 */

static uint16_t gOC4Mode;

/**
  Section: Driver Interface
 */


void OC4_Initialize(void) {
    OC4_InitializeCON1();
    OC4_InitializeCON2();
    // OC4RS 0; 
    OC4RS = 0x00;
    // OC4R 0; 
    OC4R = 0x00;

    gOC4Mode = OC4CON1bits.OCM;
}

void OC4_InitializeCON1(void) {
    // Output Compare 3 continues to operate in CPU Idle mode
    OC4CON1bits.OCSIDL = 0;
    // Output Compare Clock Select is T2CLK
    OC4CON1bits.OCTSEL = 0b000;
    // Output Compare Fault B input (OCFB) is disabled
    OC4CON1bits.ENFLTB = 0;
    // Output Compare Fault A input (OCFA) is disabled
    OC4CON1bits.ENFLTA = 0;
    // No PWM Fault B condition on OCFB pin has occurred
    OC4CON1bits.OCFLTB = 0;
    // No PWM Fault A condition on OCFA pin has occurred
    OC4CON1bits.OCFLTA = 0;
    // TRIGSTAT is cleared only by software
    OC4CON1bits.TRIGMODE = 0;
    // Output compare channel is disabled
    OC4CON1bits.OCM = 0b000;
}

void OC4_InitializeCON2(void) {
    // Fault mode is maintained until the Fault source is removed and a new PWM period starts
    OC4CON2bits.FLTMD = 0;
    // PWM output is driven low on a Fault
    OC4CON2bits.FLTOUT = 0;
    // OC3 pin I/O state is defined by the FLTOUT bit on a Fault condition
    OC4CON2bits.FLTTRIEN = 0;
    // OC3 output is not inverted
    OC4CON2bits.OCINV = 0;
    // Cascade module operation is disabled
    OC4CON2bits.OC32 = 0;
    // Synchronizes OCx with the source designated by the SYNCSELx bits
    OC4CON2bits.OCTRIG = 0;
    // Timer source has not been triggered and is being held clear
    OC4CON2bits.TRIGSTAT = 0;
    // Output Compare 3 module drives the OC3 pin
    OC4CON2bits.OCTRIS = 0;
    // No Sync or Trigger source
    OC4CON2bits.SYNCSEL = 0b00000;
}

void __attribute__((weak)) OC4_CallBack(void) {
    // Add your custom callback code here
}

void OC4_Tasks(void) {
    if (IFS1bits.OC4IF) {
        // OC4 callback function 
        OC4_CallBack();
        IFS1bits.OC4IF = 0;
    }
}

void OC4_Start(void) {
    OC4CON1bits.OCM = gOC4Mode;
}

void OC4_Stop(void) {
    OC4CON1bits.OCM = 0;
}

void OC4_SecondaryValueSet(uint16_t secVal) {

    OC4RS = secVal;
}

void OC4_PrimaryValueSet(uint16_t priVal) {

    OC4R = priVal;
}

bool OC4_IsCompareCycleComplete(void) {
    return (IFS1bits.OC4IF);
}

bool OC4_FaultStatusGet(OC4_FAULTS faultNum) {
    bool status;
    /* Return the status of the fault condition */

    switch (faultNum) {
        case OC4_FAULT0:
            status = OC4CON1bits.OCFLT0;
            break;
        case OC4_FAULT1:
            status = OC4CON1bits.OCFLT1;
            break;
        default:
            break;

    }
    return (status);
}

void OC4_FaultStatusClear(OC4_FAULTS faultNum) {

    switch (faultNum) {
        case OC4_FAULT0:
            OC4CON1bits.OCFLT0 = 0;
            break;
        case OC4_FAULT1:
            OC4CON1bits.OCFLT1 = 0;
            break;
        default:
            break;
    }
}

void OC4_ManualTriggerSet(void) {
    OC4CON2bits.TRIGSTAT = true;
}

bool OC4_TriggerStatusGet(void) {
    return ( OC4CON2bits.TRIGSTAT);
}

void OC4_TriggerStatusClear(void) {
    /* Clears the trigger status */
    OC4CON2bits.TRIGSTAT = 0;
}

/**
 End of File
 */
