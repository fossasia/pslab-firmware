/**
 * @file tmr.c
 * @author Alexander Bessman (alexander.bessman@gmail.com)
 * @brief Timer (TMR) driver
 *
 * Timers can be used as inputs or drivers for many other modules.
 *
 * For hardware specifics, see Microchip document S11, "Timers".
 */

#include <stddef.h>
#include <stdint.h>

#include "types.h"
#include "../helpers/delay.h"

#include "tmr.h"

/*********/
/* Types */
/*********/

struct TMRCONbits {
    uint16_t :1;
    uint16_t TCS :1;
    uint16_t TSYNC :1; // Only in TMR1
    uint16_t T32 :1;   // Only in TMR2 & TMR4
    uint16_t TCKPS :2;
    uint16_t TGATE :1;
    uint16_t :6;
    uint16_t TSIDL :1;
    uint16_t :1;
    uint16_t TON :1;
};

struct TMRRegisters {
    struct TMRCONbits volatile *const p_conbits;
    uint16_t volatile *const p_pr;
    uint16_t volatile *const p_tmr;
    uint16_t volatile *const p_tmrhld;
};

/*********************/
/* Static Prototypes */
/*********************/

/***********/
/* Externs */
/***********/

extern struct TMRCONbits volatile T1CONbits;
extern struct TMRCONbits volatile T2CONbits;
extern struct TMRCONbits volatile T3CONbits;
extern struct TMRCONbits volatile T4CONbits;
extern struct TMRCONbits volatile T5CONbits;

extern uint16_t volatile PR1;
extern uint16_t volatile PR2;
extern uint16_t volatile PR3;
extern uint16_t volatile PR4;
extern uint16_t volatile PR5;

extern uint16_t volatile TMR1;
extern uint16_t volatile TMR2;
extern uint16_t volatile TMR3;
extern uint16_t volatile TMR4;
extern uint16_t volatile TMR5;

extern uint16_t volatile TMR3HLD;
extern uint16_t volatile TMR5HLD;

/*************/
/* Constants */
/*************/

static struct TMRRegisters const g_TMR_REGS[TMR_N_TIMERS] = {
    [TMR_TIMER_1] = {
        .p_conbits = &T1CONbits,
        .p_pr = &PR1,
        .p_tmr = &TMR1,
        .p_tmrhld = NULL,
    },
    [TMR_TIMER_2] = {
        .p_conbits = &T2CONbits,
        .p_pr = &PR2,
        .p_tmr = &TMR2,
        .p_tmrhld = NULL,
    },
    [TMR_TIMER_3] = {
        .p_conbits = &T3CONbits,
        .p_pr = &PR3,
        .p_tmr = &TMR3,
        .p_tmrhld = &TMR3HLD,
    },
    [TMR_TIMER_4] = {
        .p_conbits = &T4CONbits,
        .p_pr = &PR4,
        .p_tmr = &TMR4,
        .p_tmrhld = NULL,
    },
    [TMR_TIMER_5] = {
        .p_conbits = &T5CONbits,
        .p_pr = &PR5,
        .p_tmr = &TMR5,
        .p_tmrhld = &TMR5HLD,
    }
};

/********************/
/* Static Functions */
/********************/

static inline enum Status check_timer(TMR_Timer const tmr)
{
    return (
        tmr == TMR_TIMER_NONE || tmr >= TMR_N_TIMERS
        ? E_BAD_ARGUMENT
        : E_OK
    );
}

static inline void set_period(TMR_Timer const timer, uint16_t const period)
{
    *g_TMR_REGS[timer].p_pr = period;
}

static inline void set_prescaler(
    TMR_Timer const timer,
    enum TMR_Prescaler const prescaler
) {
    g_TMR_REGS[timer].p_conbits->TCKPS = prescaler;
}


/********************/
/* Public Functions */
/********************/

enum Status TMR_reset(TMR_Timer const tmr)
{
    enum Status status = E_OK;

    status = check_timer(tmr);
    if (status) { return status; }

    struct TMRConf {
        struct TMRCONbits conbits;
        uint16_t pr;
        uint16_t tmr;
        uint16_t tmrhld;
    } const tmr_conf_default = {
        // clang-format off
        .conbits = { .TCS = 0 },
        // clang-format on
        .pr = 0,
        .tmr = 0,
        .tmrhld = 0,
    };
    struct TMRRegisters const *const regs = &g_TMR_REGS[tmr];
    *regs->p_conbits = tmr_conf_default.conbits;
    *regs->p_pr = tmr_conf_default.pr;
    *regs->p_tmr = tmr_conf_default.tmr;

    if (regs->p_tmrhld) { // p_tmrhld is NULL for TMR{1,2,4}.
        *regs->p_tmrhld = tmr_conf_default.tmrhld;
    }

    return status;
}

enum Status TMR_get_period_prescaler(
    uint32_t const ns,
    uint16_t *const pr,
    enum TMR_Prescaler *const tckps
) {
    enum { _1E9 = 1000000000UL };
    uint32_t const ns_max = (
        UINT16_MAX * TMR_prescaler_vals[TMR_PRESCALER_MAX] * _1E9 / FCY - 1
    );
    if (ns > ns_max) { return E_BAD_ARGUMENT; }

    for (enum TMR_Prescaler i = TMR_TCKPS_1; i < TMR_N_PRESCALERS; ++i) {
        // Every prescaler divides FCY; effective_clock is not truncated.
        uint64_t const effective_clock = FCY / TMR_prescaler_vals[i];
        // Round to nearest integer period value.
        uint64_t const tmp = (effective_clock * ns + _1E9 / 2) / _1E9;
        if (tmp > UINT16_MAX) { continue; }

        *pr = tmp;
        *tckps = i;
        return E_OK;
    }

    // This should be unreachable.
    return E_FAILED;
}

enum Status TMR_set_prescaler(
    TMR_Timer const tmr,
    enum TMR_Prescaler const tckps
) {
    enum Status status = E_OK;
    status = check_timer(tmr);
    if (status) { return status; }

    if (tckps >= TMR_N_PRESCALERS) { return E_BAD_ARGUMENT; }

    set_prescaler(tmr, tckps);
    return status;
}

enum Status TMR_set_period(TMR_Timer const tmr, uint16_t const pr)
{
    enum Status status = E_OK;
    status = check_timer(tmr);
    if (status) { return status; }

    set_period(tmr, pr);
    return status;
}


enum Status TMR_start(TMR_Timer const tmr)
{
    enum Status status = E_OK;
    status = check_timer(tmr);
    if (status) { return status; }

    g_TMR_REGS[tmr].p_conbits->TON = 1;
    return status;
}
