#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <xc.h>

#include "../commands.h"
#include "../registers/timers/tmr1.h"
#include "../registers/timers/tmr2.h"
#include "../registers/timers/tmr3.h"
#include "../registers/timers/tmr4.h"
#include "../registers/memory/dma.h"
#include "../registers/comparators/oc1.h"
#include "../registers/comparators/oc2.h"
#include "../registers/comparators/oc3.h"
#include "../registers/comparators/oc4.h"
#include "../registers/system/pin_manager.h"

#include "wavegenerator.h"

#define HIGH_RESOLUTION                 1
#define LOW_RESOLUTION                  0

static uint16_t *g_wave_1_long = NULL;
static uint16_t *g_wave_2_long = NULL;

static uint16_t *g_wave_1_short = NULL;
static uint16_t *g_wave_2_short = NULL;

static void downsample(
    uint16_t const *const table_full,
    uint16_t *const table_short
) {
    int const step = 16;
    int const norm = 64;

    for (int i = 0; i < WAVE_TABLE_SHORT_LENGTH; ++i) {
        table_short[i] = (
            (table_full[i * step] * norm + WAVE_TABLE_FULL_LENGTH / 2 - 1)
            / WAVE_TABLE_FULL_LENGTH
        );
    }
}

enum Status WAVEGENERATOR_load_wave_1(
    uint8_t **args,
    uint16_t const args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    if (args_size != WAVE_TABLE_FULL_LENGTH * sizeof(*g_wave_1_long)) {
        return E_BAD_ARGSIZE;
    }

    if (g_wave_1_long) { return E_RESOURCE_BUSY; }
    g_wave_1_short = malloc(WAVE_TABLE_SHORT_LENGTH * sizeof(*g_wave_1_short));
    if ( !g_wave_1_short ) { return E_MEMORY_INSUFFICIENT; }

    g_wave_1_long = *(uint16_t **)args;
    *args = NULL;

    downsample(g_wave_1_long, g_wave_1_short);
    return E_OK;
}

enum Status WAVEGENERATOR_read_wave_1(
    __attribute__((unused)) uint8_t **args,
    __attribute__((unused)) uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    if ( !g_wave_1_long ) { return E_RESOURCE_NOT_INITIALIZED; }

    *rets = (uint8_t *)g_wave_1_long;
    *rets_size = WAVE_TABLE_FULL_LENGTH * sizeof(*g_wave_1_long);
    return E_OK;
}

enum Status WAVEGENERATOR_stop_wave_1(
    __attribute__((unused)) uint8_t **args,
    __attribute__((unused)) uint16_t const args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    OC3_Stop();
    DMA_ChannelDisable(DMA_CHANNEL_2);
    TMR3_Stop();
    free(g_wave_1_long);
    free(g_wave_1_short);
    return E_OK;
}

enum Status WAVEGENERATOR_load_wave_2(
    uint8_t **args,
    uint16_t const args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    if (args_size != WAVE_TABLE_FULL_LENGTH * sizeof(*g_wave_2_long)) {
        return E_BAD_ARGSIZE;
    }

    if (g_wave_2_long) { return E_RESOURCE_BUSY; }
    g_wave_2_long = *(uint16_t **)args;
    *args = NULL;

    g_wave_2_short = malloc(WAVE_TABLE_SHORT_LENGTH * sizeof(*g_wave_2_short));
    if ( !g_wave_2_short ) { return E_MEMORY_INSUFFICIENT; }

    downsample(g_wave_2_long, g_wave_2_short);
    return E_OK;
}

enum Status WAVEGENERATOR_read_wave_2(
    __attribute__((unused)) uint8_t **args,
    __attribute__((unused)) uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    if ( !g_wave_2_long ) { return E_RESOURCE_NOT_INITIALIZED; }

    *rets = (uint8_t *)g_wave_2_long;
    *rets_size = WAVE_TABLE_FULL_LENGTH * sizeof(*g_wave_2_long);
    return E_OK;
}

enum Status WAVEGENERATOR_stop_wave_2(
    __attribute__((unused)) uint8_t **args,
    __attribute__((unused)) uint16_t const args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    OC4_Stop();
    DMA_ChannelDisable(DMA_CHANNEL_3);
    TMR4_Stop();
    free(g_wave_2_long);
    free(g_wave_2_short);
    return E_OK;
}

