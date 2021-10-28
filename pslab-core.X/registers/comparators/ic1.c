#include "ic1.h"

void IC1_Initialize(void) {
    IC1_InitializeCON1();
    IC1_InitializeCON2();
}

void IC1_InitializeCON1(void) {
    // Input capture continues to operate in CPU idle mode
    IC1CON1bits.ICSIDL = 0;
    // Input capture clock source is set to timer 3
    IC1CON1bits.ICTSEL = IC_PARAMS_CAPTURE_TIMER3;
    // Interrupt on every capture event
    IC1CON1bits.ICI = IC_PARAMS_CAPTURE_INTERRUPT_EVERY_EVENT;
    // Capture event flags (Read Only; but settable)
    IC1CON1bits.ICOV = 0; // No overflow
    IC1CON1bits.ICBNE = 0; // Buffer empty
    // Input capture mode is turned off
    IC1CON1bits.ICM = IC_PARAMS_CAPTURE_MODE_OFF;
}

void IC1_InitializeCON2(void) {
    // Input capture modules are not concatenated
    IC1CON2bits.IC32 = 0;
    // Input source is used to sync the capture timer
    IC1CON2bits.ICTRIG = 0;
    // IC1TMR is cleared and not triggered
    IC1CON2bits.TRIGSTAT = 0;
    // Input capture source is none
    IC1CON2bits.SYNCSEL = IC_PARAMS_CAPTURE_SOURCE_NONE;
}

void IC1_Start(IC_PARAMS_CAPTURE_MODE mode) {
    IC1CON1bits.ICM = mode;
}

void IC1_Stop(void) {
    IC1CON1bits.ICM = IC_PARAMS_CAPTURE_MODE_OFF;
}

uint16_t IC1_CaptureDataRead(void) {
    return (IC1BUF);
}

void IC1_ManualTriggerSet(void) {
    IC1CON2bits.TRIGSTAT = true;
}

bool IC1_TriggerStatusGet(void) {
    return (IC1CON2bits.TRIGSTAT);
}

void IC1_TriggerStatusClear(void) {
    IC1CON2bits.TRIGSTAT = 0;
}

bool IC1_HasCaptureBufferOverflowed(void) {
    return (IC1CON1bits.ICOV);
}

bool IC1_IsCaptureBufferEmpty(void) {
    return (!IC1CON1bits.ICBNE);
}
