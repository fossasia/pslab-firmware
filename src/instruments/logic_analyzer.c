/**
 * @file logic_analyzer.c
 * @author Alexander Bessman (alexander.bessman@gmail.com)
 * @brief High-level driver for the PSLab's Logic Analyzer instrument
 * @details
 * # Implementation
 *
 * The logic analyzer uses the following resources:
 *
 * ## Pins LA1-4
 *
 * When the logic level on an active pin changes, a timestamp is logged to the
 * buffer. Three types of logic level changes (edges) can be captured:
 * ANY, FALLING, or RISING.
 *
 * If the configured edge type is ANY, a timestamp is stored every time the
 * logic level changes from low to high, or from high to low.
 *
 * If the configured edge type is RISING, a timestamp is stored every time the
 * logic level changes from low to high, but not from high to low. Vice versa
 * for edge type FALLING.
 *
 * Up to 10k timestamps can be captured, across all four channels.
 *
 * ## Input Capture (IC) channels IC1-4
 *
 * Each ICx channel is associated with the corresponding LAx pin. When the
 * configured edge type is detected on LAx, the current value of ICxTMR is
 * copied to ICxBUF.
 *
 * IC interrupt is used to trigger delayed capture, if edge type is FALLING or
 * RISING. If edge type is ANY, delayed capture is instead triggered by CN.
 *
 * ## Input Change Notification (CN)
 *
 * One pin may be designated as the trigger pin, in which case capture begins
 * when the configured edge type is detected on that pin. If no pin is selected
 * as trigger, capture begins immediatedely.
 *
 * If the edge type is ANY, CN interrupt is used to start capture. If the edge
 * type is FALLING or RISING, IC interrupt is used instead. This is because CN
 * cannot distinguish between rising and falling edges.
 *
 * ## Timer TMR5
 *
 * When the trigger condition is met, TMR5 is started. TMR5 is used as trigger
 * source to start the enabled IC channels' ICxTMR, as well as clock source to
 * clock the same.
 *
 * ## Direct Memory Access (DMA) channels DMA0-3
 *
 * ICx drives DMA(x-1). Every time a new value is copied to ICxBUF, DMA(x-1)
 * copies it to the buffer.
 *
 * When the requested number of timestamps have been captured on LAx, DMA(x-1)
 * interrupts and resets itself and ICx. If ICx is the last active channel,
 * TMR5 is reset.
 *
 * ## Memory
 *
 * Captured timestamps are stored in a heap allocated buffer.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../bus/uart/uart.h"
#include "commands.h"
#include "registers_ng/cn.h"
#include "registers_ng/dma.h"
#include "registers_ng/ic.h"
#include "registers_ng/pins.h"
#include "registers_ng/tmr.h"
#include "types.h"

/*********************/
/* Static Prototypes */
/*********************/

/**
 * @brief Start TMR and DMA
 * @details
 * May be called directly to trigger manually, or by an interrupt callback.
 */
static void trigger(void);

/**
 * @brief Trigger from Input Capture interrupt
 * @details
 * Register with `IC_interrupt_enable`. Disables IC interrupt and then calls
 * `trigger`. Used when triggering on either FALLING or RISING edges, not both.
 *
 * @param channel
 */
static void ic_callback(Channel channel);

/**
 * @brief Trigger from Input Change Notification interrupt
 * @details
 * Register with `CN_interrupt_enable`. Disables CN interrupt and then calls
 * `trigger`. Used when triggering on ANY edge.
 *
 * @param channel
 * Unused.
 */
static void cn_callback(Channel channel);

/**
 * @brief Stop IC, DMA, and TMR when all events have been captured
 *
 * @details Called by DMA interrupts to cleanup after capture is complete.
 *
 * @param channel
 */
static void cleanup_callback(Channel channel);

/**
 * @brief Capture logic level changes on on LA1-4
 *
 * @param n_channels
 * @param events
 * @param edge
 * @param trigger_pin
 */
