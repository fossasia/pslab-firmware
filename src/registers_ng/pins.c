#include <stdint.h>

void PINS_initialize(void)
{
    extern struct {
        uint16_t IC1R :7;
        uint16_t :1;
        uint16_t IC2R :7;
    } volatile RPINR7bits;

    extern struct {
        uint16_t IC3R :7;
        uint16_t :1;
        uint16_t IC4R :7;
    } volatile RPINR8bits;

    // The LA pins (8-11 in the pin diagram) map to the values 42-45 in the
    // remappable peripheral numbering scheme.
    enum LAPins {
        LA1 = 42,
        LA2,
        LA3,
        LA4,
    };

    RPINR7bits.IC1R = LA1;
    RPINR7bits.IC2R = LA2;
    RPINR8bits.IC3R = LA3;
    RPINR8bits.IC4R = LA4;
}

uint8_t PINS_get_la_states(void)
{
    extern uint16_t const volatile PORTB;
    // The LA pins (8-11 in the pin diagram) map to bits 10-13 in the PORTB
    // register.
    enum PORTBPinNum {
        LA1 = 10,
        LA2,
        LA3,
        LA4,
    };
    return (PORTB >> (uint8_t)LA1) & 0xF;
}
