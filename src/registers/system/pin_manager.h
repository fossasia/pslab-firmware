#ifndef _PIN_MANAGER_H
#define _PIN_MANAGER_H

#include <xc.h>
#include "../../states.h"
#include "../../commands.h"

#define OUTPUT_PIN                     0
#define INPUT_PIN                      1

#define LED_SetHigh()                  (_LATB15 = 1)
#define LED_SetLow()                   (_LATB15 = 0)
#define LED_Toggle()                   (_LATB15 ^= 1)
#define LED_GetValue()                 _RB15
#define LED_SetDigitalInput()          (_TRISB15 = 1)
#define LED_SetDigitalOutput()         (_TRISB15 = 0)

#define RGB_LED_Setter                 _LATB2
#define RGB_LED_SetHigh()              (_LATB2 = 1)
#define RGB_LED_SetLow()               (_LATB2 = 0)
#define RGB_LED_Toggle()               (_LATB2 ^= 1)
#define RGB_LED_GetValue()             _RB2
#define RGB_LED_SetDigitalInput()      (_TRISB2 = 1)
#define RGB_LED_SetDigitalOutput()     (_TRISB2 = 0)

#define BOOT_SetHigh()                 (_LATC5 = 1)
#define BOOT_SetLow()                  (_LATC5 = 0)
#define BOOT_Toggle()                  (_LATC5 ^= 1)
#define BOOT_GetValue()                _RC5
#define BOOT_SetDigitalInput()         (_TRISC5 = 1)
#define BOOT_SetDigitalOutput()        (_TRISC5 = 0)

/*******************************************************************************
 * Serial Peripheral Interface (SPI)
 ******************************************************************************/
#define SDO1_SetHigh()                 (_LATA4 = 1)
#define SDO1_SetLow()                  (_LATA4 = 0)
#define SDO1_Toggle()                  (_LATA4 ^= 1)
#define SDO1_GetValue()                _RA4
#define SDO1_SetDigitalInput()         (_TRISA4 = 1)
#define SDO1_SetDigitalOutput()        (_TRISA4 = 0)
#define SDO1_SetDigital()              (_ANSA4 = 0)
#define SDO1_SetAnalog()               (_ANSA4 = 1)

#define SDI1_SetHigh()                 (_LATA9 = 1)
#define SDI1_SetLow()                  (_LATA9 = 0)
#define SDI1_Toggle()                  (_LATA9 ^= 1)
#define SDI1_GetValue()                _RA9
#define SDI1_SetDigitalInput()         (_TRISA9 = 1)
#define SDI1_SetDigitalOutput()        (_TRISA9 = 0)

#define SCK1_SetHigh()                 (_LATC3 = 1)
#define SCK1_SetLow()                  (_LATC3 = 0)
#define SCK1_Toggle()                  (_LATC3 ^= 1)
#define SCK1_GetValue()                _RC3
#define SCK1_SetDigitalInput()         (_TRISC3 = 1)
#define SCK1_SetDigitalOutput()        (_TRISC3 = 0)

#define CS_MUX1_SetHigh()               (_LATA10 = 1)
#define CS_MUX1_SetDigitalOutput()      (_TRISA10 = 0)

#define CS_MUX2_SetHigh()               (_LATA7 = 1)
#define CS_MUX2_SetDigitalOutput()      (_TRISA7 = 0)

#define CS_MUX3_SetHigh()               (_LATB9 = 1)
#define CS_MUX3_SetDigitalOutput()      (_TRISB9 = 0)

// This pin is NC in V6.
#define CS_V5_SetHigh()               (_LATC4 = 1)
#define CS_V5_SetDigitalOutput()      (_TRISC4 = 0)

/*******************************************************************************
 * I2C
 ******************************************************************************/
#define I2C_SCL_SetHigh()              (_LATB4 = 1)
#define I2C_SCL_SetLow()               (_LATB4 = 0)
#define I2C_SCL_Toggle()               (_LATB4 ^= 1)
#define I2C_SCL_GetValue()             _RB4
#define I2C_SCL_SetDigitalInput()      (_TRISB4 = 1)
#define I2C_SCL_SetDigitalOutput()     (_TRISB4 = 0)
#define I2C_SCL_SetAsOpenDrain()       (_ODCB4 = 1)
#define I2C_SCL_PullUp()               (_CNPUB4 = 1)

