/**
 * @file cn.c
 * @author Alexander Bessman (alexander.bessman@gmail.com)
 * @brief Input Change Notification (CN) driver
 *
 * CN generates an interrupt when the state on an input pin changes.
 *
 * For hardware specifics, refer to Microchip document DS70000598,
 * "I/O Ports".
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "types.h"

#include "cn.h"

/*********/
/* Types */
/*********/

/**
 * @brief Aggregate of configuration values
 */
struct TriggerConf {
    Channel pin;
    InterruptCallback callback;
};

struct CNInterruptControlRegister {
    uint16_t :3;
    uint16_t CN :1;
    uint16_t :12;
};

/*********************/
/* Static Prototypes */
/*********************/

/**
 * @brief Enable or disable CN interrupt
 *
 * @param ctrl
 */
static void interrupt_enable(bool ctrl);

/**
 * @brief Clear CN interrupt flag
 */
static void interrupt_clear(void);

/**
 * @brief Default interrupt callback
 * @details
 * Disables the CN interrupt.
 *
 * @param channel
 * Unused
 */
static void default_callback(Channel channel);

/***********/
/* Externs */
/***********/

extern union {
    struct {
        uint16_t :10;
        uint16_t all :4;
        uint16_t :2;
    };
    struct {
        uint16_t :10;
        uint16_t la1 :1;
        uint16_t la2 :1;
        uint16_t la3 :1;
        uint16_t la4 :1;
        uint16_t :2;
    };
} volatile CNENB;

/***********/
/* Globals */
/***********/

static struct TriggerConf g_conf = {
    .callback = default_callback,
    .pin = CHANNEL_NONE,
};

/**************/
/* Interrupts */
/**************/

/**
 * @brief Input Change Notification (CN) interrupt
 * @details
 * Interrupt occurs when the logic level on the configured trigger pin changes.
 */

#ifndef LINTING

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void)
{
    g_conf.callback(CHANNEL_NONE);
    interrupt_clear();
}

#endif // LINTING

/********************/
/* Static Functions */
/********************/

static void interrupt_enable(bool const ctrl)
{
    extern struct CNInterruptControlRegister volatile IEC1;
    IEC1.CN = ctrl;
}

static void interrupt_clear(void)
{
    extern struct CNInterruptControlRegister volatile IFS1;
    IFS1.CN = 0;
}

static void default_callback(__attribute__((unused)) Channel const channel)
{
    interrupt_enable(false);
}

/********************/
/* Public Functions */
/********************/

void CN_reset(void)
{
    interrupt_enable(false);
    interrupt_clear();
    g_conf.callback = default_callback;
    g_conf.pin = CHANNEL_NONE;
}

void CN_interrupt_enable(Channel const pin, InterruptCallback const callback)
{
    g_conf.pin = pin;
    g_conf.callback = callback;

    switch (pin) {
    case CHANNEL_1:
        CNENB.la1 = 1;
        break;
    case CHANNEL_2:
        CNENB.la2 = 1;
        break;
    case CHANNEL_3:
        CNENB.la3 = 1;
        break;
    case CHANNEL_4:
        CNENB.la4 = 1;
        break;
    default: // No trigger pin == start immediately.
        callback(CHANNEL_NONE);
        return;
    }

    interrupt_enable(true);
}
