/* 
 * File:   functions.h
 * Author: jithin
 *
 * Created on October 17, 2014, 12:33 PM
 */

#ifndef FUNCTIONS_H
#define	FUNCTIONS_H

/*
 * File:   functions.h
 * Author: jithin
 *
 * Created on October 17, 2014, 11:48 AM
 */

#include <p24EP256GP204.h>
#include <libpic30.h>
#include "COMMANDS.h"

#define CSCH1 _LATA10
#define CSCH2 _LATA7
#define SCL_PIN _LATB4
#define SDA_PIN _RA8

#define CS1 _LATC5
#define CS2 _LATC4

#define CSOUT1 _LATB5
#define CSOUT2 _LATB6

#define CSNUM_A1 1
#define CSNUM_A2 2
#define CSNUM_CS1 6
#define CSNUM_CS2 7
#define CSNUM_WG 8
#define CSNUM_OUT1 9
#define CSNUM_OUT2 10

#define COUT1_HIGH _LATB5=1
#define COUT1_LOW  _LATB5=0
#define COUT2_HIGH _LATB6=1;Nop();
#define COUT2_LOW  _LATB6=0;Nop();

__prog__ unsigned int __attribute__((section("CALIBS"), space(prog), aligned(_FLASH_PAGE * 2))) dat1[15][_FLASH_PAGE];


//__eds__ unsigned int ADCbuffer[BUFFER_SIZE] __attribute__((space(eds))); 
int __attribute__((section("adcbuff"), far)) ADCbuffer[BUFFER_SIZE];
int *endbuff;
int *buffpointer, *endpointer, dma_channel_length = 10, I2CSamples = 0;
BYTE *bytebuff1, *bytebuff2;

unsigned int dest[_FLASH_ROW * 8];
int ulsb, umsb; //DAC_OFFSETS[4],

/*------UART VARIABLES-----*/
unsigned int TCD = 1000;

/*------LOGIC ANALYZER VARIABLES-----*/
BYTE INITIAL_DIGITAL_STATES = 0, INITIAL_DIGITAL_STATES_ERR = 0, DIGITAL_TRIGGER_CHANNEL = 32, DIGITAL_TRIGGER_STATE = 0, b1, b2, COMPARATOR_CONFIG = 7 | (3 << 4), I2CConvDone = 1;
unsigned int lsb, msb, blk[8], tmp_int2, tmp_int3, tmp_int4, tmp_int5, tmp_int6;

unsigned int LAFinished = 1, LASamples;
unsigned int I2CTotalSamples = BUFFER_SIZE;
unsigned long val, l1, l2;
BYTE DIN_REMAPS[] = {ID1_REMAP, ID2_REMAP, ID3_REMAP, ID4_REMAP, COMP4_REMAP, RP41_REMAP, FREQ_REMAP}, LAM1 = 0, LAM2 = 0, LAM3 = 0, LAM4 = 0;

/*-----OSCILLOSCOPE VARIABLES-------*/
BYTE SH = 5, ICG = 15, I2C_TRIGGER_CHANNEL = 0, I2C_TRIGGERED = 0, I2C_TRIGGER_READY = 0, I2C_SCOPE_LOCATION = 0x00, I2C_SCOPE_ADDRESS = 0x00, I2C_SCOPE_BYTES = 0;

BYTE frequency_scaling = 1, frequency_ready = FALSE;
unsigned int freq_lsb, freq_msb, freq2_lsb, freq2_msb;
_prog_addressT p, pProg;

/*--------Stepper Motor--------*/
BYTE motor_phases[] = {12, 6, 3, 9}, current_motor_phase = 0;

BYTE chan = 1;
BYTE data[32];
BYTE cb, cc;
long addr_count = 0xAAAA01;

void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void);
void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt(void); //For frequency counter



void set_cap_voltage(BYTE v, unsigned int time);
unsigned int get_cc_capacitance(BYTE, BYTE, unsigned int);
unsigned int get_cap_range(unsigned int);
unsigned int get_ctmu_voltage(BYTE, BYTE, BYTE);
void get_high_frequency(BYTE, BYTE);
void init_IC_for_frequency(BYTE capture_pin, BYTE capture_mode, BYTE captures_per_interrupt);
void startCounting(BYTE channel);
void TimingMeasurements(BYTE, BYTE, BYTE, BYTE, BYTE, BYTE);
void Interval(BYTE, BYTE, BYTE, BYTE);
void disableCTMUSource(void);
void start_1chan_LA(unsigned int, BYTE, BYTE, BYTE);
void start_2chan_LA(unsigned int, BYTE, BYTE);
void start_3chan_LA(unsigned int, unsigned int, BYTE);

void start_4chan_LA(unsigned int, unsigned int, BYTE);
void disable_input_capture();
void setMultiFuncPortMode(BYTE);

void init(void);
void setFlashPointer(BYTE);
void delayTMR4(int);
void set_RGB(unsigned long);
void set_RGB_now(unsigned long);
void PrepareTrigger(void);

void enableLogicAnalyser(void);
void disableLogicAnalyser(void);

void mapReferenceOscillator(BYTE, BYTE);
void Delay_with_pulse(unsigned int);

void setPGA(char, char);
void setSensorChannel(char);

void read_all_from_flash(_prog_addressT pointer);
void load_to_flash(_prog_addressT pointer, BYTE location, unsigned int * blk);
void read_flash(_prog_addressT pointer, BYTE location);

#endif	/* FUNCTIONS_H */

