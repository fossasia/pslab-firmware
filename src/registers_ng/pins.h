#ifndef PSLAB_PINS_H
#define PSLAB_PINS_H

#include <stdint.h>

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
uint8_t PINS_get_la_states(void);

#endif // PSLAB_PINS_H
