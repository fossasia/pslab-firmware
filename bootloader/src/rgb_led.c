#include <xc.h>
#include "mcc_generated_files/pin_manager.h"

void Light_RGB(uint8_t red, uint8_t green, uint8_t blue) {
    
    uint8_t data[] = {green, red, blue};
    uint8_t location = 0;
    uint8_t byte = 0;
    uint8_t bit = 0;

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
