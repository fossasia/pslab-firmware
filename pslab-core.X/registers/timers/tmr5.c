#include <stdio.h>
#include "tmr5.h"

/**
 Section: File specific functions
 */

/**
  Section: Data Type Definitions
 */

/** TMR Driver Hardware Instance Object

  @Summary
    Defines the object required for the maintenance of the hardware instance.

  @Description
    This defines the object required for the maintenance of the hardware
    instance. This object exists once per hardware instance of the peripheral.

  Remarks:
    None.
 */

typedef struct _TMR_OBJ_STRUCT {
    /* Timer Elapsed */
    volatile bool timerElapsed;
    /*Software Counter value*/
    volatile uint8_t count;

} TMR_OBJ;

static TMR_OBJ tmr5_obj;

/**
  Section: Driver Interface
 */

void TMR5_Initialize(void) {
    //TMR5 0; 
    TMR5 = 0x00;
    //Period = 0 s; Frequency = 64000000 Hz; PR5 0; 
    PR5 = 0x00;
    //TCKPS 1:1; TON enabled; TSIDL disabled; TCS FOSC/2; TGATE disabled; 
    T5CON = 0x0000;

    tmr5_obj.timerElapsed = false;

}

void TMR5_Tasks_16BitOperation(void) {
    /* Check if the Timer Interrupt/Status is set */
    if (IFS1bits.T5IF) {
        tmr5_obj.count++;
        tmr5_obj.timerElapsed = true;
        IFS1bits.T5IF = false;
    }
}

void TMR5_Period16BitSet(uint16_t value) {
    /* Update the counter values */
    PR5 = value;
    /* Reset the status information */
    tmr5_obj.timerElapsed = false;
}

uint16_t TMR5_Period16BitGet(void) {
    return ( PR5);
}

void TMR5_Counter16BitSet(uint16_t value) {
    /* Update the counter values */
    TMR5 = value;
    /* Reset the status information */
    tmr5_obj.timerElapsed = false;
}

uint16_t TMR5_Counter16BitGet(void) {
    return ( TMR5);
}

void TMR5_Start(void) {
    /* Reset the status information */
    tmr5_obj.timerElapsed = false;


    /* Start the Timer */
    T5CONbits.TON = 1;
}

void TMR5_Stop(void) {
    /* Stop the Timer */
    T5CONbits.TON = false;

}

bool TMR5_GetElapsedThenClear(void) {
    bool status;

    status = tmr5_obj.timerElapsed;

    if (status == true) {
        tmr5_obj.timerElapsed = false;
    }
    return status;
}

int TMR5_SoftwareCounterGet(void) {
    return tmr5_obj.count;
}

void TMR5_SoftwareCounterClear(void) {
    tmr5_obj.count = 0;
}
