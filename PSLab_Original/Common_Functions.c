#include "COMMANDS.h"
#include "Common_Functions.h"

char *error_readpos = &errors[0], *error_writepos = &errors[0];
uint16 i = 0;
char errors[ERROR_BUFFLEN];

void Delay_us(uint16 delay) {
    uint16 i;
    for (i = 0; i < delay; i++) {
        __asm__ volatile ("repeat #63");
        __asm__ volatile ("nop");
    }
}

void Delay_us_by8(uint16 delay) {
    uint16 i;
    for (i = 0; i < delay; i++) {
        __asm__ volatile ("repeat #7");
        __asm__ volatile ("nop");
    }
}

void Delay_ms(uint16 delay) {
    uint16 i, i2;
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