#ifndef CN_H
#define CN_H

#include <stdint.h>

#include "types.h"

/*************/
/* Functions */
/*************/

/**
 * @brief Reset CN to default configuration
 *
 * @return enum Status
 */
enum Status CN_reset(void);

/**
 * @brief Enable CN on pin
 *
 * This function may be called multiple times without intermediate reset.
 *
 * @param pin
 *
 * @return enum Status
 */
enum Status CN_pin_enable(Channel pin);

/**
 * @brief Disable CN on pin
 *
 * This function may be called multiple times without intermediate reset.
 *
 * @param pin
 *
 * @return enum Status
 */
enum Status CN_pin_disable(Channel pin);

/**
 * @brief Call @c callback when a logic level change occurs on a configured pin
 *
 * CN provides a single interrupt for all enabled pins. There is no indication
 * of which pin triggered the interrupt. The callback's @c channel argument
 * should therefore not be used.
 *
 * Consider using Input Capture (IC) interrupts instead if it is necessary to
 * know which pin triggered the interrupt.
 *
 * To change interrupt callback, first call CN_disable_interrupt before calling
 * this function again.
 *
 * @param callback
 *   Function which is called when CN interrupt occurs.
 *
 * @return enum Status
 */
enum Status CN_interrupt_enable(InterruptCallback callback);

/**
 * @brief Disable CN interrupt
 *
 * @return enum Status
 */
enum Status CN_interrupt_disable(void);

#endif // CN_H
