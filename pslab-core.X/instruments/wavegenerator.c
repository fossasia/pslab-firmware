#include <xc.h>
#include "../commands.h"
#include "../bus/uart1.h"
#include "../registers/timers/tmr1.h"
#include "../registers/timers/tmr3.h"
#include "../registers/timers/tmr4.h"
#include "../registers/memory/dma.h"
#include "../registers/comparators/oc1.h"
#include "../registers/comparators/oc2.h"
#include "../registers/comparators/oc3.h"
#include "../registers/comparators/oc4.h"
#include "../registers/system/pin_manager.h"

#define WAVE_TABLE_FULL_LENGTH          512
#define WAVE_TABLE_SHORT_LENGTH         32

#define HIGH_RESOLUTION                 1
#define LOW_RESOLUTION                  0

int __attribute__((section(".sine_table1"))) sine_table_1[] = {
    256, 252, 249, 246, 243, 240, 237, 234, 230, 227, 224, 221, 218, 215, 212,
    209, 206, 203, 200, 196, 193, 190, 187, 184, 181, 178, 175, 172, 169, 166,
    164, 161, 158, 155, 152, 149, 146, 143, 141, 138, 135, 132, 130, 127, 124,
    121, 119, 116, 114, 111, 108, 106, 103, 101, 98, 96, 93, 91, 89, 86, 84, 82,
    79, 77, 75, 73, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 47, 45, 43,
    41, 40, 38, 36, 35, 33, 32, 30, 29, 27, 26, 25, 23, 22, 21, 19, 18, 17, 16,
    15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 2, 2, 2, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7,
    8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 29,
    30, 32, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 54, 56, 58, 60, 62,
    64, 66, 68, 70, 73, 75, 77, 79, 82, 84, 86, 89, 91, 93, 96, 98, 101, 103,
    106, 108, 111, 114, 116, 119, 121, 124, 127, 130, 132, 135, 138, 141, 143,
    146, 149, 152, 155, 158, 161, 164, 166, 169, 172, 175, 178, 181, 184, 187,
    190, 193, 196, 200, 203, 206, 209, 212, 215, 218, 221, 224, 227, 230, 234,
    237, 240, 243, 246, 249, 252, 256, 259, 262, 265, 268, 271, 274, 277, 281,
    284, 287, 290, 293, 296, 299, 302, 305, 308, 311, 315, 318, 321, 324, 327,
    330, 333, 336, 339, 342, 345, 347, 350, 353, 356, 359, 362, 365, 368, 370,
    373, 376, 379, 381, 384, 387, 390, 392, 395, 397, 400, 403, 405, 408, 410,
    413, 415, 418, 420, 422, 425, 427, 429, 432, 434, 436, 438, 441, 443, 445,
    447, 449, 451, 453, 455, 457, 459, 461, 463, 464, 466, 468, 470, 471, 473,
    475, 476, 478, 479, 481, 482, 484, 485, 486, 488, 489, 490, 492, 493, 494,
    495, 496, 497, 498, 499, 500, 501, 502, 503, 503, 504, 505, 505, 506, 507,
    507, 508, 508, 509, 509, 509, 510, 510, 510, 511, 511, 511, 511, 511, 511,
    511, 511, 511, 511, 511, 510, 510, 510, 509, 509, 509, 508, 508, 507, 507,
    506, 505, 505, 504, 503, 503, 502, 501, 500, 499, 498, 497, 496, 495, 494,
    493, 492, 490, 489, 488, 486, 485, 484, 482, 481, 479, 478, 476, 475, 473,
    471, 470, 468, 466, 464, 463, 461, 459, 457, 455, 453, 451, 449, 447, 445,
    443, 441, 438, 436, 434, 432, 429, 427, 425, 422, 420, 418, 415, 413, 410,
    408, 405, 403, 400, 397, 395, 392, 390, 387, 384, 381, 379, 376, 373, 370,
    368, 365, 362, 359, 356, 353, 350, 347, 345, 342, 339, 336, 333, 330, 327,
    324, 321, 318, 315, 311, 308, 305, 302, 299, 296, 293, 290, 287, 284, 281,
    277, 274, 271, 268, 265, 262, 259
};
int __attribute__((section(".sine_table1_short"))) sine_table_1_short[] = {
    32, 26, 20, 14, 9, 5, 2, 1, 0, 1, 2, 5, 9, 14, 20, 26, 32, 38, 44, 50, 55,
    59, 62, 63, 64, 63, 62, 59, 55, 50, 44, 38
};
int __attribute__((section(".sine_table2"))) sine_table_2[] = {
    256, 252, 249, 246, 243, 240, 237, 234, 230, 227, 224, 221, 218, 215, 212,
    209, 206, 203, 200, 196, 193, 190, 187, 184, 181, 178, 175, 172, 169, 166,
    164, 161, 158, 155, 152, 149, 146, 143, 141, 138, 135, 132, 130, 127, 124,
    121, 119, 116, 114, 111, 108, 106, 103, 101, 98, 96, 93, 91, 89, 86, 84, 82,
    79, 77, 75, 73, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 47, 45, 43,
    41, 40, 38, 36, 35, 33, 32, 30, 29, 27, 26, 25, 23, 22, 21, 19, 18, 17, 16,
    15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 2, 2, 2, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7,
    8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 29,
    30, 32, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 54, 56, 58, 60, 62,
    64, 66, 68, 70, 73, 75, 77, 79, 82, 84, 86, 89, 91, 93, 96, 98, 101, 103,
    106, 108, 111, 114, 116, 119, 121, 124, 127, 130, 132, 135, 138, 141, 143,
    146, 149, 152, 155, 158, 161, 164, 166, 169, 172, 175, 178, 181, 184, 187,
    190, 193, 196, 200, 203, 206, 209, 212, 215, 218, 221, 224, 227, 230, 234,
    237, 240, 243, 246, 249, 252, 256, 259, 262, 265, 268, 271, 274, 277, 281,
    284, 287, 290, 293, 296, 299, 302, 305, 308, 311, 315, 318, 321, 324, 327,
    330, 333, 336, 339, 342, 345, 347, 350, 353, 356, 359, 362, 365, 368, 370,
    373, 376, 379, 381, 384, 387, 390, 392, 395, 397, 400, 403, 405, 408, 410,
    413, 415, 418, 420, 422, 425, 427, 429, 432, 434, 436, 438, 441, 443, 445,
    447, 449, 451, 453, 455, 457, 459, 461, 463, 464, 466, 468, 470, 471, 473,
    475, 476, 478, 479, 481, 482, 484, 485, 486, 488, 489, 490, 492, 493, 494,
    495, 496, 497, 498, 499, 500, 501, 502, 503, 503, 504, 505, 505, 506, 507,
    507, 508, 508, 509, 509, 509, 510, 510, 510, 511, 511, 511, 511, 511, 511,
    511, 511, 511, 511, 511, 510, 510, 510, 509, 509, 509, 508, 508, 507, 507,
    506, 505, 505, 504, 503, 503, 502, 501, 500, 499, 498, 497, 496, 495, 494,
    493, 492, 490, 489, 488, 486, 485, 484, 482, 481, 479, 478, 476, 475, 473,
    471, 470, 468, 466, 464, 463, 461, 459, 457, 455, 453, 451, 449, 447, 445,
    443, 441, 438, 436, 434, 432, 429, 427, 425, 422, 420, 418, 415, 413, 410,
    408, 405, 403, 400, 397, 395, 392, 390, 387, 384, 381, 379, 376, 373, 370,
    368, 365, 362, 359, 356, 353, 350, 347, 345, 342, 339, 336, 333, 330, 327,
    324, 321, 318, 315, 311, 308, 305, 302, 299, 296, 293, 290, 287, 284, 281,
    277, 274, 271, 268, 265, 262, 259
};
int __attribute__((section(".sine_table2_short"))) sine_table_2_short[] = {
    32, 26, 20, 14, 9, 5, 2, 1, 0, 1, 2, 5, 9, 14, 20, 26, 32, 38, 44, 50, 55,
    59, 62, 63, 64, 63, 62, 59, 55, 50, 44, 38
};

