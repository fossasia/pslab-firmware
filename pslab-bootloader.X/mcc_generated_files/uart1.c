/**
  UART1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    uart1.c

  @Summary
    This is the generated driver implementation file for the UART1 driver using Foundation Services Library

  @Description
    This header file provides implementations for driver APIs for UART1.
    Generation Information :
        Product Revision  :  Foundation Services Library - pic24-dspic-pic32mm : v1.26
        Device            :  PIC24EP256GP204
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.30
        MPLAB             :  MPLAB X 3.45
 */

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */

/**
  Section: Included Files
 */
#include <xc.h>
#include "uart1.h"

/**
  Section: UART1 APIs
 */

void UART1_Initialize(void) {
    // STSEL 1; IREN disabled; PDSEL 8N; UARTEN enabled; RTSMD disabled; USIDL disabled; WAKE disabled; ABAUD disabled; LPBACK disabled; BRGH enabled; URXINV disabled; UEN TX_RX; 
    U1MODE = (0x8008 & ~(1 << 15)); // disabling UARTEN bit   
    // UTXISEL0 TX_ONE_CHAR; UTXINV disabled; OERR NO_ERROR_cleared; URXISEL RX_ONE_CHAR; UTXBRK COMPLETED; UTXEN disabled; ADDEN disabled; 
    U1STA = 0x0000;
    // BaudRate = 460800; Frequency = 64000000 Hz; BRG 34; 
    U1BRG = 0x22;

    U1MODEbits.UARTEN = 1; // enabling UARTEN bit
    U1STAbits.UTXEN = 1;
}

uint8_t UART1_Read(void) {
    while (!(U1STAbits.URXDA == 1)) {
    }

    if ((U1STAbits.OERR == 1)) {
        U1STAbits.OERR = 0;
    }

    return U1RXREG;
}

void UART1_Write(uint8_t txData) {
    while (U1STAbits.UTXBF == 1) {
    }

    U1TXREG = txData; // Write the data byte to the USART.
}

bool UART1_IsRxReady(void) {
    return U1STAbits.URXDA;
}

bool UART1_IsTxReady(void) {
    return (U1STAbits.TRMT && U1STAbits.UTXEN);
}

bool UART1_IsTxDone(void) {
    return U1STAbits.TRMT;
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
UART1_STATUS __attribute__((deprecated)) UART1_StatusGet(void) {
    return U1STA;
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
bool __attribute__((deprecated)) UART1_DataReady(void) {
    return UART1_IsRxReady();
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
bool __attribute__((deprecated)) UART1_is_tx_ready(void) {
    return UART1_IsTxReady();
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
bool __attribute__((deprecated)) UART1_is_rx_ready(void) {
    return UART1_IsRxReady();
}

/* !!! Deprecated API - This function may not be supported in a future release !!! */
bool __attribute__((deprecated)) UART1_is_tx_done(void) {
    return UART1_IsTxDone();
}

