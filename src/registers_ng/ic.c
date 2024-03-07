/**
 * @file ic.c
 * @author Alexander Bessman (alexander.bessman@gmail.com)
 * @brief Input Capture (IC) driver
 *
 * IC is used to capture clock values in response to events. When an event is
 * detected on ICx, the value of ICx's clock is copied to ICxBUF.
 *
 * An event can be a falling edge, a rising edge, or either one.
 *
 * For hardware specifics, refer to Microchip document 70000352D,
 * "Input Capture with Dedicated Timer".
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "types.h"

#include "ic.h"

/*********/
/* Types */
/*********/

/**
 * @brief Accessor for individual bits of the ICxCON1 register
 */
struct ICxCON1Bits {
    uint16_t ICM :3;
    uint16_t ICBNE :1;
    uint16_t ICOV :1; // Read-only.
    uint16_t ICI :2;  // Read-only.
    uint16_t :3;
    uint16_t ICTSEL :3;
    uint16_t ICSIDL :1;
};

/**
 * @brief Accessor for individual bits of the ICxCON2 register
 */
struct ICxCON2Bits {
    uint16_t SYNCSEL :5;
    uint16_t :1;
    uint16_t TRIGSTAT :1;
    uint16_t ICTRIG :1;
    uint16_t IC32 :1;
};

/**
 * @brief Aggregate of pointers to IC channel registers
 */
struct ICxRegisters {
    struct ICxCON1Bits volatile *const con1bits_p;
    struct ICxCON2Bits volatile *const con2bits_p;
};

/**
 * @brief IC capture mode
 */
enum ICM {
    ICM_NONE = 0b00,
    ICM_ANY = 0b01,
    ICM_FALLING = 0b10,
    ICM_RISING = 0b11,
};

enum SyncSel {
    SYNCSEL_NONE = 0b00000,
    SYNCSEL_TMR1 = 0b01011,
};

struct IC1IC2InterruptControlRegister {
    uint16_t :1;
    uint16_t IC1 :1;
    uint16_t :3;
    uint16_t IC2 :1;
};

struct IC3IC4InterruptControlRegister {
    uint16_t :5;
    uint16_t IC3 :1;
    uint16_t IC4 :1;
};

/*********************/
/* Static Prototypes */
/*********************/

/**
 * @brief Convert value of `Edge` type to the corresponding ICM value
 *
 * @param edge
 * @return enum ICM
 */
static enum ICM edge2icm(Edge edge);

/**
 * @brief Convert an ICTSEL source to the corresponding SYNCSEL source
 * @details
 * ICTSEL is a clock which increments ICxTMR. SYNCSEL synchronizes ICxTMR by
 * resetting its value to zero when the source output is high.
 *
 * @param timer
 * @return enum SyncSel
 */
static enum SyncSel ictsel2syncsel(IC_Timer timer);

/**
 * @brief
 *
 */
static void default_callback(Channel channel);

/**
 * @brief Get register pointer aggregate for IC channel
 */
static struct ICxRegisters get_registers(Channel channel);

static void interrupt_enable(Channel channel, bool ctrl);

static void interrupt_clear(Channel channel);

/***********/
/* Externs */
/***********/

extern struct ICxCON1Bits volatile IC1CON1bits;
extern struct ICxCON1Bits volatile IC2CON1bits;
extern struct ICxCON1Bits volatile IC3CON1bits;
extern struct ICxCON1Bits volatile IC4CON1bits;

extern struct ICxCON2Bits volatile IC1CON2bits;
extern struct ICxCON2Bits volatile IC2CON2bits;
extern struct ICxCON2Bits volatile IC3CON2bits;
extern struct ICxCON2Bits volatile IC4CON2bits;

/*************/
/* Constants */
/*************/

/**
 * @brief Aggregate of IC1 register pointers
 */
static struct ICxRegisters const g_IC1_REGS = {
    .con1bits_p = &IC1CON1bits,
    .con2bits_p = &IC1CON2bits,
};

/**
 * @brief Aggregate of IC2 register pointers
 */
static struct ICxRegisters const g_IC2_REGS = {
    .con1bits_p = &IC2CON1bits,
    .con2bits_p = &IC2CON2bits,
};

/**
 * @brief Aggregate of IC3 register pointers
 */
static struct ICxRegisters const g_IC3_REGS = {
    .con1bits_p = &IC3CON1bits,
    .con2bits_p = &IC3CON2bits,
};

/**
 * @brief Aggregate of IC4 register pointers
 */
static struct ICxRegisters const g_IC4_REGS = {
    .con1bits_p = &IC4CON1bits,
    .con2bits_p = &IC4CON2bits,
};

/***********/
/* Globals */
/***********/

