#ifndef IC_H
#define IC_H

#include <stdint.h>

#include "types.h"

/*********/
/* Types */
/*********/

typedef enum IC_Timer {
    IC_TIMER_TMR5 = 0b011,
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

/**
 * @brief Function to call when an IC interrupt is generated
 */
typedef void (*IC_InterruptCallback)(Channel channel);

/*************/
/* Functions */
/*************/

/**
 * @brief Reset IC channel to default settings
 *
 * @param channel
 */
void IC_reset(Channel channel);

/**
 * @brief Configure IC capture
 * @details
 * `timer` is used both to drive and synchronize ICxTMR, i.e. ICxTMR is
 * incremented by the `timer` clock, and ICxTMR is reset when `timer` sync
 * output is asserted.
 *
 * @param channel
 * @param timer
 */
void IC_setup(Channel channel, IC_Timer timer);

/**
 * @brief Start input capture
 *
 * @details
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
void IC_start(Channel channel, Edge edge);

/**
 * @brief Start capturing edges of `edge` type on IC1
 * @details
 * Prefer `IC_start` over `ICx_start_fast`. Use the latter only in
 * timing-critical sections.
 *
 * @param edge
 */
static inline void IC1_start_fast(Edge const edge)
{
    extern uint16_t volatile IC1CON1;
    uint16_t const icm = edge + 1;
    uint16_t const icm_mask = ~7;
    IC1CON1 = (IC1CON1 & icm_mask) | icm;
}

/**
 * @brief Start capturing edges of `edge` type on IC2
 *
 * @param edge
 */
static inline void IC2_start_fast(Edge const edge)
{
    extern uint16_t volatile IC2CON1;
    uint16_t const icm = edge + 1;
    uint16_t const icm_mask = ~7;
    IC2CON1 = (IC2CON1 & icm_mask) | icm;
}

/**
 * @brief Start capturing edges of `edge` type on IC3
 *
 * @param edge
 */
static inline void IC3_start_fast(Edge const edge)
{
    extern uint16_t volatile IC3CON1;
    uint16_t const icm = edge + 1;
    uint16_t const icm_mask = ~7;
    IC3CON1 = (IC3CON1 & icm_mask) | icm;
}

/**
 * @brief Start capturing edges of `edge` type on IC4
 *
 * @param edge
 */
static inline void IC4_start_fast(Edge const edge)
{
    extern uint16_t volatile IC4CON1;
    uint16_t const icm = edge + 1;
    uint16_t const icm_mask = ~7;
    IC4CON1 = (IC4CON1 & icm_mask) | icm;
}

/**
 * @brief Enable Input Capture interrupt on channel
 * @details
 * The trigger edge type is set via `IC_start`, and cannot be ANY. If it is
 * necessary to interrupt on ANY edge, use the CN module instead.
 *
 * @param channel
 * @param callback
 */
void IC_interrupt_enable(Channel channel, InterruptCallback callback);

/**
 * @brief Disable Input Capture interrupt on channel
 *
 * @param channel
 */
void IC_interrupt_disable(Channel channel);

#endif /* IC_H */
