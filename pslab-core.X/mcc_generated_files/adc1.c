#include "adc1.h"

// Space in memory to save ADC data
int __attribute__((section(".adc_buffer"), far)) ADC1_Buffer[10000];

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

void __attribute__((__interrupt__, auto_psv, weak)) _AD1Interrupt(void) {
    if (IFS0bits.AD1IF) {
        // clear the ADC interrupt flag
        IFS0bits.AD1IF = false;
    }
}
