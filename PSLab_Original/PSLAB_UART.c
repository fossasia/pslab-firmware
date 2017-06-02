/******************************************************************************/
/******** This file contains UART control modules of function.c file **********/
/******************************************************************************/
#include <p24EP256GP204.h>
#include <libpic30.h>
#include "COMMANDS.h"
#include "PSLAB_UART.h"

void __attribute__((__interrupt__, no_auto_psv)) _U2RXInterrupt(void) {
    asm("CLRWDT");
    //while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = U2RXREG;
    _U2RXIF = 0;
}

void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void) {
    asm("CLRWDT");
    while (U2STAbits.UTXBF); //wait for transmit buffer empty
    U2TXREG = U1RXREG;
    _U1RXIF = 0;
}

void initUART(unsigned int BAUD) {
    /*---------UART------------*/
    TRISBbits.TRISB8 = 1; // B8 set as input(RX). connected to TX of MCP2200
    ANSELBbits.ANSB8 = 0; // set B8 as digital input.
    TRISBbits.TRISB7 = 0; // set as output. connected to RX of MCP2200

    RPOR2bits.RP39R = 0x01; //Map B7(RP39) to UART TX
    RPINR18bits.U1RXR = 0x28; //Map B8(RP40) to UART1 RX

    U1MODEbits.STSEL = 0; //1 stop bit
    U1MODEbits.PDSEL = 0; //no parity, 8-data bits
    U1MODEbits.ABAUD = 0; //disable auto-baud
    U1MODEbits.BRGH = 1; //high speed mode
    U1BRG = BAUD;
    U1MODEbits.UEN = 0;
    U1MODEbits.RTSMD = 1;

    U1STAbits.URXISEL = 0; //interrupt on 1 char recv

    //IEC0bits.U1TXIE = 1; //enable TX interrupt

    U1MODEbits.UARTEN = 1; //enable UART
    U1STAbits.UTXEN = 1; //enable UART TX

    U1MODEbits.URXINV = 0;

    DELAY_105uS
    while (hasChar())getChar(); //clear buffer

}

bool hasChar() {
    return U1STAbits.URXDA;
}

void sendChar(BYTE val) {
    while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = val;
}

void sendInt(unsigned int val) {
    while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = val & 0xff;
    while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = (val >> 8)&0xff;
}

void sendLong(unsigned int lsb, unsigned int msb) {
    while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = lsb & 0xff;
    while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = (lsb >> 8)&0xff;
    while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = msb & 0xff;
    while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = (msb >> 8)&0xff;
}

void ack(BYTE response) {
    while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = response;
}

char getChar() {
    while (!hasChar());
    if (U1STAbits.FERR == 1) {
        U1RXREG;
        U1STAbits.OERR = 0;
        return 0;
    }
    return U1RXREG;
}

unsigned int getInt() {
    return ((getChar()&0xFF << 8) | (getChar()&0xFF));
}

void configUART2(unsigned int BAUD) {
    _TRISB5 = 0;
    _TRISB6 = 1;
    RPOR1bits.RP37R = 0x03;
    RPINR19bits.U2RXR = 38;

    U2MODEbits.STSEL = 0; //1 stop bit
    U2MODEbits.PDSEL = 0; //no parity, 8-data bits
    U2MODEbits.ABAUD = 0; //disable auto-baud
    U2MODEbits.BRGH = 1; //high speed mode

    U2BRG = BAUD;

    U2MODEbits.UEN = 0;
    U2MODEbits.RTSMD = 1;

    U2STAbits.URXISEL = 0; //interrupt on 1 char recv

    U2MODEbits.UARTEN = 1; //enable UART
    U2STAbits.UTXEN = 1; //enable UART TX

    U2MODEbits.URXINV = 0;
    while (U2STAbits.URXDA) U2RXREG;
}

bool hasChar2(void) {
    return U2STAbits.URXDA;
}

char getChar2(void) {
    if (!hasChar2())return 0;
    if (U2STAbits.FERR == 1) {
        U2RXREG;
        U2STAbits.OERR = 0;
        U2STAbits.FERR = 0;
        return 0;
    }
    return U2RXREG;
}

unsigned int getInt2(void) {
    return ((getChar2()&0xFF << 8) | (getChar2()&0xFF));
}

void sendChar2(char val) {
    while (U2STAbits.UTXBF); //wait for transmit buffer empty
    U2TXREG = val;
}

void sendInt2(unsigned int val) {
    while (U2STAbits.UTXBF); //wait for transmit buffer empty
    U2TXREG = val & 0xff;
    while (U2STAbits.UTXBF); //wait for transmit buffer empty
    U2TXREG = (val >> 8)&0xff;
}

void sendAddress2(char address) { //9-bit mode only
    while (U2STAbits.UTXBF); //wait for transmit buffer empty
    U2TXREG = 0x100 + address; // send the address with the 9th bit set
}

/*----UART 2 on SCL, SDA----------------*/
void initUART2_passthrough(unsigned int BAUD) {
    /*---------UART2 pass through------------*/
    configUART2(BAUD);
    _U1RXIE = 1; //enable receive interrupt for UART1
    _U2RXIE = 1; //enable receive interrupt for UART2

    DELAY_105uS
}
