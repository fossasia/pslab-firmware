#include "ctmu.h"

void CTMU_Initialize(void) {

    CTMU_InitializeCON1();
    CTMU_InitializeCON2();
    // Nominal current output level is set by IRNG
    CTMUICONbits.ITRIM = 0b000000;
    // Current source output level is set to 1000 times the base current
    CTMUICONbits.IRNG = 0b00;
}

void CTMU_InitializeCON1(void) {
    // CTMU module is disabled
    CTMUCON1bits.CTMUEN = 0;
    // CTMU module continues operating in idle modes
    CTMUCON1bits.CTMUSIDL = 0;
    // Disables edge delay generation
    CTMUCON1bits.TGEN = 0;
    // Software is used to trigger edges
    CTMUCON1bits.EDGEN = 0;
    // No edge sequence is needed
    CTMUCON1bits.EDGSEQEN = 0;
    // Analog current source output is not grounded
    CTMUCON1bits.IDISSEN = 0;
    // CTMU does not trigger ADC conversion
    CTMUCON1bits.CTTRIG = 0;
}

void CTMU_InitializeCON2(void) {
    // Edge 1 is level sensitive
    CTMUCON2bits.EDG1MOD = 0;
    // Edge 1 is programmed for a negative edge response
    CTMUCON2bits.EDG1POL = 0;
    // Edge 1 source is Timer 1 module
    CTMUCON2bits.EDG1SEL = 0b0000;
    // Edge 1 has not occured
    CTMUCON2bits.EDG1STAT = 0;
    // Edge 2 is level sensitive
    CTMUCON2bits.EDG2MOD = 0;
    // Edge 2 is programmed for a negative edge response
    CTMUCON2bits.EDG2POL = 0;
    // Edge 2 source is IC1 module
    CTMUCON2bits.EDG2SEL = 0b0000;
    // Edge 2 has not occured
    CTMUCON2bits.EDG2STAT = 0;
}