response_t WAVEGENERATOR_LoadWaveForm1(void) {

    uint16_t i;

    for (i = 0; i < WAVE_TABLE_FULL_LENGTH; i++) {
        sine_table_1[i] = UART1_ReadInt();
    }

    for (i = 0; i < WAVE_TABLE_SHORT_LENGTH; i++) {
        sine_table_1_short[i] = UART1_Read();
    }

    return SUCCESS;
}

response_t WAVEGENERATOR_ReadWaveForm1(void) {

    uint16_t i;

    for (i = 0; i < WAVE_TABLE_FULL_LENGTH; i++) {
        UART1_WriteInt(sine_table_1[i]);
    }

    for (i = 0; i < WAVE_TABLE_SHORT_LENGTH; i++) {
        UART1_Write(sine_table_1_short[i]);
    }

    return SUCCESS;
}

response_t WAVEGENERATOR_LoadWaveForm2(void) {

    uint16_t i;

    for (i = 0; i < WAVE_TABLE_FULL_LENGTH; i++) {
        sine_table_2[i] = UART1_ReadInt();
    }

    for (i = 0; i < WAVE_TABLE_SHORT_LENGTH; i++) {
        sine_table_2_short[i] = UART1_Read();
    }

    return SUCCESS;
}

response_t WAVEGENERATOR_ReadWaveForm2(void) {

    uint16_t i;

    for (i = 0; i < WAVE_TABLE_FULL_LENGTH; i++) {
        UART1_WriteInt(sine_table_2[i]);
    }

    for (i = 0; i < WAVE_TABLE_SHORT_LENGTH; i++) {
        UART1_Write(sine_table_2_short[i]);
    }

    return SUCCESS;
}

