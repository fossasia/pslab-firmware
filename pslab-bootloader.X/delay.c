#define FCY (_XTAL_FREQ / 2)


#include <libpic30.h>
#include "mcc_generated_files/clock.h"


void DELAY_ms(unsigned long d) {
    __delay_ms(d);
}

void DELAY_us(unsigned long d) {
    __delay_us(d);
}