#define I2C_SDA_SetHigh()              (_LATA8 = 1)
#define I2C_SDA_SetLow()               (_LATA8 = 0)
#define I2C_SDA_Toggle()               (_LATA8 ^= 1)
#define I2C_SDA_GetValue()             _RA8
#define I2C_SDA_SetDigitalInput()      (_TRISA8 = 1)
#define I2C_SDA_SetDigitalOutput()     (_TRISA8 = 0)
#define I2C_SDA_SetAsOpenDrain()       (_ODCA8 = 1)
#define I2C_SDA_PullUp()               (_CNPUA8 = 1)

/*******************************************************************************
 * UART
 ******************************************************************************/
#define UART1_RX_SetHigh()             (_LATB8 = 1)
#define UART1_RX_SetLow()              (_LATB8 = 0)
#define UART1_RX_Toggle()              (_LATB8 ^= 1)
#define UART1_RX_GetValue()            _RB8
#define UART1_RX_SetDigitalInput()     (_TRISB8 = 1)
#define UART1_RX_SetDigitalOutput()    (_TRISB8 = 0)

#define UART1_TX_SetHigh()             (_LATB7 = 1)
#define UART1_TX_SetLow()              (_LATB7 = 0)
#define UART1_TX_Toggle()              (_LATB7 ^= 1)
#define UART1_TX_GetValue()            _RB7
#define UART1_TX_SetDigitalInput()     (_TRISB7 = 1)
#define UART1_TX_SetDigitalOutput()    (_TRISB7 = 0)

#define UART2_RX_SetHigh()             (_LATB5 = 1)
#define UART2_RX_SetLow()              (_LATB5 = 0)
#define UART2_RX_Toggle()              (_LATB5 ^= 1)
#define UART2_RX_GetValue()            _RB5
#define UART2_RX_SetDigitalInput()     (_TRISB5 = 1)
#define UART2_RX_SetDigitalOutput()    (_TRISB5 = 0)

#define UART2_TX_SetHigh()             (_LATB6 = 1)
#define UART2_TX_SetLow()              (_LATB6 = 0)
#define UART2_TX_Toggle()              (_LATB6 ^= 1)
#define UART2_TX_GetValue()            _RB6
#define UART2_TX_SetDigitalInput()     (_TRISB6 = 1)
#define UART2_TX_SetDigitalOutput()    (_TRISB6 = 0)

/*******************************************************************************
 * Oscilloscope
 ******************************************************************************/
#define OSC_CH1_SetHigh()              (_LATB1 = 1)
#define OSC_CH1_SetLow()               (_LATB1 = 0)
#define OSC_CH1_Toggle()               (_LATB1 ^= 1)
#define OSC_CH1_GetValue()             _RB1
#define OSC_CH1_SetDigitalInput()      (_TRISB1 = 1)
#define OSC_CH1_SetDigitalOutput()     (_TRISB1 = 0)
#define OSC_CH1_SetDigital()           (_ANSB1 = 0)
#define OSC_CH1_SetAnalog()            (_ANSB1 = 1)

#define OSC_CH2_SetHigh()              (_LATA0 = 1)
#define OSC_CH2_SetLow()               (_LATA0 = 0)
#define OSC_CH2_Toggle()               (_LATA0 ^= 1)
#define OSC_CH2_GetValue()             _RA0
#define OSC_CH2_SetDigitalInput()      (_TRISA0 = 1)
#define OSC_CH2_SetDigitalOutput()     (_TRISA0 = 0)
#define OSC_CH2_SetDigital()           (_ANSA0 = 0)
#define OSC_CH2_SetAnalog()            (_ANSA0 = 1)

#define OSC_CH3_SetHigh()              (_LATA1 = 1)
#define OSC_CH3_SetLow()               (_LATA1 = 0)
#define OSC_CH3_Toggle()               (_LATA1 ^= 1)
#define OSC_CH3_GetValue()             _RA1
#define OSC_CH3_SetDigitalInput()      (_TRISA1 = 1)
#define OSC_CH3_SetDigitalOutput()     (_TRISA1 = 0)
#define OSC_CH3_SetDigital()           (_ANSA1 = 0)
#define OSC_CH3_SetAnalog()            (_ANSA1 = 1)

