#include <stdint.h>
#include <string.h>

#include "../commands.h"
#include "../helpers/delay.h"
#include "../registers/converters/adc1.h"
#include "../registers/converters/ctmu.h"
#include "../registers/memory/dma.h"
#include "../registers/system/pin_manager.h"
#include "../registers/timers/tmr5.h"
#include "multimeter.h"

enum {
    DISCHARGE,
    CHARGE,
};

/* Function prototypes */
void ChargeCapacitor(uint8_t charge, uint16_t period);
void GetCapacitance_InitCTMU_TMR5(uint8_t current_range, uint8_t trim,
        uint16_t charge_time);
void GetCapacitance_ConfigADC_CTMU_TMR5();

void ChargeCapacitor(uint8_t charge, uint16_t period) {
    CAP_OUT_SetDigitalOutput();
    CAP_OUT_SetDigital();

    if (charge) {
        CAP_OUT_SetHigh();
    } else {
        CAP_OUT_SetLow();
    }

    // Wait out until the capacitor is charged or discharged
    DELAY_us(period);

    CAP_OUT_SetLow();
    // Switch to high impedance state
    CAP_OUT_SetDigitalInput();
    CAP_OUT_SetAnalog();
}

void GetCapacitance_InitCTMU_TMR5(uint8_t current_range, uint8_t trim,
        uint16_t charge_time) {
    CTMU_Initialize();
    // Enables edge delay generation
    CTMUCON1bits.TGEN = 1;
    // Set current level for current source
    CTMUICONbits.ITRIM = trim;
    // Current range for current source (0.53uA for base current)
    CTMUICONbits.IRNG = current_range;

    TMR5_Initialize();
    TMR5_SetPrescaler(TMR_PRESCALER_64);
    TMR5_Period16BitSet(charge_time);
}

void GetCapacitance_ConfigADC_CTMU_TMR5() {
    // Begin ADC sampling
    ADC1_Enable();
    DELAY_us(20);
    ADC1_SoftwareTriggerEnable();

    CTMU_Enable();
    DELAY_us(1000);
    // Analog current source output is grounded
    CTMUCON1bits.IDISSEN = 1;
    DELAY_us(1500);
    // Disconnect current source from ground
    CTMUCON1bits.IDISSEN = 0;
    // Edge 1 has occurred and capacitor is now being charged
    CTMUCON2bits.EDG1STAT = 1;

    TMR5_Start();
    TMR5_WaitForInterruptEvent();
}

uint16_t GetVoltage_Summed(uint8_t channel) {

    ADC1_SetOperationMode(ADC1_12BIT_AVERAGING_MODE, channel, 0);

    ADC1_Enable();
    DELAY_us(20);
    ADC1_AutomaticSamplingEnable();

    ADC1_WaitForInterruptEvent();

    while (!ADC1_IsConversionComplete());

    ADC1_AutomaticSamplingDisable();
    ADC1_Disable();

    uint16_t voltage_sum =
            (ADC1BUF0) + (ADC1BUF1) + (ADC1BUF2) + (ADC1BUF3) +
            (ADC1BUF4) + (ADC1BUF5) + (ADC1BUF6) + (ADC1BUF7) +
            (ADC1BUF8) + (ADC1BUF9) + (ADC1BUFA) + (ADC1BUFB) +
            (ADC1BUFC) + (ADC1BUFD) + (ADC1BUFE) + (ADC1BUFF);

    return voltage_sum;
}

enum Status MULTIMETER_get_voltage(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    struct Input {
        uint8_t channel;
        uint8_t _pad[0];
    } *input = (struct Input *)args;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) {
        return E_BAD_ARGSIZE;
    }

    ADC1_InterruptDisable();
    ADC1_InterruptFlagClear();

    DMA_ChannelDisable(DMA_CHANNEL_0);

    ADC1_Disable();

    ADC1_InitializeCON1();
    AD1CON1bits.AD12B = 1;
    AD1CON1bits.SSRC = 0b111;

    ADC1_InitializeCON2();

    AD1CON3bits.ADRC = 1;
    AD1CON3bits.SAMC = 0b11111;
    AD1CON3bits.ADCS = 0b00001001;

    ADC1_InitializeCON4();

    AD1CHS0bits.CH0SA = input->channel & 0xF;
    AD1CHS0bits.CH0NA = 0;

    AD1CSSH = 0x0000;
    AD1CSSL = 0x0000;

    ADC1_Enable();
    DELAY_us(20);
    ADC1_SoftwareTriggerEnable();

    while (!ADC1_IsConversionComplete());

    uint16_t result = ADC1_ConversionResultGet(channel_CTMU);
    *rets = args;
    *rets_size = sizeof(result);
    memcpy(*rets, &result, *rets_size);

    return E_OK;
}