enum Status WAVEGENERATOR_set_sine_1(
    uint8_t **args,
    uint16_t const args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    if (!g_wave_1_long) { return E_RESOURCE_NOT_INITIALIZED; }

    struct Input {
        uint8_t resolution;
        uint16_t wavelength;
        uint8_t _pad[1];
    } *input = NULL;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) {
        return E_BAD_ARGSIZE;
    }

    input = *(struct Input **)args;
    T2CONbits.T32 = 0;
    TMR3_Initialize();

    DMA_InterruptDisable(DMA_CHANNEL_2);
    DMA_InitializeChannel2();
    // DMA Reads from RAM address, writes to peripheral address
    DMA2CONbits.DIR = 1;

    if (input->resolution & HIGH_RESOLUTION) {
        OC3_PrimaryValueSet(WAVE_TABLE_FULL_LENGTH >> 1);
        OC3_SecondaryValueSet(WAVE_TABLE_FULL_LENGTH);
        DMA_StartAddressAFullSet(DMA_CHANNEL_2,
                __builtin_dmaoffset(&g_wave_1_long),
                __builtin_dmapage(&g_wave_1_long));
        DMA_TransferCountSet(DMA_CHANNEL_2, WAVE_TABLE_FULL_LENGTH - 1);
    } else {
        OC3_PrimaryValueSet(WAVE_TABLE_SHORT_LENGTH);
        OC3_SecondaryValueSet(WAVE_TABLE_SHORT_LENGTH << 1);
        DMA_StartAddressAFullSet(
            DMA_CHANNEL_2,
            __builtin_dmaoffset(&g_wave_1_short),
            __builtin_dmapage(&g_wave_1_short)
        );
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
    DMA_PeripheralIrqNumberSet(DMA_CHANNEL_2, DMA_PERIPHERAL_IRQ_TMR3);

    DMA_FlagInterruptClear(DMA_CHANNEL_2);
    DMA_InterruptEnable(DMA_CHANNEL_2);

    DMA_ChannelEnable(DMA_CHANNEL_2);

    TMR3_Period16BitSet(input->wavelength);
    // 11 -- 1:256
    // 10 -- 1:64
    // 01 -- 1:8
    // 00 -- 1:1
    T3CONbits.TCKPS = (input->resolution >> 1) & 3;

    // Link OC3 pin to SINE1 output
    RPOR6bits.RP57R = RPN_OC3_PORT;

    TMR3_Start();

    return E_OK;
}

enum Status WAVEGENERATOR_set_sine_2(
    uint8_t **args,
    uint16_t const args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    if (!g_wave_2_long) { return E_RESOURCE_NOT_INITIALIZED; }

    struct Input {
            uint8_t resolution;
            uint16_t wavelength;
            uint8_t _pad[1];
    } *input = NULL;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) {
        return E_BAD_ARGSIZE;
    }

    input = *(struct Input **)args;

    TMR4_Initialize();

    DMA_InterruptDisable(DMA_CHANNEL_3);
    DMA_InitializeChannel3();
    // DMA Reads from RAM address, writes to peripheral address
    DMA3CONbits.DIR = 1;

    if (input->resolution & HIGH_RESOLUTION) {
        OC4_PrimaryValueSet(WAVE_TABLE_FULL_LENGTH >> 1);
        OC4_SecondaryValueSet(WAVE_TABLE_FULL_LENGTH);
        DMA_StartAddressASet(
            DMA_CHANNEL_3,
            __builtin_dmaoffset(&g_wave_2_long)
        );
        DMA_TransferCountSet(DMA_CHANNEL_3, WAVE_TABLE_FULL_LENGTH - 1);
    } else {
        OC4_PrimaryValueSet(WAVE_TABLE_SHORT_LENGTH);
        OC4_SecondaryValueSet(WAVE_TABLE_SHORT_LENGTH << 1);
        DMA_StartAddressASet(DMA_CHANNEL_3,
                __builtin_dmaoffset(&g_wave_2_short));
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
    DMA_PeripheralIrqNumberSet(DMA_CHANNEL_3, DMA_PERIPHERAL_IRQ_TMR4);

    DMA_FlagInterruptClear(DMA_CHANNEL_3);
    DMA_InterruptEnable(DMA_CHANNEL_3);

    DMA_ChannelEnable(DMA_CHANNEL_3);

    TMR4_Period16BitSet(input->wavelength);
    // 11 -- 1:256
    // 10 -- 1:64
    // 01 -- 1:8
    // 00 -- 1:1
    T4CONbits.TCKPS = (input->resolution >> 1) & 3;

    // Link OC4 pin to SINE2 output
    RPOR6bits.RP56R = RPN_OC4_PORT;

    TMR4_Start();

    return E_OK;
}