#define OSC_MIC_SetHigh()              (_LATB0 = 1)
#define OSC_MIC_SetLow()               (_LATB0 = 0)
#define OSC_MIC_Toggle()               (_LATB0 ^= 1)
#define OSC_MIC_GetValue()             _RB0
#define OSC_MIC_SetDigitalInput()      (_TRISB0 = 1)
#define OSC_MIC_SetDigitalOutput()     (_TRISB0 = 0)
#define OSC_MIC_SetDigital()           (_ANSB0 = 0)
#define OSC_MIC_SetAnalog()            (_ANSB0 = 1)

/*******************************************************************************
 * Logic Analyzer
 ******************************************************************************/
#define LA1_SetHigh()                  (_LATB10 = 1)
#define LA1_SetLow()                   (_LATB10 = 0)
#define LA1_Toggle()                   (_LATB10 ^= 1)
#define LA1_GetValue()                 _RB10
#define LA1_SetDigitalInput()          (_TRISB10 = 1)
#define LA1_SetDigitalOutput()         (_TRISB10 = 0)

#define LA2_SetHigh()                  (_LATB11 = 1)
#define LA2_SetLow()                   (_LATB11 = 0)
#define LA2_Toggle()                   (_LATB11 ^= 1)
#define LA2_GetValue()                 _RB11
#define LA2_SetDigitalInput()          (_TRISB11 = 1)
#define LA2_SetDigitalOutput()         (_TRISB11 = 0)

#define LA3_SetHigh()                  (_LATB12 = 1)
#define LA3_SetLow()                   (_LATB12 = 0)
#define LA3_Toggle()                   (_LATB12 ^= 1)
#define LA3_GetValue()                 _RB12
#define LA3_SetDigitalInput()          (_TRISB12 = 1)
#define LA3_SetDigitalOutput()         (_TRISB12 = 0)

#define LA4_SetHigh()                  (_LATB13 = 1)
#define LA4_SetLow()                   (_LATB13 = 0)
#define LA4_Toggle()                   (_LATB13 ^= 1)
#define LA4_GetValue()                 _RB13
#define LA4_SetDigitalInput()          (_TRISB13 = 1)
#define LA4_SetDigitalOutput()         (_TRISB13 = 0)

/*******************************************************************************
 * Multimeter
 ******************************************************************************/
#define FQY_SetHigh()                  (_LATB14 = 1)
#define FQY_SetLow()                   (_LATB14 = 0)
#define FQY_Toggle()                   (_LATB14 ^= 1)
#define FQY_GetValue()                 _RB14
#define FQY_SetDigitalInput()          (_TRISB14 = 1)
#define FQY_SetDigitalOutput()         (_TRISB14 = 0)

#define CAP_IN_SetHigh()               (_LATB3 = 1)
#define CAP_IN_SetLow()                (_LATB3 = 0)
#define CAP_IN_Toggle()                (_LATB3 ^= 1)
#define CAP_IN_GetValue()              _RB3
#define CAP_IN_SetDigitalInput()       (_TRISB3 = 1)
#define CAP_IN_SetDigitalOutput()      (_TRISB3 = 0)
#define CAP_IN_SetDigital()            (_ANSB3 = 0)
#define CAP_IN_SetAnalog()             (_ANSB3 = 1)

#define CAP_OUT_SetHigh()              (_LATC0 = 1)
#define CAP_OUT_SetLow()               (_LATC0 = 0)
#define CAP_OUT_Toggle()               (_LATC0 ^= 1)
#define CAP_OUT_GetValue()             _RC0
#define CAP_OUT_SetDigitalInput()      (_TRISC0 = 1)
#define CAP_OUT_SetDigitalOutput()     (_TRISC0 = 0)
#define CAP_OUT_SetDigital()           (_ANSC0 = 0)
#define CAP_OUT_SetAnalog()            (_ANSC0 = 1)

