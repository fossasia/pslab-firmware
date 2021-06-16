#ifndef _PIN_MANAGER_H
#define _PIN_MANAGER_H
/**
    Section: Includes
 */
#include <xc.h>

#define OUTPUT_PIN                     0
#define INPUT_PIN                      1

#define channel_AN0_SetHigh()          (_LATA0 = 1)
#define channel_AN0_SetLow()           (_LATA0 = 0)
#define channel_AN0_Toggle()           (_LATA0 ^= 1)
#define channel_AN0_GetValue()         _RA0
#define channel_AN0_SetDigitalInput()  (_TRISA0 = 1)
#define channel_AN0_SetDigitalOutput() (_TRISA0 = 0)

#define channel_AN1_SetHigh()          (_LATA1 = 1)
#define channel_AN1_SetLow()           (_LATA1 = 0)
#define channel_AN1_Toggle()           (_LATA1 ^= 1)
#define channel_AN1_GetValue()         _RA1
#define channel_AN1_SetDigitalInput()  (_TRISA1 = 1)
#define channel_AN1_SetDigitalOutput() (_TRISA1 = 0)

#define SDO1_SetHigh()                 (_LATA4 = 1)
#define SDO1_SetLow()                  (_LATA4 = 0)
#define SDO1_Toggle()                  (_LATA4 ^= 1)
#define SDO1_GetValue()                _RA4
#define SDO1_SetDigitalInput()         (_TRISA4 = 1)
#define SDO1_SetDigitalOutput()        (_TRISA4 = 0)

#define SDI1_SetHigh()                 (_LATA9 = 1)
#define SDI1_SetLow()                  (_LATA9 = 0)
#define SDI1_Toggle()                  (_LATA9 ^= 1)
#define SDI1_GetValue()                _RA9
#define SDI1_SetDigitalInput()         (_TRISA9 = 1)
#define SDI1_SetDigitalOutput()        (_TRISA9 = 0)

#define channel_AN2_SetHigh()          (_LATB0 = 1)
#define channel_AN2_SetLow()           (_LATB0 = 0)
#define channel_AN2_Toggle()           (_LATB0 ^= 1)
#define channel_AN2_GetValue()         _RB0
#define channel_AN2_SetDigitalInput()  (_TRISB0 = 1)
#define channel_AN2_SetDigitalOutput() (_TRISB0 = 0)

#define SDCard_CS_SetHigh()            (_LATB9 = 1)
#define SDCard_CS_SetLow()             (_LATB9 = 0)
#define SDCard_CS_Toggle()             (_LATB9 ^= 1)
#define SDCard_CS_GetValue()           _RB9
#define SDCard_CS_SetDigitalInput()    (_TRISB9 = 1)
#define SDCard_CS_SetDigitalOutput()   (_TRISB9 = 0)

#define SCK1_SetHigh()                 (_LATC3 = 1)
#define SCK1_SetLow()                  (_LATC3 = 0)
#define SCK1_Toggle()                  (_LATC3 ^= 1)
#define SCK1_GetValue()                _RC3
#define SCK1_SetDigitalInput()         (_TRISC3 = 1)
#define SCK1_SetDigitalOutput()        (_TRISC3 = 0)

#define LED_SetHigh()                  (_LATB15 = 1)
#define LED_SetLow()                   (_LATB15 = 0)
#define LED_Toggle()                   (_LATB15 ^= 1)
#define LED_GetValue()                 _RB15
#define LED_SetDigitalInput()          (_TRISB15 = 1)
#define LED_SetDigitalOutput()         (_TRISB15 = 0)

#define RGB_LED_SetHigh()              (_LATB2 = 1)
#define RGB_LED_SetLow()               (_LATB2 = 0)
#define RGB_LED_Toggle()               (_LATB2 ^= 1)
#define RGB_LED_GetValue()             _RB2
#define RGB_LED_SetDigitalInput()      (_TRISB2 = 1)
#define RGB_LED_SetDigitalOutput()     (_TRISB2 = 0)

/**
    Section: Function Prototypes
 */