enum Status MULTIMETER_get_voltage_summed(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    struct Input {
        uint8_t channel;
        uint8_t _pad[0];
    } *input = (struct Input *)args;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) {
        return E_BAD_ARGSIZE;
    }



    uint16_t result = GetVoltage_Summed(input->channel);
    *rets_size = sizeof(result);
    *rets = args;
    memcpy(*rets, &result, *rets_size);

    return E_OK;
}

enum Status MULTIMETER_charge_capacitor(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    struct Input {
        uint8_t charge;
        uint16_t period;
        uint8_t _pad[1];
    } *input = (struct Input *)args;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) { return E_BAD_ARGSIZE; }

    ChargeCapacitor(input->charge, input->period);
    return E_OK;
}

enum Status MULTIMETER_get_cap_range(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    struct Input {
        uint16_t charge_time;
        uint8_t _pad[0];
    } *input = (struct Input *)args;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) {return E_BAD_ARGSIZE;}

    ChargeCapacitor(CHARGE, 50000);

    ADC1_SetOperationMode(ADC1_12BIT_AVERAGING_MODE, CH0_CHANNEL_CAP, 0);

    TMR5_Initialize();
    TMR5_Period16BitSet(input->charge_time);
    TMR5_SetPrescaler(TMR_PRESCALER_64);
    TMR5_Start();

    // Start charging the capacitor through 10K resistor
    CAP_OUT_SetDigitalOutput();
    CAP_OUT_SetHigh();

    TMR5_WaitForInterruptEvent();

    // Stop the charging process
    CAP_OUT_SetDigitalInput();
    CAP_OUT_SetLow();

    uint16_t range = GetVoltage_Summed(CH0_CHANNEL_CAP);
    *rets = args;
    *rets_size = sizeof(range);
    memcpy(*rets, &range, *rets_size);

    return E_OK;
}

enum Status MULTIMETER_get_capacitance(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    struct Input {
        uint8_t current_range;
        uint8_t trim;
        uint16_t charge_time;
        uint8_t _pad[0];
    } *input = (struct Input *)args;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) { return E_BAD_ARGSIZE; }

    LED_SetLow();

    ADC1_SetOperationMode(ADC1_CTMU_MODE, CH0_CHANNEL_CAP, 0);
    // Initiate CTMU and TMR5 registers to measure capacitance discharge rate
    GetCapacitance_InitCTMU_TMR5(input->current_range, input->trim, input->charge_time);
    // Fully discharge the measuring capacitor
    ChargeCapacitor(DISCHARGE, 50000);
    // Configure ADC, CTMU and TMR5 register bits to start measuring
    GetCapacitance_ConfigADC_CTMU_TMR5();
    // Stop sampling
    ADC1_SoftwareTriggerDisable();
    // Pause charging the capacitor
    CTMUCON2bits.EDG1STAT = 0;

    ADC1_WaitForInterruptEvent();
    while (!ADC1_IsConversionComplete());

    uint16_t result = (ADC1BUF0) & 0xFFF;

    // Reset modules
    CTMU_Initialize();
    ADC1_Disable();

    *rets = args;
    *rets_size = sizeof(result);
    memcpy(*rets, &result, *rets_size);

    LED_SetHigh();

    return E_OK;
}

enum Status MULTIMETER_get_ctmu_volts(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    uint8_t config = 0;

    if (args_size != sizeof(config)) {
        return E_BAD_ARGSIZE;
    }

    config = *args;

    CTMU_Initialize();
    // Edge delay generation
    CTMUCON1bits.TGEN = (config >> 7) & 0x1;
    // Current source output
    CTMUICONbits.IRNG = (config >> 5) & 0x3;

    // Internal temperature
    if ((config & 0x1F) == 30) CTMU_EnableEdge2();

    CTMU_Enable();
    DELAY_us(1000);
    CTMU_DrainOutput();
    DELAY_us(1500);
    CTMU_FloatOutput();
    CTMU_EnableEdge1();

    uint16_t result = GetVoltage_Summed(config & 0x1F);

    CTMU_DisableModule();

    *rets = args;
    *rets_size = sizeof(result);
    memcpy(*rets, &result, *rets_size);

    return E_OK;
}
