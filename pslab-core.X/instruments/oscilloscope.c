#include <stdint.h>
#include "../registers/converters/adc1.h"
#include "../registers/system/pin_manager.h"
#include "../registers/timers/tmr5.h"
#include "../bus/spi/spi_driver.h"
#include "../bus/uart/uart1.h"
#include "../helpers/buffer.h"
#include "../commands.h"

#define MAX_CHANNELS 4

/* Static globals */
static uint8_t volatile TRIGGERED = 0;
static uint8_t volatile TRIGGER_READY = 0;
static uint8_t TRIGGER_CHANNEL = 0;
static uint8_t CHANNELS = 0;

static uint16_t DELAY;
static const uint16_t TRIGGER_TIMEOUT = 50000;
static uint16_t volatile TRIGGER_WAITING = 0;
static uint16_t TRIGGER_LEVEL = 0;
static uint16_t TRIGGER_PRESCALER = 0;

static int16_t volatile* volatile BUFFER_IDX[MAX_CHANNELS];
static uint16_t volatile* ADCVALS[MAX_CHANNELS] = {
    &ADC1BUF0, &ADC1BUF1, &ADC1BUF2, &ADC1BUF3
};
static uint16_t volatile SAMPLES_CAPTURED;
static uint16_t SAMPLES_REQUESTED;

/* Static function prototypes */
static void Capture(void);
static void ResetTrigger(void);
static void SetTimeGap(void);
static void SetCS(uint8_t channel, uint8_t value);

/**
 * @brief Handle trigger and data collection from ADC.
 * 
 * @description
 * This interrupt handler is called every time the ADC finishes a conversion, if
 * the ADC interrupt is enabled. It checks if the trigger condition is
 * fulfilled, and if so copies ADC values into the buffer.
 */
void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void) {
    uint16_t adval;
    int i;
    
    ADC1_InterruptFlagClear();
    LED_SetHigh();
    
    if (TRIGGERED) {
        for (i = 0; i <= CHANNELS; i++) *(BUFFER_IDX[i]++) = *ADCVALS[i];
        
        SAMPLES_CAPTURED++;
        LED_SetLow();
        if (SAMPLES_CAPTURED == SAMPLES_REQUESTED) {
            ADC1_InterruptDisable();
            LED_SetHigh();
        }
    } else {
        for (i = 0; i < MAX_CHANNELS; i++) {
            if (TRIGGER_CHANNEL & 1 << i) adval = *ADCVALS[i];
        }

        // If the trigger hasn't timed out, check for trigger condition.
        if (TRIGGER_WAITING < TRIGGER_TIMEOUT) {
            TRIGGER_WAITING += (DELAY >> TRIGGER_PRESCALER);
            if (!TRIGGER_READY && adval > TRIGGER_LEVEL + 10)TRIGGER_READY = 1;
            else if (adval <= TRIGGER_LEVEL && TRIGGER_READY) {
                TRIGGERED = 1;
            }
        }
        // If the trigger has timed out, then proceed to data acquisition.
        else {
            TRIGGERED = 1;
        }
    }
}

response_t OSCILLOSCOPE_CaptureOne(void) {
    CHANNELS = 0; // Capture one channel.
    Capture();    
    return SUCCESS;
}

response_t OSCILLOSCOPE_CaptureTwo(void) {
    CHANNELS = 1; // Capture two channels.
    Capture();
    return SUCCESS;
}

response_t OSCILLOSCOPE_CaptureThree(void) {
    CHANNELS = 2; // Capture four channels, but ignore the fourth.
    Capture();
    return SUCCESS;
}

response_t OSCILLOSCOPE_CaptureFour(void) {
    CHANNELS = 3; // Capture four channels.
    Capture();
    return SUCCESS;
}

static void Capture(void) {
    uint8_t config = UART1_Read();
    SAMPLES_REQUESTED = UART1_ReadInt();
    DELAY = UART1_ReadInt();  // Wait DELAY / 8 us between samples.

    uint8_t ch0sa = config & 0x0F;
    uint8_t ch123sa = config & 0x10;
    uint8_t trigger = config & 0x80;

    AD1CON2bits.CHPS = CHANNELS;
    ADC1_SetOperationMode(ADC1_10BIT_SIMULTANEOUS_MODE, ch0sa, ch123sa);
    AD1CON2bits.CHPS = CHANNELS;

    if (trigger) ResetTrigger();
    else TRIGGERED = 1;

    int i;
    for (i = 0; i <= CHANNELS; i++) {
        BUFFER_IDX[i] = &BUFFER[i * SAMPLES_REQUESTED];
    }
    
    SAMPLES_CAPTURED = 0;
    BUFFER_IDX[0] = &BUFFER[0];
    SetTimeGap();
    ADC1_InterruptFlagClear();
    ADC1_InterruptEnable();
    LED_SetLow();   
}

static void ResetTrigger(void) {
    TRIGGER_WAITING = 0;
    TRIGGER_READY = 0;
    TRIGGERED = 0;
}

static void SetTimeGap(void) {
    T5CONbits.TCKPS = 1; // Prescaler; Slow down T5 clock to 8 MHz.
    TMR5_Period16BitSet(DELAY - 1);
    TMR5 = 0;
    TMR5_Start();
}

response_t OSCILLOSCOPE_GetCaptureStatus(void) {
    uint8_t conversion_done;
    conversion_done = SAMPLES_CAPTURED == SAMPLES_REQUESTED;
    UART1_Write(conversion_done);
    UART1_WriteInt(SAMPLES_CAPTURED);
    return SUCCESS;
}

response_t OSCILLOSCOPE_ConfigureTrigger(void) {
    uint8_t config = UART1_Read();
    TRIGGER_CHANNEL = config & 0x03;
    TRIGGER_PRESCALER = config >> 4;
    TRIGGER_LEVEL = UART1_ReadInt();
    return SUCCESS;
}

response_t OSCILLOSCOPE_SetPGAGain(void) {
    uint8_t channel = UART1_Read();
    uint8_t gain = UART1_Read();
    uint16_t write_register = 0x4000;
    uint16_t cmd = write_register | gain;

    SPI_DRIVER_Close();
    SPI_DRIVER_Open(PGA_CONFIG);
    SetCS(channel, 0);
    SPI_DRIVER_ExchangeWord(cmd);
    SetCS(channel, 1);
    SPI_DRIVER_Close();
    LED_SetHigh();

    return SUCCESS;
}

static void SetCS(uint8_t channel, uint8_t value) {
    switch(channel) {
        case 1:
            CS_CH1_Setter = value;
            break;
        case 2:
            CS_CH2_Setter = value;
            break;
        default:
            return;
    }
}
