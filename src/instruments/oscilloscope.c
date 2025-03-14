#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <xc.h>

#include "../registers/converters/adc1.h"
#include "../registers/memory/dma.h"
#include "../registers/system/pin_manager.h"
#include "../registers/timers/tmr5.h"
#include "../bus/spi/spi.h"
#include "../helpers/buffer.h"
#include "../commands.h"

/* Static function prototypes */
static enum Status capture(
    uint8_t args[],
    uint16_t args_size,
    uint8_t *rets[],
    uint16_t *rets_size
);
static void ResetTrigger(void);
static void SetTimeGap(void);

enum Status OSCILLOSCOPE_fetch_samples(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *const rets_size
) {
    struct Input {
        uint16_t samples;
        uint8_t _pad[0];
    } *input = (struct Input *)args;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) { return E_BAD_ARGSIZE; }

    *rets = (uint8_t *)&BUFFER_sample_buffer;
    *rets_size = input->samples * sizeof(*BUFFER_sample_buffer);

    return E_OK;
}

enum Status OSCILLOSCOPE_capture_one(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *rets_size
) {
    SetCHANNELS(0); // Capture one channel.
    return capture(args, args_size, rets, rets_size);
}

enum Status OSCILLOSCOPE_capture_two(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *rets_size
) {
    SetCHANNELS(1); // Capture two channels.
    return capture(args, args_size, rets, rets_size);
}

enum Status OSCILLOSCOPE_capture_three(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *rets_size
) {
    SetCHANNELS(2); // Capture four channels, but ignore the fourth.
    return capture(args, args_size, rets, rets_size);
}

enum Status OSCILLOSCOPE_capture_four(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *rets_size
) {
    SetCHANNELS(3);  // Capture four channels.
    return capture(args, args_size, rets, rets_size);
}

static enum Status capture(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__((unused)) uint8_t *rets[],
    __attribute__((unused)) uint16_t *rets_size
) {
    struct Input {
        uint8_t config;
        uint16_t samples;
        uint16_t delay;
        uint8_t _pad[1];
    } *input = (struct Input *)args;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) { return E_BAD_ARGSIZE; }

    SetSAMPLES_REQUESTED(input->samples);
    SetDELAY(input->delay); // Wait DELAY / 8 us between samples.

    uint8_t ch0sa = input->config & 0x0F;
    uint8_t ch123sa = input->config & 0x10;
    uint8_t trigger = input->config & 0x80;

    ADC1_SetOperationMode(ADC1_10BIT_SIMULTANEOUS_MODE, ch0sa, ch123sa);

    /* Check if the trigger channel is sampled. If not, convert the trigger
     * channel in addition to the sampled channels. */
    if (trigger && GetTRIGGER_CHANNEL() > GetCHANNELS()) {
        ADC1_ConversionChannelsSet(GetTRIGGER_CHANNEL());
        ResetTrigger();
    } else if (trigger) {
        ADC1_ConversionChannelsSet(GetCHANNELS());
        ResetTrigger();
    } else {
        ADC1_ConversionChannelsSet(GetCHANNELS());
        SetTRIGGERED(1);
    }

    for (int i = 0; i <= GetCHANNELS(); ++i) {
        SetBUFFER_IDX(i, &BUFFER_sample_buffer[i * GetSAMPLES_REQUESTED()]);
    }

    SetCONVERSION_DONE(0);
    SetSAMPLES_CAPTURED(0);
    SetBUFFER_IDX(0, &BUFFER_sample_buffer[0]);
    SetTimeGap();
    ADC1_InterruptFlagClear();
    ADC1_InterruptEnable();
    LED_SetLow();

    return E_OK;
}