enum Status WAVEGENERATOR_set_sine_dual(
    uint8_t **args,
    uint16_t const args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    if (!g_wave_1_long) { return E_RESOURCE_NOT_INITIALIZED; }
    if (!g_wave_2_long) { return E_RESOURCE_NOT_INITIALIZED; }

    struct Input {
        uint16_t wavelength_1;
        uint16_t wavelength_2;
        uint16_t table_offset;
        uint16_t timer_offset;
        uint8_t resolution;
        uint8_t _pad[1];
    } *input = NULL;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) {
        return E_BAD_ARGSIZE;
    }

    input = *(struct Input **)args;

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

    if (input->resolution & HIGH_RESOLUTION) {
        OC3_PrimaryValueSet(WAVE_TABLE_FULL_LENGTH >> 1);
        OC3_SecondaryValueSet(WAVE_TABLE_FULL_LENGTH);
        DMA_StartAddressAFullSet(
            DMA_CHANNEL_2,
            __builtin_dmaoffset(&g_wave_1_long),
            __builtin_dmapage(&g_wave_1_long)
        );
        DMA_TransferCountSet(DMA_CHANNEL_2, WAVE_TABLE_FULL_LENGTH - 1);
    } else {
        OC3_PrimaryValueSet(WAVE_TABLE_SHORT_LENGTH);
        OC3_SecondaryValueSet(WAVE_TABLE_SHORT_LENGTH << 1);
        DMA_StartAddressAFullSet(
            DMA_CHANNEL_2,
            __builtin_dmaoffset(&g_wave_1_short),
            __builtin_dmapage(&g_wave_1_short)
        );
        DMA_TransferCountSet(DMA_CHANNEL_2, WAVE_TABLE_SHORT_LENGTH - 1);
    }

    if ((input->resolution >> 1) & HIGH_RESOLUTION) {
        OC4_PrimaryValueSet(WAVE_TABLE_FULL_LENGTH >> 1);
        OC4_SecondaryValueSet(WAVE_TABLE_FULL_LENGTH);
        DMA_StartAddressAFullSet(
            DMA_CHANNEL_3,
            __builtin_dmaoffset(&g_wave_2_long),
            __builtin_dmapage(&g_wave_2_long)
        );
        DMA_TransferCountSet(DMA_CHANNEL_3, WAVE_TABLE_FULL_LENGTH - 1);
    } else {
        OC4_PrimaryValueSet(WAVE_TABLE_SHORT_LENGTH);
        OC4_SecondaryValueSet(WAVE_TABLE_SHORT_LENGTH << 1);
        DMA_StartAddressAFullSet(
            DMA_CHANNEL_3,
            __builtin_dmaoffset(&g_wave_2_short),
            __builtin_dmapage(&g_wave_2_short)
        );
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
    for (unsigned i = 0; i < input->table_offset; ++i) {
        DMA3REQbits.FORCE = 1;
        while (DMA3REQbits.FORCE);
    }

    TMR3_Period16BitSet(input->wavelength_1);
    TMR4_Period16BitSet(input->wavelength_2);

    TMR3_Counter16BitSet(input->timer_offset);
    // 11 -- 1:256
    // 10 -- 1:64
    // 01 -- 1:8
    // 00 -- 1:1
    T3CONbits.TCKPS = (input->resolution >> 2) & 3;
    T4CONbits.TCKPS = (input->resolution >> 4) & 3;

    RPOR6bits.RP56R = RPN_OC4_PORT;
    RPOR6bits.RP57R = RPN_OC3_PORT;

    TMR3_Start();
    TMR4_Start();

    return E_OK;
}

enum Status WAVEGENERATOR_set_square_1(
    uint8_t **args,
    uint16_t const args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    struct Input {
        uint16_t wavelength;
        uint16_t high_time;
        uint8_t scale;
        uint8_t _pad[1];
    } *input = NULL;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) {
        return E_BAD_ARGSIZE;
    }

    input = *(struct Input **)args;
    OC1_Initialize();
    TMR1_Initialize();

    // Output Compare Clock Select is TMR 1
    OC1CON1bits.OCTSEL = 0b100;
    // Output set high when OC1TMR=0 and set low when OC1TMR=OC1R
    OC1CON1bits.OCM = 0b110;
    // Timer 1 trigger event is used for synchronization
    OC1CON2bits.SYNCSEL = 0b01011;

    // Set pulse turn on time
    OC1_PrimaryValueSet(input->high_time - 1);
    // Set pulse width
    TMR1_Period16BitSet(input->wavelength - 1);

    T1CONbits.TCKPS = input->scale & 0x3;

    RPOR5bits.RP54R = RPN_OC1_PORT;

    TMR1_Start();

    return E_OK;
}

