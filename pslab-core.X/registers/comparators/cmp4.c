#include "cmp4.h"

void CMP4_Initialize(void) {
    // PSIDL disabled; 
    CMSTAT = 0x00;
    // CON enabled; CPOL Not Inverted; EVPOL Disabled; COE disabled; CCH C4IN1-; CREF C4IN1+; CEVT disabled; 
    CM4CON = 0x8000 & ~(0x8000); //disabling Comparator CON bit
    // SELSRCC PWM1L; SELSRCB PWM1L; SELSRCA PWM1L; 
    CM4MSKSRC = 0x00;
    // AANEN disabled; ABNEN disabled; ACNEN disabled; HLMS disabled; OANEN disabled; ABEN disabled; ACEN disabled; AAEN disabled; PAGS disabled; OBEN disabled; OCEN disabled; NAGS disabled; OCNEN disabled; OBNEN disabled; OAEN disabled; 
    CM4MSKCON = 0x00;
    // CFSEL FOSC/2; CFLTREN disabled; CFDIV 1:1; 
    CM4FLTR = 0x00;

    CM4CONbits.CON = 1; //enabling Comparator CON bit
}

bool CMP4_OutputStatusGet(void) {
    return (CM4CONbits.COUT);
}

bool CMP4_EventStatusGet(void) {
    return (CM4CONbits.CEVT);
}

void CMP4_EventStatusReset(void) {
    CM4CONbits.CEVT = 0;
}

bool CMP4_HLMSStatusGet(void) {
    return (CM4MSKCONbits.HLMS);
}

void CMP4_ComparatorDisable(void) {
    CM4CONbits.CON = 0;
}

void CMP4_ComparatorEnable(void) {
    CM4CONbits.CON = 1;
}

void __attribute__((weak)) CMP4_CallBack(void) {
    // Add your custom callback code here
}

void CMP4_Tasks(void) {
    if (IFS1bits.CMIF) {
        // CMP4 callback function 
        CMP4_CallBack();

        // Clear the CEVT bit to enable further interrupts
        CMP4_EventStatusReset();

        // clear the CMP4 interrupt flag
        IFS1bits.CMIF = 0;
    }
}