#define RES_SetHigh()                  (_LATC1 = 1)
#define RES_SetLow()                   (_LATC1 = 0)
#define RES_Toggle()                   (_LATC1 ^= 1)
#define RES_GetValue()                 _RC1
#define RES_SetDigitalInput()          (_TRISC1 = 1)
#define RES_SetDigitalOutput()         (_TRISC1 = 0)
#define RES_SetDigital()               (_ANSC1 = 0)
#define RES_SetAnalog()                (_ANSC1 = 1)

#define VOL_SetHigh()                  (_LATC2 = 1)
#define VOL_SetLow()                   (_LATC2 = 0)
#define VOL_Toggle()                   (_LATC2 ^= 1)
#define VOL_GetValue()                 _RC2
#define VOL_SetDigitalInput()          (_TRISC2 = 1)
#define VOL_SetDigitalOutput()         (_TRISC2 = 0)
#define VOL_SetDigital()               (_ANSC2 = 0)
#define VOL_SetAnalog()                (_ANSC2 = 1)

/*******************************************************************************
 * Wave Generator
 ******************************************************************************/
#define SQR1_Setter                    _LATC6
#define SQR1_SetHigh()                 (_LATC6 = 1)
#define SQR1_SetLow()                  (_LATC6 = 0)
#define SQR1_Toggle()                  (_LATC6 ^= 1)
#define SQR1_GetValue()                _RC6
#define SQR1_SetDigitalInput()         (_TRISC6 = 1)
#define SQR1_SetDigitalOutput()        (_TRISC6 = 0)

#define SQR2_Setter                    _LATC7
#define SQR2_SetHigh()                 (_LATC7 = 1)
#define SQR2_SetLow()                  (_LATC7 = 0)
#define SQR2_Toggle()                  (_LATC7 ^= 1)
#define SQR2_GetValue()                _RC7
#define SQR2_SetDigitalInput()         (_TRISC7 = 1)
#define SQR2_SetDigitalOutput()        (_TRISC7 = 0)

#define SQR3_Setter                    _LATC8
#define SQR3_SetHigh()                 (_LATC8 = 1)
#define SQR3_SetLow()                  (_LATC8 = 0)
#define SQR3_Toggle()                  (_LATC8 ^= 1)
#define SQR3_GetValue()                _RC8
#define SQR3_SetDigitalInput()         (_TRISC8 = 1)
#define SQR3_SetDigitalOutput()        (_TRISC8 = 0)

#define SQR4_Setter                    _LATC9
#define SQR4_SetHigh()                 (_LATC9 = 1)
#define SQR4_SetLow()                  (_LATC9 = 0)
#define SQR4_Toggle()                  (_LATC9 ^= 1)
#define SQR4_GetValue()                _RC9
#define SQR4_SetDigitalInput()         (_TRISC9 = 1)
#define SQR4_SetDigitalOutput()        (_TRISC9 = 0)

/*******************************************************************************
 * Digital Pin Mappings
 ******************************************************************************/
#define PIN_MANAGER_DIGITAL_PINS_LA1     42 // I/O
#define PIN_MANAGER_DIGITAL_PINS_LA2     43 // I/O
#define PIN_MANAGER_DIGITAL_PINS_LA3     44 // I
#define PIN_MANAGER_DIGITAL_PINS_LA4     45 // I
#define PIN_MANAGER_DIGITAL_PINS_COMP4    4 // Virtual
#define PIN_MANAGER_DIGITAL_PINS_SPI_CS  52 // I
#define PIN_MANAGER_DIGITAL_PINS_FREQ    46 // I

/**
    Section: Variable Prototypes
 */
extern uint8_t PIN_MANAGER_DIGITAL_PINS[];

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
    Set port configuration for wave generator (SQR) pins

  @Description
    This method provides an interface to clear the logic value at SQR pins and
    assign desired values to PORT register

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

 */
response_t PIN_MANAGER_SetWavePinState(void);

/**
  @Summary
    Get port values on logic analyzer (LA) pins

  @Description
    This method filters and fetches PORT register values correspond to logic
    analyzer pins

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

 */
response_t PIN_MANAGER_GetLAPinState(void);

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
