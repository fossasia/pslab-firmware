#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../commands.h"
#include "../registers/comparators/cmp4.h"
#include "../registers/comparators/cvr.h"
#include "../registers/comparators/ic1.h"
#include "../registers/comparators/ic2.h"
#include "../registers/comparators/ic3.h"
#include "../registers/comparators/ic4.h"
#include "../registers/comparators/ic_params.h"
#include "../registers/memory/dma.h"
#include "../registers/system/interrupt_manager.h"
#include "../registers/system/pin_manager.h"
#include "../registers/timers/tmr2.h"

static uint8_t DIGITAL_STATES = 0;
void SetDIGITAL_STATES(uint8_t V) { DIGITAL_STATES = V; }
void SetDefaultDIGITAL_STATES(void) { DIGITAL_STATES = ((PORTB >> 10) & 0xF) | (_C4OUT << 4); }
uint8_t GetDIGITAL_STATES(void) { return DIGITAL_STATES; }

static uint8_t DIGITAL_STATES_ERROR = 0;
void SetDefaultDIGITAL_STATES_ERROR(void) { DIGITAL_STATES_ERROR = ((PORTB >> 10) & 0xF) | (_C4OUT << 4); }
uint8_t GetDIGITAL_STATES_ERROR(void) { return DIGITAL_STATES_ERROR; }

static uint16_t *g_buffer = NULL;
static uint16_t g_n_buffer_items = 0;

enum Status INTERVAL_CaptureOne(uint16_t count, uint8_t channel, uint8_t mode, uint8_t trig) {
    if (!count) { return E_OK; }
    if (g_buffer) { return E_RESOURCE_BUSY; }

    uint8_t const n_channels = 1;

    if ( !(g_buffer = malloc(n_channels * count * sizeof(*g_buffer))) ) {
        return E_MEMORY_INSUFFICIENT;
    }

    g_n_buffer_items = n_channels * count;

    INTERRUPT_ClearPinChangeInterruptsFlag();
    INTERRUPT_DisablePinChangeInterrupts();

    DMA_ChannelDisable(DMA_CHANNEL_0);

    DMA_PrepareChannel0(count, g_buffer + count * 0, DMA_PERIPHERAL_IRQ_IC1);
    DMA_InterruptEnable(DMA_CHANNEL_0);

    IC1_Initialize();

    SetDMA_MODE(DMA_MODES_ONE_CHANNEL);

    if (channel == 4) {
        CVR_SetupComparator();
        CMP4_SetupComparator();
    }
    RPINR7bits.IC1R = PIN_MANAGER_DIGITAL_PINS[channel];

    IC1_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER_PERIPHERAL);
    IC1_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);

    if (trig & 7) {
        if ((trig >> 4) == 4) {
            CVR_SetupComparator();
            CMP4_SetupComparator();
        }
        RPINR8bits.IC4R = PIN_MANAGER_DIGITAL_PINS[(trig >> 4) & 0xF];
        IC4_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER_PERIPHERAL);
        IC1_SetCaptureSource(IC_PARAMS_CAPTURE_SOURCE_IC4);
    }

    DMA_ChannelEnable(DMA_CHANNEL_0);
    IC1_SetCaptureMode(mode);
    IC4_SetCaptureMode(trig & 7);

    return E_OK;
}

enum Status INTERVAL_CaptureTwo(uint16_t count, uint8_t mode, uint8_t channel) {
    if (!count) { return E_OK; }
    if (g_buffer) { return E_RESOURCE_BUSY; }

    uint8_t const n_channels = 2;

    if ( !(g_buffer = malloc(n_channels * count * sizeof(*g_buffer))) ) {
        return E_MEMORY_INSUFFICIENT;
    }

    g_n_buffer_items = n_channels * count;

    DMA_ChannelDisable(DMA_CHANNEL_0);
    DMA_ChannelDisable(DMA_CHANNEL_1);

    DMA_PrepareChannel0(count, g_buffer + count * 0, DMA_PERIPHERAL_IRQ_IC1);
    DMA_PrepareChannel1(count, g_buffer + count * 1, DMA_PERIPHERAL_IRQ_IC2);

    DMA_InterruptEnable(DMA_CHANNEL_0);

    IC_PARAMS_InitiateAll();

    SetDMA_MODE(DMA_MODES_TWO_CHANNEL);

    RPINR7bits.IC1R = PIN_MANAGER_DIGITAL_PINS[channel & 0xF];
    RPINR7bits.IC2R = PIN_MANAGER_DIGITAL_PINS[(channel >> 4) & 0xF];

    IC_PARAMS_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER_PERIPHERAL);
    IC_PARAMS_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);

    DMA_EnableAllChannels();

    IC1_SetCaptureMode(mode & 0xF);
    IC2_SetCaptureMode((mode >> 4) & 0xF);

    return E_OK;
}

enum Status INTERVAL_CaptureThree(uint16_t count, uint16_t mode, uint8_t trig) {
    if (!count) { return E_OK; }
    if (g_buffer) { return E_RESOURCE_BUSY; }

    uint8_t const n_channels = 3;

    if ( !(g_buffer = malloc(n_channels * count * sizeof(*g_buffer))) ) {
        return E_MEMORY_INSUFFICIENT;
    }

    g_n_buffer_items = n_channels * count;

    DMA_ChannelDisable(DMA_CHANNEL_0);
    DMA_ChannelDisable(DMA_CHANNEL_1);
    DMA_ChannelDisable(DMA_CHANNEL_2);

