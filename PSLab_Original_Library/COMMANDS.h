/* 
 * File:   COMMANDS.h
 * Author: jithin
 *
 * Created on October 17, 2014, 12:34 PM
 */

#ifndef COMMANDS_H
#define	COMMANDS_H

//Define for OLED Compilation
//#define OLED 1

#define ACKNOWLEDGE 254
/*----flash memory----*/
#define FLASH 1
#define READ_FLASH   1
#define WRITE_FLASH  2
#define WRITE_BULK_FLASH  3
#define READ_BULK_FLASH  4


/*-----ADC------*/
#define ADC 2
#define CAPTURE_ONE 1
#define CAPTURE_TWO 2
#define CAPTURE_DMASPEED 3
#define CAPTURE_FOUR 4
#define CONFIGURE_TRIGGER 5
#define GET_CAPTURE_STATUS 6
#define GET_CAPTURE_CHANNEL 7
#define SET_PGA_GAIN 8
#define GET_VOLTAGE 9
#define GET_VOLTAGE_SUMMED 10
//#define START_ADC_STREAMING 11  //Removed. makes the system unstable. re-implement with some checks
#define SELECT_PGA_CHANNEL  12
#define CAPTURE_12BIT   13
//#define CAPTURE_MULTIPLE   14   //removed

#define SET_HI_CAPTURE   15
#define SET_LO_CAPTURE   16

#define SET_HI_CAPTURE12   17
#define SET_LO_CAPTURE12   18
#define CAPTURE_DMASPEED12 19
#define MULTIPOINT_CAPACITANCE 20
#define SET_CAP 21

#define PULSE_TRAIN 22

/*-----SPI--------*/
#define SPI 3
#define START_SPI 1
#define SEND_SPI8 2
#define SEND_SPI16 3
#define STOP_SPI 4
#define SET_SPI_PARAMETERS 5
//#define SEND_SPI8_BURST 6
//#define SEND_SPI16_BURST 7 //Removed. Unnecessary . 

/*------I2C-------*/
#define I2C 4
#define I2C_START 1
#define I2C_SEND 2
#define I2C_STOP 3
#define I2C_RESTART 4
#define I2C_READ_END 5
#define I2C_READ_MORE 6
#define I2C_WAIT 7
#define I2C_SEND_BURST 8
#define I2C_CONFIG 9
#define I2C_STATUS 10
#define I2C_READ_BULK 11
#define I2C_WRITE_BULK 12
#define I2C_ENABLE_SMBUS 13
#define I2C_INIT 14
#define PULLDOWN_SCL 15
#define I2C_DISABLE_SMBUS 16

/*------UART2--------*/
#define UART_2 5
#define SEND_CHAR 1
#define SEND_INT 2
#define SEND_ADDRESS 3
#define SET_BAUD 4
#define SET_MODE 5

#define READ_BYTE 6
#define READ_INT 7
#define READ_UART2_STATUS 8

/*-----------DAC--------*/
#define DAC 6
#define SET_DAC 1
#define SET_CALIBRATED_DAC 2


/*--------WAVEGEN-----*/
#define WAVEGEN 7
#define SET_WG 1
#define SET_SQR1  3
#define SET_SQR2  4
//#define SET_SQRS  5 //removed. use sqr4 instead
#define SQR4  7
#define MAP_REFERENCE 8
#define SET_WG_PHASE 9
#define SET_WAVEFORM_TYPE 10
#define SELECT_FREQ_REGISTER 11

#define DELAY_GENERATOR 12
#define SET_SINE1 13
#define SET_SINE2 14

#define LOAD_WAVEFORM1 15
#define LOAD_WAVEFORM2 16

//#define SQR1_PATTERN  17 //removed

/*-----digital outputs----*/
#define DOUT 8
#define SET_STATE 1

/*-----digital inputs-----*/
#define DIN   9
#define GET_STATE  1
#define GET_STATES 2

/*------TIMING FUNCTIONS-----*/
#define TIMING 10
#define GET_TIMING 1
#define START_ONE_CHAN_LA 4
#define START_TWO_CHAN_LA 5
#define START_FOUR_CHAN_LA 6
#define FETCH_DMA_DATA 7
#define FETCH_INT_DMA_DATA 8
#define FETCH_LONG_DMA_DATA 9

#define COMPARATOR_TO_LA 10
#define GET_INITIAL_DIGITAL_STATES 11

#define TIMING_MEASUREMENTS 12
#define INTERVAL_MEASUREMENTS 13
#define CONFIGURE_COMPARATOR 14

#define START_ALTERNATE_ONE_CHAN_LA 15
#define START_THREE_CHAN_LA 16
#define STOP_LA 17

/*--------MISCELLANEOUS------*/
#define COMMON 11

#define GET_CTMU_VOLTAGE 1
#define GET_CAPACITANCE 2
#define GET_FREQUENCY   3
#define GET_INDUCTANCE  4

#define GET_VERSION 5

#define RETRIEVE_BUFFER     8
#define GET_HIGH_FREQUENCY  9
#define CLEAR_BUFFER 10

#define SETRGB  11
#define READ_PROGRAM_ADDRESS 12
#define WRITE_PROGRAM_ADDRESS 13
#define READ_DATA_ADDRESS 14
#define WRITE_DATA_ADDRESS 15

