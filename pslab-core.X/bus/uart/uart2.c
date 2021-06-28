#include <xc.h>
#include "uart2.h"

void UART2_Initialize(void) {
    /**    
         Set the UART2 module to the options selected in the user interface.
         Make sure to set LAT bit corresponding to TxPin as high before UART initialization
     */
    // STSEL 1; IREN disabled; PDSEL 8N; UARTEN enabled; RTSMD disabled; USIDL disabled; WAKE disabled; ABAUD disabled; LPBACK disabled; BRGH enabled; URXINV disabled; UEN TX_RX; 
    // Data Bits = 8; Parity = None; Stop Bits = 1;
    U2MODE = (0x8008 & ~(1 << 15)); // disabling UARTEN bit
    // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; 
    U2STA = 0x00;
    // BaudRate = 460800; Frequency = 64000000 Hz; BRG 34; 
    U2BRG = 0x22;

    U2MODEbits.UARTEN = 1; // enabling UART ON bit
    U2STAbits.UTXEN = 1;
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

    U2TXREG = txData; // Write the data byte to the USART.
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
