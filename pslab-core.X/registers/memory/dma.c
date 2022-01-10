#include "dma.h"
#include "../../instruments/logicanalyzer.h"
#include "../../registers/comparators/ic1.h"
#include "../../registers/comparators/ic2.h"
#include "../../registers/comparators/ic3.h"
#include "../../registers/comparators/ic4.h"

static DMA_MODES DMA_MODE;
void SetDMA_MODE(DMA_MODES mode) { DMA_MODE = mode; }
DMA_MODES GetDMA_MODE(void) { return DMA_MODE; }

void __attribute__((__interrupt__, no_auto_psv)) _DMA0Interrupt(void) {
    IC1_TriggerStatusClear();
    IC1_Stop();
    DMA_ChannelDisable(DMA_CHANNEL_0);
    DMA_FlagInterruptClear(DMA_CHANNEL_0);
    DMA_InterruptDisable(DMA_CHANNEL_0);

    switch (DMA_MODE) {
        case DMA_MODES_ONE_CHANNEL:
        case DMA_MODES_TWO_CHANNEL:
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
    DMA_InitializeChannel0();
    DMA_InitializeChannel1();
    DMA_InitializeChannel2();
    DMA_InitializeChannel3();
}

void DMA_InitializeChannel0(void) {
    // Channel is disabled
    DMA0CONbits.CHEN = 0;
    // DMA Data Transfer Size bit is word wise
    DMA0CONbits.SIZE = 0;
    // Reads from peripheral address, writes to RAM address
    DMA0CONbits.DIR = 0;
    // Initiates interrupt when all of the data have been moved
    DMA0CONbits.HALF = 0;
    // Normal operation
    DMA0CONbits.NULLW = 0;
    // Register Indirect with Post-Increment mode
    DMA0CONbits.AMODE = 0b00;
    // DMA Channel operating mode is continuous with Ping-Pong modes disabled
    DMA0CONbits.MODE = DMA_OPERATING_MODE_CONTINUOUS;
    // IRQSEL INT0; FORCE disabled; 
    DMA0REQ = DMA_PERIPHERAL_IRQ_INT0;
    // STA 0; 
    DMA0STAH = 0x00;
    // STA 0; 
    DMA0STAL = 0x00;
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
}

void DMA_InitializeChannel1(void) {
    // Channel is disabled
    DMA1CONbits.CHEN = 0;
    // DMA Data Transfer Size bit is word wise
    DMA1CONbits.SIZE = 0;
    // Reads from peripheral address, writes to RAM address
    DMA1CONbits.DIR = 0;
    // Initiates interrupt when all of the data have been moved
    DMA1CONbits.HALF = 0;
    // Normal operation
    DMA1CONbits.NULLW = 0;
    // Register Indirect with Post-Increment mode
    DMA1CONbits.AMODE = 0b00;
    // DMA Channel operating mode is continuous with Ping-Pong modes disabled
    DMA1CONbits.MODE = DMA_OPERATING_MODE_CONTINUOUS;
    // IRQSEL INT0; FORCE disabled; 
    DMA1REQ = DMA_PERIPHERAL_IRQ_INT0;
    // STA 0; 
    DMA1STAH = 0x00;
    // STA 0; 
    DMA1STAL = 0x00;
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
    DMA2CONbits.MODE = DMA_OPERATING_MODE_CONTINUOUS;
    // IRQSEL INT0; FORCE disabled; 
    DMA2REQ = DMA_PERIPHERAL_IRQ_INT0;
    // STA 0; 
    DMA2STAH = 0x00;
    // STA 0; 
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
}

void DMA_InitializeChannel3(void) {
    // Channel is disabled
    DMA3CONbits.CHEN = 0;
    // DMA Data Transfer Size bit is word wise
    DMA3CONbits.SIZE = 0;
    // Reads from peripheral address, writes to RAM address
    DMA3CONbits.DIR = 0;
    // Initiates interrupt when all of the data have been moved
    DMA3CONbits.HALF = 0;
    // Normal operation
    DMA3CONbits.NULLW = 0;
    // Register Indirect with Post-Increment mode
    DMA3CONbits.AMODE = 0b00;
    // DMA Channel operating mode is continuous with Ping-Pong modes disabled
    DMA3CONbits.MODE = DMA_OPERATING_MODE_CONTINUOUS;
    // IRQSEL INT0; FORCE disabled; 
    DMA3REQ = DMA_PERIPHERAL_IRQ_INT0;
    // STA 0; 
    DMA3STAH = 0x00;
    // STA 0; 
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
}

void DMA_SetLogicAnalyzerChannelMode(DMA_MODES mode) {
    DMA_MODE = mode;
}

void DMA_PrepareChannel0(uint16_t count, volatile uint16_t* low_address, 
                                            DMA_PERIPHERAL_IRQ_NUMBER trigger) {
    DMA_InitializeChannel0();
    DMA0CONbits.MODE = DMA_OPERATING_MODE_ONE_SHOT;

    DMA0PAD = (int) &IC1BUF;
    DMA0REQ = trigger;
    DMA0CNT = count - 1;
    DMA0STAH = 0;
    DMA0STAL = (uint16_t) low_address;

    DMA_FlagInterruptClear(DMA_CHANNEL_0);
    DMA_InterruptDisable(DMA_CHANNEL_0);
}

void DMA_PrepareChannel1(uint16_t count, volatile uint16_t* low_address, 
                                            DMA_PERIPHERAL_IRQ_NUMBER trigger) {
    DMA_InitializeChannel1();
    DMA1CONbits.MODE = DMA_OPERATING_MODE_ONE_SHOT;

    DMA1PAD = (int) &IC2BUF;
    DMA1REQ = trigger;
    DMA1CNT = count - 1;
    DMA1STAH = 0;
    DMA1STAL = (uint16_t) low_address;

    DMA_FlagInterruptClear(DMA_CHANNEL_1);
    DMA_InterruptDisable(DMA_CHANNEL_1);
}

void DMA_PrepareChannel2(uint16_t count, volatile uint16_t* low_address, 
                                            DMA_PERIPHERAL_IRQ_NUMBER trigger) {
    DMA_InitializeChannel2();
    DMA2CONbits.MODE = DMA_OPERATING_MODE_ONE_SHOT;

    DMA2PAD = (int) &IC3BUF;
    DMA2REQ = trigger;
    DMA2CNT = count - 1;
    DMA2STAH = 0;
    DMA2STAL = (uint16_t) low_address;

    DMA_FlagInterruptClear(DMA_CHANNEL_2);
    DMA_InterruptDisable(DMA_CHANNEL_2);
}

void DMA_PrepareChannel3(uint16_t count, volatile uint16_t* low_address, 
                                            DMA_PERIPHERAL_IRQ_NUMBER trigger) {
    DMA_InitializeChannel3();
    DMA3CONbits.MODE = DMA_OPERATING_MODE_ONE_SHOT;

    DMA3PAD = (int) &IC4BUF;
    DMA3REQ = trigger;
    DMA3CNT = count - 1;
    DMA3STAH = 0;
    DMA3STAL = (uint16_t) low_address;

    DMA_FlagInterruptClear(DMA_CHANNEL_3);
    DMA_InterruptDisable(DMA_CHANNEL_3);
}

void DMA_EnableAllChannels(void) {
    DMA_ChannelEnable(DMA_CHANNEL_0);
    DMA_ChannelEnable(DMA_CHANNEL_1);
    DMA_ChannelEnable(DMA_CHANNEL_2);
    DMA_ChannelEnable(DMA_CHANNEL_3);
}

void DMA_DisableAllChannels(void) {
    DMA_ChannelDisable(DMA_CHANNEL_0);
    DMA_ChannelDisable(DMA_CHANNEL_1);
    DMA_ChannelDisable(DMA_CHANNEL_2);
    DMA_ChannelDisable(DMA_CHANNEL_3);
}