/**
  @Summary
    Configures the pin settings of the PIC24EP256GP204
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description
    This is the generated manager file for the PIC24 / dsPIC33 / PIC32MM MCUs device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    void SYSTEM_Initialize(void)
    {
        // Other initializers are called from this function
        PIN_MANAGER_Initialize();
    }
    </code>

 */
void PIN_MANAGER_Initialize(void);

/**
  @Summary
    Output selection for re-mappable pins (RPn). Refer to Table 11-3
 */
typedef enum {
    RPN_DEFAULT_PORT = 0b000000, // Default PORT
    RPN_U1TX_PORT = 0b000001, // UART1 Transmit
    RPN_U2TX_PORT = 0b000011, // UART2 Transmit
    RPN_SDO2_PORT = 0b001000, // SPI2 Data Output
    RPN_SCK2_PORT = 0b001001, // SPI2 Clock Output
    RPN_SS2_PORT = 0b001010, // SPI2 Slave Select
    RPN_C1TX_PORT = 0b001110, // CAN1 Transmit
    RPN_OC1_PORT = 0b010000, // Output Compare 1 Output
    RPN_OC2_PORT = 0b010001, // Output Compare 2 Output
    RPN_OC3_PORT = 0b010010, // Output Compare 3 Output
    RPN_OC4_PORT = 0b010011, // Output Compare 4 Output
    RPN_C1OUT_PORT = 0b011000, // Comparator Output 1
    RPN_C2OUT_PORT = 0b011001, // Comparator Output 2
    RPN_C3OUT_PORT = 0b011010, // Comparator Output 3
    RPN_SYNCO1_PORT = 0b101101, // PWM Primary Time Base Sync Output
    RPN_QEI1CCMP_PORT = 0b101111, // QEI 1 Counter Comparator Output     
    RPN_REFCLKO_PORT = 0b110001, // Reference Clock Output
    RPN_C4OUT_PORT = 0b110010 // Comparator Output 4
} RPN_TIE;

/**
  @Summary
    Input selection for re-mappable pins (RPIn). Refer to Table 11-2
 */
typedef enum {
    RPI_VSS = 0b0000000,
    RPI_C1OUT = 0b0000001,
    RPI_C2OUT = 0b0000010,
    RPI_C3OUT = 0b0000011,
    RPI_C4OUT = 0b0000100,
    RPI_PTGO30 = 0b0000110,
    RPI_PTGO31 = 0b0000111,
    RPI_FINDX1 = 0b0001000,
    RPI_FHOME1 = 0b0001001,
    RPI_RP20 = 0b0010100,
    RPI_RPI24 = 0b0011000,
    RPI_RPI25 = 0b0011001,
    RPI_RPI27 = 0b0011011,
    RPI_RPI28 = 0b0011100,
    RPI_RPI32 = 0b0100000,
    RPI_RPI33 = 0b0100001,
    RPI_RPI34 = 0b0100010,
    RPI_RP35 = 0b0100011,
    RPI_RP36 = 0b0100100,
    RPI_RP37 = 0b0100101,
    RPI_RP38 = 0b0100110,
    RPI_RP39 = 0b0100111,
    RPI_RPI45 = 0b0101101,
    RPI_RPI46 = 0b0101110,
    RPI_RPI47 = 0b0101111,
    RPI_RPI51 = 0b0110011,
    RPI_RPI52 = 0b0110100,
    RPI_RPI53 = 0b0110101,
    RPI_RP54 = 0b0110110,
    RPI_RP55 = 0b0110111,
    RPI_RP56 = 0b0111000,
    RPI_RP57 = 0b0111001,
    RPI_RPI58 = 0b0111010,
    RPI_RP40 = 0b0101000,
    RPI_RP41 = 0b0101001,
    RPI_RP42 = 0b0101010,
    RPI_RP43 = 0b0101011,
    RPI_RPI44 = 0b0101100,
    RPI_RPI94 = 0b1011110,
    RPI_RPI95 = 0b1011111,
    RPI_RPI96 = 0b1100000,
    RPI_RP97 = 0b1100001,
    RPI_RP118 = 0b1110110,
    RPI_RPI119 = 0b1110111,
    RPI_RP120 = 0b1111000,
    RPI_RPI121 = 0b1111001
} RPI_TIE;

#endif