static enum Status capture(
    uint8_t n_channels,
    uint16_t events,
    Edge edge,
    Channel trigger_pin
);

/**
 * @brief Choose trigger method based on pin and edge type
 * @details
 * Manually triggers immediately if `trigger_pin` is NONE.
 *
 * Uses IC interrupt if `edge` is RISING or FALLING, CN interrupt if edge is
 * ANY.
 *
 * @param edge
 * @param trigger_pin
 */
static enum Status configure_trigger(Edge edge, Channel trigger_pin);

/**
 * @brief Convert TMR to corresponding IC clock source
 *
 * @param timer
 * @return IC_Timer
 */
static IC_Timer timer2ictsel(TMR_Timer timer);

/***********/
/* Globals */
/***********/

static uint16_t *g_buffer;
static uint16_t g_buffer_n_items;

static uint8_t g_n_channels;

static uint8_t g_initial_states;

static TMR_Timer const g_TIMER = TMR_TIMER_5;

/********************/
/* Static Functions */
/********************/

static void trigger(void)
{
    TMR_start(g_TIMER);
    // ICs start approximately now.
    g_initial_states = PINS_get_la_states();

    /* When DMA starts, every time a value is copied to ICxBUF it will be
     * further copied to buffer.
     * DMA channels cannot be started simultaneously. It is possible we might
     * miss edges between timer start and DMA start. The alternative is to
     * start DMA first, which risks copying spurious zeros to buffer.
     */
    /* Unroll loop; saving even a single clock cycle between DMA channel
     * starts is meaningful. */
    switch (g_n_channels) {
    case CHANNEL_4:
        DMA_start(CHANNEL_4);
    case CHANNEL_3:
        DMA_start(CHANNEL_3);
    case CHANNEL_2:
        DMA_start(CHANNEL_2);
    case CHANNEL_1:
        DMA_start(CHANNEL_1);
    default:
        break;
    }

    // Timer sync output is only needed once; disable it after trigger is done.
    TMR_set_period(g_TIMER, 0);
}

static void ic_callback(Channel const channel)
{
    trigger();
    IC_interrupt_disable(channel);
}

static void cn_callback(__attribute__((unused)) Channel const channel)
{
    trigger();
    CN_reset();
}

static void cleanup_callback(Channel const channel)
{
    DMA_reset(channel);
    IC_reset(channel);
    --g_n_channels;

    // Reset the clock if this was the last active channel.
    if (!g_n_channels) {
        TMR_reset(g_TIMER);
    }
}

static void reset(void)
{
    for (int i = CHANNEL_1; i <= g_n_channels; ++i) {
        cleanup_callback(i);
    }
    free(g_buffer);
    g_buffer = NULL;
    g_buffer_n_items = 0;
    g_n_channels = 0;
}

static enum Status setup_buffer(uint16_t const n_items)
{
    g_buffer = malloc(n_items * sizeof(*g_buffer));
    if (!g_buffer) { return E_MEMORY_INSUFFICIENT; }
    g_buffer_n_items = n_items;
    return E_OK;
}

static enum Status capture(
    uint8_t const n_channels,
    uint16_t const events,
    Edge const edge,
    Channel const trigger_pin
) {
    if (g_buffer) { return E_RESOURCE_BUSY; }

    enum Status status = E_OK;
    TRY(setup_buffer(n_channels * events));
    g_n_channels = n_channels;

    // Set timer period to one to send sync signal immediately on timer start.
    TRY(TMR_set_period(g_TIMER, 1));

    for (Channel i = CHANNEL_1; i < n_channels; ++i) {
        uint16_t *address = g_buffer + i * events;
        TRY(DMA_setup(i, events, (size_t)address, DMA_SOURCE_IC));
        /* DMA interrupt is enabled here, but the DMA transfer itself is
         * started in trigger callback. */
        TRY(DMA_interrupt_enable(i, cleanup_callback));
        /* IC is started here. IC will now begin copying the value of ICxTMR to
         * ICxBUF whenever an event occurs. Until the trigger event starts the
         * clock source, ICxTMR will be held at zero. This is not a problem,
         * because although zeros will be copied to ICxBUF, they won't be
         * copied to the buffer until DMA is started by the trigger callback.
         */
        TRY(IC_start(i, edge, timer2ictsel(g_TIMER)));
    }

    TRY(configure_trigger(edge, trigger_pin));
    return status;

error:
    reset();
    return status;
}

