#include <xc.h>
#include <stdio.h>
#include "pin_manager.h"

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