    DMA_PrepareChannel0(count, g_buffer + count * 0, DMA_PERIPHERAL_IRQ_IC1);
    DMA_PrepareChannel1(count, g_buffer + count * 1, DMA_PERIPHERAL_IRQ_IC2);
    DMA_PrepareChannel2(count, g_buffer + count * 2, DMA_PERIPHERAL_IRQ_IC3);

    DMA_InterruptEnable(DMA_CHANNEL_0);

    IC_PARAMS_InitiateAll();

    SetDMA_MODE(DMA_MODES_THREE_CHANNEL);

    RPINR7bits.IC1R = PIN_MANAGER_DIGITAL_PINS_LA1;
    RPINR7bits.IC2R = PIN_MANAGER_DIGITAL_PINS_LA2;
    RPINR8bits.IC3R = PIN_MANAGER_DIGITAL_PINS_LA3;

    IC_PARAMS_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER_PERIPHERAL);

    IC1_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);
    IC2_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);
    IC3_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);

    if (trig & 7) {
        if ((trig >> 4) == 4) {
            CVR_SetupComparator();
            CMP4_SetupComparator();
        }
        RPINR8bits.IC4R = PIN_MANAGER_DIGITAL_PINS[(trig >> 4) & 0xF];
        IC1_SetCaptureSource(IC_PARAMS_CAPTURE_SOURCE_IC4);
        IC2_SetCaptureSource(IC_PARAMS_CAPTURE_SOURCE_IC4);
        IC3_SetCaptureSource(IC_PARAMS_CAPTURE_SOURCE_IC4);
    }

    DMA_ChannelEnable(DMA_CHANNEL_0);
    DMA_ChannelEnable(DMA_CHANNEL_1);
    DMA_ChannelEnable(DMA_CHANNEL_2);

    IC1_SetCaptureMode(mode & 0xF);
    IC2_SetCaptureMode((mode >> 4) & 0xF);
    IC3_SetCaptureMode((mode >> 8) & 0xF);
    IC4_SetCaptureMode(trig & 7);

    return E_OK;
}

enum Status INTERVAL_CaptureFour(uint16_t count, uint16_t mode, uint8_t prescaler) {
    if (!count) { return E_OK; }
    if (g_buffer) { return E_RESOURCE_BUSY; }

    uint8_t const n_channels = 4;

    if ( !(g_buffer = malloc(n_channels * count * sizeof(*g_buffer))) ) {
        return E_MEMORY_INSUFFICIENT;
    }

    g_n_buffer_items = n_channels * count;

    DMA_DisableAllChannels();

    DMA_PrepareChannel0(count, g_buffer + count * 0, DMA_PERIPHERAL_IRQ_IC1);
    DMA_PrepareChannel1(count, g_buffer + count * 1, DMA_PERIPHERAL_IRQ_IC2);
    DMA_PrepareChannel2(count, g_buffer + count * 2, DMA_PERIPHERAL_IRQ_IC3);
    DMA_PrepareChannel3(count, g_buffer + count * 3, DMA_PERIPHERAL_IRQ_IC4);

    DMA_InterruptEnable(DMA_CHANNEL_0);

    IC_PARAMS_InitiateAll();

    SetDMA_MODE(DMA_MODES_FOUR_CHANNEL);

    RPINR7bits.IC1R = PIN_MANAGER_DIGITAL_PINS_LA1;
    RPINR7bits.IC2R = PIN_MANAGER_DIGITAL_PINS_LA2;
    RPINR8bits.IC3R = PIN_MANAGER_DIGITAL_PINS_LA3;
    RPINR8bits.IC4R = PIN_MANAGER_DIGITAL_PINS_LA4;

    TMR2_Initialize();
    TMR2_SetPrescaler(prescaler & 0xF);
    TMR2_Counter16BitSet(1);

    IC_PARAMS_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER2);
    IC_PARAMS_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);

    DMA_EnableAllChannels();

    IC1_SetCaptureMode(mode & 0xF);
    IC2_SetCaptureMode((mode >> 4) & 0xF);
    IC3_SetCaptureMode((mode >> 8) & 0xF);
    IC4_SetCaptureMode((mode >> 12) & 0xF);

    return E_OK;
}

enum Status INTERVAL_get_state(
    __attribute__ ((unused)) uint8_t **args,
    __attribute__ ((unused)) uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    struct Output {
        uint16_t base;
        uint16_t dma0;
        uint16_t dma1;
        uint16_t dma2;
        uint16_t dma3;
        uint16_t states;
    };
    struct Output *output = malloc(sizeof(struct Output));

    if (!output) { return E_MEMORY_INSUFFICIENT; }

    output->base = (uint16_t)g_buffer;
    output->dma0 = DMA0STAL;
    output->dma1 = DMA1STAL;
    output->dma2 = DMA2STAL;
    output->dma3 = DMA3STAL;
    output->states = DIGITAL_STATES + (DIGITAL_STATES_ERROR << 8);
    *rets = (uint8_t *)output;
    *rets_size = sizeof(struct Output);

    return E_OK;
}

enum Status INTERVAL_fetch_buffer(
    __attribute__ ((unused)) uint8_t const **args,
    __attribute__ ((unused)) uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    if (!g_buffer) { return E_RESOURCE_NOT_INITIALIZED; }
    *rets = (uint8_t *)g_buffer;
    *rets_size = g_n_buffer_items;
    g_n_buffer_items = 0;
    // Transport layer frees buffer.
    return E_OK;
}
