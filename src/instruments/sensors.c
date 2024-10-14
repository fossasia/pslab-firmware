/*******************************************************************************
 * This file contains secondary measurements and configurations used by other
 * instruments
 ******************************************************************************/

#include "../registers/comparators/cmp4.h"
#include "../registers/comparators/cvr.h"
#include "../registers/comparators/ic1.h"
#include "../registers/comparators/ic2.h"
#include "../registers/comparators/ic3.h"
#include "../registers/comparators/ic4.h"
#include "../bus/uart/uart.h"
#include "../registers/timers/tmr2.h"
#include "../registers/system/pin_manager.h"

void SENSORS_ConfigureInterval(uint8_t pin1, uint8_t pin2,
        IC_PARAMS_CAPTURE_MODE pin1_edge, IC_PARAMS_CAPTURE_MODE pin2_edge) {

    // Map pin1 as IC1 source pin
    RPINR7bits.IC1R = PIN_MANAGER_DIGITAL_PINS[pin1];
    // Map pin2 as IC3 source pin
    RPINR8bits.IC3R = PIN_MANAGER_DIGITAL_PINS[pin2];

    IC1_Initialize();
    IC2_Initialize();
    IC3_Initialize();
    IC4_Initialize();

    IC1_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER_PERIPHERAL);
    IC2_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER_PERIPHERAL);
    IC3_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER_PERIPHERAL);
    IC4_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER_PERIPHERAL);

    // Combine IC1 and IC2 modules to capture one type of edge
    IC1_CombineOddEvenICModules();
    IC2_CombineOddEvenICModules();
    // Combine IC3 and IC4 modules to capture another type of edge
    IC3_CombineOddEvenICModules();
    IC4_CombineOddEvenICModules();

    IC1_InterruptFlagClear();
    IC3_InterruptFlagClear();

    IC1_SetCaptureMode(pin1_edge);
    IC2_SetCaptureMode(pin1_edge);
    IC3_SetCaptureMode(pin2_edge);
    IC4_SetCaptureMode(pin2_edge);

    IC1_ManualTriggerSet();
    IC2_ManualTriggerSet();
    IC3_ManualTriggerSet();
    IC4_ManualTriggerSet();
}

response_t SENSORS_StartCounter(void) {

    uint8_t channel = UART2_Read();

    TMR2_Initialize();
    // Select external source as clock source
    T2CONbits.TCS = 1;

    if (channel == 4) {
        CVR_SetupComparator();
        CMP4_SetupComparator();
    }

    // Map incoming pin to TMR2 
    RPINR3bits.T2CKR = PIN_MANAGER_DIGITAL_PINS[channel];
    TMR2_Start();

    return SUCCESS;
}

response_t SENSORS_GetCounter(void) {
    // Fetch timer 2 value and send it over
    UART2_WriteInt(TMR2_Counter16BitGet());
    return SUCCESS;
}