static enum Status configure_trigger(Edge const edge, Channel const trigger_pin)
{
    enum Status status = E_OK;

    if (trigger_pin == CHANNEL_NONE) {
        // Start immediately.
        trigger();
        return status;
    }

    if (edge == EDGE_ANY) {
        /* Input capture cannot interrupt on both falling and rising edge, only
         * one or the other. Must use Change Notification instead. */
        TRY(CN_pin_enable(trigger_pin));
        TRY(CN_interrupt_enable(cn_callback));
    }

    TRY(IC_interrupt_enable(trigger_pin, ic_callback));
    return status;

error:
    return status;
}

static IC_Timer timer2ictsel(TMR_Timer const timer)
{
    switch (timer) {
    case TMR_TIMER_1:
        return IC_TIMER_TMR1;
    default:
        return IC_TIMER_PERIPHERAL;
    }
}

/********************/
/* Public Functions */
/********************/

enum Status LA_capture(
    uint8_t const n_channels,
    uint16_t const events,
    Edge const edge,
    Channel const trigger
) {
    enum Status status = E_OK;
    if ( (status = CHANNEL_check(n_channels)) ) { return E_BAD_ARGUMENT; }
    if ( (status = CHANNEL_check(trigger)) ) { return E_BAD_ARGUMENT; }
    capture(n_channels, events, edge, trigger);
    return status;
}

enum Status LA_fetch(uint16_t **buffer, uint16_t *n_items)
{
    *buffer = g_buffer;
    *n_items = g_buffer_n_items;
    g_buffer = NULL;
    g_buffer_n_items = 0;
    return E_OK;
}

enum Status LA_stop(void)
{
    CN_reset();
    TMR_reset(g_TIMER);

    for (size_t i = 0; i < CHANNEL_NUMEL; ++i) {
        IC_reset(i);
        DMA_reset(i);
    }

    free(g_buffer);
    g_buffer = NULL;
    g_buffer_n_items = 0;

    return E_OK;
}

enum Status LA_get_initial_states(uint8_t *const initial_states)
{
    *initial_states = g_initial_states;
    return E_OK;
}


/*********************/
/* Command Functions */
/******************* */

enum Status LA_cmd_capture(
    uint8_t **args,
    uint16_t args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    struct Input {
        uint16_t events;
        uint8_t edge;
        uint8_t n_channels;
        uint8_t trigger;
        uint8_t _pad[1];
    } *input = NULL;

    uint16_t const expected_size = sizeof(struct Input) - sizeof(input->_pad);
    if (args_size != expected_size) { return E_BAD_ARGSIZE; }
    input = *(struct Input **)args;

    return LA_capture(
        input->n_channels,
        input->events,
        (Edge)input->edge,
        (Channel)input->trigger
    );
}

enum Status LA_cmd_fetch(
    __attribute__((unused)) uint8_t **args,
    __attribute__((unused)) uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    return LA_fetch((uint16_t **)rets, rets_size);
}

enum Status LA_cmd_stop(
    __attribute__((unused)) uint8_t **args,
    __attribute__((unused)) uint16_t args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    return LA_stop();
}

enum Status LA_cmd_get_initial_states(
    __attribute__((unused)) uint8_t **args,
    __attribute__((unused)) uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    uint8_t *initial_states = malloc(sizeof(*initial_states));
    if (!initial_states) { return E_MEMORY_INSUFFICIENT; }
    *rets = initial_states;
    *rets_size = sizeof(*initial_states);
    return LA_get_initial_states(initial_states);
}
