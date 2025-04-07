#ifndef IC_H
#define IC_H

#include <stdint.h>

#include "types.h"

/*********/
/* Types */
/*********/

typedef enum IC_Timer {
    IC_TIMER_TMR1 = 0b100,
    IC_TIMER_PERIPHERAL = 0b111,
} IC_Timer;

typedef enum IC_Task {
    IC_TASK_SYNC = 0b00,
    IC_TASK_TRIGGER = 0b01,
} IC_Task;

typedef enum IC_TriggerStatus {
    IC_TRIGGER_CLEAR = 0b00,
    IC_TRIGGER_RUN = 0b01,
} IC_TriggerStatus;

/*************/
/* Functions */
/*************/

/**
 * @brief Reset IC channel to default settings.
 *
 * @param channel
 */
enum Status IC_reset(Channel channel);

/**
 * @brief Start input capture
 *
 * @details
 * `timer` is used both to drive and synchronize ICxTMR, i.e. ICxTMR is
 * incremented by the `timer` clock, and ICxTMR is reset when `timer` sync
 * output is asserted.
 *
 * Whenever a logic level change of type `edge` occurs, the value of ICxTMR is
 * copied to ICxBUF.
 *
 * NB: Capture starts immediately, but ICxTMR is not started until triggered by
 * the sync output of `timer`. Until then, a value of zero is copied to ICxBUF
 * whenever an edge is detected.
 *
 * @param channel
 * @param edge
 * @param timer
 */
enum Status IC_start(Channel channel, Edge edge, IC_Timer timer);

/**
 * @brief Enable Input Capture interrupt on channel
 * @details
 * The trigger edge type is set via `IC_start`, and cannot be ANY. If it is
 * necessary to interrupt on ANY edge, use the CN module instead.
 *
 * @param channel
 * @param callback
 */
enum Status IC_interrupt_enable(Channel channel, InterruptCallback callback);

/**
 * @brief Disable Input Capture interrupt on channel
 *
 * @param channel
 */
enum Status IC_interrupt_disable(Channel channel);

#endif /* IC_H */
