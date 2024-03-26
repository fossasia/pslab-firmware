#ifndef PSLAB_PINS_H
#define PSLAB_PINS_H

#include <stdint.h>
#include <xc.h> // REMOVEME

/**
 * @brief Map pins to peripheral inputs
 */
void PINS_initialize(void);

/**
 * @brief Get the states of the LA1-4 pins
 *
 * @return uint8_t
 * The low nibble of this byte holds the states of LA1-4.
 */
static inline uint8_t PINS_get_la_states(void)
{
    // TODO: Uncomment when system.c no longer includes xc.h.
    // extern uint16_t const volatile PORTB;
    enum PORTBPinNum {
        LA1 = 10,
        LA2,
        LA3,
        LA4,
    };
    return (PORTB >> (uint8_t)LA1) & 0xF;
}

#endif // PSLAB_PINS_H
