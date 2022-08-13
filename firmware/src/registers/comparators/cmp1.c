#include "cmp1.h"

void CMP1_Initialize(void) {
    // PSIDL disabled; 
    CMSTAT = 0x00;
    // CON enabled; CPOL Not Inverted; OPMODE Comparator; EVPOL Disabled; COE disabled; CCH C1IN1-; CREF C1IN1+; CEVT disabled; 
    CM1CON = 0x8000 & ~(0x8000); //disabling Comparator CON bit
    // SELSRCC PWM1L; SELSRCB PWM1L; SELSRCA PWM1L; 
    CM1MSKSRC = 0x00;
    // AANEN disabled; ABNEN disabled; ACNEN disabled; HLMS disabled; OANEN disabled; ABEN disabled; ACEN disabled; AAEN disabled; PAGS disabled; OBEN disabled; OCEN disabled; NAGS disabled; OCNEN disabled; OBNEN disabled; OAEN disabled; 
    CM1MSKCON = 0x00;
    // CFSEL FOSC/2; CFLTREN disabled; CFDIV 1:1; 
    CM1FLTR = 0x00;

    CM1CONbits.CON = 1; //enabling Comparator CON bit
}

bool CMP1_OutputStatusGet(void) {
    return (CM1CONbits.COUT);
}

bool CMP1_EventStatusGet(void) {
    return (CM1CONbits.CEVT);
}

void CMP1_EventStatusReset(void) {
    CM1CONbits.CEVT = 0;
}

bool CMP1_HLMSStatusGet(void) {
    return (CM1MSKCONbits.HLMS);
}

void CMP1_ComparatorDisable(void) {
    CM1CONbits.CON = 0;
}

void CMP1_ComparatorEnable(void) {
    CM1CONbits.CON = 1;
}

void __attribute__((weak)) CMP1_CallBack(void) {
    // Add your custom callback code here
}

void CMP1_Tasks(void) {
    if (IFS1bits.CMIF) {
        // CMP1 callback function 
        CMP1_CallBack();

        // Clear the CEVT bit to enable further interrupts
        CMP1_EventStatusReset();

        // clear the CMP1 interrupt flag
        IFS1bits.CMIF = 0;
    }
}
