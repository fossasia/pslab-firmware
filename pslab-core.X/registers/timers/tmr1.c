#include <stdio.h>
#include "tmr1.h"

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

static TMR_OBJ tmr1_obj;

/**
  Section: Driver Interface
 */

void TMR1_Initialize(void) {
    //TMR1 0; 
    TMR1 = 0x00;
    //Period = 0 s; Frequency = 64000000 Hz; PR1 0; 
    PR1 = 0x00;
    //TCKPS 1:1; TON enabled; TSIDL disabled; TCS FOSC/2; TSYNC disabled; TGATE disabled; 
    T1CON = 0x8000;


    tmr1_obj.timerElapsed = false;

}

void TMR1_Tasks_16BitOperation(void) {
    /* Check if the Timer Interrupt/Status is set */
    if (IFS0bits.T1IF) {
        tmr1_obj.count++;
        tmr1_obj.timerElapsed = true;
        IFS0bits.T1IF = false;
    }
}

void TMR1_Period16BitSet(uint16_t value) {
    /* Update the counter values */
    PR1 = value;
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;
}

uint16_t TMR1_Period16BitGet(void) {
    return ( PR1);
}

void TMR1_Counter16BitSet(uint16_t value) {
    /* Update the counter values */
    TMR1 = value;
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;
}

uint16_t TMR1_Counter16BitGet(void) {
    return ( TMR1);
}

void TMR1_Start(void) {
    /* Reset the status information */
    tmr1_obj.timerElapsed = false;


    /* Start the Timer */
    T1CONbits.TON = 1;
}

void TMR1_Stop(void) {
    /* Stop the Timer */
    T1CONbits.TON = false;

}

bool TMR1_GetElapsedThenClear(void) {
    bool status;

    status = tmr1_obj.timerElapsed;

    if (status == true) {
        tmr1_obj.timerElapsed = false;
    }
    return status;
}

int TMR1_SoftwareCounterGet(void) {
    return tmr1_obj.count;
}

void TMR1_SoftwareCounterClear(void) {
    tmr1_obj.count = 0;
}
