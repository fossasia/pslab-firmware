#include "dma.h"
#include "../../instruments/logicanalyzer.h"
#include "../../registers/comparators/ic1.h"
#include "../../registers/comparators/ic2.h"
#include "../../registers/comparators/ic3.h"
#include "../../registers/comparators/ic4.h"

static unsigned char DMA_MODE;

void __attribute__((__interrupt__, no_auto_psv)) _DMA0Interrupt(void) {
    IC1_TriggerStatusClear();
    IC1_Stop();
    DMA_ChannelDisable(DMA_CHANNEL_0);
    DMA_FlagInterruptClear(DMA_CHANNEL_0);
    DMA_InterruptDisable(DMA_CHANNEL_0);

    switch (DMA_MODE) {
        case DMA_LA_ONE_CHAN:
        case DMA_LA_TWO_CHAN:
            IC2_Stop();
            DMA_ChannelDisable(DMA_CHANNEL_1);
            DMA_FlagInterruptClear(DMA_CHANNEL_1);
            DMA_InterruptDisable(DMA_CHANNEL_1);
            break;
        default:
            break;
    }
}

void __attribute__((__interrupt__, no_auto_psv)) _DMA1Interrupt(void) {
    IC2_TriggerStatusClear();
    IC2_Stop();
    DMA_ChannelDisable(DMA_CHANNEL_1);
    DMA_FlagInterruptClear(DMA_CHANNEL_1);
    DMA_InterruptDisable(DMA_CHANNEL_1);
}

void __attribute__((__interrupt__, no_auto_psv)) _DMA2Interrupt(void) {
    IC3_TriggerStatusClear();
    IC3_Stop();
    DMA_FlagInterruptClear(DMA_CHANNEL_2);
}

void __attribute__((__interrupt__, no_auto_psv)) _DMA3Interrupt(void) {
    IC4_TriggerStatusClear();
    IC4_Stop();
    DMA_FlagInterruptClear(DMA_CHANNEL_3);
}

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

void DMA_SetLogicAnalyzerChannelMode(LOGICANALYZER_DMA_MODES mode) {
    DMA_MODE = mode;
}
