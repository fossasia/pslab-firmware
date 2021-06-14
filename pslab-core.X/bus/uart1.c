#include <xc.h>
#include "uart1.h"
#include "../registers/system/watchdog.h"

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void) {
    WATCHDOG_TimerClear();
    while (U2STAbits.UTXBF);
    U2TXREG = U1RXREG;
    _U1RXIF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _U2RXInterrupt(void) {
    WATCHDOG_TimerClear();
    while (U1STAbits.UTXBF);
    U1TXREG = U2RXREG;
    _U2RXIF = 0;
}

void UART1_Initialize(void) {
    /**    
     Make sure to set LAT bit corresponding to TxPin as high before UART Init
     */
    
    // UARTx is disabled; all UARTx pins are controlled by PORT latches; 
    // UARTx power consumption is minimal
    U1MODEbits.UARTEN = 0;
    // Continues module operation in Idle mode
    U1MODEbits.USIDL = 0;
    // IrDA encoder and decoder are disabled
    U1MODEbits.IREN = 0;
    // UxRTS pin is in Flow Control mode
    U1MODEbits.RTSMD = 0;
    // UxTX and UxRX pins are enabled and used; 
    // UxCTS and UxRTS/BCLKx pins are controlled by PORT latches
    U1MODEbits.UEN = 0b00;
    // No wake-up is enabled
    U1MODEbits.WAKE = 0;
    // Loop-back mode is disabled
    U1MODEbits.LPBACK = 0;
    // Baud rate measurement is disabled or completed
    U1MODEbits.ABAUD = 0;
    // UxRX Idle state is '1'
    U1MODEbits.URXINV = 0;
    // BRG generates 16 clocks per bit period (16x baud clock, Standard mode)
    U1MODEbits.BRGH = 1;
    // 8-bit data, no parity
    U1MODEbits.PDSEL = 0b00;
    // One Stop bit
    U1MODEbits.STSEL = 0;

    // Interrupt when a character is transferred to the Transmit Shift Register 
    // :this implies there is at least one character open in the transmit buffer
    U1STAbits.UTXISEL1 = 0;
    U1STAbits.UTXISEL0 = 0;
    // UxTX Idle state is '1'
    U1STAbits.UTXINV = 0;
    // Sync Break transmission is disabled or completed
    U1STAbits.UTXBRK = 0;
    // Transmit is disabled, any pending transmission is aborted and buffer is
    // reset; U1 TXpin is controlled by the PORT
    U1STAbits.UTXEN = 0;
    // Transmit buffer is not full, at least one more character can be written
    U1STAbits.UTXBF = 0;
    // Transmit Shift Register isn't empty, a transmission is in progress/queued
    U1STAbits.TRMT = 0;
    // Interrupt is set when any character is received and transferred from the 
    // UxRSR to the receive buffer; receive buffer has one or more characters
    U1STAbits.URXISEL = 0b00;
    // Address Detect mode is disabled
    U1STAbits.ADDEN = 0;
    // Receiver is active
    U1STAbits.RIDLE = 0;
    // Parity error has not been detected
    U1STAbits.PERR = 0;
    // Framing error has not been detected
    U1STAbits.FERR = 0;
    // Receive buffer has not overflowed; clearing a previously set OERRbit
    // (1 -> 0 transition) resets the RX buffer and U1RSR to the empty state
    U1STAbits.OERR = 0;
    // Receive buffer is empty
    U1STAbits.URXDA = 0;

    // UxBRG = [FCY / (16 * BAUD)] - 1
    // BaudRate = 1000000; Frequency = 64000000 Hz; BRG 15;
    U1BRG = 0x0F;

    UART1_Enable();
}

void UART1_Enable(void) {
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;
}

void UART1_Disable(void) {
    U1MODEbits.UARTEN = 0;
    U1STAbits.UTXEN = 0;
}

uint8_t UART1_Read(void) {
    while (!(U1STAbits.URXDA == 1));

    if ((U1STAbits.OERR == 1)) {
        U1STAbits.OERR = 0;
    }

    return U1RXREG;
}

uint16_t UART1_ReadInt(void) {
    uint8_t byte1, byte2;
    byte1 = UART1_Read() & 0xFF;
    byte2 = UART1_Read() & 0xFF;
    return (byte2 << 8) | byte1;
}

void UART1_Write(uint8_t txData) {
    while (U1STAbits.UTXBF == 1);
    U1TXREG = txData;
}

void UART1_WriteInt(uint16_t txData) {
    UART1_Write(txData & 0xFF);
    UART1_Write((txData >> 8) & 0xFF);
}

bool UART1_IsRxReady(void) {
    return U1STAbits.URXDA;
}

bool UART1_IsTxReady(void) {
    return ((!U1STAbits.UTXBF) && U1STAbits.UTXEN);
}

bool UART1_IsTxDone(void) {
    return U1STAbits.TRMT;
}

uint16_t UART1_StatusGet(void) {
    return U1STA;
}

void ClearBuffer(void) {
    while (UART1_IsRxReady()) UART1_Read();
}