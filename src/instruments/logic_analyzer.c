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
 * When the logic level on an active pin changes, a timestamp is stored in the
 * sample buffer. Three types of logic level changes (edges) can be captured:
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
 * type is FALLING or RISING, IC interrupt is used instead.
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
 * copies it to the sample buffer.
 *
 * When the requested number of timestamps have been captured on LAx, DMA(x-1)
 * interrupts and resets itself and ICx. If ICx is the last active channel,
 * TMR5 is reset.
 *
 * ## Sample Buffer
 *
 * Captured timestamps are stored in the sample buffer.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../bus/uart/uart.h"
#include "commands.h"
#include "helpers/buffer.h"
#include "helpers/delay.h"
#include "registers_ng/cn.h"
#include "registers_ng/dma.h"
#include "registers_ng/ic.h"
#include "registers_ng/pins.h"
#include "registers_ng/tmr.h"
#include "types.h"

/*********/
/* Types */
/*********/

/**
 * @brief Instrument state
 */
struct LogicAnalyzerState {
    bool locked;
    uint8_t configured_channels;
    uint8_t active_channels;
    Edge capture_edge;
    Edge trigger_edge;
    uint8_t initial_states;
    uint16_t events_per_channel;
    uint16_t *initial_addresses[CHANNEL_NUMEL];
    uint16_t final_progress[CHANNEL_NUMEL];
};

/*********************/
/* Static Prototypes */
/*********************/

/**
 * @brief Start TMR and IC
 * @details
 * May be called directly to trigger manually, or by an interrupt callback.
 */
static void trigger(Channel channel);

/**
 * @brief Stop IC, DMA, and TMR when all events have been captured
 *
 * @details
 * Called by DMA interrupts to release resources after capture is complete.
 *
 * @param channel
 */
static void cleanup_callback(Channel channel);

/**
 * @brief Configure logic analyzer to capture edges on on LA1-4
 *
 * @param num_channels
 * @param events
 * @param edge
 */
static void setup(uint8_t num_channels, uint16_t events, Edge edge);

/**
 * @brief Arm trigger
 * @details
 * Manually triggers immediately if `channel` or `edge` is NONE.
 *
 * Uses IC interrupt if `edge` is RISING or FALLING, CN interrupt if edge is
 * ANY.
 *
 * @param channel
 * @param edge
 */
static void arm(Channel channel, Edge edge);

/**
 * @brief Convert TMR to corresponding IC clock source
 *
 * @param timer
 * @return IC_Timer
 */
static IC_Timer timer2ictsel(TMR_Timer timer);

/*************/
/* Constants */
/*************/

static TMR_Timer const g_TIMER = TMR_TIMER_5;

/***********/
/* Globals */
/***********/

static struct LogicAnalyzerState volatile g_state = {
    .locked = false,
    .configured_channels = 0,
    .active_channels = 0,
    .capture_edge = EDGE_NONE,
    .trigger_edge = EDGE_NONE,
    .initial_states = 0,
    .events_per_channel = 0,
    .initial_addresses = {NULL, NULL, NULL, NULL},
    .final_progress = {0},
};
static struct LogicAnalyzerState const g_STATE_DEFAULT = {
    .locked = false,
    .configured_channels = 0,
    .active_channels = 0,
    .capture_edge = EDGE_NONE,
    .trigger_edge = EDGE_NONE,
    .initial_states = 0,
    .events_per_channel = 0,
    .initial_addresses = {NULL, NULL, NULL, NULL},
    .final_progress = {0},
};

/********************/
/* Static Functions */
/********************/

static void trigger(Channel const channel)
{
    // If this function was called from an interrupt, the trigger edge occurred
    // ~100 ns ago. Now, we need to
    //
    // 1. Turn on IC for all configured channels.
    // 2. Turn on TMR5, which triggers the ICxTMR timers when TMR5 == 1, since
    //    PR5 has previously been set to 1.
    // 3. Turn on DMA for all configured channels.
    // 4. Store initial pin states.
    //
    // (1) must preceed (2) in order for the ICxTMR timers to be synchronized.
    // (3) needs to happen after (1) and (2) to prevent capture of invalid
    // edges prior to ICxTMR start. (4) needs to happen as soon as possible
    // after (1) and (2) in order to reduce the probability of an edge
    // occurring after IC is started but before the initial pin state is
    // recorded. If that happens, the recorded initial pin state is incorrect.

    Edge const capture_edge = g_state.capture_edge;

    switch (g_state.configured_channels) {
    case 4: // NOLINT(readability-magic-numbers)
        IC4_start_fast(capture_edge);
        IC4BUF; // Clear invalid edge in case IC was trigger source.
        DMA3_start_fast();
    case 3: // NOLINT(readability-magic-numbers)
        IC3_start_fast(capture_edge);
        IC3BUF;
        DMA2_start_fast();
    case 2:
        IC2_start_fast(capture_edge);
        IC2BUF;
        DMA1_start_fast();
    case 1:
        IC1_start_fast(capture_edge);
        IC1BUF;
        DMA0_start_fast();
    default:
        break;
    }

    TMR5_start_fast(); // ICxTMR starts one clock cycle after this.
    g_state.initial_states = PINS_get_la_states();
    // End of timing critical section.

    g_state.active_channels = g_state.configured_channels;
    // Timer sync output is only needed once; disable it after trigger is done.
    TMR_set_period(g_TIMER, 0);
    Edge const trigger_edge = g_state.trigger_edge;

    if (trigger_edge == EDGE_RISING || trigger_edge == EDGE_FALLING) {
        IC_interrupt_disable(channel);
    } else if (trigger_edge == EDGE_ANY) {
        CN_reset();
    }
}

