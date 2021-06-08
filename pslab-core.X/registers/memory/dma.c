#include "dma.h"

// TODO: define Interrupt functions here

void DMA_Initialize(void) {
    // Initialize channels which are enabled    

    // AMODE Register Indirect with Post-Increment mode; CHEN disabled; DIR Reads from peripheral address, writes to RAM address; HALF Initiates interrupt when all of the data has been moved; SIZE 16 bit; NULLW disabled; MODE Continuous, Ping-Pong modes are disabled; 
    DMA0CON = 0x00 & 0x7FFF; //Enable DMA Channel later;
    // FORCE disabled; IRQSEL INT0; 
    DMA0REQ = 0x00;
    // STA 0; 
    DMA0STAH = 0x00;
    // STA 4096; 
    DMA0STAL = 0x1000;
    // STB 0; 
    DMA0STBH = 0x00;
    // STB 0; 
    DMA0STBL = 0x00;
    // PAD 0; 
    DMA0PAD = 0x00;
    // CNT 0; 
    DMA0CNT = 0x00;
    // Clearing Channel 0 Interrupt Flag;
    IFS0bits.DMA0IF = false;
    // Enabling Channel 0 Interrupt

    // AMODE Register Indirect with Post-Increment mode; CHEN disabled; SIZE 16 bit; DIR Reads from peripheral address, writes to RAM address; NULLW disabled; HALF Initiates interrupt when all of the data has been moved; MODE Continuous, Ping-Pong modes are disabled; 
    DMA1CON = 0x00 & 0x7FFF; //Enable DMA Channel later;
    // FORCE disabled; IRQSEL INT0; 
    DMA1REQ = 0x00;
    // STA 0; 
    DMA1STAH = 0x00;
    // STA 4096; 
    DMA1STAL = 0x1000;
    // STB 0; 
    DMA1STBH = 0x00;
    // STB 0; 
    DMA1STBL = 0x00;
    // PAD 0; 
    DMA1PAD = 0x00;
    // CNT 0; 
    DMA1CNT = 0x00;
    // Clearing Channel 1 Interrupt Flag;
    IFS0bits.DMA1IF = false;
    // Enabling Channel 1 Interrupt

    DMA_InitializeChannel2();

    // MODE Continuous, Ping-Pong modes are disabled; AMODE Register Indirect with Post-Increment mode; CHEN disabled; HALF Initiates interrupt when all of the data has been moved; SIZE 16 bit; DIR Reads from peripheral address, writes to RAM address; NULLW disabled; 
    DMA3CON = 0x00 & 0x7FFF; //Enable DMA Channel later;
    // IRQSEL INT0; FORCE disabled; 
    DMA3REQ = 0x00;
    // STA 0; 
    DMA3STAH = 0x00;
    // STA 4096; 
    DMA3STAL = 0x1000;
    // STB 0; 
    DMA3STBH = 0x00;
    // STB 0; 
    DMA3STBL = 0x00;
    // PAD 0; 
    DMA3PAD = 0x00;
    // CNT 0; 
    DMA3CNT = 0x00;
    // Clearing Channel 3 Interrupt Flag;
    IFS2bits.DMA3IF = false;
    // Enabling Channel 3 Interrupt

}

void DMA_InitializeChannel2(void) {
    // Channel is disabled
    DMA2CONbits.CHEN = 0;
    // DMA Data Transfer Size bit is word wise
    DMA2CONbits.SIZE = 0;
    // Reads from peripheral address, writes to RAM address
    DMA2CONbits.DIR = 0;
    // Initiates interrupt when all of the data have been moved
    DMA2CONbits.HALF = 0;
    // Normal operation
    DMA2CONbits.NULLW = 0;
    // Register Indirect with Post-Increment mode
    DMA2CONbits.AMODE = 0b00;
    // DMA Channel operating mode is continuous with Ping-Pong modes disabled
    DMA2CONbits.MODE = 0b00;
    // IRQSEL INT0; FORCE disabled; 
    DMA2REQ = 0x00;
    // STA 0; 
    DMA2STAH = 0x00;
    // STA 4096; 
    DMA2STAL = 0x00;
    // STB 0; 
    DMA2STBH = 0x00;
    // STB 0; 
    DMA2STBL = 0x00;
    // PAD 0; 
    DMA2PAD = 0x00;
    // CNT 0; 
    DMA2CNT = 0x00;
    // Clearing Channel 2 Interrupt Flag;
    IFS1bits.DMA2IF = false;
    // Enabling Channel 2 Interrupt
}

void __attribute__((weak)) DMA_Channel0_CallBack(void) {
    // Add your custom callback code here
}

void DMA_Channel0_Tasks(void) {
    if (IFS0bits.DMA0IF) {
        // DMA Channel0 callback function 
        DMA_Channel0_CallBack();

        IFS0bits.DMA0IF = 0;
    }
}

void __attribute__((weak)) DMA_Channel1_CallBack(void) {
    // Add your custom callback code here
}

void DMA_Channel1_Tasks(void) {
    if (IFS0bits.DMA1IF) {
        // DMA Channel1 callback function 
        DMA_Channel1_CallBack();

        IFS0bits.DMA1IF = 0;
    }
}

void __attribute__((weak)) DMA_Channel2_CallBack(void) {
    // Add your custom callback code here
}

void DMA_Channel2_Tasks(void) {
    if (IFS1bits.DMA2IF) {
        // DMA Channel2 callback function 
        DMA_Channel2_CallBack();

        IFS1bits.DMA2IF = 0;
    }
}

void __attribute__((weak)) DMA_Channel3_CallBack(void) {
    // Add your custom callback code here
}

void DMA_Channel3_Tasks(void) {
    if (IFS2bits.DMA3IF) {
        // DMA Channel3 callback function 
        DMA_Channel3_CallBack();

        IFS2bits.DMA3IF = 0;
    }
}

/**
  End of File
 */
