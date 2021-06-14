#include "oc2.h"

static uint16_t gOC2Mode;

void OC2_Initialize(void) {
    
    OC2_InitializeCON1();
    OC2_InitializeCON2();
    OC2RS = 0x00;
    OC2R = 0x00;

    gOC2Mode = OC2CON1bits.OCM;
}

void OC2_InitializeCON1(void) {
    // Output Compare 2 continues to operate in CPU Idle mode
    OC2CON1bits.OCSIDL = 0;
    // Output Compare Clock Select is T2CLK
    OC2CON1bits.OCTSEL = 0b000;
    // Output Compare Fault B input (OCFB) is disabled
    OC2CON1bits.ENFLTB = 0;
    // Output Compare Fault A input (OCFA) is disabled
    OC2CON1bits.ENFLTA = 0;
    // No PWM Fault B condition on OCFB pin has occurred
    OC2CON1bits.OCFLTB = 0;
    // No PWM Fault A condition on OCFA pin has occurred
    OC2CON1bits.OCFLTA = 0;
    // TRIGSTAT is cleared only by software
    OC2CON1bits.TRIGMODE = 0;
    // Output compare channel is disabled
    OC2CON1bits.OCM = 0b000;
}

void OC2_InitializeCON2(void) {
    // Fault mode is maintained until the Fault source is removed and a new PWM period starts
    OC2CON2bits.FLTMD = 0;
    // PWM output is driven low on a Fault
    OC2CON2bits.FLTOUT = 0;
    // OC2 pin I/O state is defined by the FLTOUT bit on a Fault condition
    OC2CON2bits.FLTTRIEN = 0;
    // OC2 output is not inverted
    OC2CON2bits.OCINV = 0;
    // Cascade module operation is disabled
    OC2CON2bits.OC32 = 0;
    // Synchronizes OCx with the source designated by the SYNCSELx bits
    OC2CON2bits.OCTRIG = 0;
    // Timer source has not been triggered and is being held clear
    OC2CON2bits.TRIGSTAT = 0;
    // Output Compare 2 module drives the OC2 pin
    OC2CON2bits.OCTRIS = 0;
    // No Sync or Trigger source
    OC2CON2bits.SYNCSEL = 0b00000;
}

void OC2_Start(void) {
    OC2CON1bits.OCM = gOC2Mode;
}

void OC2_Stop(void) {
    OC2CON1bits.OCM = 0;
}

void OC2_SecondaryValueSet(uint16_t secVal) {
    OC2RS = secVal;
}

void OC2_PrimaryValueSet(uint16_t priVal) {
    OC2R = priVal;
}

bool OC2_IsCompareCycleComplete(void) {
    return (IFS0bits.OC2IF);
}

bool OC2_FaultStatusGet(OC2_FAULTS faultNum) {
    bool status;
    /* Return the status of the fault condition */

    switch (faultNum) {
        case OC2_FAULT0:
            status = OC2CON1bits.OCFLT0;
            break;
        case OC2_FAULT1:
            status = OC2CON1bits.OCFLT1;
            break;
        default:
            break;

    }
    return (status);
}

void OC2_FaultStatusClear(OC2_FAULTS faultNum) {

    switch (faultNum) {
        case OC2_FAULT0:
            OC2CON1bits.OCFLT0 = 0;
            break;
        case OC2_FAULT1:
            OC2CON1bits.OCFLT1 = 0;
            break;
        default:
            break;
    }
}

void OC2_ManualTriggerSet(void) {
    OC2CON2bits.TRIGSTAT = true;
}

bool OC2_TriggerStatusGet(void) {
    return ( OC2CON2bits.TRIGSTAT);
}

void OC2_TriggerStatusClear(void) {
    /* Clears the trigger status */
    OC2CON2bits.TRIGSTAT = 0;
}