static void cleanup_callback(Channel const channel)
{
    // If we reach this point, the DMA has finished copying the requested
    // number of timestamps to the sample buffer.
    g_state.final_progress[channel] = g_state.events_per_channel;
    DMA_reset(channel);
    IC_reset(channel);
    uint8_t const active_channels = --g_state.active_channels;

    // Reset the clock if this was the last active channel.
    if (!active_channels) {
        TMR_reset(g_TIMER);
        // Release instrument.
        g_state.locked = false;
    }
}

static void setup(
    uint8_t const num_channels,
    uint16_t const events,
    Edge const edge
)
{
    // Lock instrument to prevent reuse before capture is complete.
    g_state.locked = true;
    // Store instrument state for use in e.g. interrupt callbacks.
    g_state.configured_channels = num_channels;
    g_state.capture_edge = edge;
    g_state.events_per_channel = events;

    for (uint8_t i = 0; i < num_channels; ++i) {
        IC_reset(i);
        DMA_reset(i);
        size_t const address =
            (size_t)(BUFFER + i * BUFFER_SIZE / num_channels);
        // Configure DMAx with ICx as driver.
        DMA_setup(i, events, address, DMA_SOURCE_IC);
        // Store initial address to be able to send captured timestamps later.
        g_state.initial_addresses[i] = (uint16_t *)address;
        // Clear sample buffer.
        BUFFER_set((uint16_t *)address, 0, events);
        // DMA interrupt is enabled here. Transfer begins when DMA is enabled
        // in trigger function.
        DMA_interrupt_enable(i, cleanup_callback);
        // Configure ICx with TMR5 as clock and trigger source.
        IC_setup(i, timer2ictsel(g_TIMER));
    }
}

static void arm(Channel const channel, Edge const edge)
{
    // Set timer period to a small value to assert sync when timer starts.
    TMR_set_period(g_TIMER, 1);
    g_state.trigger_edge = edge;

    if (channel == CHANNEL_NONE || edge == EDGE_NONE) {
        // Start immediately.
        trigger(CHANNEL_NONE);
        return;
    }

    if (edge == EDGE_ANY) {
        CN_reset();
        CN_interrupt_enable(channel, trigger);
        return;
    }

    IC_start(channel, edge);
    IC_interrupt_enable(channel, trigger);
}

static IC_Timer timer2ictsel(TMR_Timer const timer)
{
    switch (timer) {
    case TMR_TIMER_5:
        return IC_TIMER_TMR5;
    default:
        return IC_TIMER_PERIPHERAL;
    }
}

/********************/
/* Public Functions */
/********************/

response_t LA_capture(void)
{
    uint8_t const num_channels = UART1_Read();
    uint16_t const events = UART1_ReadInt();
    Edge const edge = (int8_t)UART1_Read();
    Channel const trigger_pin = (int8_t)UART1_Read();
    Edge const trigger_edge = (int8_t)UART1_Read();

    if (g_state.locked) {
        return FAILED;
    }

    if (num_channels == 0) {
        return ARGUMENT_ERROR;
    }

    if (num_channels > CHANNEL_NUMEL) {
        return ARGUMENT_ERROR;
    }

    if (events * num_channels > BUFFER_SIZE) {
        return ARGUMENT_ERROR;
    }

    if (edge == EDGE_NONE) {
        return ARGUMENT_ERROR;
    }

    setup(num_channels, events, edge);
    arm(trigger_pin, trigger_edge);
    return SUCCESS;
}

response_t LA_stop(void)
{
    if (!g_state.locked) {
        // Logic analyzer is already stopped.
        return SUCCESS;
    }

    CN_reset();
    TMR_reset(g_TIMER);

    for (size_t i = 0; i < CHANNEL_NUMEL; ++i) {
        IC_reset(i);
        g_state.final_progress[i] = DMA_get_progress(i);
        DMA_reset(i);
    }

    g_state.locked = false;
    return SUCCESS;
}

response_t LA_get_states(void)
{
    UART1_Write(PINS_get_la_states());
    return SUCCESS;
}

response_t LA_get_progress(void)
{
    uint8_t const channels = g_state.configured_channels;
    UART1_Write(channels);
    bool const done = !g_state.locked;
    UART1_Write(done);

    for (uint8_t i = 0; i < channels; ++i) {
        // If capture is complete, DMA has already been released.
        UART1_WriteInt(done ? g_state.final_progress[i] : DMA_get_progress(i));
    }

    return SUCCESS;
}

response_t LA_get_timestamps(void)
{
    // Send initial pin states.
    UART1_Write(g_state.initial_states);

    // Changing the timer prescaler is unsupported for now, but may be added
    // in the future.
    uint8_t const prescaler = 1;
    // Send scaling factor to convert timestamps to seconds.
    uint32_t const scaling = FCY / prescaler;
    UART1_write_u32(scaling);

    // Send number of configured channels.
    uint8_t const channels = g_state.configured_channels;
    UART1_Write(channels);

    for (uint8_t i = 0; i < channels; ++i) {
        // Send number of captured events on channel.
        uint16_t const events = g_state.final_progress[i];
        UART1_WriteInt(events);
        // Send event timestamp deltas.
        uint16_t const *const address = g_state.initial_addresses[i];
        uint16_t previous = 0;
        for (uint16_t j = 0; j < events; ++j) {
            uint16_t const timestamp = *(address + j);
            uint16_t const timedelta = timestamp - previous;
            UART1_WriteInt(timedelta);
            previous = timestamp;
        }
    }

    return SUCCESS;
}
