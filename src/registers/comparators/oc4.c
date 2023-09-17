#include "oc4.h"

static uint16_t gOC4Mode;

void OC4_Initialize(void) {
    OC4_InitializeCON1();
    OC4_InitializeCON2();
    OC4RS = 0x00;
    OC4R = 0x00;

    gOC4Mode = OC4CON1bits.OCM;
}

void OC4_InitializeCON1(void) {
    // Output Compare 4 continues to operate in CPU Idle mode
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
    // OC4 pin I/O state is defined by the FLTOUT bit on a Fault condition
    OC4CON2bits.FLTTRIEN = 0;
    // OC4 output is not inverted
    OC4CON2bits.OCINV = 0;
    // Cascade module operation is disabled
    OC4CON2bits.OC32 = 0;
    // Synchronizes OCx with the source designated by the SYNCSELx bits
    OC4CON2bits.OCTRIG = 0;
    // Timer source has not been triggered and is being held clear
    OC4CON2bits.TRIGSTAT = 0;
    // Output Compare 4 module drives the OC4 pin
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
    bool status = true;
    /* Return the status of the fault condition */

    switch (faultNum) {
        case OC4_FAULT0:
            status = OC4CON1bits.OCFLT0;
            break;
        case OC4_FAULT1:
            status = OC4CON1bits.OCFLT1;
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
