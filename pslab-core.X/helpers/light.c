#include <xc.h>
#include "../bus/uart/uart1.h"
#include "../registers/system/interrupt_manager.h"
#include "../registers/system/pin_manager.h"
#include "light.h"

void LIGHT_RGB(uint8_t red, uint8_t green, uint8_t blue) {
    uint8_t data[] = {green, red, blue};
    uint8_t location;

    RGB_LED_SetLow();

    __asm__ volatile ("repeat #3264");
    Nop();

    for (location = 0; location < 3; location++) {
        uint8_t bit;
        bit = data[location];
        uint8_t byte;
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

response_t LIGHT_Onboard(void) {
    
    uint8_t count = UART1_Read();
    uint8_t colors[count];
    
    uint8_t i;
    for (i = 0; i < count; i++) {
        colors[i] = UART1_Read();
    }
    
    INTERRUPT_GlobalDisable();
    
    for (i = 0; i < count; i = i + 3) {
        uint8_t data[] = {colors[i+1], colors[i], colors[i+2]};
        uint8_t location;

        RGB_LED_SetLow();

        __asm__ volatile ("repeat #3264");
        Nop();

        for (location = 0; location < 3; location++) {
            uint8_t bit;
            bit = data[location];
            uint8_t byte;
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
    
    INTERRUPT_GlobalEnable();
    
    return SUCCESS;
}

response_t LIGHT_One(void) {
    
    uint8_t count = UART1_Read();
    uint8_t colors[count];
    
    uint8_t i;
    for (i = 0; i < count; i++) {
        colors[i] = UART1_Read();
    }
    
    INTERRUPT_GlobalDisable();
    
    for (i = 0; i < count; i = i + 3) {
        uint8_t data[] = {colors[i+1], colors[i], colors[i+2]};
        uint8_t location;

        SQR1_SetLow();

        __asm__ volatile ("repeat #3264");
        Nop();

        for (location = 0; location < 3; location++) {
            uint8_t bit;
            bit = data[location];
            uint8_t byte;
            for (byte = 0; byte < 8; byte++) {
                if (bit & 0x80) {
                    SQR1_SetHigh();
                    __asm__ __volatile__("repeat #45");
                    Nop();
                    SQR1_SetLow();
                    __asm__ __volatile__("repeat #38");
                    Nop();
                } else {
                    SQR1_SetHigh();
                    __asm__ __volatile__("repeat #22");
                    Nop();
                    SQR1_SetLow();
                    __asm__ __volatile__("repeat #51");
                    Nop();
                }
                bit = bit << 1;
            }
        }
    }
    
    INTERRUPT_GlobalEnable();
    
    return SUCCESS;
}

response_t LIGHT_Two(void) {
    
    uint8_t count = UART1_Read();
    uint8_t colors[count];
    
    uint8_t i;
    for (i = 0; i < count; i++) {
        colors[i] = UART1_Read();
    }
    
    INTERRUPT_GlobalDisable();
    
    for (i = 0; i < count; i = i + 3) {
        uint8_t data[] = {colors[i+1], colors[i], colors[i+2]};
        uint8_t location;

        SQR2_SetLow();

        __asm__ volatile ("repeat #3264");
        Nop();

        for (location = 0; location < 3; location++) {
            uint8_t bit;
            bit = data[location];
            uint8_t byte;
            for (byte = 0; byte < 8; byte++) {
                if (bit & 0x80) {
                    SQR2_SetHigh();
                    __asm__ __volatile__("repeat #45");
                    Nop();
                    SQR2_SetLow();
                    __asm__ __volatile__("repeat #38");
                    Nop();
                } else {
                    SQR2_SetHigh();
                    __asm__ __volatile__("repeat #22");
                    Nop();
                    SQR2_SetLow();
                    __asm__ __volatile__("repeat #51");
                    Nop();
                }
                bit = bit << 1;
            }
        }
    }
    
    INTERRUPT_GlobalEnable();
    
    return SUCCESS;
}