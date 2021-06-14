#include "oc3.h"

static uint16_t gOC3Mode;

void OC3_Initialize(void) {
    OC3_InitializeCON1();
    OC3_InitializeCON2();
    OC3RS = 0x00;
    OC3R = 0x00;

    gOC3Mode = OC3CON1bits.OCM;
}

void OC3_InitializeCON1(void) {
    // Output Compare 3 continues to operate in CPU Idle mode
    OC3CON1bits.OCSIDL = 0;
    // Output Compare Clock Select is T2CLK
    OC3CON1bits.OCTSEL = 0b000;
    // Output Compare Fault B input (OCFB) is disabled
    OC3CON1bits.ENFLTB = 0;
    // Output Compare Fault A input (OCFA) is disabled
    OC3CON1bits.ENFLTA = 0;
    // No PWM Fault B condition on OCFB pin has occurred
    OC3CON1bits.OCFLTB = 0;
    // No PWM Fault A condition on OCFA pin has occurred
    OC3CON1bits.OCFLTA = 0;
    // TRIGSTAT is cleared only by software
    OC3CON1bits.TRIGMODE = 0;
    // Output compare channel is disabled
    OC3CON1bits.OCM = 0b000;
}

void OC3_InitializeCON2(void) {
    // Fault mode is maintained until the Fault source is removed and a new PWM period starts
    OC3CON2bits.FLTMD = 0;
    // PWM output is driven low on a Fault
    OC3CON2bits.FLTOUT = 0;
    // OC3 pin I/O state is defined by the FLTOUT bit on a Fault condition
    OC3CON2bits.FLTTRIEN = 0;
    // OC3 output is not inverted
    OC3CON2bits.OCINV = 0;
    // Cascade module operation is disabled
    OC3CON2bits.OC32 = 0;
    // Synchronizes OCx with the source designated by the SYNCSELx bits
    OC3CON2bits.OCTRIG = 0;
    // Timer source has not been triggered and is being held clear
    OC3CON2bits.TRIGSTAT = 0;
    // Output Compare 3 module drives the OC3 pin
    OC3CON2bits.OCTRIS = 0;
    // No Sync or Trigger source
    OC3CON2bits.SYNCSEL = 0b00000;
}

void OC3_Start(void) {
    OC3CON1bits.OCM = gOC3Mode;
}

void OC3_Stop(void) {
    OC3CON1bits.OCM = 0;
}

void OC3_SecondaryValueSet(uint16_t secVal) {
    OC3RS = secVal;
}

void OC3_PrimaryValueSet(uint16_t priVal) {
    OC3R = priVal;
}

bool OC3_IsCompareCycleComplete(void) {
    return (IFS1bits.OC3IF);
}

bool OC3_FaultStatusGet(OC3_FAULTS faultNum) {
    bool status;
    /* Return the status of the fault condition */

    switch (faultNum) {
        case OC3_FAULT0:
            status = OC3CON1bits.OCFLT0;
            break;
        case OC3_FAULT1:
            status = OC3CON1bits.OCFLT1;
            break;
        default:
            break;
    }
    return (status);
}

void OC3_FaultStatusClear(OC3_FAULTS faultNum) {

    switch (faultNum) {
        case OC3_FAULT0:
            OC3CON1bits.OCFLT0 = 0;
            break;
        case OC3_FAULT1:
            OC3CON1bits.OCFLT1 = 0;
            break;
        default:
            break;
    }
}

void OC3_ManualTriggerSet(void) {
    OC3CON2bits.TRIGSTAT = true;
}

bool OC3_TriggerStatusGet(void) {
    return ( OC3CON2bits.TRIGSTAT);
}

void OC3_TriggerStatusClear(void) {
    /* Clears the trigger status */
    OC3CON2bits.TRIGSTAT = 0;
}
