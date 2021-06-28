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
}
