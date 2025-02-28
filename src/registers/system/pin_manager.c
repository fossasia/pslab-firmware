#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <xc.h>

#include "pin_manager.h"

uint8_t PIN_MANAGER_DIGITAL_PINS[] = {
    PIN_MANAGER_DIGITAL_PINS_LA1,
    PIN_MANAGER_DIGITAL_PINS_LA2,
    PIN_MANAGER_DIGITAL_PINS_LA3,
    PIN_MANAGER_DIGITAL_PINS_LA4,
    PIN_MANAGER_DIGITAL_PINS_COMP4,
    PIN_MANAGER_DIGITAL_PINS_SPI_CS,
    PIN_MANAGER_DIGITAL_PINS_FREQ
};

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

    CS_MUX1_SetHigh();
    CS_MUX2_SetHigh();
    CS_MUX3_SetHigh();
    CS_V5_SetHigh();

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

    CS_MUX1_SetDigitalOutput();
    CS_MUX2_SetDigitalOutput();
    CS_MUX3_SetDigitalOutput();
    CS_V5_SetDigitalOutput();

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
    RPOR1bits.RP37R = RPN_U2TX_PORT; // RB5->UART2:U2TX
    RPINR19bits.U2RXR = RPI_RP38; // RB6->UART2:U2RX

    LED_SetHigh();
}

enum Status PIN_MANAGER_set_sq_pin_state(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {
    union Input {
        struct {
            uint8_t sq_pin_state;
        };
        uint8_t const *buffer;
    } input = {{0}};

    if (args_size != sizeof(input)) {return E_BAD_ARGSIZE;}
    input.buffer = args;

    if (input.sq_pin_state & 0b00010000) {
        RPOR5bits.RP54R = RPN_DEFAULT_PORT; // SQ1: C6
    }
    if (input.sq_pin_state & 0b00100000) {
        RPOR5bits.RP55R = RPN_DEFAULT_PORT; // SQ2: C7
    }
    if (input.sq_pin_state & 0b01000000) {
        RPOR6bits.RP56R = RPN_DEFAULT_PORT; // SQ3: C8
    }
    if (input.sq_pin_state & 0b10000000) {
        RPOR6bits.RP57R = RPN_DEFAULT_PORT; // SQ4: C9
    }

    // Clear C6-C9 bits using MSBs [XXXX_....]
    LATC &= ~((input.sq_pin_state & 0x00F0) << 2);
    // Set C6-C9 bits using LSBs [...._XXXX]
    LATC |= ((input.sq_pin_state & 0x000F) << 6);

    return E_OK;
}

enum Status PIN_MANAGER_get_la_pin_state(
    uint8_t *const args,
    __attribute__ ((unused)) uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *rets_size
) {
    uint8_t const la_pin_state = (PORTB >> 10) & 0xF;
    *rets = args;
    *rets_size = sizeof(la_pin_state);
    memcpy(*rets, &la_pin_state, *rets_size);
    return E_OK;
}
