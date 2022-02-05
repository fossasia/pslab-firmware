/*******************************************************************************
 * This file contains secondary measurements and configurations used by other
 * instruments
 ******************************************************************************/

#include <stdint.h>
#include "../bus/uart/uart1.h"
#include "../helpers/delay.h"
#include "../registers/comparators/cmp4.h"
#include "../registers/comparators/cvr.h"
#include "../registers/comparators/ic1.h"
#include "../registers/comparators/ic2.h"
#include "../registers/comparators/ic3.h"
#include "../registers/comparators/ic4.h"
#include "../registers/comparators/ic_params.h"
#include "../registers/memory/dma.h"
#include "../registers/timers/tmr2.h"
#include "../registers/system/pin_manager.h"
#include "../states.h"

void SENSORS_ConfigureInterval(uint8_t pin1, IC_PARAMS_CAPTURE_MODE pin1_edge, 
    uint8_t pin2, IC_PARAMS_CAPTURE_MODE pin2_edge) {

    // Map pin1 as IC1 source pin
    RPINR7bits.IC1R = PIN_MANAGER_DIGITAL_PINS[pin1];
    // Map pin2 as IC3 source pin
    RPINR8bits.IC3R = PIN_MANAGER_DIGITAL_PINS[pin2];

    IC_PARAMS_InitiateAll();

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

    IC_PARAMS_ManualTriggerAll();
}

response_t SENSORS_StartCounter(void) {

    uint8_t channel = UART1_Read();

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
    UART1_WriteInt(TMR2_Counter16BitGet());
    return SUCCESS;
}

response_t SENSORS_HCSR04(void) {
    
    uint16_t timeout = UART1_ReadInt();
    
    // Unassign SQR1 pin from it's original functions and reuse with HCSR04
    RPOR5bits.RP54R = RPN_DEFAULT_PORT;
    // Disable DMA functions on Channel 2 and Channel 3
    DMA_ChannelDisable(DMA_CHANNEL_2);
    DMA_FlagInterruptClear(DMA_CHANNEL_2);
    DMA_InterruptDisable(DMA_CHANNEL_2);
    DMA_ChannelDisable(DMA_CHANNEL_3);
    DMA_FlagInterruptClear(DMA_CHANNEL_3);
    DMA_InterruptDisable(DMA_CHANNEL_3);
    
    SENSORS_ConfigureInterval(
        PIN_MANAGER_DIGITAL_PINS_LA1, IC_PARAMS_CAPTURE_MODE_EVERY_RISING_EDGE,
        PIN_MANAGER_DIGITAL_PINS_LA1, IC_PARAMS_CAPTURE_MODE_EVERY_FALLING_EDGE
    );

    // Initiate trigger from TRIG pin
    SQR1_SetHigh();
    // Delay for 10 micro seconds to trigger transmitter
    DELAY_us(10);
    SQR1_SetLow();
    
    // Wait until IC1 captures the rising edge from ECHO pin to start timing
    while ((!_IC1IF) && (IC2TMR < timeout));
    UART1_WriteInt(IC1_CaptureDataRead());
    UART1_WriteInt(IC2_CaptureDataRead());

    // Wait until IC3 captures the falling edge from ECHO pin to stop the timer
    while ((!_IC3IF) && (IC4TMR < timeout));
    UART1_WriteInt(IC3_CaptureDataRead());
    UART1_WriteInt(IC4_CaptureDataRead());
    // Fetch the timer value from IC2 module to measure ToF from python side
    UART1_WriteInt(IC2TMR);

    // Reset and disable all input capture modules
    IC_PARAMS_DisableAllModules();
    
    return SUCCESS;
}
