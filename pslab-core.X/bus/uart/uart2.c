#include <xc.h>
#include "uart2.h"
#include "uart1.h"
#include "../../registers/system/pin_manager.h"
#include "../../registers/system/watchdog.h"
#include "../../helpers/delay.h"
#include "../../commands.h"


static uint16_t UART2_BAUD_RATE = 0x0F;
void SetUART2_BAUD_RATE(uint16_t V) { UART2_BAUD_RATE = V; }
uint16_t GetUART2_BAUD_RATE(void) { return UART2_BAUD_RATE; }

void __attribute__((__interrupt__, no_auto_psv)) _U2RXInterrupt(void) {
    WATCHDOG_TimerClear();
    // Wait until UART1 finish sending out data
    while (U1STAbits.UTXBF);
    // Load what's received from UART2 interface to UART1 transmit buffer
    U1TXREG = U2RXREG;
    // Clear the interrupt flag and await more data from UART2 interface
    UART2_InterruptFlagClear();
}

void UART2_Initialize() {
    // UART2 is disabled; all UART2 pins are controlled by PORT latches; 
    // UART2 power consumption is minimal
    U2MODEbits.UARTEN = 0;
    // Continues module operation in Idle mode
    U2MODEbits.USIDL = 0;
    // IrDA encoder and decoder are disabled
    U2MODEbits.IREN = 0;
    // U2RTS pin is in Simplex mode
    U2MODEbits.RTSMD = 1;
    // U2TX and U2RX pins are enabled and used; 
    // U2CTS and U2RTS/BCLKx pins are controlled by PORT latches
    U2MODEbits.UEN = 0b00;
    // No wake-up is enabled
    U2MODEbits.WAKE = 0;
    // Loop-back mode is disabled
    U2MODEbits.LPBACK = 0;
    // Baud rate measurement is disabled or completed
    U2MODEbits.ABAUD = 0;
    // U2RX Idle state is '1'
    U2MODEbits.URXINV = 0;
    // BRG generates 16 clocks per bit period (16x baud clock, Standard mode)
    U2MODEbits.BRGH = 1;
    // 8-bit data, no parity
    U2MODEbits.PDSEL = 0b00;
    // One Stop bit
    U2MODEbits.STSEL = 0;

    // Interrupt when a character is transferred to the Transmit Shift Register 
    // :this implies there is at least one character open in the transmit buffer
    U2STAbits.UTXISEL1 = 0;
    U2STAbits.UTXISEL0 = 0;
    // U2TX Idle state is '1'
    U2STAbits.UTXINV = 0;
    // Sync Break transmission is disabled or completed
    U2STAbits.UTXBRK = 0;
    // Transmit is disabled, any pending transmission is aborted and buffer is
    // reset; U2 TXpin is controlled by the PORT
    U2STAbits.UTXEN = 0;
    // Transmit buffer is not full, at least one more character can be written
    U2STAbits.UTXBF = 0;
    // Transmit Shift Register isn't empty, a transmission is in progress/queued
    U2STAbits.TRMT = 0;
    // Interrupt is set when any character is received and transferred from the 
    // U2RSR to the receive buffer; receive buffer has one or more characters
    U2STAbits.URXISEL = 0b00;
    // Address Detect mode is disabled
    U2STAbits.ADDEN = 0;
    // Receiver is active
    U2STAbits.RIDLE = 0;
    // Parity error has not been detected
    U2STAbits.PERR = 0;
    // Framing error has not been detected
    U2STAbits.FERR = 0;
    // Receive buffer has not overflowed; clearing a previously set OERRbit
    // (1 -> 0 transition) resets the RX buffer and U1RSR to the empty state
    U2STAbits.OERR = 0;
    // Receive buffer is empty
    U2STAbits.URXDA = 0;

    // U2BRG = [FCY / (16 * BAUD)] - 1
    // BaudRate = 1000000; Frequency = 64000000 Hz; BRG 15;
    U2BRG = GetUART2_BAUD_RATE();

    UART2_Enable();
}

uint8_t UART2_Read(void) {
    while (!(U2STAbits.URXDA == 1));

    if ((U2STAbits.OERR == 1)) {
        U2STAbits.OERR = 0;
    }

    return U2RXREG;
}

void UART2_Write(uint8_t txData) {
    while (U2STAbits.UTXBF == 1);
    U2TXREG = txData;
}

bool UART2_IsRxReady(void) {
    return U2STAbits.URXDA;
}

bool UART2_IsTxReady(void) {
    return ((!U2STAbits.UTXBF) && U2STAbits.UTXEN);
}

bool UART2_IsTxDone(void) {
    return U2STAbits.TRMT;
}

void UART2_Enable(void) {
    U2MODEbits.UARTEN = 1;
    U2STAbits.UTXEN = 1;
}

void UART2_Disable(void) {
    U2MODEbits.UARTEN = 0;
    U2STAbits.UTXEN = 0;
}

void UART2_ClearBuffer(void) {
    while (UART2_IsRxReady()) UART2_Read();
}
