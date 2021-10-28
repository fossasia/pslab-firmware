#include "ic2.h"

void IC2_Initialize(void) {
    IC2_InitializeCON1();
    IC2_InitializeCON2();
}

void IC2_InitializeCON1(void) {
    // Input capture continues to operate in CPU idle mode
    IC2CON1bits.ICSIDL = 0;
    // Input capture clock source is set to timer 3
    IC2CON1bits.ICTSEL = IC_PARAMS_CAPTURE_TIMER3;
    // Interrupt on every capture event
    IC2CON1bits.ICI = IC_PARAMS_CAPTURE_INTERRUPT_EVERY_EVENT;
    // Capture event flags (Read Only; but settable)
    IC2CON1bits.ICOV = 0; // No overflow
    IC2CON1bits.ICBNE = 0; // Buffer empty
    // Input capture mode is turned off
    IC2CON1bits.ICM = IC_PARAMS_CAPTURE_MODE_OFF;
}

void IC2_InitializeCON2(void) {
    // Input capture modules are not concatenated
    IC2CON2bits.IC32 = 0;
    // Input source is used to sync the capture timer
    IC2CON2bits.ICTRIG = 0;
    // IC2TMR is cleared and not triggered
    IC2CON2bits.TRIGSTAT = 0;
    // Input capture source is none
    IC2CON2bits.SYNCSEL = IC_PARAMS_CAPTURE_SOURCE_NONE;
}

void IC2_Start(IC_PARAMS_CAPTURE_MODE mode) {
    IC2CON1bits.ICM = mode;
}

void IC2_Stop(void) {
    IC2CON1bits.ICM = IC_PARAMS_CAPTURE_MODE_OFF;
}

uint16_t IC2_CaptureDataRead(void) {
    return (IC2BUF);
}

void IC2_ManualTriggerSet(void) {
    IC2CON2bits.TRIGSTAT = true;
}

bool IC2_TriggerStatusGet(void) {
    return (IC2CON2bits.TRIGSTAT);
}

void IC2_TriggerStatusClear(void) {
    IC2CON2bits.TRIGSTAT = 0;
}

bool IC2_HasCaptureBufferOverflowed(void) {
    return (IC2CON1bits.ICOV);
}

bool IC2_IsCaptureBufferEmpty(void) {
    return (!IC2CON1bits.ICBNE);
}
