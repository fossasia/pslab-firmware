#include "oc1.h"

static uint16_t gOC1Mode;

void OC1_Initialize(void) {

    OC1_InitializeCON1();
    OC1_InitializeCON2();
    OC1RS = 0x00;
    OC1R = 0x00;

    gOC1Mode = OC1CON1bits.OCM;
}

void OC1_InitializeCON1(void) {
    // Output Compare 1 continues to operate in CPU Idle mode
    OC1CON1bits.OCSIDL = 0;
    // Output Compare Clock Select is T2CLK
    OC1CON1bits.OCTSEL = 0b000;
    // Output Compare Fault B input (OCFB) is disabled
    OC1CON1bits.ENFLTB = 0;
    // Output Compare Fault A input (OCFA) is disabled
    OC1CON1bits.ENFLTA = 0;
    // No PWM Fault B condition on OCFB pin has occurred
    OC1CON1bits.OCFLTB = 0;
    // No PWM Fault A condition on OCFA pin has occurred
    OC1CON1bits.OCFLTA = 0;
    // TRIGSTAT is cleared only by software
    OC1CON1bits.TRIGMODE = 0;
    // Output compare channel is disabled
    OC1CON1bits.OCM = 0b000;
}

void OC1_InitializeCON2(void) {
    // Fault mode is maintained until the Fault source is removed and a new PWM period starts
    OC1CON2bits.FLTMD = 0;
    // PWM output is driven low on a Fault
    OC1CON2bits.FLTOUT = 0;
    // OC1 pin I/O state is defined by the FLTOUT bit on a Fault condition
    OC1CON2bits.FLTTRIEN = 0;
    // OC1 output is not inverted
    OC1CON2bits.OCINV = 0;
    // Cascade module operation is disabled
    OC1CON2bits.OC32 = 0;
    // Synchronizes OCx with the source designated by the SYNCSELx bits
    OC1CON2bits.OCTRIG = 0;
    // Timer source has not been triggered and is being held clear
    OC1CON2bits.TRIGSTAT = 0;
    // Output Compare 1 module drives the OC3 pin
    OC1CON2bits.OCTRIS = 0;
    // No Sync or Trigger source
    OC1CON2bits.SYNCSEL = 0b00000;
}

void OC1_Start(void) {
    OC1CON1bits.OCM = gOC1Mode;
}

void OC1_Stop(void) {
    OC1CON1bits.OCM = 0;
}

void OC1_SecondaryValueSet(uint16_t secVal) {
    OC1RS = secVal;
}

void OC1_PrimaryValueSet(uint16_t priVal) {
    OC1R = priVal;
}

bool OC1_IsCompareCycleComplete(void) {
    return (IFS0bits.OC1IF);
}

bool OC1_FaultStatusGet(OC1_FAULTS faultNum) {
    bool status;
    /* Return the status of the fault condition */

    switch (faultNum) {
        case OC1_FAULT0:
            status = OC1CON1bits.OCFLT0;
            break;
        case OC1_FAULT1:
            status = OC1CON1bits.OCFLT1;
            break;
        default:
            break;

    }
    return (status);
}

void OC1_FaultStatusClear(OC1_FAULTS faultNum) {

    switch (faultNum) {
        case OC1_FAULT0:
            OC1CON1bits.OCFLT0 = 0;
            break;
        case OC1_FAULT1:
            OC1CON1bits.OCFLT1 = 0;
            break;
        default:
            break;
    }
}

void OC1_ManualTriggerSet(void) {
    OC1CON2bits.TRIGSTAT = true;
}

bool OC1_TriggerStatusGet(void) {
    return ( OC1CON2bits.TRIGSTAT);
}

void OC1_TriggerStatusClear(void) {
    /* Clears the trigger status */
    OC1CON2bits.TRIGSTAT = 0;
}
