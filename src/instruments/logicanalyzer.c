#include <stdint.h>
#include "../bus/uart/uart1.h"
#include "../helpers/interval.h"
#include "../registers/comparators/ic1.h"
#include "../registers/comparators/ic2.h"
#include "../registers/comparators/ic3.h"
#include "../registers/comparators/ic4.h"
#include "../registers/comparators/ic_params.h"
#include "../registers/system/interrupt_manager.h"
#include "../registers/system/pin_manager.h"
#include "../registers/timers/tmr2.h"
#include "../commands.h"

#define FALLING_EDGE        1
#define RISING_EDGE         0

static uint8_t LA_TRIGGER_CHANNEL = 32;
void SetLA_TRIGGER_CHANNEL(uint8_t V) { LA_TRIGGER_CHANNEL = V; }
uint8_t GetLA_TRIGGER_CHANNEL(void) { return LA_TRIGGER_CHANNEL; }

static uint8_t LA_TRIGGER_STATE = 0;
void SetLA_TRIGGER_STATE(uint8_t V) { LA_TRIGGER_STATE = V; }
uint8_t GetLA_TRIGGER_STATE(void) { return LA_TRIGGER_STATE; }

response_t LOGICANALYZER_OneChannel(void) {
    
    uint16_t points = UART1_ReadInt();
    uint8_t trigger = UART1_Read();
    uint8_t config = UART1_Read();
    
    if (trigger & 1) {
        INTCON2bits.INT2EP = trigger & 2 ? FALLING_EDGE : RISING_EDGE;
        RPINR1bits.INT2R = PIN_MANAGER_DIGITAL_PINS[(trigger >> 4) & 0xF];
    }
    
    INTERVAL_CaptureOne(points, (config >> 4) & 0xF, config & 0xF, 0);
    
    if (trigger & 1) {
        INTERRUPT_ClearExternalInterrupt2Flag();
        INTERRUPT_EnableExternalInterrupt2();
    } else {
        SetDefaultDIGITAL_STATES();
        IC1_ManualTriggerSet();
        IC2_ManualTriggerSet();
    }

    return SUCCESS;
}

response_t LOGICANALYZER_OneChannelAlt(void) {
    
    uint16_t points = UART1_ReadInt();
    uint8_t config = UART1_Read();
    uint8_t trigger = UART1_Read();
    
    IC4_InterruptHighPriority();
    IC4_InterruptFlagClear();
    IC4_InterruptDisable();
    
    if (trigger & 7) {
        INTERVAL_CaptureOne(points, (config >> 4) & 0xF, config & 0xF, trigger);
        IC4_InterruptEnable();
    } else {
        INTERVAL_CaptureOne(points, (config >> 4) & 0xF, config & 0xF, 0);
        SetDefaultDIGITAL_STATES();
        IC1_ManualTriggerSet();
        IC2_ManualTriggerSet();
    }
    
    return SUCCESS;
}

response_t LOGICANALYZER_TwoChannel(void) {
    
    uint16_t points = UART1_ReadInt();
    uint8_t trigger = UART1_Read();
    uint8_t config = UART1_Read();
    uint8_t channel = UART1_Read();

    if (trigger & 1) {
        INTCON2bits.INT2EP = trigger & 2 ? FALLING_EDGE : RISING_EDGE;
        RPINR1bits.INT2R = PIN_MANAGER_DIGITAL_PINS[(trigger >> 4) & 0xF];
    }
    
    INTERVAL_CaptureTwo(points, config, channel);
    
    if (trigger & 1) {
        INTERRUPT_ClearExternalInterrupt2Flag();
        INTERRUPT_EnableExternalInterrupt2();
    } else {
        SetDefaultDIGITAL_STATES();
        IC_PARAMS_ManualTriggerAll();
        SetDefaultDIGITAL_STATES_ERROR();
    }

    return SUCCESS;
    
    // TODO: Implement trigger actions for any of the two channels. Right now
    // only one channel can be set as trigger source. Using CN interrupts, this
    // can be extended to listen to multiple trigger sources.
}

response_t LOGICANALYZER_ThreeChannel(void) {
    
    uint16_t points = UART1_ReadInt();
    uint8_t config = UART1_ReadInt();
    uint8_t trigger = UART1_Read();
    
    IC4_InterruptHighPriority();
    IC4_InterruptFlagClear();
    IC4_InterruptDisable();
    
    if (trigger & 7) {
        INTERVAL_CaptureThree(points, config & 0x0FFF, trigger);
        IC4_InterruptEnable();
    } else {
        INTERVAL_CaptureThree(points, config & 0x0FFF, 0);
        SetDefaultDIGITAL_STATES();
        IC1_ManualTriggerSet();
        IC2_ManualTriggerSet();
        IC3_ManualTriggerSet();
        SetDefaultDIGITAL_STATES_ERROR();
    }

    return SUCCESS;
    
    // TODO: Implement trigger actions for any of the three channels. Right now
    // only one channel can be set as trigger source. Using CN interrupts, this
    // can be extended to listen to multiple trigger sources.
}

response_t LOGICANALYZER_FourChannel(void) {
    
    uint16_t points = UART1_ReadInt();
    uint16_t mode = UART1_ReadInt();
    uint8_t prescaler = UART1_Read();
    uint8_t trigger = UART1_Read();
    
    SetDIGITAL_STATES(0);
    INTERVAL_CaptureFour(points, mode, prescaler);
    
    if (trigger & 1) {
        LA_TRIGGER_STATE = trigger & 2 ? RISING_EDGE : FALLING_EDGE;
        LA_TRIGGER_CHANNEL = 0;
        
        if ((trigger >> 2) & 1) {
            INTERRUPT_LA1PinChange(true);
            LA_TRIGGER_CHANNEL |= 1;
        }
        if ((trigger >> 3) & 1) {
            INTERRUPT_LA2PinChange(true);
            LA_TRIGGER_CHANNEL |= 2;
        }
        if ((trigger >> 4) & 1) {
            INTERRUPT_LA3PinChange(true);
            LA_TRIGGER_CHANNEL |= 4;
        }
        if ((trigger >> 5) & 1) {
            INTERRUPT_LA4PinChange(true);
            LA_TRIGGER_CHANNEL |= 8;
        }
        INTERRUPT_ClearPinChangeInterruptsFlag();
        INTERRUPT_EnablePinChangeInterrupts();
    } else {
        TMR2_Start();
        SetDefaultDIGITAL_STATES();
        IC_PARAMS_ManualTriggerAll();
        SetDefaultDIGITAL_STATES_ERROR();
    }

    return SUCCESS;
}

response_t LOGICANALYZER_Stop(void) {
    
    IC_PARAMS_DisableAllModules();

    return SUCCESS;
}
