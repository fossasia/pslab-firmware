#include <stdio.h>
#include "tmr4.h"

typedef struct _TMR_OBJ_STRUCT {
    /* Timer Elapsed */
    volatile bool timerElapsed;
    /*Software Counter value*/
    volatile uint8_t count;

} TMR_OBJ;

static TMR_OBJ tmr4_obj;

/**
  Section: Driver Interface
 */

void TMR4_Initialize(void) {
    // Clear timer 4 register
    TMR4 = 0x00;
    //Period = 0 s; Frequency = 64000000 Hz; PR4 0; 
    PR4 = 0x00;
    // Stops timer
    T4CONbits.TON = 0;
    // Continues module operation in Idle mode
    T4CONbits.TSIDL = 0;
    // Gated time accumulation is disabled
    T4CONbits.TGATE = 0;
    // Timer4 Input Clock Pre scale Select bits as 1:1
    T4CONbits.TCKPS = 0b00;
    // Timer x and Timer y act as two 16-bit timers
    T4CONbits.T32 = 0;
    // Internal clock (FP)
    T4CONbits.TCS = 0;

    tmr4_obj.timerElapsed = false;
}

void TMR4_Tasks_16BitOperation(void) {
    /* Check if the Timer Interrupt/Status is set */
    if (IFS1bits.T4IF) {
        tmr4_obj.count++;
        tmr4_obj.timerElapsed = true;
        IFS1bits.T4IF = false;
    }
}

void TMR4_Period16BitSet(uint16_t value) {
    /* Update the counter values */
    PR4 = value;
    /* Reset the status information */
    tmr4_obj.timerElapsed = false;
}

uint16_t TMR4_Period16BitGet(void) {
    return ( PR4);
}

void TMR4_Counter16BitSet(uint16_t value) {
    /* Update the counter values */
    TMR4 = value;
    /* Reset the status information */
    tmr4_obj.timerElapsed = false;
}

uint16_t TMR4_Counter16BitGet(void) {
    return ( TMR4);
}

void TMR4_Start(void) {
    /* Reset the status information */
    tmr4_obj.timerElapsed = false;

    /* Start the Timer */
    T4CONbits.TON = 1;
}

void TMR4_Stop(void) {
    /* Stop the Timer */
    T4CONbits.TON = false;

}

bool TMR4_GetElapsedThenClear(void) {
    bool status;

    status = tmr4_obj.timerElapsed;

    if (status == true) {
        tmr4_obj.timerElapsed = false;
    }
    return status;
}

int TMR4_SoftwareCounterGet(void) {
    return tmr4_obj.count;
}

void TMR4_SoftwareCounterClear(void) {
    tmr4_obj.count = 0;
}