#include "rgb_led.h"

void Light_RGB(unsigned char red, unsigned char green, unsigned char blue) {
    unsigned char data[] = {green, red, blue};
    int location = 0;
    unsigned char byte = 0;
    unsigned char bit = 0;

    RGB_LED_SetLow();

    __asm__ volatile ("repeat #3264");
    Nop();

    for (location = 0; location < 3; location++) {
        bit = data[location];
        for (byte = 0; byte < 8; byte++) {
            if (bit & 0x80) {
                RGB_LED_SetHigh();
                __asm__ __volatile__("repeat #45");
                Nop();
                RGB_LED_SetLow();
                __asm__ __volatile__("repeat #38");
                Nop();
            } else {
                RGB_LED_SetHigh();
                __asm__ __volatile__("repeat #22");
                Nop();
                RGB_LED_SetLow();
                __asm__ __volatile__("repeat #51");
                Nop();
            }
            bit = bit << 1;
        }
    }
}
