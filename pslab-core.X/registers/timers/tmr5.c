#include <stdio.h>
#include "tmr5.h"

void TMR5_Initialize(void) {
    //TMR5 0; 
    TMR5 = 0x00;
    //Period = 0 s; Frequency = 64000000 Hz; PR5 0; 
    PR5 = 0x00;
    //TCKPS 1:1; TON enabled; TSIDL disabled; TCS FOSC/2; TGATE disabled; 
    T5CON = 0x0000;
    // Disable interrupts
    _T5IE = 0;
    _T5IF = 0;
}

void TMR5_Period16BitSet(uint16_t value) {
    /* Update the counter values */
    PR5 = value;
}

uint16_t TMR5_Period16BitGet(void) {
    return (PR5);
}

void TMR5_Counter16BitSet(uint16_t value) {
    /* Update the counter values */
    TMR5 = value;
}

uint16_t TMR5_Counter16BitGet(void) {
    return (TMR5);
}

void TMR5_Start(void) {
    /* Start the Timer */
    T5CONbits.TON = 1;
}

void TMR5_Stop(void) {
    /* Stop the Timer */
    T5CONbits.TON = false;
}

void TMR5_WaitForInterruptEvent(void) {
    _T5IF = 0;
    while (!_T5IF);
    _T5IF = 0;
}
