#ifndef TMR_H
#define TMR_H

#include <stdint.h>

/*********/
/* Types */
/*********/

typedef enum TMR_Timer {
    TMR_TIMER_1,
    TMR_TIMER_2,
    TMR_TIMER_3,
    TMR_TIMER_4,
    TMR_TIMER_5,
    TMR_TIMER_NUMEL,
} TMR_Timer;

/*************/
/* Functions */
/*************/

/**
 * @brief Reset timer to default configuration
 *
 * @param timer
 */
void TMR_reset(TMR_Timer timer);

/**
 * @brief Start timer
 *
 * @param timer
 */
void TMR_start(TMR_Timer timer);

/**
 * @brief Set timer period
 * @details
 * Timer sync output is asserted high on period match, and a timer interrupt is
 * generated if enabled.
 *
 * @param timer
 * @param period
 */
void TMR_set_period(TMR_Timer timer, uint16_t period);

#endif // TMR_H