enum Status WAVEGENERATOR_set_square_2(
    uint8_t **args,
    uint16_t const args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    struct Input {
        uint16_t wavelength;
        uint16_t high_time;
        uint8_t scale;
        uint8_t _pad[1];
    } *input = NULL;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) {
        return E_BAD_ARGSIZE;
    }

    input = *(struct Input **)args;

    OC2_Initialize();
    TMR2_Initialize();

    // Output Compare Clock Select is TMR 2
    OC2CON1bits.OCTSEL = 0b000;
    // Output set high when OC2TMR=0 and set low when OC2TMR=OC2R
    OC2CON1bits.OCM = 0b110;
    // Timer 2 trigger event is used for synchronization
    OC2CON2bits.SYNCSEL = 0b01100;

    // Set pulse turn on time
    OC2_PrimaryValueSet(input->high_time - 1);
    // Set pulse width
    TMR2_Period16BitSet(input->wavelength - 1);

    T2CONbits.TCKPS = input->scale & 0x3;

    RPOR5bits.RP55R = RPN_OC2_PORT;

    TMR2_Start();

    return E_OK;
}

enum Status WAVEGENERATOR_set_square_all(
    uint8_t **args,
    uint16_t const args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    struct Input {
            uint16_t wavelength;
            uint16_t high_time_1;
            uint16_t low_time_2;
            uint16_t high_time_2;
            uint16_t low_time_3;
            uint16_t high_time_3;
            uint16_t low_time_4;
            uint16_t high_time_4;
            uint8_t configuration;
            uint8_t _pad[1];
    } *input = NULL;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) {
        return E_BAD_ARGSIZE;
    }

    input = *(struct Input **)args;

    DMA_InterruptDisable(DMA_CHANNEL_2);
    DMA_FlagInterruptClear(DMA_CHANNEL_2);
    DMA_ChannelDisable(DMA_CHANNEL_2);
    DMA_InterruptDisable(DMA_CHANNEL_3);
    DMA_FlagInterruptClear(DMA_CHANNEL_3);
    DMA_ChannelDisable(DMA_CHANNEL_3);

    TMR1_Initialize();
    T1CONbits.TCKPS = input->configuration & 0b11; // ...._..XX

    OC1_Initialize();
    OC1_PrimaryValueSet(0);
    OC1_SecondaryValueSet(input->high_time_1);
    OC2_Initialize();
    OC2_PrimaryValueSet(input->low_time_2);
    OC2_SecondaryValueSet(input->high_time_2);
    OC3_Initialize();
    OC3_PrimaryValueSet(input->low_time_3);
    OC3_SecondaryValueSet(input->high_time_3);
    OC4_Initialize();
    OC4_PrimaryValueSet(input->low_time_4);
    OC4_SecondaryValueSet(input->high_time_4);

    if ((input->configuration >> 6) & 1) { // .X.._....
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

    if ((input->configuration >> 5) & 1) { // ..X._....
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
        OC2CON2bits.OCINV = (input->configuration >> 2) & 1; // ...._.X..
        OC3CON2bits.OCINV = (input->configuration >> 3) & 1; // ...._X...
        OC4CON2bits.OCINV = (input->configuration >> 4) & 1; // ...X_....
    }

    // T1CLK is the clock source of the Output Compare modules
    OC1CON1bits.OCTSEL = 0b100;
    OC2CON1bits.OCTSEL = 0b100;
    OC3CON1bits.OCTSEL = 0b100;
    OC4CON1bits.OCTSEL = 0b100;

    TMR1_Period16BitSet(input->wavelength);
    TMR1_Start();

    RPOR5bits.RP54R = RPN_OC1_PORT;
    RPOR5bits.RP55R = RPN_OC2_PORT;
    RPOR6bits.RP56R = RPN_OC3_PORT;
    RPOR6bits.RP57R = RPN_OC4_PORT;

    return E_OK;
}

enum Status WAVEGENERATOR_map_reference(
    uint8_t **args,
    uint16_t const args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    struct Input {
        uint8_t port;
        uint8_t scale;
        uint8_t _pad[0];
    } *input = NULL;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) {
        return E_BAD_ARGSIZE;
    }

    input = *(struct Input **)args;

    // Reference Oscillator output is disabled
    REFOCONbits.ROON = 0;
    // System clock is used as the reference clock
    REFOCONbits.ROSEL = 0;
    // Reference Oscillator output is disabled in Sleep
    REFOCONbits.ROSSLP = 0;
    // Clock scaling as 2^scale (scale = 10 --> 1024 ticks for one toggle)
    REFOCONbits.RODIV = input->scale;

    // Configure output pin/s (multiple ports are supported)
    if (input->port & 1) { // ...._...X
        RPOR5bits.RP54R = RPN_REFCLKO_PORT; // SQR1
    }
    if (input->port & 2) { // ...._..X.
        RPOR5bits.RP55R = RPN_REFCLKO_PORT; // SQR2
    }
    if (input->port & 4) { // ...._.X..
        RPOR6bits.RP56R = RPN_REFCLKO_PORT; // SQR3
    }
    if (input->port & 8) { // ...._X...
        RPOR6bits.RP57R = RPN_REFCLKO_PORT; // SQR4
    }

    // Turn on oscillator output
    REFOCONbits.ROON = 1;

    return E_OK;
}
