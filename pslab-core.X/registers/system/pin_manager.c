#include <xc.h>
#include <stdio.h>
#include "pin_manager.h"
#include "../../bus/uart1.h"

void PIN_MANAGER_Initialize(void) {

    // PTG WatchDogTimer is disabled
    PTGCONbits.PTGWDT = 0;
    // Enable WatchDogTimer
    RCONbits.SWDTEN = 1;

    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000;
    LATB = 0x0080;
    LATC = 0x0000;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x0797;
    TRISB = 0xFD7F;
    TRISC = 0x03F7;

    TRISBbits.TRISB2 = OUTPUT_PIN; // RGB_LED

    TRISBbits.TRISB10 = INPUT_PIN; // LA1_
    TRISBbits.TRISB11 = INPUT_PIN; // LA2_
    TRISBbits.TRISB12 = INPUT_PIN; // LA3_
    TRISBbits.TRISB13 = INPUT_PIN; // LA4_

    TRISCbits.TRISC6 = OUTPUT_PIN; // SQR1_
    TRISCbits.TRISC7 = OUTPUT_PIN; // SQR2_
    TRISCbits.TRISC8 = OUTPUT_PIN; // SQR3_
    TRISCbits.TRISC9 = OUTPUT_PIN; // SQR4_

    LED_SetDigitalOutput();

    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPDA = 0x0000;
    CNPDB = 0x0000;
    CNPDC = 0x0000;
    CNPUA = 0x0000;
    CNPUB = 0x0000;
    CNPUC = 0x0000;

    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x0000;
    ODCC = 0x0000;

    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;

    /****************************************************************************
     * Assign pin mappings
     ***************************************************************************/
    RPOR2bits.RP39R = RPN_U1TX_PORT; //RB7->UART1:U1TX
    RPINR18bits.U1RXR = RPI_RP40; //RB8->UART1:U1RX

    LED_SetHigh();
}

response_t PIN_MANAGER_SetWavePinState(void) {

    uint8_t pin_state = UART1_Read();

    if (pin_state & 0b00010000) {
        RPOR5bits.RP54R = RPN_DEFAULT_PORT; // SQ1: C6
    }
    if (pin_state & 0b00100000) {
        RPOR5bits.RP55R = RPN_DEFAULT_PORT; // SQ2: C7
    }
    if (pin_state & 0b01000000) {
        RPOR6bits.RP56R = RPN_DEFAULT_PORT; // SQ3: C8
    }
    if (pin_state & 0b10000000) {
        RPOR6bits.RP57R = RPN_DEFAULT_PORT; // SQ4: C9
    }

    // Clear C6-C9 bits using MSBs [XXXX_....]
    LATC &= ~((pin_state & 0x00F0) << 2);
    // Set C6-C9 bits using LSBs [...._XXXX]
    LATC |= ((pin_state & 0x000F) << 6);

    return SUCCESS;
}

response_t PIN_MANAGER_GetLAPinState(void) {

    uint16_t la_pin_state = (PORTB >> 10) & 0xF;

    UART1_WriteInt(la_pin_state);

    return SUCCESS;
}
