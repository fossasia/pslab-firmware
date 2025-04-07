/**
 * @file dma.c
 * @author Alexander Bessman (alexander.bessman@gmail.com)
 * @brief Direct Memory Access (DMA) driver
 *
 * DMA is used to move data from a peripheral to a location in memory, or
 * vice versa.
 *
 * For hardware specifics, refer to Microchip document 70348C,
 * "Direct Memory Access (DMA)".
 */

#include <stdbool.h>
#include <stdint.h>

#include "types.h"

#include "dma.h"

/*********/
/* Types */
/*********/

/**
 * @brief Struct for accessing individual bits of the DMAxCON register
 */
struct DMACONBits {
    uint16_t MODE :2;
    uint16_t :2;
    uint16_t AMODE :2;
    uint16_t :5;
    uint16_t NULLW :1;
    uint16_t HALF :1;
    uint16_t DIR :1;
    uint16_t SIZE :1;
    uint16_t CHEN :1;
};

/**
 * @brief Struct for accessing individual bits of the DMAxREQ register
 */
struct DMAREQBits {
    uint16_t IRQSEL :8;
    uint16_t :7;
    uint16_t FORCE :1;
};

/**
 * @brief Bit-mask for interrupt enable and clear flags in IECx and IFSx.
 */
enum InterruptMask {
    DMA0_INTERRUPT_MASK = 1 << 4,
    DMA1_INTERRUPT_MASK = 1 << 14,
    DMA2_INTERRUPT_MASK = 1 << 8,
    DMA3_INTERRUPT_MASK = 1 << 4,
};

/**
 * @brief Aggregate of DMA register pointers
 */
struct DMARegisters {
    struct DMACONBits volatile *const p_conbits;
    struct DMAREQBits volatile *const p_reqbits;
    uint16_t volatile *const p_stal;
    uint16_t volatile *const p_stah;
    uint16_t volatile *const p_pad;
    uint16_t volatile *const p_cnt;
    uint16_t volatile *const p_iec;
    uint16_t volatile *const p_ifs;
    enum InterruptMask const interrupt_mask;
};

/**
 * @brief Peripheral Interrupt Request number
 * @details
 *     The DMA transaction is driven by interrupts from DMA capable
 *     peripherals. Each time the peripheral raises an interrupt flag, the DMA
 *     moves one word of data from/to the Peripheral Address to/from the memory
 *     area defined by the DMAxSTAL/H registers.
 */
enum PeripheralIRQ {
    DMA_PERIPHERAL_IRQ_INT0 = 0x00,
    DMA_PERIPHERAL_IRQ_IC1 = 0x01,
    DMA_PERIPHERAL_IRQ_IC2 = 0x05,
    DMA_PERIPHERAL_IRQ_IC3 = 0x25,
    DMA_PERIPHERAL_IRQ_IC4 = 0x26,
    DMA_PERIPHERAL_IRQ_OC1 = 0x02,
    DMA_PERIPHERAL_IRQ_OC2 = 0x06,
    DMA_PERIPHERAL_IRQ_OC3 = 0x19,
    DMA_PERIPHERAL_IRQ_OC4 = 0x1A,
    DMA_PERIPHERAL_IRQ_TMR2 = 0x07,
    DMA_PERIPHERAL_IRQ_TMR3 = 0x08,
    DMA_PERIPHERAL_IRQ_TMR4 = 0x1B,
    DMA_PERIPHERAL_IRQ_TMR5 = 0x1C,
    DMA_PERIPHERAL_IRQ_SPI1 = 0x0A,
    DMA_PERIPHERAL_IRQ_SPI2 = 0x21,
    DMA_PERIPHERAL_IRQ_UART1_TX = 0x0C,
    DMA_PERIPHERAL_IRQ_UART1_RX = 0x0B,
    DMA_PERIPHERAL_IRQ_UART2_TX = 0x1F,
    DMA_PERIPHERAL_IRQ_UART2_RX = 0x1E,
    DMA_PERIPHERAL_IRQ_ADC1 = 0x0D
};

