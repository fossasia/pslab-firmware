/**
 * @file cn.c
 * @author Alexander Bessman (alexander.bessman@gmail.com)
 * @brief Input Change Notification (CN) driver
 *
 * CN generates an interrupt when the state on an input pin changes.
 *
 * CN is supported for the logic analyzer input pins (LA1-4).
 *
 * For hardware specifics, refer to Microchip document DS70000598,
 * "I/O Ports".
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "hardware.h"
#include "types.h"

#include "cn.h"

/**********/
/* Macros */
/**********/

#define PINS_REG CNENB
#define INTERRUPT_CTRL_REG IEC1
#define INTERRUPT_FLAG_REG IFS1
#define CTRL_BIT IEC1_BITS_CNIE
#define FLAG_BIT IFS1_BITS_CNIF

/*********************/
/* Static Prototypes */
/*********************/

/**
 * @brief Check whether CN is in use
 *
 * @return true
 * @return false
 */
static inline bool is_busy(void);

/**
 * @brief Enable CN interrupt
 */
static void interrupt_enable(void);

/**
 * @brief Disable CN interrupt
 */
static void interrupt_disable(void);

/**
 * @brief Clear CN interrupt flag
 */
static void interrupt_clear(void);

/**
 * @brief Enable CN on pin
 *
 * @param pin
 */
static void pin_enable(enum CNENB_Bits pin);

/**
 * @brief Disable CN on pin
 *
 * @param pin
 */
static void pin_disable(enum CNENB_Bits pin);

/**
 * @brief Default interrupt callback
 *
 * Disables the CN interrupt.
 *
 * @param channel
 * Unused
 */
static void default_callback(Channel channel);

/*************/
/* Constants */
/*************/

static enum CNENB_Bits const g_CHANNEL_PIN_MAP[CHANNEL_NUMEL] = {
    [CHANNEL_1] = CNENB_BITS_LA1,
    [CHANNEL_2] = CNENB_BITS_LA2,
    [CHANNEL_3] = CNENB_BITS_LA3,
    [CHANNEL_4] = CNENB_BITS_LA4,
};

/***********/
/* Globals */
/***********/

static InterruptCallback g_callback = default_callback;

/**************/
/* Interrupts */
/**************/

/**
 * @brief Input Change Notification (CN) interrupt
 *
 * Interrupt occurs when the logic level on an enabled pin changes.
 *
 * Hardware only supports a single interrupt for all enabled pins.
 */

#ifndef LINTING

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void)
{
    interrupt_clear();
    // Any of the enabled pins may have triggered the interrupt. The hardware
    // provides no way to tell which one.
    g_callback(CHANNEL_NONE);
}

#endif // LINTING

/********************/
/* Static Functions */
/********************/

static inline bool is_busy(void) {
    return g_callback != default_callback;
}

static inline void interrupt_enable(void)
{
    INTERRUPT_CTRL_REG |= CTRL_BIT;
}

static inline void interrupt_disable(void)
{
    INTERRUPT_CTRL_REG &= ~CTRL_BIT;
}

static inline void interrupt_clear(void)
{
    INTERRUPT_FLAG_REG &= ~FLAG_BIT;
}

static inline void pin_enable(enum CNENB_Bits const pin)
{
    PINS_REG |= pin;
}

static inline void pin_disable(enum CNENB_Bits const pin)
{
    PINS_REG &= ~pin;
}

static void default_callback(__attribute__((unused)) Channel const channel)
{
    interrupt_disable();
}

/********************/
/* Public Functions */
/********************/

enum Status CN_reset(void)
{
    interrupt_disable();
    interrupt_clear();
    uint16_t const valid_pins = (
        CNENB_BITS_LA1 | CNENB_BITS_LA2 | CNENB_BITS_LA3 | CNENB_BITS_LA4
    );
    PINS_REG &= ~valid_pins;
    g_callback = default_callback;
    return E_OK;
}

enum Status CN_pin_enable(Channel const channel)
{
    if (!CHANNEL_check(channel)) { return E_BAD_ARGUMENT; }
    pin_enable(g_CHANNEL_PIN_MAP[channel]);
    return E_OK;
}

enum Status CN_pin_disable(Channel const channel)
{
    if (!CHANNEL_check(channel)) { return E_BAD_ARGUMENT; }
    pin_disable(g_CHANNEL_PIN_MAP[channel]);
    return E_OK;
}

enum Status CN_interrupt_enable(InterruptCallback const callback)
{
    if (is_busy()) { return E_RESOURCE_BUSY; }

    g_callback = callback;
    interrupt_enable();
    return E_OK;
}

enum Status CN_interrupt_disable(void)
{
    interrupt_disable();
    interrupt_clear();
    g_callback = default_callback;
    return E_OK;
}