static IC_InterruptCallback g_callbacks[CHANNEL_NUMEL] = {
    [CHANNEL_1] = default_callback,
    [CHANNEL_2] = default_callback,
    [CHANNEL_3] = default_callback,
    [CHANNEL_4] = default_callback,
};

/**************/
/* Interrupts */
/**************/

#ifndef LINTING
void __attribute__((__interrupt__, no_auto_psv)) _IC1Interrupt(void)
{
    g_callbacks[CHANNEL_1](CHANNEL_1);
    interrupt_clear(CHANNEL_1);
}

void __attribute__((__interrupt__, no_auto_psv)) _IC2Interrupt(void)
{
    g_callbacks[CHANNEL_2](CHANNEL_2);
    interrupt_clear(CHANNEL_2);
}

void __attribute__((__interrupt__, no_auto_psv)) _IC3Interrupt(void)
{
    g_callbacks[CHANNEL_3](CHANNEL_3);
    interrupt_clear(CHANNEL_3);
}

void __attribute__((__interrupt__, no_auto_psv)) _IC4Interrupt(void)
{
    g_callbacks[CHANNEL_4](CHANNEL_4);
    interrupt_clear(CHANNEL_4);
}
#endif // LINTING

/********************/
/* Static Functions */
/********************/

static enum ICM edge2icm(Edge const edge) { return edge + 1; }

static enum SyncSel ictsel2syncsel(IC_Timer const timer)
{
    switch (timer) {
    case IC_TIMER_TMR1:
        return SYNCSEL_TMR1;
    // Add more as necessary.
    default:
        return SYNCSEL_NONE;
    }
}

static void default_callback(Channel const channel)
{
    interrupt_enable(channel, false);
    interrupt_clear(channel);
}

static struct ICxRegisters get_registers(Channel const channel)
{
    struct ICxRegisters const regs[CHANNEL_NUMEL] = {
        [CHANNEL_1] = g_IC1_REGS,
        [CHANNEL_2] = g_IC2_REGS,
        [CHANNEL_3] = g_IC3_REGS,
        [CHANNEL_4] = g_IC4_REGS,
    };
    return regs[channel];
}

static void interrupt_enable(Channel const channel, bool const ctrl)
{
    extern struct IC1IC2InterruptControlRegister volatile IEC0;
    extern struct IC3IC4InterruptControlRegister volatile IEC2;

    switch (channel) {
    case CHANNEL_1:
        IEC0.IC1 = ctrl;
        return;
    case CHANNEL_2:
        IEC0.IC2 = ctrl;
        return;
    case CHANNEL_3:
        IEC2.IC3 = ctrl;
        return;
    case CHANNEL_4:
        IEC2.IC4 = ctrl;
        return;
    default:
        return;
    }
}

static void interrupt_clear(Channel const channel)
{
    extern struct IC1IC2InterruptControlRegister volatile IFS0;
    extern struct IC3IC4InterruptControlRegister volatile IFS2;

    switch (channel) {
    case CHANNEL_1:
        IFS0.IC1 = 0;
        return;
    case CHANNEL_2:
        IFS0.IC2 = 0;
        return;
    case CHANNEL_3:
        IFS2.IC3 = 0;
        return;
    case CHANNEL_4:
        IFS2.IC4 = 0;
        return;
    default:
        return;
    }
}

/********************/
/* Public Functions */
/********************/

void IC_reset(Channel const channel)
{
    static struct ICConf {
        struct ICxCON1Bits const con1bits;
        struct ICxCON2Bits const con2bits;
        // clang-format off
    } const ic_default_conf = {
        .con1bits = {
            .ICTSEL = IC_TIMER_TMR1,
        },
        .con2bits = {
            .ICTRIG = IC_TASK_TRIGGER,
            .SYNCSEL = SYNCSEL_TMR1,
        },
    }; // clang-format on

    struct ICxRegisters const regs = get_registers(channel);
    *regs.con1bits_p = ic_default_conf.con1bits;
    *regs.con2bits_p = ic_default_conf.con2bits;
    g_callbacks[channel] = default_callback;
}

void IC_start(Channel const channel, Edge const edge, IC_Timer const timer)
{
    /* NB: This function does not start ICxTMR. It must be started by the
     * trigger source given by SYNCSEL or by manually setting TRIGSTAT. */
    struct ICxRegisters regs = get_registers(channel);
    regs.con1bits_p->ICTSEL = (uint16_t)timer;
    regs.con2bits_p->SYNCSEL = (uint16_t)ictsel2syncsel(timer);
    regs.con1bits_p->ICM = (uint16_t)edge2icm(edge);
}

void IC_interrupt_enable(
    Channel const channel,
    InterruptCallback const callback
)
{
    g_callbacks[channel] = callback;
    interrupt_enable(channel, true);
}

void IC_interrupt_disable(Channel const channel)
{
    interrupt_enable(channel, false);
}
