#ifndef PSLAB_BLINK_H
#define PSLAB_BLINK_H

#include <stdint.h>

#include "../commands.h"

/**
 * @brief Blink the onboard RGB LED.
 *
 * @param period  uint16_t, blink period in milliseconds
 * @param red     uint8_t, intensity of red channel
 * @param green   uint8_t, intensity of green channel
 * @param blue    uint8_t, intensity of blue channel
 *
 * @return enum Status
 */
enum Status BLINK_start(
    uint8_t args[],
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Stop blinking.
 *
 * @return enum Status
 */
enum Status BLINK_stop(
    uint8_t args[],
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

#endif
