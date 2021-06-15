#include "ic1.h"

/**
  IC Mode.

  @Summary
    Defines the IC Mode.

  @Description
    This data type defines the IC Mode of operation.

 */

static uint16_t gIC1Mode;

/**
  Section: Driver Interface
 */

void IC1_Initialize(void) {
    // ICSIDL disabled; ICM Off; ICTSEL TMR3; ICI Every; 
    IC1CON1 = 0x00;
    // SYNCSEL TMR3; TRIGSTAT disabled; IC32 disabled; ICTRIG Sync; 
    IC1CON2 = 0x0D;

    gIC1Mode = IC1CON1bits.ICM;

}

void __attribute__((weak)) IC1_CallBack(void) {
    // Add your custom callback code here
}

void IC1_Tasks(void) {
    if (IFS0bits.IC1IF) {
        // IC1 callback function 
        IC1_CallBack();
        IFS0bits.IC1IF = 0;
    }
}

void IC1_Start(void) {
    IC1CON1bits.ICM = gIC1Mode;
}

void IC1_Stop(void) {
    IC1CON1bits.ICM = 0;
}

uint16_t IC1_CaptureDataRead(void) {
    return (IC1BUF);
}

void IC1_ManualTriggerSet(void) {
    IC1CON2bits.TRIGSTAT = true;
}

bool IC1_TriggerStatusGet(void) {
    return ( IC1CON2bits.TRIGSTAT);
}

void IC1_TriggerStatusClear(void) {
    /* Clears the trigger status */
    IC1CON2bits.TRIGSTAT = 0;
}

bool IC1_HasCaptureBufferOverflowed(void) {
    return ( IC1CON1bits.ICOV);
}

bool IC1_IsCaptureBufferEmpty(void) {
    return ( !IC1CON1bits.ICBNE);
}
