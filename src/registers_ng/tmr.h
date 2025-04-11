#ifndef TMR_H
#define TMR_H

#include <stdint.h>

#include "types.h"

#define TMR_PRESCALER_MAX TMR_TCKPS_256

/*********/
/* Types */
/*********/

typedef enum TMR_Timer {
    TMR_TIMER_NONE,
    TMR_TIMER_1,
    TMR_TIMER_2,
    TMR_TIMER_3,
    TMR_TIMER_4,
    TMR_TIMER_5,
    TMR_N_TIMERS,
} TMR_Timer;

enum TMR_Prescaler {
    TMR_TCKPS_1,
    TMR_TCKPS_8,
    TMR_TCKPS_64,
    TMR_TCKPS_256,
    TMR_N_PRESCALERS
};

static uint16_t const TMR_prescaler_vals[TMR_N_PRESCALERS] = {
    [TMR_TCKPS_1] = 1,
    [TMR_TCKPS_8] = 8,
    [TMR_TCKPS_64] = 64,
    [TMR_TCKPS_256] = 256,
};

/*************/
/* Functions */
/*************/

/**
 * @brief Reset timer to default configuration
 *
 * @param tmr
 */
enum Status TMR_reset(TMR_Timer tmr);

/**
 * @brief Set timer period and prescaler.
 *
 * @param tmr
 *   TMR_Timer object.
 *
 * @param period
 *   Timer period in ns.
 *
 * @return enum Status
 */

 enum Status TMR_get_period_prescaler(
    uint32_t ns,
    uint16_t *pr,
    enum TMR_Prescaler *tckps
);

enum Status TMR_set_period(TMR_Timer tmr, uint16_t pr);

enum Status TMR_set_prescaler(TMR_Timer tmr, enum TMR_Prescaler tckps);

/**
 * @brief Start timer
 *
 * @param tmr
 */
enum Status TMR_start(TMR_Timer timer);

#endif // TMR_H
