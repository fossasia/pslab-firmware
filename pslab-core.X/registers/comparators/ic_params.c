#include "ic1.h"
#include "ic2.h"
#include "ic3.h"
#include "ic4.h"
#include "../system/pin_manager.h"

void IC_PARAMS_InitiateAll(void) {
    IC1_Initialize();
    IC2_Initialize();
    IC3_Initialize();
    IC4_Initialize();
}

void IC_PARAMS_StopAllModules(void) {
    IC1_Stop();
    IC2_Stop();
    IC3_Stop();
    IC4_Stop();
}

void IC_PARAMS_ClearTriggerStatus(void) {
    IC1_TriggerStatusClear();
    IC2_TriggerStatusClear();
    IC3_TriggerStatusClear();
    IC4_TriggerStatusClear();
}

void IC_PARAMS_ClearBufferOverflow(void) {
    IC1CON1bits.ICOV = 0;
    IC2CON1bits.ICOV = 0;
    IC3CON1bits.ICOV = 0;
    IC4CON1bits.ICOV = 0; 
}

void IC_PARAMS_DisableAllModules(void) {
    IC_PARAMS_ClearTriggerStatus();
    IC_PARAMS_StopAllModules();
    IC_PARAMS_ClearBufferOverflow();
}

void IC_PARAMS_ManualTriggerAll(void) {
    IC1_ManualTriggerSet();
    IC2_ManualTriggerSet();
    IC3_ManualTriggerSet();
    IC4_ManualTriggerSet();
}

void IC_PARAMS_UseSourceTo(IC_PARAMS_SOURCE_TASK t) {
    IC1_UseSourceTo(t);
    IC2_UseSourceTo(t);
    IC3_UseSourceTo(t);
    IC4_UseSourceTo(t);
}

void IC_PARAMS_CombineOddEvenModules(void) {
    IC1_CombineOddEvenICModules();
    IC2_CombineOddEvenICModules();
    IC3_CombineOddEvenICModules();
    IC4_CombineOddEvenICModules();
}

void IC_PARAMS_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER t) {
    IC1_SetCaptureTimer(t);
    IC2_SetCaptureTimer(t);
    IC3_SetCaptureTimer(t);
    IC4_SetCaptureTimer(t);
}

void IC_PARAMS_ConfigureIntervalCaptureWithIC1AndIC2(uint8_t pin, 
        IC_PARAMS_CAPTURE_TIMER timer, 
        IC_PARAMS_CAPTURE_INTERRUPT intrpt,
        IC_PARAMS_CAPTURE_MODE mode) {
    
    IC1_InterruptFlagClear();
    
    RPINR7bits.IC1R = PIN_MANAGER_DIGITAL_PINS[pin];
    
    IC1_Initialize();
    IC1_CombineOddEvenICModules();
    IC1_SetCaptureTimer(timer);
    IC1_InputCaptureInterruptOn(intrpt);
    IC1_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);
    IC1_SetCaptureMode(mode);
    
    IC2_Initialize();
    IC2_CombineOddEvenICModules();
    IC2_SetCaptureTimer(timer);
    IC2_InputCaptureInterruptOn(intrpt);    
    IC2_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);
    IC2_SetCaptureMode(mode);
}

void IC_PARAMS_ConfigureIntervalCaptureWithIC3AndIC4(uint8_t pin, 
        IC_PARAMS_CAPTURE_TIMER timer, 
        IC_PARAMS_CAPTURE_INTERRUPT intrpt,
        IC_PARAMS_CAPTURE_MODE mode) {
    
    IC3_InterruptFlagClear();
    
    RPINR8bits.IC3R = PIN_MANAGER_DIGITAL_PINS[pin];
    
    IC3_Initialize();
    IC3_CombineOddEvenICModules();
    IC3_SetCaptureTimer(timer);
    IC3_InputCaptureInterruptOn(intrpt);
    IC3_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);
    IC3_SetCaptureMode(mode);
    
    IC4_Initialize();
    IC4_CombineOddEvenICModules();
    IC4_SetCaptureTimer(timer);
    IC4_InputCaptureInterruptOn(intrpt);    
    IC4_UseSourceTo(IC_PARAMS_SOURCE_TASK_TRIGGER);
    IC4_SetCaptureMode(mode);
}