response_t WAVEGENERATOR_SetSine1(void) {

    uint8_t resolution = UART1_Read();
    uint16_t wave_length = UART1_ReadInt();

    TMR4_Initialize();

    DMA_InterruptDisable(DMA_CHANNEL_2);
    DMA_InitializeChannel2();
    // DMA Reads from RAM address, writes to peripheral address
    DMA2CONbits.DIR = 1;

    if (resolution & HIGH_RESOLUTION) {
        OC3_PrimaryValueSet(WAVE_TABLE_FULL_LENGTH >> 1);
        OC3_SecondaryValueSet(WAVE_TABLE_FULL_LENGTH);
        DMA_StartAddressAFullSet(DMA_CHANNEL_2,
                __builtin_dmaoffset(&sine_table_1),
                __builtin_dmapage(&sine_table_1));
        DMA_TransferCountSet(DMA_CHANNEL_2, WAVE_TABLE_FULL_LENGTH - 1);
    } else {
        OC3_PrimaryValueSet(WAVE_TABLE_SHORT_LENGTH);
        OC3_SecondaryValueSet(WAVE_TABLE_SHORT_LENGTH << 1);
        DMA_StartAddressAFullSet(DMA_CHANNEL_2,
                __builtin_dmaoffset(&sine_table_1_short),
                __builtin_dmapage(&sine_table_1_short));
        DMA_TransferCountSet(DMA_CHANNEL_2, WAVE_TABLE_SHORT_LENGTH - 1);
    }

    OC3_InitializeCON1();
    // Output Compare Clock Select is Peripheral clock
    OC3CON1bits.OCTSEL = 0b111;
    // Output set high when OC3TMR=0 and set low when OC3TMR=OC3R
    OC3CON1bits.OCM = 0b110;

    OC3_InitializeCON2();
    // OC3RS compare event is used for synchronization
    OC3CON2bits.SYNCSEL = 0b11111;

    DMA_PeripheralAddressSet(DMA_CHANNEL_2, (volatile uint16_t) (&OC3R));

    // Automatic DMA transfer initiation by DMA request
    DMA2REQbits.FORCE = 0;
    DMA_PeripheralIrqNumberSet(DMA_CHANNEL_2, DMA_PERIPHERAL_IRQ_TMR4);

    DMA_FlagInterruptClear(DMA_CHANNEL_2);
    DMA_InterruptEnable(DMA_CHANNEL_2);

    DMA_ChannelEnable(DMA_CHANNEL_2);

    TMR4_Period16BitSet(wave_length);
    // 11 -- 1:256 
    // 10 -- 1:64 
    // 01 -- 1:8 
    // 00 -- 1:1
    T4CONbits.TCKPS = (resolution >> 1) & 3;

    // Link OC3 pin to SINE1 output
    RPOR6bits.RP57R = RPN_OC3_PORT;

    TMR4_Start();

    return SUCCESS;
}