/*********************/
/* Static prototypes */
/*********************/

/**
 * @brief Enable interrupt on a DMA channel
 *
 * @param channel
 */
static void interrupt_enable(Channel channel);

/**
 * @brief Disable interrupt on a DMA channel
 *
 * @param channel
 */
static void interrupt_disable(Channel channel);

/**
 * @brief Clear interrupt flag on DMA channel
 *
 * @param channel
 */
static void interrupt_clear(Channel channel);

/**
 * @brief Get the Peripheral Address of a DMA source
 *
 * Some PADs are associated with a specific DMA channel. For example,
 * the buffer of Input Capture 1 (IC1BUF) is used by DMA0. This is a
 * software limitation and could be generalized.
 *
 * @param channel
 * @param source
 * @return uint16_t
 */
static uint16_t volatile *get_pad(Channel channel, DMA_Source source);

/**
 * @brief Get the Peripheral Interrupt Request number of a DMA source
 *
 * @param channel
 * @param source
 * @return enum PeripheralIRQ
 */
static enum PeripheralIRQ get_irq(Channel channel, DMA_Source source);

/**
 * @brief Turn off DMA once transaction is complete
 *
 * @param channel
 */
static void default_callback(Channel channel);

/***********/
/* Externs */
/***********/

extern struct DMACONBits volatile DMA0CONbits;
extern struct DMACONBits volatile DMA1CONbits;
extern struct DMACONBits volatile DMA2CONbits;
extern struct DMACONBits volatile DMA3CONbits;

extern struct DMAREQBits volatile DMA0REQbits;
extern struct DMAREQBits volatile DMA1REQbits;
extern struct DMAREQBits volatile DMA2REQbits;
extern struct DMAREQBits volatile DMA3REQbits;

extern uint16_t volatile DMA0STAL;
extern uint16_t volatile DMA1STAL;
extern uint16_t volatile DMA2STAL;
extern uint16_t volatile DMA3STAL;

extern uint16_t volatile DMA0STAH;
extern uint16_t volatile DMA1STAH;
extern uint16_t volatile DMA2STAH;
extern uint16_t volatile DMA3STAH;

extern uint16_t volatile DMA0PAD;
extern uint16_t volatile DMA1PAD;
extern uint16_t volatile DMA2PAD;
extern uint16_t volatile DMA3PAD;

extern uint16_t volatile DMA0CNT;
extern uint16_t volatile DMA1CNT;
extern uint16_t volatile DMA2CNT;
extern uint16_t volatile DMA3CNT;

extern uint16_t volatile IEC0;
extern uint16_t volatile IEC1;
extern uint16_t volatile IEC2;

extern uint16_t volatile IFS0;
extern uint16_t volatile IFS1;
extern uint16_t volatile IFS2;

/*************/
/* Constants */
/*************/

