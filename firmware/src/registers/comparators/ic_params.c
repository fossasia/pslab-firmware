#include "ic1.h"
#include "ic2.h"
#include "ic3.h"
#include "ic4.h"

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