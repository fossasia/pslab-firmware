#include <stdint.h>
#include "../registers/converters/adc1.h"
#include "../registers/system/pin_manager.h"
#include "../registers/timers/tmr5.h"
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
static uint16_t TRIGGER_TIMEOUT = 100;
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
static void ResetTrigger(void);
static void Setup10BitMode(void);

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
    uint8_t pin = UART1_Read();
    SAMPLES_REQUESTED = UART1_ReadInt();
    DELAY = UART1_ReadInt();

    CHANNELS = 0; //capture one channel
    AD1CON2bits.CHPS = 0;
    ADC1_SetOperationMode(ADC1_10BIT_SIMULTANEOUS_MODE, pin & 0x7F, 0);
    AD1CON2bits.CHPS = 0;

    if (pin & 0x80) ResetTrigger();
    else TRIGGERED = 1;

    SAMPLES_CAPTURED = 0;
    BUFFER_IDX[0] = &BUFFER[0];
    Setup10BitMode();
    ADC1_InterruptFlagClear();
    ADC1_InterruptEnable();
    LED_SetLow();
    
    return SUCCESS;
}

static void ResetTrigger(void) {
    TRIGGER_WAITING = 0;
    TRIGGER_READY = 0;
    TRIGGERED = 0;
}

static void Setup10BitMode(void) {
    T5CONbits.TCKPS = 1;
    TMR5_Period16BitSet(DELAY - 1);
    TMR5 = 0;
    TMR5_Start();
}

response_t OSCILLOSCOPE_GetCaptureStatus(void) {
    uint8_t conversion_done;
    conversion_done = SAMPLES_CAPTURED == SAMPLES_REQUESTED ? 1 : 0;
    UART1_Write(conversion_done);
    UART1_WriteInt(SAMPLES_CAPTURED);
    return SUCCESS;
}
