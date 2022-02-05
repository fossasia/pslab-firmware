#include "../bus/uart/uart1.h"
#include "../registers/comparators/cmp4.h"
#include "../registers/comparators/cvr.h"
#include "../registers/comparators/ic1.h"
#include "../registers/comparators/ic2.h"
#include "../registers/comparators/ic3.h"
#include "../registers/comparators/ic4.h"
#include "../registers/comparators/ic_params.h"
#include "../registers/memory/dma.h"
#include "../registers/system/interrupt_manager.h"
#include "../registers/system/pin_manager.h"
#include "../registers/system/watchdog.h"
#include "../registers/timers/tmr2.h"
#include "buffer.h"

static uint8_t DIGITAL_STATES = 0;
void SetDIGITAL_STATES(uint8_t V) { DIGITAL_STATES = V; }
void SetDefaultDIGITAL_STATES(void) { DIGITAL_STATES = ((PORTB >> 10) & 0xF) | (_C4OUT << 4); }
uint8_t GetDIGITAL_STATES(void) { return DIGITAL_STATES; }

static uint8_t DIGITAL_STATES_ERROR = 0;
void SetDefaultDIGITAL_STATES_ERROR(void) { DIGITAL_STATES_ERROR = ((PORTB >> 10) & 0xF) | (_C4OUT << 4); }
uint8_t GetDIGITAL_STATES_ERROR(void) { return DIGITAL_STATES_ERROR; }

void INTERVAL_CaptureOne(uint16_t count, uint8_t channel, uint8_t mode, uint8_t trig) {
    
    INTERRUPT_ClearPinChangeInterruptsFlag();
    INTERRUPT_DisablePinChangeInterrupts();
    
    DMA_ChannelDisable(DMA_CHANNEL_0);
    DMA_ChannelDisable(DMA_CHANNEL_1);
    
    DMA_PrepareChannel0(count, BUFFER, DMA_PERIPHERAL_IRQ_IC1);
    DMA_PrepareChannel1(count, BUFFER + BUFFER_SIZE / 4, DMA_PERIPHERAL_IRQ_IC1);
    DMA_InterruptEnable(DMA_CHANNEL_0);
    
    IC1_Initialize();
    IC2_Initialize();
    
    SetDMA_MODE(DMA_MODES_ONE_CHANNEL);
    
    if (channel == 4) {
        CVR_SetupComparator();
        CMP4_SetupComparator();
    }
    RPINR7bits.IC1R = PIN_MANAGER_DIGITAL_PINS[channel];

    IC1_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER_PERIPHERAL);
    IC1_CombineOddEvenICModules();
    IC1_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);
    
    IC2_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER_PERIPHERAL);
    IC2_CombineOddEvenICModules();
    IC2_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);
    
    if (trig & 7) {
        if ((trig >> 4) == 4) {
            CVR_SetupComparator();
            CMP4_SetupComparator();
        }
        RPINR8bits.IC4R = PIN_MANAGER_DIGITAL_PINS[(trig >> 4) & 0xF];
        IC4_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER_PERIPHERAL);
        IC1_SetCaptureSource(IC_PARAMS_CAPTURE_SOURCE_IC4);
        IC2_SetCaptureSource(IC_PARAMS_CAPTURE_SOURCE_IC4);
    }
    
    DMA_ChannelEnable(DMA_CHANNEL_0);
    DMA_ChannelEnable(DMA_CHANNEL_1);

    IC1_SetCaptureMode(mode);
    IC2_SetCaptureMode(mode);
    
    IC4_SetCaptureMode(trig & 7);
}

void INTERVAL_CaptureTwo(uint16_t count, uint8_t mode, uint8_t channel) {

    DMA_DisableAllChannels();
    
    DMA_PrepareChannel0(count, BUFFER, DMA_PERIPHERAL_IRQ_IC1);
    DMA_PrepareChannel2(count, BUFFER + BUFFER_SIZE / 2, DMA_PERIPHERAL_IRQ_IC3);
    DMA_PrepareChannel1(count, BUFFER + BUFFER_SIZE / 4, DMA_PERIPHERAL_IRQ_IC1);
    DMA_PrepareChannel3(count, BUFFER + 3 * BUFFER_SIZE / 4, DMA_PERIPHERAL_IRQ_IC3);
    
    DMA_InterruptEnable(DMA_CHANNEL_0);

    IC_PARAMS_InitiateAll();

    SetDMA_MODE(DMA_MODES_TWO_CHANNEL);

    RPINR7bits.IC1R = PIN_MANAGER_DIGITAL_PINS[channel & 0xF];
    RPINR8bits.IC3R = PIN_MANAGER_DIGITAL_PINS[(channel >> 4) & 0xF];

    IC_PARAMS_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER_PERIPHERAL);
    IC_PARAMS_CombineOddEvenModules();
    IC_PARAMS_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);

    DMA_EnableAllChannels();

    IC1_SetCaptureMode(mode & 0xF);
    IC2_SetCaptureMode(mode & 0xF);
    IC3_SetCaptureMode((mode >> 4) & 0xF);
    IC4_SetCaptureMode((mode >> 4) & 0xF);
}

