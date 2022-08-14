#include "ic3.h"

void IC3_Initialize(void) {
    IC3_InitializeCON1();
    IC3_InitializeCON2();
}

void IC3_InitializeCON1(void) {
    // Input capture continues to operate in CPU idle mode
    IC3CON1bits.ICSIDL = 0;
    // Input capture clock source is set to timer 3
    IC3CON1bits.ICTSEL = IC_PARAMS_CAPTURE_TIMER3;
    // Interrupt on every capture event
    IC3CON1bits.ICI = IC_PARAMS_CAPTURE_INTERRUPT_EVERY_EVENT;
    // Capture event flags (Read Only; but settable)
    IC3CON1bits.ICOV = 0; // No overflow
    IC3CON1bits.ICBNE = 0; // Buffer empty
    // Input capture mode is turned off
    IC3CON1bits.ICM = IC_PARAMS_CAPTURE_MODE_OFF;
}

void IC3_InitializeCON2(void) {
    // Input capture modules are not concatenated
    IC3CON2bits.IC32 = 0;
    // Input source is used to sync the capture timer
    IC3CON2bits.ICTRIG = 0;
    // IC3TMR is cleared and not triggered
    IC3CON2bits.TRIGSTAT = 0;
    // Input capture source is none
    IC3CON2bits.SYNCSEL = IC_PARAMS_CAPTURE_SOURCE_NONE;
}

void IC3_Start(IC_PARAMS_CAPTURE_MODE mode) {
    IC3CON1bits.ICM = mode;
}

void IC3_Stop(void) {
    IC3CON1bits.ICM = IC_PARAMS_CAPTURE_MODE_OFF;
}

uint16_t IC3_CaptureDataRead(void) {
    return (IC3BUF);
}

void IC3_ManualTriggerSet(void) {
    IC3CON2bits.TRIGSTAT = true;
}

bool IC3_TriggerStatusGet(void) {
    return (IC3CON2bits.TRIGSTAT);
}

void IC3_TriggerStatusClear(void) {
    IC3CON2bits.TRIGSTAT = 0;
}

bool IC3_HasCaptureBufferOverflowed(void) {
    return (IC3CON1bits.ICOV);
}

bool IC3_IsCaptureBufferEmpty(void) {
    return (!IC3CON1bits.ICBNE);
}
