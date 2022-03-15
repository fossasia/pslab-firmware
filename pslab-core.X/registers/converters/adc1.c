#include "adc1.h"
#include "../memory/dma.h"
#include "../timers/tmr5.h"
#include "../../helpers/delay.h"
#include "../../commands.h"
#include "../system/pin_manager.h"

/* Static globals */
static uint16_t volatile* ADCVALS[MAX_CHANNELS] = {
    &ADC1BUF0, &ADC1BUF1, &ADC1BUF2, &ADC1BUF3
};

/* Static function prototypes */
static void Init10BitMode(void);
static void InitDMAMode(ADC1_RESOLUTION_TYPE resolution);
static void InitModesCommon(void);
static void EnableDMA(void);

static uint8_t volatile TRIGGERED = 0;
void SetTRIGGERED(uint8_t V) { TRIGGERED = V; }
uint8_t GetTRIGGERED(void) { return TRIGGERED; }

static uint16_t volatile TRIGGER_WAITING = 0;
void SetTRIGGER_WAITING(uint16_t V) { TRIGGER_WAITING = V; }
uint16_t GetTRIGGER_WAITING(void) { return TRIGGER_WAITING; }

static uint8_t TRIGGER_CHANNEL = 0;
void SetTRIGGER_CHANNEL(uint8_t V) { TRIGGER_CHANNEL = V; }
uint8_t GetTRIGGER_CHANNEL(void) { return TRIGGER_CHANNEL; }

static uint8_t volatile TRIGGER_READY = 0;
void SetTRIGGER_READY(uint8_t V) { TRIGGER_READY = V; }
uint8_t GetTRIGGER_READY(void) { return TRIGGER_READY; }

static uint16_t TRIGGER_LEVEL = 0;
void SetTRIGGER_LEVEL(uint16_t V) { TRIGGER_LEVEL = V; }
uint16_t GetTRIGGER_LEVEL(void) { return TRIGGER_LEVEL; }

static uint16_t TRIGGER_PRESCALER = 0;
void SetTRIGGER_PRESCALER(uint16_t V) { TRIGGER_PRESCALER = V; }
uint16_t GetTRIGGER_PRESCALER(void) { return TRIGGER_PRESCALER; }

static uint8_t CHANNELS = 0;
void SetCHANNELS(uint8_t V) { CHANNELS = V; }
uint8_t GetCHANNELS(void) { return CHANNELS; }

static uint16_t SAMPLES_REQUESTED;
void SetSAMPLES_REQUESTED(uint16_t V) { SAMPLES_REQUESTED = V; }
uint16_t GetSAMPLES_REQUESTED(void) { return SAMPLES_REQUESTED; }

static uint16_t volatile SAMPLES_CAPTURED;
void SetSAMPLES_CAPTURED(uint16_t V) { SAMPLES_CAPTURED = V; }
uint16_t GetSAMPLES_CAPTURED(void) { return SAMPLES_CAPTURED; }

static uint16_t DELAY;
void SetDELAY(uint16_t V) { DELAY = V; }
uint16_t GetDELAY(void) { return DELAY; }

static uint8_t volatile CONVERSION_DONE = 1;
void SetCONVERSION_DONE(uint8_t V) { CONVERSION_DONE = V; }
uint8_t GetCONVERSION_DONE(void) { return CONVERSION_DONE; }

static uint16_t volatile* volatile BUFFER_IDX[MAX_CHANNELS];
void SetBUFFER_IDX(uint8_t idx, volatile uint16_t *V) {
    BUFFER_IDX[idx] = V;
}

/**
 * @brief Handle trigger and data collection from ADC.
 * 
 * @description
 * This interrupt handler is called every time the ADC finishes a conversion, if
 * the ADC interrupt is enabled. It checks if the trigger condition is
 * fulfilled, and if so copies ADC values into the buffer.
 */