void INTERVAL_CaptureThree(uint16_t count, uint16_t mode, uint8_t trig) {
    
    DMA_ChannelDisable(DMA_CHANNEL_0);
    DMA_ChannelDisable(DMA_CHANNEL_1);
    DMA_ChannelDisable(DMA_CHANNEL_2);

    DMA_PrepareChannel0(count, BUFFER, DMA_PERIPHERAL_IRQ_IC1);
    DMA_PrepareChannel1(count, BUFFER + BUFFER_SIZE / 4, DMA_PERIPHERAL_IRQ_IC2);
    DMA_PrepareChannel2(count, BUFFER + BUFFER_SIZE / 2, DMA_PERIPHERAL_IRQ_IC3);
    
    DMA_InterruptEnable(DMA_CHANNEL_0);

    IC_PARAMS_InitiateAll();
    
    SetDMA_MODE(DMA_MODES_THREE_CHANNEL);
    
    RPINR7bits.IC1R = PIN_MANAGER_DIGITAL_PINS_LA1;
    RPINR7bits.IC2R = PIN_MANAGER_DIGITAL_PINS_LA2;
    RPINR8bits.IC3R = PIN_MANAGER_DIGITAL_PINS_LA3;

    IC_PARAMS_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER_PERIPHERAL);
    
    IC1_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);
    IC2_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);
    IC3_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);

    if (trig & 7) {
        if ((trig >> 4) == 4) {
            CVR_SetupComparator();
            CMP4_SetupComparator();
        }
        RPINR8bits.IC4R = PIN_MANAGER_DIGITAL_PINS[(trig >> 4) & 0xF]; 
        IC1_SetCaptureSource(IC_PARAMS_CAPTURE_SOURCE_IC4);
        IC2_SetCaptureSource(IC_PARAMS_CAPTURE_SOURCE_IC4);
        IC3_SetCaptureSource(IC_PARAMS_CAPTURE_SOURCE_IC4);
    }

    DMA_ChannelEnable(DMA_CHANNEL_0);
    DMA_ChannelEnable(DMA_CHANNEL_1);
    DMA_ChannelEnable(DMA_CHANNEL_2);

    IC1_SetCaptureMode(mode & 0xF);
    IC2_SetCaptureMode((mode >> 4) & 0xF);
    IC3_SetCaptureMode((mode >> 8) & 0xF);
    IC4_SetCaptureMode(trig & 7);
}

void INTERVAL_CaptureFour(uint16_t count, uint16_t mode, uint8_t prescaler) {
    
    DMA_DisableAllChannels();
    
    DMA_PrepareChannel0(count, BUFFER, DMA_PERIPHERAL_IRQ_IC1);
    DMA_PrepareChannel1(count, BUFFER + BUFFER_SIZE / 4, DMA_PERIPHERAL_IRQ_IC2);
    DMA_PrepareChannel2(count, BUFFER + BUFFER_SIZE / 2, DMA_PERIPHERAL_IRQ_IC3);
    DMA_PrepareChannel3(count, BUFFER + 3 * BUFFER_SIZE / 4, DMA_PERIPHERAL_IRQ_IC4);

    DMA_InterruptEnable(DMA_CHANNEL_0);

    IC_PARAMS_InitiateAll();
    
    SetDMA_MODE(DMA_MODES_FOUR_CHANNEL);
    
    RPINR7bits.IC1R = PIN_MANAGER_DIGITAL_PINS_LA1;
    RPINR7bits.IC2R = PIN_MANAGER_DIGITAL_PINS_LA2;
    RPINR8bits.IC3R = PIN_MANAGER_DIGITAL_PINS_LA3;
    RPINR8bits.IC4R = PIN_MANAGER_DIGITAL_PINS_LA4;
    
    TMR2_Initialize();
    TMR2_PrescalerSet(prescaler & 0xF);
    TMR2_Counter16BitSet(1);

    IC_PARAMS_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER2);
    IC_PARAMS_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);
    
    DMA_EnableAllChannels();

    IC1_SetCaptureMode(mode & 0xF);
    IC2_SetCaptureMode((mode >> 4) & 0xF);
    IC3_SetCaptureMode((mode >> 8) & 0xF);
    IC4_SetCaptureMode((mode >> 12) & 0xF);
}

response_t INTERVAL_GetState(void) {
    
    UART1_WriteInt(__builtin_dmaoffset(&BUFFER));
    UART1_WriteInt(DMA0STAL);
    UART1_WriteInt(DMA1STAL);
    UART1_WriteInt(DMA2STAL);
    UART1_WriteInt(DMA3STAL);
    UART1_Write(DIGITAL_STATES);
    UART1_Write(DIGITAL_STATES_ERROR);
    
    return SUCCESS;
}