static struct DMARegisters const g_DMA_REGS[CHANNEL_NUMEL] = {
    [CHANNEL_1] = {
        .p_conbits = &DMA0CONbits,
        .p_reqbits = &DMA0REQbits,
        .p_stal = &DMA0STAL,
        .p_stah = &DMA0STAH,
        .p_pad = &DMA0PAD,
        .p_cnt = &DMA0CNT,
        .p_iec = &IEC0,
        .p_ifs = &IFS0,
        .interrupt_mask = DMA0_INTERRUPT_MASK,
    },
    [CHANNEL_2] = {
        .p_conbits = &DMA1CONbits,
        .p_reqbits = &DMA1REQbits,
        .p_stal = &DMA1STAL,
        .p_stah = &DMA1STAH,
        .p_pad = &DMA1PAD,
        .p_cnt = &DMA1CNT,
        .p_iec = &IEC0,
        .p_ifs = &IFS0,
        .interrupt_mask = DMA1_INTERRUPT_MASK,
    },
    [CHANNEL_3] = {
        .p_conbits = &DMA2CONbits,
        .p_reqbits = &DMA2REQbits,
        .p_stal = &DMA2STAL,
        .p_stah = &DMA2STAH,
        .p_pad = &DMA2PAD,
        .p_cnt = &DMA2CNT,
        .p_iec = &IEC1,
        .p_ifs = &IFS1,
        .interrupt_mask = DMA2_INTERRUPT_MASK,
    },
    [CHANNEL_4] = {
        .p_conbits = &DMA3CONbits,
        .p_reqbits = &DMA3REQbits,
        .p_stal = &DMA3STAL,
        .p_stah = &DMA3STAH,
        .p_pad = &DMA3PAD,
        .p_cnt = &DMA3CNT,
        .p_iec = &IEC2,
        .p_ifs = &IFS2,
        .interrupt_mask = DMA3_INTERRUPT_MASK,
    }
};

/***********/
/* Globals */
/***********/

static InterruptCallback g_callbacks[CHANNEL_NUMEL] = {
    default_callback,
    default_callback,
    default_callback,
    default_callback,
};

/**************/
/* Interrupts */
/**************/

/**
 * @brief DMA interrupt
 *
 * @details
 *     The interrupt occurs when all DMA transers have finished, i.e. once
 *     DMAxCNT reaches zero. At this point, it is often necessary to do
 *     some cleanup. The actual cleanup is handled by a callback function,
 *     since what needs to be cleaned up depends on what the DMA was used for.
 */

#ifndef LINTING

void __attribute__((__interrupt__, no_auto_psv)) _DMA0Interrupt(void)
{
    g_callbacks[CHANNEL_1](CHANNEL_1);
    interrupt_clear(CHANNEL_1);
}

void __attribute__((__interrupt__, no_auto_psv)) _DMA1Interrupt(void)
{
    g_callbacks[CHANNEL_2](CHANNEL_2);
    interrupt_clear(CHANNEL_2);
}

void __attribute__((__interrupt__, no_auto_psv)) _DMA2Interrupt(void)
{
    g_callbacks[CHANNEL_3](CHANNEL_3);
    interrupt_clear(CHANNEL_3);
}

void __attribute__((__interrupt__, no_auto_psv)) _DMA3Interrupt(void)
{
    g_callbacks[CHANNEL_4](CHANNEL_4);
    interrupt_clear(CHANNEL_4);
}

#endif

/********************/
/* Static functions */
/********************/

static inline void interrupt_enable(Channel const channel)
{
    struct DMARegisters const *const reg = &g_DMA_REGS[channel];
    *reg->p_iec |= reg->interrupt_mask;
}

static inline void interrupt_disable(Channel const channel)
{
    struct DMARegisters const *const reg = &g_DMA_REGS[channel];
    *reg->p_iec &= ~(reg->interrupt_mask);
}

static inline void interrupt_clear(Channel const channel)
{
    struct DMARegisters const *const reg = &g_DMA_REGS[channel];
    *reg->p_ifs &= ~(reg->interrupt_mask);
}

static uint16_t volatile *get_pad(Channel const channel, DMA_Source const source)
{
    // NOLINTNEXTLINE(readability-isolate-declaration)
    extern uint16_t volatile ADC1BUF0, ADC1BUF1, ADC1BUF2, ADC1BUF3;
    uint16_t volatile *const adcbufs[CHANNEL_NUMEL] = {
        [CHANNEL_1] = &ADC1BUF0,
        [CHANNEL_2] = &ADC1BUF1,
        [CHANNEL_3] = &ADC1BUF2,
        [CHANNEL_4] = &ADC1BUF3,
    };

    // NOLINTNEXTLINE(readability-isolate-declaration)
    extern uint16_t volatile IC1BUF, IC2BUF, IC3BUF, IC4BUF;
    uint16_t volatile *const icbufs[CHANNEL_NUMEL] = {
        [CHANNEL_1] = &IC1BUF,
        [CHANNEL_2] = &IC2BUF,
        [CHANNEL_3] = &IC3BUF,
        [CHANNEL_4] = &IC4BUF,
    };

    switch (source) {
    case DMA_SOURCE_ADC:
        return adcbufs[channel];

    case DMA_SOURCE_IC:
        return icbufs[channel];

    default:
        return NULL;
    }
}

