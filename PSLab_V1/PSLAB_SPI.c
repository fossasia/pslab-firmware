/******************************************************************************/
/******** This file contains SPI control modules of function.c file ***********/
/******************************************************************************/
#include "COMMANDS.h"
#include "PSLAB_SPI.h"

BYTE location, value, ADC_MODE = NOT_READY, SPI_MODE = NOT_READY, DMA_MODE = NOT_READY, ADC_STREAMING = 0;
BYTE SPI_PPRE = 0, SPI_SPRE = 2, SPI_CKE = 1, SPI_CKP = 0, SPI_SMP = 1;

void setSPIMode(BYTE mode){
    if(SPI_MODE == mode)return;
    else{
        SPI_MODE = mode;
        initSPI();
    }
}

void initSPI(){

    SPI1STAT = 0; //disable SPI
    _TRISB5 = 0;     _LATB5=1;                   //CSOUT-1
    _TRISB6 = 0;     _LATB6=1;                   //CSOUT-2  
    IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
    IEC0bits.SPI1IE = 0; // Disable the interrupt

    SPI1CON1bits.PPRE = SPI_PPRE; //primary prescale 64:1
    SPI1CON1bits.SPRE = SPI_SPRE; //secondary prescale 7:1
    SPI1CON1bits.DISSCK = 0; //enable internal clock

    SPI1CON1bits.DISSDO = 0; //SDO controlled by module.
    SPI1CON1bits.SSEN = 0; // CS not used
    if(SPI_MODE==SPI_8)SPI1CON1bits.MODE16 = 0; //0=8,1=16 bits wide data
    else SPI1CON1bits.MODE16 = 1; //0=8,1=16 bits wide data
    SPI1CON1bits.CKE = SPI_CKE; //  Read data on rising edge of SCK; //SDO does not keep state
    SPI1CON1bits.CKP = SPI_CKP; //Clock idle state
    SPI1CON1bits.MSTEN = 1; //enable master mode

    SPI1CON1bits.SMP = SPI_SMP;

    SPI1STATbits.SPIEN = 1; //enable SPI1
}

BYTE spi_write8(BYTE value) {
    setSPIMode(SPI_8);
    SPI1STATbits.SPIROV = 0;
    SPI1BUF = value;
    while (SPI1STATbits.SPITBF); // wait for the data to be sent out
    while (!SPI1STATbits.SPIRBF); // wait for dummy byte to clock in
    return SPI1BUF&0xFF;
}

uint16 spi_write16(uint16 value) {
    setSPIMode(SPI_16);
    SPI1STATbits.SPIROV = 0;
    SPI1BUF = value;
    while (SPI1STATbits.SPITBF); // wait for the data to be sent out
    while (!SPI1STATbits.SPIRBF); // wait for dummy byte to clock in
    return SPI1BUF; // dummy read of the SPI1BUF register to clear the SPIRBF flag
}
