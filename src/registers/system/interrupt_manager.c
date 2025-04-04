#include <xc.h>
#include <stdbool.h>

#include "interrupt_manager.h"

#define DISABLE     false
#define ENABLE      true

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