void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void) {
    int i;

    ADC1_InterruptFlagClear();

    if (CONVERSION_DONE) {
        // Usually in methods like capacitance measurement, this ISR will be
        // triggered, but no need to process further. Hence abort and return.
        return;
    }
    LED_SetHigh();

    if (TRIGGERED) {
        for (i = 0; i <= CHANNELS; i++) *(BUFFER_IDX[i]++) = *ADCVALS[i];

        SAMPLES_CAPTURED++;
        LED_SetLow();
        if (SAMPLES_CAPTURED == SAMPLES_REQUESTED) {
            ADC1_InterruptDisable();
            ADC1_InterruptFlagClear();
            CONVERSION_DONE = 1;
            LED_SetHigh();
        }
    } else {
        uint16_t adval;
        for (i = 0; i < MAX_CHANNELS; i++) {
            if (TRIGGER_CHANNEL & (1 << i)) adval = *ADCVALS[i];
        }

        // If the trigger hasn't timed out, check for trigger condition.
        if (TRIGGER_WAITING < TRIGGER_TIMEOUT) {
            TRIGGER_WAITING += (DELAY >> TRIGGER_PRESCALER);
            if (!TRIGGER_READY && adval > TRIGGER_LEVEL + 10) TRIGGER_READY = 1;
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

void ADC1_Initialize(void) {

    ADC1_InitializeCON1();
    ADC1_InitializeCON2();
    ADC1_InitializeCON3();
    ADC1_InitializeCON4();
    // CH0SA AN0; CH0SB AN0; CH0NB AVSS; CH0NA AVSS; 
    AD1CHS0 = 0x00;
    // CSS26 disabled; CSS25 disabled; CSS24 disabled; CSS31 disabled; CSS30 disabled; 
    AD1CSSH = 0x00;
    // CSS2 disabled; CSS1 disabled; CSS0 disabled; CSS8 disabled; CSS7 disabled; CSS6 disabled; CSS5 disabled; CSS4 disabled; CSS3 disabled; 
    AD1CSSL = 0x00;
    // CH123SA CH1=OA2/AN0,CH2=AN1,CH3=AN2; CH123SB CH1=OA2/AN0,CH2=AN1,CH3=AN2; CH123NA CH1=VREF-,CH2=VREF-,CH3=VREF-; CH123NB CH1=VREF-,CH2=VREF-,CH3=VREF-; 
    AD1CHS123 = 0x00;

    ADC1_InterruptDisable();
    ADC1_InterruptFlagClear();
}

void ADC1_InitializeCON1(void) {
    // ADC module is off
    AD1CON1bits.ADON = 0;
    // Continues mode operation in idle mode
    AD1CON1bits.ADSIDL = 0;
    // DMA buffers are written in Scatter/Gather mode; 
    // the module provides a Scatter/Gather address to the DMA channel, based
    // on the index of the analog input and the size of the DMA buffer.
    AD1CON1bits.ADDMABM = 0;
    // 10-bit, 4 channel ADC operation
    AD1CON1bits.AD12B = 0;
    // Data output format: Integer (0000 00DD DDDD DDDD)
    AD1CON1bits.FORM = 0b00;
    // Clearing the Sample bit (SAMP) ends sampling and starts conversion
    AD1CON1bits.SSRC = 0b000;
    // Sample Trigger Source Group
    AD1CON1bits.SSRCG = 0;
    // Samples multiple channels individually in sequence
    AD1CON1bits.SIMSAM = 0;
    // Sampling begins when the SAMP bit is set
    AD1CON1bits.ASAM = 0;
    // ADC Sample Enable: Sample-and-Hold amplifiers are holding
    AD1CON1bits.SAMP = 0;
    // ADC conversion has not started or is in progress
    AD1CON1bits.DONE = 0;
}

void ADC1_InitializeCON2(void) {
    // Converter voltage reference is AVDD and AVSS
    AD1CON2bits.VCFG = 0b000;
    // Does not scan inputs
    AD1CON2bits.CSCNA = 0;
    // Converts CH0 (default)
    AD1CON2bits.CHPS = 0b00;
    // Buffer fill status: filling the first half of the buffer
    AD1CON2bits.BUFS = 0;
    // Always starts filling the buffer from the start address
    AD1CON2bits.BUFM = 0;
    // Generates interrupt after completion of every sample/conversion operation
    AD1CON2bits.SMPI = 0b00000;
    // Always uses channel input selects for Sample MUXA
    AD1CON2bits.ALTS = 0;
}

void ADC1_InitializeCON3(void) {
    // Clock derived from system clock
    AD1CON3bits.ADRC = 0;
    // Auto-sample time bits: <REG_VAL>*TAD
    AD1CON3bits.SAMC = 0b00000;
    // ADC conversion clock select bits: <REG_VAL + 1>*TP = TAD
    AD1CON3bits.ADCS = 0b00000000;
}

void ADC1_InitializeCON4(void) {
    // Conversion results are stored in ADC1BUF0 to ADC1BUFF registers; No DMA
    AD1CON4bits.ADDMAEN = 0;
    // Allocates 1 word of buffer to each analog input
    AD1CON4bits.DMABL = 0b000;
}

void ADC1_SetOperationMode(
        ADC1_PSLAB_MODES mode, uint8_t channel_0, uint8_t channel_123) { 
    if (
            channel_0 == CH0_CHANNEL_RES || 
            channel_0 == CH0_CHANNEL_CAP) {
        CM4CONbits.CON = 0;
        PMD3bits.CMPMD = 1;
    }

    ADC1_Initialize();
    // Set input channels.
    ADC1_ChannelSelectSet(channel_0);
    ADC1_Positive123ChannelSelect(channel_123);

    switch (mode) {
        case ADC1_10BIT_SIMULTANEOUS_MODE:
            Init10BitMode();
            break;
        case ADC1_10BIT_DMA_MODE:
            InitDMAMode(ADC1_RESOLUTION_10_BIT);
            break;
        case ADC1_12BIT_DMA_MODE:
            InitDMAMode(ADC1_RESOLUTION_12_BIT);
            break;
        case ADC1_12BIT_NORMAL_MODE:
            // initADC12()
            break;
        case ADC1_12BIT_SCOPE_MODE:
            // initADC12bit_scope()
            break;
        case ADC1_12BIT_AVERAGING_MODE:
            // Disable DMA channel
            DMA_ChannelDisable(DMA_CHANNEL_0);
            ADC1_ResolutionModeSet(ADC1_RESOLUTION_12_BIT);
            // Internal counter ends sampling and starts auto conversion
            AD1CON1bits.SSRC = 0b111;
            // Generate interrupt after 16th sample conversion
            AD1CON2bits.SMPI = 0b01111;
            // Clock settings
            AD1CON3bits.SAMC = 0b10000; // 16*TAD auto sample time
            ADC1_ConversionClockPrescalerSet(10 + 1); // TAD = X*11 = 171.875 ns
            break;
        case ADC1_CTMU_MODE:
            // Disable DMA channel
            DMA_ChannelDisable(DMA_CHANNEL_0);
            ADC1_ResolutionModeSet(ADC1_RESOLUTION_12_BIT);
            // Disable Comparator module
            CM4CONbits.CON = 0;
            PMD3bits.CMPMD = 1;
            // Clock settings
            AD1CON3bits.SAMC = 0b10000; // 16*TAD auto sample time
            ADC1_ConversionClockPrescalerSet(10 + 1); // TAD = X*11 = 171.875 ns
            break;
        default:
            break;
    }
}

static void Init10BitMode(void) {
    DMA_ChannelDisable(DMA_CHANNEL_0);
    InitModesCommon();
}

static void InitDMAMode(ADC1_RESOLUTION_TYPE resolution) {
    ADC1_ResolutionModeSet(resolution);
    InitModesCommon();

    DMA_Initialize();
    DMA_SetOneShotMode(DMA_CHANNEL_0);
    DMA_PeripheralIrqNumberSet(DMA_CHANNEL_0, DMA_PERIPHERAL_IRQ_ADC1);
    DMA_SetLogicAnalyzerChannelMode(DMA_MODES_ONE_CHANNEL);
    EnableDMA();
}

static void InitModesCommon(void) {
    ADC1_SelectSampleTrigger(ADC1_TMR5_SOURCE);
    ADC1_AutomaticSamplingEnable();
    ADC1_SimultaneousSamplingEnable();
    ADC1_ConversionClockPrescalerSet(2); // Conversion rate = 16 MHz
    ADC1_Enable();
    DELAY_us(20);
}

static void EnableDMA(void) {
    AD1CON1bits.ADDMABM = 1;
    AD1CON4bits.ADDMAEN = 1;
}

void ADC1_WaitForInterruptEvent(void) {
    ADC1_InterruptFlagClear();
    while (!_AD1IF);
    ADC1_InterruptFlagClear();
}
