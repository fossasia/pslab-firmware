#include <xc.h>
#include <stdbool.h>

#include "../../instruments/logicanalyzer.h"
#include "../timers/tmr2.h"
#include "../../helpers/interval.h"
#include "../comparators/ic_params.h"
#include "interrupt_manager.h"

#define DISABLE     false
#define ENABLE      true

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void) {
    
    if ((((PORTB >> 10) & GetLA_TRIGGER_CHANNEL()) > 0) == GetLA_TRIGGER_STATE()) {
        INTERRUPT_LA1PinChange(DISABLE);
        INTERRUPT_LA2PinChange(DISABLE);
        INTERRUPT_LA3PinChange(DISABLE);
        INTERRUPT_LA4PinChange(DISABLE);
        INTERRUPT_DisablePinChangeInterrupts();
        
        TMR2_Start();
        SetDefaultDIGITAL_STATES();
        IC_PARAMS_ManualTriggerAll();
        SetDefaultDIGITAL_STATES_ERROR();
    }
    INTERRUPT_ClearPinChangeInterruptsFlag();
}

void __attribute__((__interrupt__, no_auto_psv)) _INT2Interrupt(void) {
    
    SetDefaultDIGITAL_STATES();
    IC_PARAMS_ManualTriggerAll();
    SetDefaultDIGITAL_STATES_ERROR();
    
    INTERRUPT_ClearExternalInterrupt2Flag();
    INTERRUPT_DisableExternalInterrupt2();
}

void INTERRUPT_Initialize(void) {
    //    ADI: ADC1 Convert Done
    //    Priority: 1
    IPC3bits.AD1IP = 1;
    //    MICI: I2C2 Master Events
    //    Priority: 1
    IPC12bits.MI2C2IP = 1;
    //    SICI: I2C2 Slave Events
    //    Priority: 1
    IPC12bits.SI2C2IP = 1;

    _INT1IF = 0;
    _INT1IE = 0;

    _U1RXIF = 0;
    _U1RXIE = 0;

    _U2RXIF = 0;
    _U2RXIE = 0;
}
