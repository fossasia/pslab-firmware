#ifndef CN_H
#define CN_H

#include <stdint.h>

#include "types.h"

/*************/
/* Functions */
/*************/

/**
 * @brief Reset CN to default configuration
 */
void CN_reset(void);

/**
 * @brief Call `callback` when a logic level change occurs on `pin`.
 *
 * @param pin
 * One of CN_PIN_LA1-4, or CN_PIN_NONE, on which to check for the trigger edge.
 * @param callback
 * Function which is called when the trigger condition is met.
 */
void CN_interrupt_enable(Channel pin, InterruptCallback callback);

#endif // CN_H