#define GET_CAP_RANGE 16
#define SETRGB2 17
#define READ_LOG 18

#define RESTORE_STANDALONE 19

#define GET_ALTERNATE_HIGH_FREQUENCY  20
#define SETRGB3 22

#define START_CTMU 23
#define STOP_CTMU 24
#define START_COUNTING 25
#define FETCH_COUNT 26

#define FILL_BUFFER 27

/*---------- BAUDRATE for main comm channel----*/
#define SETBAUD				12
#define BAUD9600			1
#define BAUD14400			2
#define BAUD19200			3
#define BAUD28800			4
#define BAUD38400			5
#define BAUD57600			6
#define BAUD115200			7
#define BAUD230400			8
#define BAUD1000000			9
#define BAUD2000000			10
#define BAUD4000000			11


/*-----NRFL01+ radio module for wireless sensors*/
#define NRFL01  13
#define NRF_SETUP 1
#define NRF_RXMODE 2
#define NRF_TXMODE 3
#define NRF_POWER_DOWN 4
#define NRF_RXCHAR 5
#define NRF_TXCHAR 6
#define NRF_HASDATA 7
#define NRF_FLUSH 8
#define NRF_WRITEREG 9
#define NRF_READREG 10
#define NRF_GETSTATUS 11
#define NRF_WRITECOMMAND 12
#define NRF_WRITEPAYLOAD 13
#define NRF_READPAYLOAD 14
#define NRF_WRITEADDRESS 15
#define NRF_TRANSACTION 16
#define NRF_START_TOKEN_MANAGER 17
#define NRF_STOP_TOKEN_MANAGER 18
#define NRF_TOTAL_TOKENS 19
#define NRF_REPORTS 20
#define NRF_WRITE_REPORT 21
#define NRF_DELETE_REPORT_ROW 22
#define NRF_WRITEADDRESSES 23

#define NONSTANDARD_IO 14

#define HCSR04 2
#define AM2302 3


#define PASSTHROUGHS 15

#define PASS_UART   1
#define PASS_SPI    2
#define PASS_UART_DMA   3




#define ESP_HEADER 254
#define BUFFER_SIZE 10000
#define NOT_READY 0
/*---------ADC definitions---------*/
#define ADC_10BIT_SIMULTANEOUS 1
#define ADC_10BIT_SEQUENTIAL 2
#define ADC_12BIT 3
#define ADC_CTMU  4
#define ADC_12BIT_AVERAGING 5
#define ADC_12BIT_SCOPE 6
#define ADC_10BIT_DMA 7
#define ADC_12BIT_DMA 8

#define SPI_8 1
#define SPI_16 2

/*------------REUSABLE I2C PORT-----*/
#define MULTIFUNC_I2C 1
#define MULTIFUNC_UART 2
#define MULTIFUNC_NONSTANDARD 4
#define MULTIFUNC_UART2_PASSTHROUGH 5

/*------------TIMER 5 modes---------*/
#define TIMER5_ADC  1
#define TIMER5_FC   2
#define TIMER5_LA   3

/*------INPUT CAPTURE---------*/
//capture modes
#define EVERY_SIXTEENTH_RISING_EDGE 0b101
#define EVERY_FOURTH_RISING_EDGE    0b100
#define EVERY_RISING_EDGE           0b011
#define EVERY_FALLING_EDGE          0b010
#define EVERY_EDGE                  0b001

#define ID1_REMAP 42
#define ID2_REMAP 43
#define ID3_REMAP 44
#define ID4_REMAP 45
#define COMP4_REMAP 4
#define RP41_REMAP 41
#define FREQ_REMAP 46 // RPI 46 ,RB14


/*---------DMA_MODES--------*/
#define DMA_LA_ONE_CHAN 1
#define DMA_LA_TWO_CHAN 2
#define DMA_LA_FOUR_CHAN 3


/*-------ACKNOWLEDGE BYTES-----*/
#define DO_NOT_BOTHER 0
#define SUCCESS 1
#define ARGUMENT_ERROR 2
#define FAILED  3


/*-------THIS SECTION IS FOR THE RF TRANSMITTER/RECEIVER MODULE----------*/

//Commands
#define R_REG  0x00
#define W_REG  0x20
#define RX_PAYLOAD  0x61
#define TX_PAYLOAD  0xA0
#define FLUSH_TX  0xE1
#define FLUSH_RX  0xE2
#define ACTIVATE  0x50
#define R_STATUS  0xFF

//Registers
#define NRF_CONFIG  0x0
#define EN_AA  0x01
#define EN_RXADDR  0x02
#define SETUP_AW  0x03
#define SETUP_RETR  0x04
#define RF_CH  0x05
#define RF_SETUP  0x06
#define NRF_STATUS  0x07
#define OBSERVE_TX  0x08
#define CD  0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define R_RX_PL_WID 0x60
#define TX_ADDR  0x10
#define RX_PW_P0  0x11
#define RX_PW_P1  0x12
#define RX_PW_P2  0x13
#define RX_PW_P3  0x14
#define RX_PW_P4  0x15
#define RX_PW_P5  0x16
#define FIFO_STATUS  0x17
#define DYNPD  0x1C
#define FEATURE  0x1D



#define NRF_REPORT_ROWS 15
#define NRF_ROW_LENGTH 20



#endif	/* COMMANDS_H */

