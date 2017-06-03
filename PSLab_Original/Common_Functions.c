#include "Common_Functions.h"

char *error_readpos = &errors[0], *error_writepos = &errors[0];
unsigned int i = 0;
char errors[ERROR_BUFFLEN];

void Delay_us(unsigned int delay) {
    unsigned int i;
    for (i = 0; i < delay; i++) {
        __asm__ volatile ("repeat #63");
        __asm__ volatile ("nop");
    }
}

void Delay_us_by8(unsigned int delay) {
    unsigned int i;
    for (i = 0; i < delay; i++) {
        __asm__ volatile ("repeat #7");
        __asm__ volatile ("nop");
    }
}

void Delay_ms(unsigned int delay) {
    unsigned int i, i2;
    for (i2 = 0; i2 < delay; i2++) {
        for (i = 0; i < 860; i++) {
            __asm__ volatile ("repeat #63");
            __asm__ volatile ("nop");
        }
        __asm__ volatile ("CLRWDT");
    }
}

void logit(char *str){
    while(*str!='\0'){
        *error_writepos++=*str++;
        if(error_writepos==&errors[ERROR_BUFFLEN])
            error_writepos=&errors[0];
    }
}