response_t WAVEGENERATOR_SetSine2(void) {

    uint8_t resolution = UART1_Read();
    uint16_t wave_length = UART1_ReadInt();

    T2CONbits.T32 = 0;
    TMR3_Initialize();

    DMA_InterruptDisable(DMA_CHANNEL_3);
    DMA_InitializeChannel3();
    // DMA Reads from RAM address, writes to peripheral address
    DMA3CONbits.DIR = 1;

    if (resolution & HIGH_RESOLUTION) {
        OC4_PrimaryValueSet(WAVE_TABLE_FULL_LENGTH >> 1);
        OC4_SecondaryValueSet(WAVE_TABLE_FULL_LENGTH);
        DMA_StartAddressASet(DMA_CHANNEL_3,
                __builtin_dmaoffset(&sine_table_2));
        DMA_TransferCountSet(DMA_CHANNEL_3, WAVE_TABLE_FULL_LENGTH - 1);
    } else {
        OC4_PrimaryValueSet(WAVE_TABLE_SHORT_LENGTH);
        OC4_SecondaryValueSet(WAVE_TABLE_SHORT_LENGTH << 1);
        DMA_StartAddressASet(DMA_CHANNEL_3,
                __builtin_dmaoffset(&sine_table_2_short));
        DMA_TransferCountSet(DMA_CHANNEL_3, WAVE_TABLE_SHORT_LENGTH - 1);
    }

    OC4_InitializeCON1();
    // Output Compare Clock Select is Peripheral clock
    OC4CON1bits.OCTSEL = 0b111;
    // Output set high when OC3TMR=0 and set low when OC3TMR=OC3R
    OC4CON1bits.OCM = 0b110;

    OC4_InitializeCON2();
    // OC3RS compare event is used for synchronization
    OC4CON2bits.SYNCSEL = 0b11111;

    DMA_PeripheralAddressSet(DMA_CHANNEL_3, (volatile uint16_t) (&OC4R));

    // Automatic DMA transfer initiation by DMA request
    DMA3REQbits.FORCE = 0;
    DMA_PeripheralIrqNumberSet(DMA_CHANNEL_3, DMA_PERIPHERAL_IRQ_TMR3);

    DMA_FlagInterruptClear(DMA_CHANNEL_3);
    DMA_InterruptEnable(DMA_CHANNEL_3);

    DMA_ChannelEnable(DMA_CHANNEL_3);

    TMR3_Period16BitSet(wave_length);
    // 11 -- 1:256 
    // 10 -- 1:64 
    // 01 -- 1:8 
    // 00 -- 1:1
    T3CONbits.TCKPS = (resolution >> 1) & 3;

    // Link OC4 pin to SINE2 output
    RPOR6bits.RP56R = RPN_OC4_PORT;

    TMR3_Start();

    return SUCCESS;
}