enum Status OSCILLOSCOPE_capture_dma(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__((unused)) uint8_t *rets[],
    __attribute__((unused)) uint16_t *rets_size
) {
    struct Input {
        uint8_t config;
        uint16_t samples;
        uint16_t delay;
        uint8_t _pad[1];
    } *input = (struct Input *)args;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) { return E_BAD_ARGSIZE; }

    SetSAMPLES_REQUESTED(input->samples);
    SetDELAY(input->delay);  // Wait DELAY / 8 us between samples.

    uint8_t ch0sa = input->config & 0x0F;
    uint8_t mode = input->config & 0x80 ? ADC1_12BIT_DMA_MODE : ADC1_10BIT_DMA_MODE;

    SetCHANNELS(0);  // Capture one channel.
    ADC1_SetOperationMode(mode, ch0sa, 0);

    DMA_StartAddressASet(DMA_CHANNEL_0, (uint16_t) &BUFFER_sample_buffer[0]);
    DMA_PeripheralAddressSet(DMA_CHANNEL_0, (uint16_t) &ADC1BUF0);
    DMA_TransferCountSet(DMA_CHANNEL_0, GetSAMPLES_REQUESTED() - 1);
    DMA_FlagInterruptClear(DMA_CHANNEL_0);
    DMA_InterruptEnable(DMA_CHANNEL_0);
    DMA_ChannelEnable(DMA_CHANNEL_0);

    SetSAMPLES_CAPTURED(GetSAMPLES_REQUESTED());
    SetCONVERSION_DONE(1); // Assume it's all over already.
    SetTimeGap();
    LED_SetLow();

    return E_OK;
}

static void ResetTrigger(void) {
    SetTRIGGER_WAITING(0);
    SetTRIGGER_READY(0);
    SetTRIGGERED(0);
}

static void SetTimeGap(void) {
    TMR5_Initialize();
    TMR5_StopWhenIdle();
    TMR5_Period16BitSet(GetDELAY() - 1);
    TMR5_SetPrescaler(TMR_PRESCALER_8);
    TMR5_InterruptFlagClear();
    TMR5_InterruptDisable();
    TMR5_Start();
}

enum Status OSCILLOSCOPE_get_capture_status(
    uint8_t args[],
    __attribute__((unused)) uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *rets_size
) {
    struct Output {
        uint8_t done;
        uint16_t progress;
        uint8_t _pad[1];
    } output = {0};

    output.done = GetCONVERSION_DONE();
    output.progress = GetSAMPLES_CAPTURED();
    *rets = args;
    *rets_size = sizeof(output) - sizeof(output._pad);
    memcpy(*rets, &output, *rets_size);

    return E_OK;
}

enum Status OSCILLOSCOPE_configure_trigger(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__((unused)) uint8_t *rets[],
    __attribute__((unused)) uint16_t *rets_size
) {
    struct Input {
        uint8_t config;
        uint16_t level;
        uint8_t _pad[1];
    } *input = (struct Input *)args;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) { return E_BAD_ARGSIZE; }

    uint8_t channelbits = input->config & 0x0F;

    for (int i = 0; i < MAX_CHANNELS; i++) {
        if (channelbits & (1 << i)) {
            SetTRIGGER_CHANNEL(i);
            break;
        }
    }

    SetTRIGGER_PRESCALER(input->config >> 4);
    SetTRIGGER_LEVEL(input->level);

    return E_OK;
}

enum Gain {
    GAIN_X1,
    GAIN_X2,
    GAIN_X4,
    GAIN_X5,
    GAIN_X8,
    GAIN_X10,
    GAIN_X16,
    GAIN_X32,
    GAIN_INVALID,
};

enum Status OSCILLOSCOPE_set_pga_gain(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__((unused)) uint8_t *rets[],
    __attribute__((unused)) uint16_t *rets_size
) {
    struct Input {
        uint8_t pin;
        uint8_t gain;
        uint8_t _pad[0];
    } *input = (struct Input *)args;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) { return E_BAD_ARGSIZE; }


    enum SPI_CS channel;

    switch (input->pin) {
    case 1:
        channel = SPI_CS_CH1;
        break;
    case 2:
        channel = SPI_CS_CH2;
        break;
    default:
        return E_BAD_ARGUMENT;
        break;
    }

    if (input->gain >= GAIN_INVALID) { return E_BAD_ARGUMENT; }

    uint16_t const write_register = 0x4000;
    uint16_t cmd = write_register | input->gain;

    enum Status status = E_OK;

    if ((status = SPI_set_mode(SPI_CPOL_LOW, SPI_CKE_TRAILING))) {
        return status;
    }
    if ((status = SPI_set_clock(SPI_SCLK_125000))) { return status; }
    if ((status = SPI_set_word_size(SPI_WORD_SIZE_16))) { return status; }

    LED_SetHigh();
    return SPI_exchange((uint8_t *)&cmd, NULL, sizeof(cmd));
}
