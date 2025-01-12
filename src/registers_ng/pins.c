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
