#include <stdio.h>
#include "tmr2.h"

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

static TMR_OBJ tmr2_obj;

/**
  Section: Driver Interface
 */

void TMR2_Initialize(void) {
    //TMR2 0; 
    TMR2 = 0x00;
    //Period = 0 s; Frequency = 64000000 Hz; PR2 0; 
    PR2 = 0x00;
    //TCKPS 1:1; T32 16 Bit; TON enabled; TSIDL disabled; TCS FOSC/2; TGATE disabled; 
    T2CON = 0x8000;


    tmr2_obj.timerElapsed = false;

}

void TMR2_Tasks_16BitOperation(void) {
    /* Check if the Timer Interrupt/Status is set */
    if (IFS0bits.T2IF) {
        tmr2_obj.count++;
        tmr2_obj.timerElapsed = true;
        IFS0bits.T2IF = false;
    }
}

void TMR2_Period16BitSet(uint16_t value) {
    /* Update the counter values */
    PR2 = value;
    /* Reset the status information */
    tmr2_obj.timerElapsed = false;
}

uint16_t TMR2_Period16BitGet(void) {
    return ( PR2);
}

void TMR2_Counter16BitSet(uint16_t value) {
    /* Update the counter values */
    TMR2 = value;
    /* Reset the status information */
    tmr2_obj.timerElapsed = false;
}

uint16_t TMR2_Counter16BitGet(void) {
    return ( TMR2);
}

void TMR2_Start(void) {
    /* Reset the status information */
    tmr2_obj.timerElapsed = false;


    /* Start the Timer */
    T2CONbits.TON = 1;
}

void TMR2_Stop(void) {
    /* Stop the Timer */
    T2CONbits.TON = false;

}

bool TMR2_GetElapsedThenClear(void) {
    bool status;

    status = tmr2_obj.timerElapsed;

    if (status == true) {
        tmr2_obj.timerElapsed = false;
    }
    return status;
}

int TMR2_SoftwareCounterGet(void) {
    return tmr2_obj.count;
}

void TMR2_SoftwareCounterClear(void) {
    tmr2_obj.count = 0;
}
