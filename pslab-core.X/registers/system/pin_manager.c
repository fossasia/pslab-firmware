#include <xc.h>
#include <stdio.h>
#include "pin_manager.h"
#include "../../bus/uart/uart1.h"

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

    UART1_TX_SetHigh();
    UART2_TX_SetHigh();

    CS_CH1_SetHigh();
    CS_CH2_SetHigh();
    CS_SPI_SetHigh();
    SDCard_CS_SetHigh();

    BOOT_SetHigh();

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x0797;
    TRISB = 0xFD7F;
    TRISC = 0x03F7;

    I2C_SCL_SetDigitalInput();
    I2C_SDA_SetDigitalInput();

    UART1_TX_SetDigitalOutput();
    UART1_RX_SetDigitalInput();
    UART2_TX_SetDigitalOutput();
    UART2_RX_SetDigitalInput();

    BOOT_SetDigitalOutput(); // Bootloader enable pin

    CS_CH1_SetDigitalOutput(); // CH1 chip select
    CS_CH2_SetDigitalOutput(); // CH2 chip select
    CS_SPI_SetDigitalOutput(); // SPI chip select
    SDCard_CS_SetDigitalOutput(); // SD card chip select

    OSC_CH1_SetDigitalInput(); // CH1
    OSC_CH2_SetDigitalInput(); // CH2
    OSC_CH3_SetDigitalInput(); // CH3
    OSC_MIC_SetDigitalInput(); // MIC

    LA1_SetDigitalInput(); // LA1_
    LA2_SetDigitalInput(); // LA2_
    LA3_SetDigitalInput(); // LA3_
    LA4_SetDigitalInput(); // LA4_

    SQR1_SetDigitalOutput(); // SQR1_
    SQR2_SetDigitalOutput(); // SQR2_
    SQR3_SetDigitalOutput(); // SQR3_
    SQR4_SetDigitalOutput(); // SQR4_

    FQY_SetDigitalInput(); // Pulse counter
    VOL_SetDigitalInput(); // Voltage
    RES_SetDigitalInput(); // Resistance
    CAP_IN_SetDigitalInput(); // Capacitor read pin
    CAP_OUT_SetDigitalInput(); // Capacitor charge pin

    SDO1_SetDigitalOutput();
    SCK1_SetDigitalOutput();
    SDI1_SetDigitalInput();

    LED_SetDigitalOutput(); // Status LED
    RGB_LED_SetDigitalOutput(); // WS2812 RGB Led

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

    VOL_SetAnalog();
    RES_SetAnalog();
    CAP_IN_SetAnalog();

    OSC_CH1_SetAnalog();
    OSC_CH2_SetAnalog();
    OSC_CH3_SetAnalog();
    OSC_MIC_SetAnalog();

    /****************************************************************************
     * Assign pin mappings
     ***************************************************************************/
    RPOR2bits.RP39R = RPN_U1TX_PORT; //RB7->UART1:U1TX
    RPINR18bits.U1RXR = RPI_RP40; //RB8->UART1:U1RX
    RPOR2bits.RP38R = RPN_U2TX_PORT; // RB6->UART2:U2TX
    RPINR19bits.U2RXR = RPI_RP37; // RB5->UART2:U2RX

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
