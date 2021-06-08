#include "../commands.h"
#include "../delay.h"
#include "../mcc_generated_files/adc1.h"
#include "../registers/memory/dma.h"
#include "../bus/uart1.h"
#include "logicanalyzer.h"

response_t MULTIMETER_GetVoltage(void) {

    unsigned char channel;
    channel = UART1_Read();

    ADC1_InterruptDisable();
    ADC1_InterruptFlagClear();

    DMA_ChannelDisable(DMA_CHANNEL_0);

    ADC1_Disable();

    AD1CON1bits.ADSIDL = 0;
    AD1CON1bits.ADDMABM = 0;
    AD1CON1bits.AD12B = 1;
    AD1CON1bits.FORM = 0b00;
    AD1CON1bits.SSRC = 0b111;
    AD1CON1bits.SSRCG = 0;
    AD1CON1bits.SIMSAM = 0;
    AD1CON1bits.ASAM = 0;
    AD1CON1bits.SAMP = 0;
    AD1CON1bits.DONE = 0;

    AD1CON2bits.VCFG = 0b000;
    AD1CON2bits.CSCNA = 0;
    AD1CON2bits.CHPS = 0b00;
    AD1CON2bits.BUFS = 0;
    AD1CON2bits.BUFM = 0;
    AD1CON2bits.SMPI = 0b00000;
    AD1CON2bits.ALTS = 0;

    AD1CON3bits.ADRC = 1;
    AD1CON3bits.SAMC = 0b11111;
    AD1CON3bits.ADCS = 0b00001001;

    AD1CON4bits.ADDMAEN = 0;
    AD1CON4bits.DMABL = 0b000;

    AD1CHS0bits.CH0SA = channel & 0xF;
    AD1CHS0bits.CH0NA = 0;

    AD1CSSH = 0x0000;
    AD1CSSL = 0x0000;

    ADC1_Enable();
    DELAY_us(20);
    ADC1_SoftwareTriggerEnable();

    while (!ADC1_IsConversionComplete(channel_CTMU)) {
    }

    UART1_WriteInt(ADC1_ConversionResultGet(channel_CTMU));

    return SUCCESS;
}

