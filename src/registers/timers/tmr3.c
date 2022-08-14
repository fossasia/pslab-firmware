#include <stdio.h>
#include "tmr3.h"

void TMR3_Initialize(void) {
    // Clear timer 4 register
    TMR3 = 0x00;
    //Period = 0 s; Frequency = 64000000 Hz; PR4 0; 
    PR3 = 0x00;
    // Stops timer
    T3CONbits.TON = 0;
    // Continues module operation in Idle mode
    T3CONbits.TSIDL = 0;
    // Gated time accumulation is disabled
    T3CONbits.TGATE = 0;
    // Timer4 Input Clock Pre scale Select bits as 1:1
    T3CONbits.TCKPS = 0b00;
    // Internal clock (FP)
    T3CONbits.TCS = 0;
}

void TMR3_Period16BitSet(uint16_t value) {
    /* Update the counter values */
    PR3 = value;
}

uint16_t TMR3_Period16BitGet(void) {
    return (PR3);
}

void TMR3_Counter16BitSet(uint16_t value) {
    /* Update the counter values */
    TMR3 = value;
}

uint16_t TMR3_Counter16BitGet(void) {
    return (TMR3);
}

void TMR3_Start(void) {
    /* Start the Timer */
    T3CONbits.TON = 1;
}

void TMR3_Stop(void) {
    /* Stop the Timer */
    T3CONbits.TON = false;
}
