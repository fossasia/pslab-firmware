#include "ic4.h"

/**
  IC Mode.

  @Summary
    Defines the IC Mode.

  @Description
    This data type defines the IC Mode of operation.

 */

static uint16_t gIC4Mode;

/**
  Section: Driver Interface
 */

void IC4_Initialize(void) {
    // ICSIDL disabled; ICM Off; ICTSEL TMR3; ICI Every; 
    IC4CON1 = 0x00;
    // SYNCSEL TMR3; TRIGSTAT disabled; IC32 disabled; ICTRIG Sync; 
    IC4CON2 = 0x0D;

    gIC4Mode = IC4CON1bits.ICM;

}

void __attribute__((weak)) IC4_CallBack(void) {
    // Add your custom callback code here
}

void IC4_Tasks(void) {
    if (IFS2bits.IC4IF) {
        // IC4 callback function 
        IC4_CallBack();
        IFS2bits.IC4IF = 0;
    }
}

void IC4_Start(void) {
    IC4CON1bits.ICM = gIC4Mode;
}

void IC4_Stop(void) {
    IC4CON1bits.ICM = 0;
}

uint16_t IC4_CaptureDataRead(void) {
    return (IC4BUF);
}

void IC4_ManualTriggerSet(void) {
    IC4CON2bits.TRIGSTAT = true;
}

bool IC4_TriggerStatusGet(void) {
    return ( IC4CON2bits.TRIGSTAT);
}

void IC4_TriggerStatusClear(void) {
    /* Clears the trigger status */
    IC4CON2bits.TRIGSTAT = 0;
}

bool IC4_HasCaptureBufferOverflowed(void) {
    return ( IC4CON1bits.ICOV);
}

bool IC4_IsCaptureBufferEmpty(void) {
    return ( !IC4CON1bits.ICBNE);
}
