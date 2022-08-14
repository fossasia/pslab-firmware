#include "cmp2.h"

void CMP2_Initialize(void) {
    // PSIDL disabled; 
    CMSTAT = 0x00;
    // CON enabled; CPOL Not Inverted; OPMODE Comparator; EVPOL Disabled; COE disabled; CCH C2IN1-; CREF C2IN1+; CEVT disabled; 
    CM2CON = 0x8000 & ~(0x8000); //disabling Comparator CON bit
    // SELSRCC PWM1L; SELSRCB PWM1L; SELSRCA PWM1L; 
    CM2MSKSRC = 0x00;
    // AANEN disabled; ABNEN disabled; ACNEN disabled; HLMS disabled; OANEN disabled; ABEN disabled; ACEN disabled; AAEN disabled; PAGS disabled; OBEN disabled; OCEN disabled; NAGS disabled; OCNEN disabled; OBNEN disabled; OAEN disabled; 
    CM2MSKCON = 0x00;
    // CFSEL FOSC/2; CFLTREN disabled; CFDIV 1:1; 
    CM2FLTR = 0x00;

    CM2CONbits.CON = 1; //enabling Comparator CON bit
}

bool CMP2_OutputStatusGet(void) {
    return (CM2CONbits.COUT);
}

bool CMP2_EventStatusGet(void) {
    return (CM2CONbits.CEVT);
}

void CMP2_EventStatusReset(void) {
    CM2CONbits.CEVT = 0;
}

bool CMP2_HLMSStatusGet(void) {
    return (CM2MSKCONbits.HLMS);
}

void CMP2_ComparatorDisable(void) {
    CM2CONbits.CON = 0;
}

void CMP2_ComparatorEnable(void) {
    CM2CONbits.CON = 1;
}

void __attribute__((weak)) CMP2_CallBack(void) {
    // Add your custom callback code here
}

void CMP2_Tasks(void) {
    if (IFS1bits.CMIF) {
        // CMP2 callback function 
        CMP2_CallBack();

        // Clear the CEVT bit to enable further interrupts
        CMP2_EventStatusReset();

        // clear the CMP2 interrupt flag
        IFS1bits.CMIF = 0;
    }
}
