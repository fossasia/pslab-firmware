#include "../commands.h"
#include "../helpers/delay.h"
#include "../registers/converters/adc1.h"
#include "../registers/memory/dma.h"
#include "../bus/uart1.h"
#include "logicanalyzer.h"

response_t MULTIMETER_GetVoltage(void) {

    uint8_t channel = UART1_Read();

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

    AD1CHS0bits.CH0SA = channel & 0xF;
    AD1CHS0bits.CH0NA = 0;

    AD1CSSH = 0x0000;
    AD1CSSL = 0x0000;

    ADC1_Enable();
    DELAY_us(20);
    ADC1_SoftwareTriggerEnable();

    while (!ADC1_IsConversionComplete(channel_CTMU));

    UART1_WriteInt(ADC1_ConversionResultGet(channel_CTMU));

    return SUCCESS;
}

response_t MULTIMETER_GetVoltageSummed(void) {
    
    uint8_t channel = UART1_Read();
    
    ADC1_SetOperationMode(ADC1_12BIT_AVERAGING_MODE, channel, 0);
    
    ADC1_Enable();
    DELAY_us(20);
    ADC1_AutomaticSamplingEnable();
    
    ADC1_InterruptFlagClear();
    while (!_AD1IF);
    ADC1_InterruptFlagClear();
    
    while (!ADC1_IsConversionComplete(channel_CTMU));
    
    ADC1_AutomaticSamplingDisable();
    ADC1_Disable();

    uint16_t voltage_sum = 
            (ADC1BUF0) + (ADC1BUF1) + (ADC1BUF2) + (ADC1BUF3) + 
            (ADC1BUF4) + (ADC1BUF5) + (ADC1BUF6) + (ADC1BUF7) + 
            (ADC1BUF8) + (ADC1BUF9) + (ADC1BUFA) + (ADC1BUFB) + 
            (ADC1BUFC) + (ADC1BUFD) + (ADC1BUFE) + (ADC1BUFF);
    
    UART1_WriteInt(voltage_sum);
    
    return SUCCESS;
}