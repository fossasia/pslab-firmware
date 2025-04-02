/*******************************************************************************
 * This file contains secondary measurements and configurations used by other
 * instruments
 ******************************************************************************/
#include <string.h>
#include <stdint.h>

#include "../registers/comparators/cmp4.h"
#include "../registers/comparators/cvr.h"
#include "../registers/comparators/ic1.h"
#include "../registers/comparators/ic2.h"
#include "../registers/comparators/ic3.h"
#include "../registers/comparators/ic4.h"
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

enum Status SENSORS_start_counter(
    uint8_t **args,
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    struct Input {
        uint8_t channel;
        uint8_t _pad[0];
    } *input = NULL;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) {
        return E_BAD_ARGSIZE;
    }

    input = *(struct Input **)args;
    TMR2_Initialize();
    // Select external source as clock source
    T2CONbits.TCS = 1;

    if (input->channel == 4) {
        CVR_SetupComparator();
        CMP4_SetupComparator();
    }

    // Map incoming pin to TMR2
    RPINR3bits.T2CKR = PIN_MANAGER_DIGITAL_PINS[input->channel];
    TMR2_Start();

    return E_OK;
}

enum Status SENSORS_get_counter(
    __attribute__ ((unused)) uint8_t *const args,
    __attribute__ ((unused)) uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    uint16_t const count = TMR2_Counter16BitGet();
    if ( !(*rets = malloc(sizeof(count))) )  { return E_MEMORY_INSUFFICIENT; }
    **rets = count;
    *rets_size = sizeof(count);
    return E_OK;
}