static enum PeripheralIRQ get_irq(Channel channel, DMA_Source source)
{
    enum PeripheralIRQ const ic_irqs[CHANNEL_NUMEL] = {
        [CHANNEL_1] = DMA_PERIPHERAL_IRQ_IC1,
        [CHANNEL_2] = DMA_PERIPHERAL_IRQ_IC2,
        [CHANNEL_3] = DMA_PERIPHERAL_IRQ_IC3,
        [CHANNEL_4] = DMA_PERIPHERAL_IRQ_IC4,
    };

    switch (source) {
    case DMA_SOURCE_ADC:
        return DMA_PERIPHERAL_IRQ_ADC1;
    case DMA_SOURCE_IC:
        return ic_irqs[channel];
    default:
        return DMA_PERIPHERAL_IRQ_INT0;
    }
}

static void default_callback(Channel const channel)
{
    interrupt_disable(channel);
}

/********************/
/* Public functions */
/********************/

void DMA_reset(Channel const channel)
{
    enum OperatingMode {
        DMA_OPERATING_MODE_CONTINUOUS = 0b00,
        DMA_OPERATING_MODE_ONE_SHOT = 0b01,
        DMA_OPERATING_MODE_CONTINUOUS_PING_PONG = 0b10,
        DMA_OPERATING_MODE_ONE_SHOT_PINT_PONG = 0b11,
    };

    static struct DMAConf {
        struct DMACONBits const conbits;
        struct DMAREQBits const reqbits;
        uint16_t const stal;
        uint16_t const stah;
        uint16_t const pad;
        uint16_t const cnt;
        // clang-format off
    } const default_conf = {
        .conbits = {
            .MODE = DMA_OPERATING_MODE_ONE_SHOT,
            .AMODE = 0,
            .NULLW = 0,
            .HALF = 0,
            .DIR = 0,
            .SIZE = 0,
            .CHEN = 0,
        },
        .reqbits = {
            .IRQSEL = 0,
            .FORCE = 0,
        },
        .stal = 0,
        .stah = 0,
        .pad = 0,
        .cnt = 0,
    }; // clang-format on

    struct DMARegisters const *const regs = &g_DMA_REGS[channel];
    *regs->p_conbits = default_conf.conbits;
    *regs->p_reqbits = default_conf.reqbits;
    *regs->p_stal = default_conf.stal;
    *regs->p_pad = default_conf.pad;
    *regs->p_cnt = default_conf.cnt;
    interrupt_disable(channel);
    interrupt_clear(channel);
    g_callbacks[channel] = default_callback;
}

void DMA_setup(
    Channel const channel,
    uint16_t const count,
    size_t const address,
    DMA_Source const source
)
{
    struct DMARegisters const *const regs = &g_DMA_REGS[channel];
    *regs->p_pad = (uint16_t)get_pad(channel, source);
    regs->p_reqbits->IRQSEL = (uint16_t)get_irq(channel, source);
    *regs->p_cnt = count - 1; // DMAxCNT == 0 results in one transfer.
    *regs->p_stal = address;
    *regs->p_stah = 0;
}

void DMA_start(Channel const channel)
{
    g_DMA_REGS[channel].p_conbits->CHEN = 1;
}

void DMA_interrupt_enable(
    Channel const channel,
    InterruptCallback const callback
)
{
    g_callbacks[channel] = callback;
    interrupt_enable(channel);
}
