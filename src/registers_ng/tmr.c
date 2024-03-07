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

#include "tmr.h"

/*********/
/* Types */
/*********/

struct TMRxCONbits {
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

struct TMRxRegisters {
    struct TMRxCONbits volatile *const p_conbits;
    uint16_t volatile *const p_pr;
    uint16_t volatile *const p_tmr;
    uint16_t volatile *const p_tmrhld;
};

/*********************/
/* Static Prototypes */
/*********************/

/**
 * @brief Get an aggregate of TMR register pointers.
 *
 * @param timer
 * @return struct TMRxRegisters
 */
static struct TMRxRegisters get_registers(TMR_Timer timer);

/***********/
/* Externs */
/***********/

extern struct TMRxCONbits volatile T1CONbits;
extern struct TMRxCONbits volatile T2CONbits;
extern struct TMRxCONbits volatile T3CONbits;
extern struct TMRxCONbits volatile T4CONbits;
extern struct TMRxCONbits volatile T5CONbits;

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

static struct TMRxRegisters const g_TMR1_REGS = {
    .p_conbits = &T1CONbits,
    .p_pr = &PR1,
    .p_tmr = &TMR1,
    .p_tmrhld = NULL,
};
static struct TMRxRegisters const g_TMR2_REGS = {
    .p_conbits = &T2CONbits,
    .p_pr = &PR2,
    .p_tmr = &TMR2,
    .p_tmrhld = NULL,
};
static struct TMRxRegisters const g_TMR3_REGS = {
    .p_conbits = &T3CONbits,
    .p_pr = &PR3,
    .p_tmr = &TMR3,
    .p_tmrhld = &TMR3HLD,
};
static struct TMRxRegisters const g_TMR4_REGS = {
    .p_conbits = &T4CONbits,
    .p_pr = &PR4,
    .p_tmr = &TMR4,
    .p_tmrhld = NULL,
};
static struct TMRxRegisters const g_TMR5_REGS = {
    .p_conbits = &T5CONbits,
    .p_pr = &PR5,
    .p_tmr = &TMR5,
    .p_tmrhld = &TMR5HLD,
};

/********************/
/* Static Functions */
/********************/

static struct TMRxRegisters get_registers(TMR_Timer const timer)
{
    struct TMRxRegisters const regs[TMR_TIMER_NUMEL] = {
        [TMR_TIMER_1] = g_TMR1_REGS,
        [TMR_TIMER_2] = g_TMR2_REGS,
        [TMR_TIMER_3] = g_TMR3_REGS,
        [TMR_TIMER_4] = g_TMR4_REGS,
        [TMR_TIMER_5] = g_TMR5_REGS,
    };
    return regs[timer];
}

/********************/
/* Public Functions */
/********************/

void TMR_reset(TMR_Timer const timer)
{
    struct TMRConf {
        struct TMRxCONbits conbits;
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
    struct TMRxRegisters regs = get_registers(timer);
    *regs.p_conbits = tmr_conf_default.conbits;
    *regs.p_pr = tmr_conf_default.pr;
    *regs.p_tmr = tmr_conf_default.tmr;

    if (regs.p_tmrhld) { // p_tmrhld is NULL for TMR{1,2,4}.
        *regs.p_tmrhld = tmr_conf_default.tmrhld;
    }
}

void TMR_start(TMR_Timer const timer)
{
    get_registers(timer).p_conbits->TON = 1;
}

void TMR_set_period(TMR_Timer const timer, uint16_t const period)
{
    *get_registers(timer).p_pr = period;
}
