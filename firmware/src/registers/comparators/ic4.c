#include "ic4.h"
#include "../../helpers/interval.h"

void __attribute__((__interrupt__, no_auto_psv)) _IC4Interrupt(void) {
    SetDefaultDIGITAL_STATES();
    IC4_Stop();
    IC4_InterruptFlagClear();
    IC4_InterruptDisable();
    SetDefaultDIGITAL_STATES_ERROR();
}

void IC4_Initialize(void) {
    IC4_InitializeCON1();
    IC4_InitializeCON2();
}

void IC4_InitializeCON1(void) {
    // Input capture continues to operate in CPU idle mode
    IC4CON1bits.ICSIDL = 0;
    // Input capture clock source is set to timer 3
    IC4CON1bits.ICTSEL = IC_PARAMS_CAPTURE_TIMER3;
    // Interrupt on every capture event
    IC4CON1bits.ICI = IC_PARAMS_CAPTURE_INTERRUPT_EVERY_EVENT;
    // Capture event flags (Read Only; but settable)
    IC4CON1bits.ICOV = 0; // No overflow
    IC4CON1bits.ICBNE = 0; // Buffer empty
    // Input capture mode is turned off
    IC4CON1bits.ICM = IC_PARAMS_CAPTURE_MODE_OFF;
}

void IC4_InitializeCON2(void) {
    // Input capture modules are not concatenated
    IC4CON2bits.IC32 = 0;
    // Input source is used to sync the capture timer
    IC4CON2bits.ICTRIG = 0;
    // IC4TMR is cleared and not triggered
    IC4CON2bits.TRIGSTAT = 0;
    // Input capture source is none
    IC4CON2bits.SYNCSEL = IC_PARAMS_CAPTURE_SOURCE_NONE;
}

void IC4_Start(IC_PARAMS_CAPTURE_MODE mode) {
    IC4CON1bits.ICM = mode;
}

void IC4_Stop(void) {
    IC4CON1bits.ICM = IC_PARAMS_CAPTURE_MODE_OFF;
}

uint16_t IC4_CaptureDataRead(void) {
    return (IC4BUF);
}

void IC4_ManualTriggerSet(void) {
    IC4CON2bits.TRIGSTAT = true;
}

bool IC4_TriggerStatusGet(void) {
    return (IC4CON2bits.TRIGSTAT);
}

void IC4_TriggerStatusClear(void) {
    IC4CON2bits.TRIGSTAT = 0;
}

bool IC4_HasCaptureBufferOverflowed(void) {
    return (IC4CON1bits.ICOV);
}

bool IC4_IsCaptureBufferEmpty(void) {
    return (!IC4CON1bits.ICBNE);
}
