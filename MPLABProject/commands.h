//PSLab  Firmware
//Author  : Jithin B.P, jithinbp@gmail.com
//License : Apache
//Date : 30-March-2017

#ifndef COMMANDS_H
#define	COMMANDS_H

#include"functions.h"

//TODO : Modify BYTE Header for PSLab python library , or change PSLab python library according to this

#define ACKNOWLEDGE 254
/*----flash memory----*/
#define FLASH 1
#define WRITE_BULK_FLASH  3
#define READ_BULK_FLASH  4


/*-----ADC------*/
#define ADC 2
#define CAPTURE_ONE 1
#define CAPTURE_TWO 2
#define CAPTURE_FOUR 4
#define CONFIGURE_TRIGGER 5
#define GET_CAPTURE_STATUS 6
#define GET_CAPTURE_CHANNEL 7
#define SET_PGA_GAIN 8
#define GET_VOLTAGE 9
#define GET_VOLTAGE_SUMMED 10
#define SELECT_PGA_CHANNEL  12
#define CAPTURE_12BIT   13
#define CAPTURE_12BIT_SCAN   14

#define SET_HI_CAPTURE   15
#define SET_LO_CAPTURE   16

#define SET_HI_CAPTURE12   17
#define SET_LO_CAPTURE12   18
//#define CAPTURE_DMASPEED12 19
#define MULTIPOINT_CAPACITANCE 20


/*------I2C-------*/ //DONE
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
#define I2C_START_SCOPE 17


/*-----------DAC--------*/
#define DAC 6
#define SET_DAC 1


/*--------WAVEGEN-----*/
#define WAVEGEN 7
#define SET_WG 1
#define SET_SQR1  3

#define SET_SINE1 13

#define LOAD_WAVEFORM1 15

/*-----digital outputs----*/
#define DOUT 8
#define SET_STATE 1

/*-----digital inputs-----*/
#define DIN   9
#define GET_STATE  1

/*------TIMING FUNCTIONS-----*/
#define TIMING 10
#define GET_TIMING 1

#define TIMING_MEASUREMENTS 12
#define INTERVAL_MEASUREMENTS 13
#define SINGLE_PIN_EDGES 14
#define DOUBLE_PIN_EDGES 15


/*--------MISCELLANEOUS------*/
#define COMMON 11

#define GET_CTMU_VOLTAGE 1
#define GET_CAPACITANCE 2
#define GET_FREQUENCY   3

#define GET_VERSION 5

#define RETRIEVE_BUFFER     8
#define GET_HIGH_FREQUENCY  9
#define CLEAR_BUFFER 10

#define READ_PROGRAM_ADDRESS 12
#define WRITE_PROGRAM_ADDRESS 13
#define READ_DATA_ADDRESS 14
#define WRITE_DATA_ADDRESS 15

#define GET_CAP_RANGE 16
#define READ_LOG 18

#define GET_ALTERNATE_HIGH_FREQUENCY  20

#define START_CTMU 23
#define STOP_CTMU 24
#define START_COUNTING 25
#define FETCH_COUNT 26

#define FILL_BUFFER 27

/*---------- BAUDRATE for main comm channel----*/
#define SETBAUD				12
#define BAUD38400			5
#define BAUD115200			7
#define BAUD1000000			9


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



/*-------ACKNOWLEDGE BYTES-----*/
#define DO_NOT_BOTHER 0
#define SUCCESS 1
#define ARGUMENT_ERROR 2
#define FAILED  3


/*---------DMA_MODES--------*/
#define DMA_ADC 4


#endif	/* COMMANDS_H */

