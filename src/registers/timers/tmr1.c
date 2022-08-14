#include <stdio.h>
#include "tmr1.h"

void TMR1_Initialize(void) {
    //TMR1 0; 
    TMR1 = 0x00;
    //Period = 0 s; Frequency = 64000000 Hz; PR1 0; 
    PR1 = 0x00;
    //TCKPS 1:1; TON disabled; TSIDL disabled; TCS FOSC/2; TSYNC disabled; TGATE disabled; 
    T1CON = 0x0000;
}

void TMR1_Period16BitSet(uint16_t value) {
    /* Update the counter values */
    PR1 = value;
}

uint16_t TMR1_Period16BitGet(void) {
    return (PR1);
}

void TMR1_Counter16BitSet(uint16_t value) {
    /* Update the counter values */
    TMR1 = value;
}

uint16_t TMR1_Counter16BitGet(void) {
    return (TMR1);
}

void TMR1_Start(void) {
    /* Start the Timer */
    T1CONbits.TON = 1;
}

void TMR1_Stop(void) {
    /* Stop the Timer */
    T1CONbits.TON = false;
}
