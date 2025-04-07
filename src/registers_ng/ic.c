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
struct ICCON1Bits {
    uint16_t ICM :3;
    uint16_t ICBNE :1;
    uint16_t ICOV :1; // Read-only
    uint16_t ICI :2; // Read-only
    uint16_t :3;
    uint16_t ICTSEL :3;
    uint16_t ICSIDL :1;
};

/**
 * @brief Accessor for individual bits of the ICxCON2 register
 */
struct ICCON2Bits {
    uint16_t SYNCSEL :5;
    uint16_t :1;
    uint16_t TRIGSTAT :1;
    uint16_t ICTRIG :1;
    uint16_t IC32 :1;
};

/**
 * @brief Bit-mask for interrupt enable and clear flags in IECx and IFSx.
 */
enum InterruptMask {
    IC1_INTERRUPT_MASK = 1 << 1,
    IC2_INTERRUPT_MASK = 1 << 5,
    IC3_INTERRUPT_MASK = 1 << 5,
    IC4_INTERRUPT_MASK = 1 << 6,
};

/**
 * @brief Aggregate of pointers to IC channel registers
 */
struct ICRegisters {
    struct ICCON1Bits volatile *const p_con1bits;
    struct ICCON2Bits volatile *const p_con2bits;
    uint16_t volatile *const p_iec;
    uint16_t volatile *const p_ifs;
    enum InterruptMask const interrupt_mask;
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

static void interrupt_enable(Channel channel);
static void interrupt_disable(Channel channel);
static void interrupt_clear(Channel channel);

/***********/
/* Externs */
/***********/

extern struct ICCON1Bits volatile IC1CON1bits;
extern struct ICCON1Bits volatile IC2CON1bits;
extern struct ICCON1Bits volatile IC3CON1bits;
extern struct ICCON1Bits volatile IC4CON1bits;

extern struct ICCON2Bits volatile IC1CON2bits;
extern struct ICCON2Bits volatile IC2CON2bits;
extern struct ICCON2Bits volatile IC3CON2bits;
extern struct ICCON2Bits volatile IC4CON2bits;

extern uint16_t volatile IEC0;
extern uint16_t volatile IEC2;

extern uint16_t volatile IFS0;
extern uint16_t volatile IFS2;

/*************/
/* Constants */
/*************/

static struct ICRegisters const g_IC_REGS[CHANNEL_NUMEL] = {
    [CHANNEL_1] = {
        .p_con1bits = &IC1CON1bits,
        .p_con2bits = &IC1CON2bits,
        .p_iec = &IEC0,
        .p_ifs = &IFS0,
        .interrupt_mask = IC1_INTERRUPT_MASK,
    },
    [CHANNEL_2] = {
        .p_con1bits = &IC2CON1bits,
        .p_con2bits = &IC2CON2bits,
        .p_iec = &IEC0,
        .p_ifs = &IFS0,
        .interrupt_mask = IC2_INTERRUPT_MASK,
    },
    [CHANNEL_3] = {
        .p_con1bits = &IC3CON1bits,
        .p_con2bits = &IC3CON2bits,
        .p_iec = &IEC2,
        .p_ifs = &IFS2,
        .interrupt_mask = IC3_INTERRUPT_MASK,
    },
    [CHANNEL_4] = {
        .p_con1bits = &IC4CON1bits,
        .p_con2bits = &IC4CON2bits,
        .p_iec = &IEC2,
        .p_ifs = &IFS2,
        .interrupt_mask = IC4_INTERRUPT_MASK,
    },
};

/***********/
/* Globals */
/***********/

static InterruptCallback g_callbacks[CHANNEL_NUMEL] = {
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
    interrupt_disable(channel);
    interrupt_clear(channel);
}

static inline void interrupt_enable(Channel const channel)
{
    struct ICRegisters const *const regs = &g_IC_REGS[channel];
    *regs->p_iec |= regs->interrupt_mask;
}

static inline void interrupt_disable(Channel const channel)
{
    struct ICRegisters const *const regs = &g_IC_REGS[channel];
    *regs->p_iec &= ~(regs->interrupt_mask);
}

static inline void interrupt_clear(Channel const channel)
{
    struct ICRegisters const *const regs = &g_IC_REGS[channel];
    *regs->p_ifs &= ~(regs->interrupt_mask);
}



/********************/
/* Public Functions */
/********************/

enum Status IC_reset(Channel const channel)
{
    enum Status status = E_OK;
    if ( (status = check_channel(channel)) ) { return status; }

    static struct ICConf {
        struct ICCON1Bits const con1bits;
        struct ICCON2Bits const con2bits;
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

    struct ICRegisters const *const regs = &g_IC_REGS[channel];
    *regs->p_con1bits = ic_default_conf.con1bits;
    *regs->p_con2bits = ic_default_conf.con2bits;
    g_callbacks[channel] = default_callback;
    return status;
}

enum Status IC_start(
    Channel const channel,
    Edge const edge,
    IC_Timer const timer
) {
    /* NB: This function does not start ICxTMR. It must be started by the
     * trigger source given by SYNCSEL or by manually setting TRIGSTAT. */
    enum Status status = E_OK;
    if ( (status = check_channel(channel)) ) { return status; }

    struct ICRegisters const *const regs = &g_IC_REGS[channel];
    regs->p_con1bits->ICTSEL = (uint16_t)timer;
    regs->p_con2bits->SYNCSEL = (uint16_t)ictsel2syncsel(timer);
    regs->p_con1bits->ICM = (uint16_t)edge2icm(edge);
    return status;
}

enum Status IC_interrupt_enable(
    Channel const channel,
    InterruptCallback const callback
) {
    enum Status status = E_OK;
    if ( (status = check_channel(channel)) ) { return status; }

    g_callbacks[channel] = callback;
    interrupt_enable(channel);
    return status;
}

enum Status IC_interrupt_disable(Channel const channel)
{
    enum Status status = E_OK;
    if ( (status = check_channel(channel)) ) { return status; }

    interrupt_disable(channel);
    return status;
}
