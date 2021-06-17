#include "cmp3.h"

void CMP3_Initialize(void) {
    // PSIDL disabled; 
    CMSTAT = 0x00;
    // CON enabled; CPOL Not Inverted; OPMODE Comparator; EVPOL Disabled; COE disabled; CCH C3IN1-; CREF C3IN1+; CEVT disabled; 
    CM3CON = 0x8000 & ~(0x8000); //disabling Comparator CON bit
    // SELSRCC PWM1L; SELSRCB PWM1L; SELSRCA PWM1L; 
    CM3MSKSRC = 0x00;
    // AANEN disabled; ABNEN disabled; ACNEN disabled; HLMS disabled; OANEN disabled; ABEN disabled; ACEN disabled; AAEN disabled; PAGS disabled; OBEN disabled; OCEN disabled; NAGS disabled; OCNEN disabled; OBNEN disabled; OAEN disabled; 
    CM3MSKCON = 0x00;
    // CFSEL FOSC/2; CFLTREN disabled; CFDIV 1:1; 
    CM3FLTR = 0x00;

    CM3CONbits.CON = 1; //enabling Comparator CON bit
}

bool CMP3_OutputStatusGet(void) {
    return (CM3CONbits.COUT);
}

bool CMP3_EventStatusGet(void) {
    return (CM3CONbits.CEVT);
}

void CMP3_EventStatusReset(void) {
    CM3CONbits.CEVT = 0;
}

bool CMP3_HLMSStatusGet(void) {
    return (CM3MSKCONbits.HLMS);
}

void CMP3_ComparatorDisable(void) {
    CM3CONbits.CON = 0;
}

void CMP3_ComparatorEnable(void) {
    CM3CONbits.CON = 1;
}

void __attribute__((weak)) CMP3_CallBack(void) {
    // Add your custom callback code here
}

void CMP3_Tasks(void) {
    if (IFS1bits.CMIF) {
        // CMP3 callback function 
        CMP3_CallBack();

        // Clear the CEVT bit to enable further interrupts
        CMP3_EventStatusReset();

        // clear the CMP3 interrupt flag
        IFS1bits.CMIF = 0;
    }
}
