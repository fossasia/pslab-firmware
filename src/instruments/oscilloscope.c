#include <stdbool.h>
#include <stdint.h>
#include <xc.h>

#include "../registers/converters/adc1.h"
#include "../registers/memory/dma.h"
#include "../registers/system/pin_manager.h"
#include "../registers/timers/tmr5.h"
#include "../bus/spi/spi.h"
#include "../bus/uart/uart.h"
#include "../helpers/buffer.h"
#include "../commands.h"

/* Static function prototypes */
static void Capture(void);
static void ResetTrigger(void);
static void SetTimeGap(void);

response_t OSCILLOSCOPE_CaptureOne(void) {
    SetCHANNELS(0); // Capture one channel.
    Capture();
    return SUCCESS;
}

response_t OSCILLOSCOPE_CaptureTwo(void) {
    SetCHANNELS(1); // Capture two channels.
    Capture();
    return SUCCESS;
}

response_t OSCILLOSCOPE_CaptureThree(void) {
    SetCHANNELS(2); // Capture four channels, but ignore the fourth.
    Capture();
    return SUCCESS;
}

response_t OSCILLOSCOPE_CaptureFour(void) {
    SetCHANNELS(3);  // Capture four channels.
    Capture();
    return SUCCESS;
}

static void Capture(void) {
    uint8_t config = UART1_Read();
    SetSAMPLES_REQUESTED(UART1_ReadInt());
    SetDELAY(UART1_ReadInt()); // Wait DELAY / 8 us between samples.

    uint8_t ch0sa = config & 0x0F;
    uint8_t ch123sa = config & 0x10;
    uint8_t trigger = config & 0x80;

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

    int i;
    for (i = 0; i <= GetCHANNELS(); i++) {
        SetBUFFER_IDX(i, &BUFFER[i * GetSAMPLES_REQUESTED()]);
    }

    SetCONVERSION_DONE(0);
    SetSAMPLES_CAPTURED(0);
    SetBUFFER_IDX(0, &BUFFER[0]);
    SetTimeGap();
    ADC1_InterruptFlagClear();
    ADC1_InterruptEnable();
    LED_SetLow();
}

response_t OSCILLOSCOPE_CaptureDMA(void) {
    uint8_t config = UART1_Read();
    SetSAMPLES_REQUESTED(UART1_ReadInt());
    SetDELAY(UART1_ReadInt());  // Wait DELAY / 8 us between samples.

    uint8_t ch0sa = config & 0x0F;
    uint8_t mode = config & 0x80 ? ADC1_12BIT_DMA_MODE : ADC1_10BIT_DMA_MODE;

    SetCHANNELS(0);  // Capture one channel.
    ADC1_SetOperationMode(mode, ch0sa, 0);

    DMA_StartAddressASet(DMA_CHANNEL_0, (uint16_t) &BUFFER[0]);
    DMA_PeripheralAddressSet(DMA_CHANNEL_0, (uint16_t) &ADC1BUF0);
    DMA_TransferCountSet(DMA_CHANNEL_0, GetSAMPLES_REQUESTED() - 1);
    DMA_FlagInterruptClear(DMA_CHANNEL_0);
    DMA_InterruptEnable(DMA_CHANNEL_0);
    DMA_ChannelEnable(DMA_CHANNEL_0);

    SetSAMPLES_CAPTURED(GetSAMPLES_REQUESTED());
    SetCONVERSION_DONE(1); // Assume it's all over already.
    SetTimeGap();
    LED_SetLow();

    return SUCCESS;
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

response_t OSCILLOSCOPE_GetCaptureStatus(void) {
    UART1_Write(GetCONVERSION_DONE());
    UART1_WriteInt(GetSAMPLES_CAPTURED());
    return SUCCESS;
}

response_t OSCILLOSCOPE_ConfigureTrigger(void) {
    uint8_t config = UART1_Read();
    uint8_t channelbits = config & 0x0F;

    int i;
    for (i = 0; i < MAX_CHANNELS; i++) {
        if (channelbits & (1 << i)) {
            SetTRIGGER_CHANNEL(i);
            break;
        }
    }

    SetTRIGGER_PRESCALER(config >> 4);
    SetTRIGGER_LEVEL(UART1_ReadInt());

    return SUCCESS;
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

response_t OSCILLOSCOPE_SetPGAGain(void) {
    tSPI_CS channel;

    switch (UART1_Read()) {
    case 1:
        channel = SPI_CH1;
        break;
    case 2:
        channel = SPI_CH2;
        break;
    default:
        UART1_Read(); // Consume remaining data.
        return FAILED;
    }

    enum Gain const gain = UART1_Read();

    if (gain >= GAIN_INVALID) {
        return FAILED;
    }

    uint16_t const write_register = 0x4000;
    uint16_t cmd = write_register | gain;

    SPI_Config const pga_config = {{{
        .PPRE = SPI_SCLK125000 >> 3,
        .SPRE = SPI_SCLK125000 & 7,
        .MSTEN = 1,
        .CKP = SPI_IDLE_LOW,
        .SSEN = 0,
        .CKE = SPI_SHIFT_TRAILING,
        .SMP = 1,
        .MODE16 = 1,
        .DISSDO = 0,
        .DISSCK = 0
    }}};

    if (SPI_configure(pga_config)) {
        LED_SetHigh();
        return SPI_exchange_int(channel, &cmd);
    }

    return FAILED;
}
