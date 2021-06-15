#include "ic2.h"

/**
  IC Mode.

  @Summary
    Defines the IC Mode.

  @Description
    This data type defines the IC Mode of operation.

 */

static uint16_t gIC2Mode;

/**
  Section: Driver Interface
 */

void IC2_Initialize(void) {
    // ICSIDL disabled; ICM Off; ICTSEL TMR3; ICI Every; 
    IC2CON1 = 0x00;
    // SYNCSEL TMR3; TRIGSTAT disabled; IC32 disabled; ICTRIG Sync; 
    IC2CON2 = 0x0D;

    gIC2Mode = IC2CON1bits.ICM;

}

void __attribute__((weak)) IC2_CallBack(void) {
    // Add your custom callback code here
}

void IC2_Tasks(void) {
    if (IFS0bits.IC2IF) {
        // IC2 callback function 
        IC2_CallBack();
        IFS0bits.IC2IF = 0;
    }
}

void IC2_Start(void) {
    IC2CON1bits.ICM = gIC2Mode;
}

void IC2_Stop(void) {
    IC2CON1bits.ICM = 0;
}

uint16_t IC2_CaptureDataRead(void) {
    return (IC2BUF);
}

void IC2_ManualTriggerSet(void) {
    IC2CON2bits.TRIGSTAT = true;
}

bool IC2_TriggerStatusGet(void) {
    return ( IC2CON2bits.TRIGSTAT);
}

void IC2_TriggerStatusClear(void) {
    /* Clears the trigger status */
    IC2CON2bits.TRIGSTAT = 0;
}

bool IC2_HasCaptureBufferOverflowed(void) {
    return ( IC2CON1bits.ICOV);
}

bool IC2_IsCaptureBufferEmpty(void) {
    return ( !IC2CON1bits.ICBNE);
}
