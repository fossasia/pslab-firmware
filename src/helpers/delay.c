#include "delay.h"
#include <libpic30.h>

void DELAY_ms(unsigned long d) {
    __delay_ms(d);
}

void DELAY_us(unsigned long d) {
    __delay_us(d);
}