response_t WAVEGENERATOR_SetSineDual(void) {

    uint16_t wave_length_1 = UART1_ReadInt();
    uint16_t wave_length_2 = UART1_ReadInt();
    uint16_t table_offset = UART1_ReadInt();
    uint16_t timer_offset = UART1_ReadInt();
    uint8_t resolution = UART1_Read();

    uint16_t i;

    TMR3_Initialize();
    TMR4_Initialize();

    DMA_InterruptDisable(DMA_CHANNEL_2);
    DMA_InitializeChannel2();
    // DMA Reads from RAM address, writes to peripheral address
    DMA2CONbits.DIR = 1;
    DMA_InterruptDisable(DMA_CHANNEL_3);
    DMA_InitializeChannel3();
    // DMA Reads from RAM address, writes to peripheral address
    DMA3CONbits.DIR = 1;

    if (resolution & HIGH_RESOLUTION) {
        OC3_PrimaryValueSet(WAVE_TABLE_FULL_LENGTH >> 1);
        OC3_SecondaryValueSet(WAVE_TABLE_FULL_LENGTH);
        DMA_StartAddressAFullSet(DMA_CHANNEL_2,
                __builtin_dmaoffset(&sine_table_1),
                __builtin_dmapage(&sine_table_1));
        DMA_TransferCountSet(DMA_CHANNEL_2, WAVE_TABLE_FULL_LENGTH - 1);
    } else {
        OC3_PrimaryValueSet(WAVE_TABLE_SHORT_LENGTH);
        OC3_SecondaryValueSet(WAVE_TABLE_SHORT_LENGTH << 1);
        DMA_StartAddressAFullSet(DMA_CHANNEL_2,
                __builtin_dmaoffset(&sine_table_1_short),
                __builtin_dmapage(&sine_table_1_short));
        DMA_TransferCountSet(DMA_CHANNEL_2, WAVE_TABLE_SHORT_LENGTH - 1);
    }

    if ((resolution >> 1) & HIGH_RESOLUTION) {
        OC4_PrimaryValueSet(WAVE_TABLE_FULL_LENGTH >> 1);
        OC4_SecondaryValueSet(WAVE_TABLE_FULL_LENGTH);
        DMA_StartAddressAFullSet(DMA_CHANNEL_3,
                __builtin_dmaoffset(&sine_table_2),
                __builtin_dmapage(&sine_table_2));
        DMA_TransferCountSet(DMA_CHANNEL_3, WAVE_TABLE_FULL_LENGTH - 1);
    } else {
        OC4_PrimaryValueSet(WAVE_TABLE_SHORT_LENGTH);
        OC4_SecondaryValueSet(WAVE_TABLE_SHORT_LENGTH << 1);
        DMA_StartAddressAFullSet(DMA_CHANNEL_3,
                __builtin_dmaoffset(&sine_table_2_short),
                __builtin_dmapage(&sine_table_2_short));
        DMA_TransferCountSet(DMA_CHANNEL_3, WAVE_TABLE_SHORT_LENGTH - 1);
    }

    OC3_InitializeCON1();
    // Output Compare Clock Select is Peripheral clock
    OC3CON1bits.OCTSEL = 0b111;
    // Output set high when OC3TMR=0 and set low when OC3TMR=OC3R
    OC3CON1bits.OCM = 0b110;

    OC3_InitializeCON2();
    // OC3RS compare event is used for synchronization
    OC3CON2bits.SYNCSEL = 0b11111;

    OC4_InitializeCON1();
    // Output Compare Clock Select is Peripheral clock
    OC4CON1bits.OCTSEL = 0b111;
    // Output set high when OC4TMR=0 and set low when OC4TMR=OC4R
    OC4CON1bits.OCM = 0b110;

    OC4_InitializeCON2();
    // OC4RS compare event is used for synchronization
    OC4CON2bits.SYNCSEL = 0b11111;

    DMA_PeripheralAddressSet(DMA_CHANNEL_2, (volatile uint16_t) (&OC3R));
    DMA_PeripheralIrqNumberSet(DMA_CHANNEL_2, DMA_PERIPHERAL_IRQ_TMR4);
    DMA_PeripheralAddressSet(DMA_CHANNEL_3, (volatile uint16_t) (&OC4R));
    DMA_PeripheralIrqNumberSet(DMA_CHANNEL_3, DMA_PERIPHERAL_IRQ_TMR3);

    DMA_FlagInterruptClear(DMA_CHANNEL_2);
    DMA_InterruptEnable(DMA_CHANNEL_2);
    DMA_FlagInterruptClear(DMA_CHANNEL_3);
    DMA_InterruptEnable(DMA_CHANNEL_3);

    DMA_ChannelEnable(DMA_CHANNEL_2);
    DMA_ChannelEnable(DMA_CHANNEL_3);

    // Fast forward DMA Channel 3 to create phase offset
    for (i = 0; i < table_offset; i++) {
        DMA3REQbits.FORCE = 1;
        while (DMA3REQbits.FORCE);
    }

    TMR3_Period16BitSet(wave_length_1);
    TMR4_Period16BitSet(wave_length_2);
    
    TMR3_Counter16BitSet(timer_offset);
    // 11 -- 1:256 
    // 10 -- 1:64 
    // 01 -- 1:8 
    // 00 -- 1:1
    T3CONbits.TCKPS = (resolution >> 2) & 3;
    T4CONbits.TCKPS = (resolution >> 4) & 3;

    RPOR6bits.RP56R = RPN_OC4_PORT;
    RPOR6bits.RP57R = RPN_OC3_PORT;

    TMR3_Start();
    TMR4_Start();

    return SUCCESS;
}

