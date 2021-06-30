#include <xc.h>

void INTERRUPT_Initialize(void) {
    //    ADI: ADC1 Convert Done
    //    Priority: 1
    IPC3bits.AD1IP = 1;
    //    MICI: I2C1 Master Events
    //    Priority: 1
    IPC4bits.MI2C1IP = 1;
    //    SICI: I2C1 Slave Events
    //    Priority: 1
    IPC4bits.SI2C1IP = 1;

    _INT1IF = 0;
    _INT1IE = 0;

    _U1RXIF = 0;
    _U1RXIE = 0;

    _U2RXIF = 0;
    _U2RXIE = 0;
}
