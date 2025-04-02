#include <xc.h>
#include "../bus/uart/uart.h"
#include "../registers/system/interrupt_manager.h"
#include "../registers/system/pin_manager.h"
#include "light.h"

#define SendZero(pin) do {\
            pin = 1;\
            __asm__ __volatile__("repeat #22");\
            Nop();\
            pin = 0;\
            __asm__ __volatile__("repeat #51");\
            Nop();\
        } while(0)

#define SendOne(pin) do {\
            pin = 1;\
            __asm__ __volatile__("repeat #45");\
            Nop();\
            pin = 0;\
            __asm__ __volatile__("repeat #38");\
            Nop();\
        } while(0)

#define SendLatch(pin) do {\
            pin = 0;\
            __asm__ volatile ("repeat #3264");\
            Nop();\
        } while(0)

#define RGBCommon(red, green, blue, pin) do {\
            uint8_t data[] = {green, red, blue};\
            uint8_t location;\
            \
            SendLatch(pin);\
            \
            for (location = 0; location < 3; location++) {\
                uint8_t bit;\
                bit = data[location];\
                uint8_t byte;\
                for (byte = 0; byte < 8; byte++) {\
                    if (bit & 0x80) {\
                        SendOne(pin);\
                    } else {\
                        SendZero(pin);\
                    }\
                    bit = bit << 1;\
                }\
            }\
        } while(0)

typedef enum {
    ONBOARD,
    SQR1,
    SQR2,
    SQR3,
    SQR4,
} PINSELECT;

void LIGHT_RGB(uint8_t red, uint8_t green, uint8_t blue) {
    RGBCommon(red, green, blue, RGB_LED_Setter);
}

enum Status LIGHT_rgb_pin(
    uint8_t **args,
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    struct Input {
        uint8_t pin;
        uint8_t count;
        uint8_t colors[];
    };

    if (args_size < sizeof(struct Input)) { return E_BAD_ARGSIZE; }

    struct Input *input = (struct Input *)args;

    if (args_size != (sizeof(input) + input->count)) { return E_BAD_ARGSIZE; }

    uint8_t const *const colors = input->colors;

    INTERRUPT_GlobalDisable();

    for (uint8_t i = 0; i < input->count; i = i + 3) {
        switch (input->pin) {
        case ONBOARD:
            RGBCommon(colors[i+1], colors[i], colors[i+2], RGB_LED_Setter);
            break;
        case SQR1:
            RGBCommon(colors[i+1], colors[i], colors[i+2], SQR1_Setter);
            break;
        case SQR2:
            RGBCommon(colors[i+1], colors[i], colors[i+2], SQR2_Setter);
            break;
        case SQR3:
            RGBCommon(colors[i+1], colors[i], colors[i+2], SQR3_Setter);
            break;
        case SQR4:
            RGBCommon(colors[i+1], colors[i], colors[i+2], SQR4_Setter);
            break;
        default:
            break;
        }
    }

    INTERRUPT_GlobalEnable();

    return E_OK;
}