response_t INTERVAL_IntervalMeasure(void) {
    
    uint16_t timeout = UART1_ReadInt(); // t * 64e6 >> 16
    uint8_t pins = UART1_Read();
    uint8_t modes = UART1_Read();
    
    IC_PARAMS_ConfigureIntervalCaptureWithIC1AndIC2(pins & 0xF, 
            IC_PARAMS_CAPTURE_TIMER_PERIPHERAL, 
            IC_PARAMS_CAPTURE_INTERRUPT_EVERY_EVENT, modes & 0x7);
    IC_PARAMS_ConfigureIntervalCaptureWithIC3AndIC4((pins >> 4) & 0xF, 
            IC_PARAMS_CAPTURE_TIMER_PERIPHERAL, 
            IC_PARAMS_CAPTURE_INTERRUPT_EVERY_EVENT, (modes >> 3) & 0x7);
    
    IC_PARAMS_ManualTriggerAll();
    
    while ((!_IC1IF) && (IC2TMR < timeout)) WATCHDOG_TimerClear();
    UART1_WriteInt(IC1BUF);
    UART1_WriteInt(IC2BUF);
    
    while ((!_IC3IF) && (IC2TMR < timeout)) WATCHDOG_TimerClear();
    UART1_WriteInt(IC3BUF);
    UART1_WriteInt(IC4BUF);
    UART1_WriteInt(IC2TMR);
    
    IC_PARAMS_DisableAllModules();
    
    return SUCCESS;
}

response_t INTERVAL_TimeMeasure(void) {
    
    uint16_t timeout = UART1_ReadInt(); // t * 64e6 >> 16
    uint8_t pins = UART1_Read();
    uint8_t modes = UART1_Read();
    uint8_t intrpts = UART1_Read();
    
    if ((pins & 0xF) == 4 || ((pins >> 4) & 0xF) == 4) {
        CMP4_SetupComparator();
        CVR_SetupComparator();
    }
    
    IC_PARAMS_ConfigureIntervalCaptureWithIC1AndIC2(pins & 0xF, 
            IC_PARAMS_CAPTURE_TIMER2, (intrpts & 0xF) - 1, modes & 0x7);
    IC_PARAMS_ConfigureIntervalCaptureWithIC3AndIC4((pins >> 4) & 0xF, 
            IC_PARAMS_CAPTURE_TIMER2, ((intrpts >> 4) & 0xF) - 1, (modes >> 3) & 0x7);
    
    TMR2_Initialize();
    
    SetDefaultDIGITAL_STATES();
    
    IC_PARAMS_ManualTriggerAll();
    TMR2_Start();
    
    if ((modes >> 6) & 0x1) {
        RPOR5bits.RP54R = RPN_DEFAULT_PORT; // Disconnect SQR1 pin
        ((modes >> 7) & 0x1) ? SQR1_SetHigh() : SQR1_SetLow();
    }
    
    while ((!_IC1IF || !_IC3IF) && (IC2TMR < timeout)) WATCHDOG_TimerClear();
    
    uint8_t i;
    for (i = 0; i < (intrpts & 0xF); i++) {
        UART1_WriteInt(IC1BUF);
        UART1_WriteInt(IC2BUF);
    }
    for (i = 0; i < ((intrpts >> 4) & 0xF); i++) {
        UART1_WriteInt(IC3BUF);
        UART1_WriteInt(IC4BUF);
    }
    
    IC1_InterruptFlagClear();
    IC3_InterruptFlagClear();
    
    UART1_WriteInt(IC2TMR);
    
    IC_PARAMS_DisableAllModules();
    TMR2_Stop();
    
    return SUCCESS;
}

response_t INTERVAL_UntilEvent(void) {
    
    uint16_t timeout = UART1_ReadInt(); // t * 64e6 >> 16
    uint8_t mode = UART1_Read();
    uint8_t pin = UART1_Read();
    
    IC_PARAMS_ConfigureIntervalCaptureWithIC1AndIC2(pin & 0xF, 
            IC_PARAMS_CAPTURE_TIMER_PERIPHERAL, (mode & 0x3) - 1, mode & 0x7);
    
    while (!_IC1IF && (IC2TMR < timeout)) WATCHDOG_TimerClear();
    
    IC1_InterruptFlagClear();
    
    UART1_WriteInt(IC2TMR);
    
    uint8_t i;
    for (i = 0; i < (mode & 0x3); i++) {
        UART1_WriteInt(IC1BUF);
        UART1_WriteInt(IC2BUF);
    }
    
    IC_PARAMS_DisableAllModules();
    TMR2_Stop();
    
    return SUCCESS;
}
