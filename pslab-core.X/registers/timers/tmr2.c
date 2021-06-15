#include <stdio.h>
#include "tmr2.h"

void TMR2_Initialize(void) {
    //TMR2 0; 
    TMR2 = 0x00;
    //Period = 0 s; Frequency = 64000000 Hz; PR2 0; 
    PR2 = 0x00;
    //TCKPS 1:1; T32 16 Bit; TON disabled; TSIDL disabled; TCS FOSC/2; TGATE disabled; 
    T2CON = 0x0000;
}
void TMR2_Period16BitSet(uint16_t value) {
    /* Update the counter values */
    PR2 = value;
}

uint16_t TMR2_Period16BitGet(void) {
    return (PR2);
}

void TMR2_Counter16BitSet(uint16_t value) {
    /* Update the counter values */
    TMR2 = value;
}

uint16_t TMR2_Counter16BitGet(void) {
    return (TMR2);
}

void TMR2_Start(void) {
    /* Start the Timer */
    T2CONbits.TON = 1;
}

void TMR2_Stop(void) {
    /* Stop the Timer */
    T2CONbits.TON = false;
}
