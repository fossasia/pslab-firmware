#include "ic3.h"

/**
  IC Mode.

  @Summary
    Defines the IC Mode.

  @Description
    This data type defines the IC Mode of operation.

 */

static uint16_t gIC3Mode;

/**
  Section: Driver Interface
 */

void IC3_Initialize(void) {
    // ICSIDL disabled; ICM Off; ICTSEL TMR3; ICI Every; 
    IC3CON1 = 0x00;
    // SYNCSEL TMR3; TRIGSTAT disabled; IC32 disabled; ICTRIG Sync; 
    IC3CON2 = 0x0D;

    gIC3Mode = IC3CON1bits.ICM;

}

void __attribute__((weak)) IC3_CallBack(void) {
    // Add your custom callback code here
}

void IC3_Tasks(void) {
    if (IFS2bits.IC3IF) {
        // IC3 callback function 
        IC3_CallBack();
        IFS2bits.IC3IF = 0;
    }
}

void IC3_Start(void) {
    IC3CON1bits.ICM = gIC3Mode;
}

void IC3_Stop(void) {
    IC3CON1bits.ICM = 0;
}

uint16_t IC3_CaptureDataRead(void) {
    return (IC3BUF);
}

void IC3_ManualTriggerSet(void) {
    IC3CON2bits.TRIGSTAT = true;
}

bool IC3_TriggerStatusGet(void) {
    return ( IC3CON2bits.TRIGSTAT);
}

void IC3_TriggerStatusClear(void) {
    /* Clears the trigger status */
    IC3CON2bits.TRIGSTAT = 0;
}

bool IC3_HasCaptureBufferOverflowed(void) {
    return ( IC3CON1bits.ICOV);
}

bool IC3_IsCaptureBufferEmpty(void) {
    return ( !IC3CON1bits.ICBNE);
}