response_t WAVEGENERATOR_SetSquare1(void) {

    uint16_t wave_length = UART1_ReadInt();
    uint16_t high_time = UART1_ReadInt();
    uint8_t scale = UART1_Read();

    OC1_Initialize();
    TMR1_Initialize();

    // Output Compare Clock Select is TMR 1
    OC1CON1bits.OCTSEL = 0b100;
    // Output set high when OC1TMR=0 and set low when OC1TMR=OC1R
    OC1CON1bits.OCM = 0b110;
    // Timer 1 trigger event is used for synchronization
    OC1CON2bits.SYNCSEL = 0b01011;

    // Set pulse turn on time
    OC1_PrimaryValueSet(high_time - 1);
    // Set pulse width
    TMR1_Period16BitSet(wave_length - 1);

    T1CONbits.TCKPS = scale & 0x3;

    RPOR5bits.RP54R = RPN_OC1_PORT;

    TMR1_Start();

    return SUCCESS;
}

response_t WAVEGENERATOR_SetSquare2(void) {

    uint16_t wave_length = UART1_ReadInt();
    uint16_t high_time = UART1_ReadInt();
    uint8_t scale = UART1_Read();

    OC2_Initialize();
    TMR3_Initialize();
    
    // Output Compare Clock Select is TMR 3
    OC2CON1bits.OCTSEL = 0b001;
    // Output set high when OC2TMR=0 and set low when OC2TMR=OC2R
    OC2CON1bits.OCM = 0b110;
    // Timer 3 trigger event is used for synchronization
    OC2CON2bits.SYNCSEL = 0b01101;

    // Set pulse turn on time
    OC2_PrimaryValueSet(high_time - 1);
    // Set pulse width
    TMR3_Period16BitSet(wave_length - 1);

    T3CONbits.TCKPS = scale & 0x3;

    RPOR5bits.RP55R = RPN_OC2_PORT;

    TMR3_Start();

    return SUCCESS;
}

