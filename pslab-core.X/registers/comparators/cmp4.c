#include "cmp4.h"
#include "cmp_params.h"

void CMP4_Initialize(void) {

    CMP4_InitializeStatus();
    CMP4_InitializeControl();
    CMP4_InitializeMaskSource();
    CMP4_InitializeMaskGatingControl();
    CMP4_InitializeFilterControl();

    CMP4_ComparatorEnable();
}

void CMP4_InitializeStatus(void) {
    // Continue operation in idle mode
    CMSTATbits.PSIDL = 0;
    // Comparator event flags did not happen
    CMSTATbits.C1EVT = 0;
    CMSTATbits.C2EVT = 0;
    CMSTATbits.C3EVT = 0;
    CMSTATbits.C4EVT = 0;
    // Comparator output status bit is set to VIN+ < VIN-
    CMSTATbits.C1OUT = 0;
    CMSTATbits.C2OUT = 0;
    CMSTATbits.C3OUT = 0;
    CMSTATbits.C4OUT = 0;
}

void CMP4_InitializeControl(void) {
    // Comparator is disabled
    CM4CONbits.CON = 0;
    // Comparator output is internal
    CM4CONbits.COE = 0;
    // Comparator output is not inverted
    CM4CONbits.CPOL = 0;
    // Comparator event did not occur
    CM4CONbits.CEVT = 0;
    // Comparator output is VIN+ < VIN-
    CM4CONbits.COUT = 0;
    // No trigger or interrupts
    CM4CONbits.EVPOL = CMP_TRIGGER_DISABLED;
    // VIN+ connects to C4IN1+ pin (AN6 -- CAP Internal Pin)
    CM4CONbits.CREF = 0;
    // VIN- connects to C4IN- pin (AN7 -- RES Pin)
    CM4CONbits.CCH = 0b00;
}

void CMP4_InitializeMaskSource(void) {
    // Mask A
    CM4MSKSRCbits.SELSRCA = CMP_MASK_PWM1L;
    // Mask B
    CM4MSKSRCbits.SELSRCB = CMP_MASK_PWM1L;
    // Mask C
    CM4MSKSRCbits.SELSRCC = CMP_MASK_PWM1L;
}

void CMP4_InitializeMaskGatingControl(void) {
    // Prevent asserted logic high signals from propagation
    CM4MSKCONbits.HLMS = 0;
    // OR gate A is not connected
    CM4MSKCONbits.OAEN = 0;
    CM4MSKCONbits.OANEN = 0;
    // OR gate B is not connected
    CM4MSKCONbits.OBEN = 0;
    CM4MSKCONbits.OBNEN = 0;
    // OR gate C is not connected
    CM4MSKCONbits.OCEN = 0;
    CM4MSKCONbits.OCNEN = 0;
    // Inverted AND gate is not connected
    CM4MSKCONbits.NAGS = 0;
    CM4MSKCONbits.PAGS = 0;
    // AND gate A is not connected
    CM4MSKCONbits.AAEN = 0;
    CM4MSKCONbits.AANEN = 0;
    // AND gate B is not connected
    CM4MSKCONbits.ABEN = 0;
    CM4MSKCONbits.ABNEN = 0;
    // AND gate C is not connected
    CM4MSKCONbits.ACEN = 0;
    CM4MSKCONbits.ACNEN = 0;
}

void CMP4_InitializeFilterControl(void) {
    // Set external clock as filter input clock
    CM4FLTRbits.CFSEL = CMP_FILTER_CLOCK_FP;
    // Set filter clock divider
    CM4FLTRbits.CFDIV = CMP_CLOCK_PRESCALER_1;
    // Filter is disabled
    CM4FLTRbits.CFLTREN = 0;
}
 
void CMP4_SetupComparator(void) {
    
    CMP4_Initialize();
    // Disable the module until configured
    CMP4_ComparatorDisable();
    // VIN- connects to C4IN1- (RES Pin) and VIN+ connects to internal CVREFIN
    CM4CONbits.CREF = 1;
    // Trigger on High to Low transitions
    CM4CONbits.EVPOL = CMP_TRIGGER_HIGH_TO_LOW;
    // Comparator output is at C4OUT pin
    CM4CONbits.COE = 1;
    
    // Filter clock pre-scaler
    CM4FLTRbits.CFDIV = CMP_CLOCK_PRESCALER_16;
    // Enable filter
    CM4FLTRbits.CFLTREN = 1;
    
    // Mask A input
    CM4MSKSRCbits.SELSRCA = CMP_MASK_PWM1H;
    
    // Asserted logic low signals will not propagate
    CM4MSKCONbits.HLMS = 1;
    // OR gate A is enabled
    CM4MSKCONbits.OAEN = 1;
    
    // Enable comparator module
    CMP4_ComparatorEnable();
    PMD3bits.CMPMD = 0;
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
