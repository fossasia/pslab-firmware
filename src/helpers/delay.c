#include "types.h"
#include "delay.h"

// This include needs to be last, otherwise the linker can't find the
// __delay_ms and __delay_us functions.
#include <libpic30.h>

void DELAY_ms(unsigned long d) {
    __delay_ms(d);
}

void DELAY_us(unsigned long d) {
    __delay_us(d);
}