response_t WAVEGENERATOR_SetSquareAll(void) {

    uint16_t wave_length = UART1_ReadInt();
    uint16_t high_time_1 = UART1_ReadInt();
    uint16_t low_time_2 = UART1_ReadInt();
    uint16_t high_time_2 = UART1_ReadInt();
    uint16_t low_time_3 = UART1_ReadInt();
    uint16_t high_time_3 = UART1_ReadInt();
    uint16_t low_time_4 = UART1_ReadInt();
    uint16_t high_time_4 = UART1_ReadInt();
    uint8_t configuration = UART1_Read();

    DMA_InterruptDisable(DMA_CHANNEL_2);
    DMA_FlagInterruptClear(DMA_CHANNEL_2);
    DMA_ChannelDisable(DMA_CHANNEL_2);
    DMA_InterruptDisable(DMA_CHANNEL_3);
    DMA_FlagInterruptClear(DMA_CHANNEL_3);
    DMA_ChannelDisable(DMA_CHANNEL_3);

    TMR1_Initialize();
    T1CONbits.TCKPS = configuration & 0b11; // ...._..XX

    OC1_Initialize();
    OC1_PrimaryValueSet(0);
    OC1_SecondaryValueSet(high_time_1);
    OC2_Initialize();
    OC2_PrimaryValueSet(low_time_2);
    OC2_SecondaryValueSet(high_time_2);
    OC3_Initialize();
    OC3_PrimaryValueSet(low_time_3);
    OC3_SecondaryValueSet(high_time_3);
    OC4_Initialize();
    OC4_PrimaryValueSet(low_time_4);
    OC4_SecondaryValueSet(high_time_4);

    if ((configuration >> 6) & 1) { // .X.._....
        // EnableComparator();
        // CMP2 module synchronizes or triggers OCx
        OC1CON2bits.SYNCSEL = 0b11001;
        OC2CON2bits.SYNCSEL = 0b11001;
        OC3CON2bits.SYNCSEL = 0b11001;
        OC4CON2bits.SYNCSEL = 0b11001;
        // Triggers OCx from the source designated by the SYNCSELx bits: CMP2
        OC1CON2bits.OCTRIG = 1;
        OC2CON2bits.OCTRIG = 1;
        OC3CON2bits.OCTRIG = 1;
        OC4CON2bits.OCTRIG = 1;
    } else {
        // Timer 1 synchronizes OCx timers
        OC1CON2bits.SYNCSEL = 0b01011;
        OC2CON2bits.SYNCSEL = 0b01011;
        OC3CON2bits.SYNCSEL = 0b01011;
        OC4CON2bits.SYNCSEL = 0b01011;
    }

    if ((configuration >> 5) & 1) { // ..X._....
        // Output set high when OCxTMR = OCxR and set low when OCxTMR = OCxRS
        OC1CON1bits.OCM = 0b111;
        OC2CON1bits.OCM = 0b111;
        OC3CON1bits.OCM = 0b111;
        OC4CON1bits.OCM = 0b111;
    } else {
        // Toggles OCx state on matches of OCxR and OCxRS for one cycle
        OC1CON1bits.OCM = 0b100;
        OC2CON1bits.OCM = 0b100;
        OC3CON1bits.OCM = 0b100;
        OC4CON1bits.OCM = 0b100;
        // Invert OCx pin output
        OC2CON2bits.OCINV = (configuration >> 2) & 1; // ...._.X..
        OC3CON2bits.OCINV = (configuration >> 3) & 1; // ...._X...
        OC4CON2bits.OCINV = (configuration >> 4) & 1; // ...X_....
    }

    // T1CLK is the clock source of the Output Compare modules
    OC1CON1bits.OCTSEL = 0b100;
    OC2CON1bits.OCTSEL = 0b100;
    OC3CON1bits.OCTSEL = 0b100;
    OC4CON1bits.OCTSEL = 0b100;

    TMR1_Period16BitSet(wave_length);
    TMR1_Start();

    RPOR5bits.RP54R = RPN_OC1_PORT;
    RPOR5bits.RP55R = RPN_OC2_PORT;
    RPOR6bits.RP56R = RPN_OC3_PORT;
    RPOR6bits.RP57R = RPN_OC4_PORT;

    return SUCCESS;
}
