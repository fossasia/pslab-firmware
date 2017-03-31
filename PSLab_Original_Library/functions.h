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
#include<libpic30.h>
#include"COMMANDS.h"

#define FP 64000000
#define BAUDRATE 1000000    //1M
#define BAUDRATE2 10000    //10K
//If BRGH=0 (16 clocks per BAUD), replace '/4' with '/16'
#define BRGVAL ((FP/BAUDRATE)/4)-1
#define BRGVAL9600 ((FP/9600)/4)-1
#define BRGVAL14400 ((FP/14400)/4)-1
#define BRGVAL19200 ((FP/19200)/4)-1
#define BRGVAL28800 ((FP/28800)/4)-1
#define BRGVAL38400 ((FP/38400)/4)-1
#define BRGVAL57600 ((FP/57600)/4)-1
#define BRGVAL115200 ((FP/115200)/4)-1
#define BRGVAL230400 ((FP/230400)/4)-1
#define BRGVAL460800 ((FP/460800)/4)-1
#define BRGVAL500000 ((FP/500000)/4)-1
#define BRGVAL1000000 ((FP/1000000)/4)-1
#define BRGVAL2000000 ((FP/2000000)/4)-1
#define BRGVAL4000000 ((FP/4000000)/4)-1



#define BRGVAL2 ((FP/BAUDRATE2)/16)-1

#define Fs   		4000
#define SAMPPRD		(FP/Fs)-1
#define BYTE unsigned char

typedef BYTE bool;
typedef unsigned int uint16;
#define true 1
#define false 0

#define ERROR_BUFFLEN 1500

#define CSCH1 _LATA10
#define CSCH2 _LATA7
#define SCL_PIN _LATB4
#define SDA_PIN _RA8

#define LEDPIN _LATB15   //status LED


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


#define CE_HIGH _LATC5=1
#define CE_LOW  _LATC5=0
#define CSN_HIGH _LATC4=1;Nop();
#define CSN_LOW  _LATC4=0;Nop();

#define COUT1_HIGH _LATB5=1
#define COUT1_LOW  _LATB5=0
#define COUT2_HIGH _LATB6=1;Nop();
#define COUT2_LOW  _LATB6=0;Nop();




#define TRUE 1
#define FALSE 0

#define DELAY_105uS asm volatile ("REPEAT, #6721"); Nop(); // 105uS delay

__prog__ unsigned int __attribute__((section("CALIBS"), space(prog), aligned(_FLASH_PAGE * 2))) dat1[15][_FLASH_PAGE];


//__eds__ unsigned int ADCbuffer[BUFFER_SIZE] __attribute__((space(eds))); 
int __attribute__((section("adcbuff"),far)) ADCbuffer[BUFFER_SIZE];
int *buff0,*buff1,*endbuff,*buff2,*buff3;
int  *buffpointer, *endpointer, dma_channel_length=10,samples=0,I2CSamples=0;
BYTE *bytebuff1,*bytebuff2;

unsigned int dest[_FLASH_ROW * 8];
int ulsb,umsb; //DAC_OFFSETS[4],

/*-----SPI VARIABLES-------*/
BYTE location, value,ADC_MODE=NOT_READY,SPI_MODE=NOT_READY,MULTIFUNC_PORT = NOT_READY,DMA_MODE=NOT_READY,ADC_STREAMING=false;
BYTE SPI_PPRE=0,SPI_SPRE=2,SPI_CKE=1,SPI_CKP=0,SPI_SMP=1;

/*------UART VARIABLES-----*/
unsigned int I2C_BRGVAL = 0x272,TCD = 1000;

/*------LOGIC ANALYZER VARIABLES-----*/
BYTE INITIAL_DIGITAL_STATES=0,INITIAL_DIGITAL_STATES_ERR=0,DIGITAL_TRIGGER_CHANNEL=32,DIGITAL_TRIGGER_STATE=0,b1,b2,COMPARATOR_CONFIG=7|(3<<4),conversion_done = 1,I2CConvDone = 1;
unsigned int i, lsb, msb, blk[8], c1, c2,adval,tmp_int1,tmp_int2,tmp_int3,tmp_int4,tmp_int5,tmp_int6;

unsigned int LAFinished = 1, LASamples;
unsigned int samples_to_fetch = BUFFER_SIZE, I2CTotalSamples = BUFFER_SIZE;
unsigned long val,l1,l2;
BYTE DIN_REMAPS[] = {ID1_REMAP,ID2_REMAP,ID3_REMAP,ID4_REMAP,COMP4_REMAP,RP41_REMAP,FREQ_REMAP},LAM1=0,LAM2=0,LAM3=0,LAM4=0;

/*-----OSCILLOSCOPE VARIABLES-------*/

BYTE ADC_CHANNELS = 0,CH123SA=0,CHOSA=3; // CH1 only
BYTE TRIGGER_CHANNEL=0,TRIGGERED=0,TRIGGER_READY=0,SH=5,ICG=15,I2C_TRIGGER_CHANNEL=0,I2C_TRIGGERED=0,I2C_TRIGGER_READY=0, I2C_SCOPE_LOCATION=0x00,I2C_SCOPE_ADDRESS=0x00,I2C_SCOPE_BYTES=0;
unsigned int TRIGGER_TIMEOUT=100,TRIGGER_WAITING=0,TRIGGER_LEVEL=0,TRIGGER_PRESCALER=0;
unsigned int ADC_DELAY = 5;

BYTE frequency_scaling=1,frequency_ready=false;
unsigned int freq_lsb,freq_msb,freq2_lsb,freq2_msb;
_prog_addressT p,pProg;

/*--------Stepper Motor--------*/
BYTE motor_phases[] = {12,6,3,9},current_motor_phase = 0;


/*--------Error handling definitions------*/
char errors[ERROR_BUFFLEN],tmpstr[25];
char *error_readpos=&errors[0],*error_writepos=&errors[0];


/*------------Sine Table--------------*/

#define WAVE_TABLE_FULL_LENGTH 512
#define WAVE_TABLE_SHORT_LENGTH 32

#define HIGH_RES_WAVE 512
#define LOW_RES_WAVE  64

int  __attribute__((section("sine_table1"))) sineTable1[]  ={
256, 252, 249, 246, 243, 240, 237, 234, 230, 227, 224, 221, 218, 215, 212, 209, 206, 203, 200, 196, 193, 190, 187, 184, 181, 178, 175, 172, 169, 166, 164, 161, 158, 155, 152, 149, 146, 143, 141, 138, 135, 132, 130, 127, 124, 121, 119, 116, 114, 111, 108, 106, 103, 101, 98, 96, 93, 91, 89, 86, 84, 82, 79, 77, 75, 73, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 47, 45, 43, 41, 40, 38, 36, 35, 33, 32, 30, 29, 27, 26, 25, 23, 22, 21, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 29, 30, 32, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 73, 75, 77, 79, 82, 84, 86, 89, 91, 93, 96, 98, 101, 103, 106, 108, 111, 114, 116, 119, 121, 124, 127, 130, 132, 135, 138, 141, 143, 146, 149, 152, 155, 158, 161, 164, 166, 169, 172, 175, 178, 181, 184, 187, 190, 193, 196, 200, 203, 206, 209, 212, 215, 218, 221, 224, 227, 230, 234, 237, 240, 243, 246, 249, 252, 256, 259, 262, 265, 268, 271, 274, 277, 281, 284, 287, 290, 293, 296, 299, 302, 305, 308, 311, 315, 318, 321, 324, 327, 330, 333, 336, 339, 342, 345, 347, 350, 353, 356, 359, 362, 365, 368, 370, 373, 376, 379, 381, 384, 387, 390, 392, 395, 397, 400, 403, 405, 408, 410, 413, 415, 418, 420, 422, 425, 427, 429, 432, 434, 436, 438, 441, 443, 445, 447, 449, 451, 453, 455, 457, 459, 461, 463, 464, 466, 468, 470, 471, 473, 475, 476, 478, 479, 481, 482, 484, 485, 486, 488, 489, 490, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 503, 504, 505, 505, 506, 507, 507, 508, 508, 509, 509, 509, 510, 510, 510, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 510, 510, 510, 509, 509, 509, 508, 508, 507, 507, 506, 505, 505, 504, 503, 503, 502, 501, 500, 499, 498, 497, 496, 495, 494, 493, 492, 490, 489, 488, 486, 485, 484, 482, 481, 479, 478, 476, 475, 473, 471, 470, 468, 466, 464, 463, 461, 459, 457, 455, 453, 451, 449, 447, 445, 443, 441, 438, 436, 434, 432, 429, 427, 425, 422, 420, 418, 415, 413, 410, 408, 405, 403, 400, 397, 395, 392, 390, 387, 384, 381, 379, 376, 373, 370, 368, 365, 362, 359, 356, 353, 350, 347, 345, 342, 339, 336, 333, 330, 327, 324, 321, 318, 315, 311, 308, 305, 302, 299, 296, 293, 290, 287, 284, 281, 277, 274, 271, 268, 265, 262, 259};

int  __attribute__((section("sine_table2"))) sineTable2[]  ={
256, 252, 249, 246, 243, 240, 237, 234, 230, 227, 224, 221, 218, 215, 212, 209, 206, 203, 200, 196, 193, 190, 187, 184, 181, 178, 175, 172, 169, 166, 164, 161, 158, 155, 152, 149, 146, 143, 141, 138, 135, 132, 130, 127, 124, 121, 119, 116, 114, 111, 108, 106, 103, 101, 98, 96, 93, 91, 89, 86, 84, 82, 79, 77, 75, 73, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 47, 45, 43, 41, 40, 38, 36, 35, 33, 32, 30, 29, 27, 26, 25, 23, 22, 21, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 6, 6, 7, 8, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 25, 26, 27, 29, 30, 32, 33, 35, 36, 38, 40, 41, 43, 45, 47, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 73, 75, 77, 79, 82, 84, 86, 89, 91, 93, 96, 98, 101, 103, 106, 108, 111, 114, 116, 119, 121, 124, 127, 130, 132, 135, 138, 141, 143, 146, 149, 152, 155, 158, 161, 164, 166, 169, 172, 175, 178, 181, 184, 187, 190, 193, 196, 200, 203, 206, 209, 212, 215, 218, 221, 224, 227, 230, 234, 237, 240, 243, 246, 249, 252, 256, 259, 262, 265, 268, 271, 274, 277, 281, 284, 287, 290, 293, 296, 299, 302, 305, 308, 311, 315, 318, 321, 324, 327, 330, 333, 336, 339, 342, 345, 347, 350, 353, 356, 359, 362, 365, 368, 370, 373, 376, 379, 381, 384, 387, 390, 392, 395, 397, 400, 403, 405, 408, 410, 413, 415, 418, 420, 422, 425, 427, 429, 432, 434, 436, 438, 441, 443, 445, 447, 449, 451, 453, 455, 457, 459, 461, 463, 464, 466, 468, 470, 471, 473, 475, 476, 478, 479, 481, 482, 484, 485, 486, 488, 489, 490, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 503, 504, 505, 505, 506, 507, 507, 508, 508, 509, 509, 509, 510, 510, 510, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 511, 510, 510, 510, 509, 509, 509, 508, 508, 507, 507, 506, 505, 505, 504, 503, 503, 502, 501, 500, 499, 498, 497, 496, 495, 494, 493, 492, 490, 489, 488, 486, 485, 484, 482, 481, 479, 478, 476, 475, 473, 471, 470, 468, 466, 464, 463, 461, 459, 457, 455, 453, 451, 449, 447, 445, 443, 441, 438, 436, 434, 432, 429, 427, 425, 422, 420, 418, 415, 413, 410, 408, 405, 403, 400, 397, 395, 392, 390, 387, 384, 381, 379, 376, 373, 370, 368, 365, 362, 359, 356, 353, 350, 347, 345, 342, 339, 336, 333, 330, 327, 324, 321, 318, 315, 311, 308, 305, 302, 299, 296, 293, 290, 287, 284, 281, 277, 274, 271, 268, 265, 262, 259};


int  __attribute__((section("sine_table1_short"))) sineTable1_short[]  ={
      32, 26, 20, 14, 9, 5, 2, 1, 0, 1, 2, 5, 9, 14, 20, 26, 32, 38, 44, 50, 55, 59, 62, 63, 64, 63, 62, 59, 55, 50, 44, 38};

int  __attribute__((section("sine_table2_short"))) sineTable2_short[]  ={
      32, 26, 20, 14, 9, 5, 2, 1, 0, 1, 2, 5, 9, 14, 20, 26, 32, 38, 44, 50, 55, 59, 62, 63, 64, 63, 62, 59, 55, 50, 44, 38};


//definitions for NRFL01+ radio

BYTE RXTX_ADDR[3] = { 0x01, 0xAA, 0xAA }; //Randomly chosen address
BYTE TOKEN_ADDR[3] = { 0xFF, 0xAA, 0xAA }; //Fixed address on pipe 2.
BYTE i2c_list[NRF_REPORT_ROWS][NRF_ROW_LENGTH];
BYTE rfCardPresent = FALSE,chan=1;
BYTE data[32];
BYTE ca,cb,cc;
long addr_count = 0xAAAA01;
BYTE nodecount;

void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void) ;
void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt(void); //For frequency counter



void set_cap_voltage(BYTE v,unsigned int time);
unsigned int get_cc_capacitance(BYTE,BYTE,unsigned int);
unsigned int get_cap_range(unsigned int);
unsigned int get_ctmu_voltage(BYTE,BYTE,BYTE);
void get_high_frequency(BYTE,BYTE);
void init_IC_for_frequency(BYTE capture_pin,BYTE capture_mode,BYTE captures_per_interrupt);
void startCounting(BYTE channel);
void TimingMeasurements(BYTE,BYTE,BYTE,BYTE,BYTE,BYTE);
void Interval(BYTE,BYTE,BYTE,BYTE);
void EnableComparator();
void DisableComparator();
void disableCTMUSource(void);
void start_1chan_LA(unsigned int,BYTE,BYTE,BYTE);
void start_2chan_LA(unsigned int,BYTE,BYTE);
void start_3chan_LA(unsigned int ,unsigned int ,BYTE );

void start_4chan_LA(unsigned int,unsigned int,BYTE);
void disable_input_capture();
void setMultiFuncPortMode(BYTE);

void init(void);
void setFlashPointer(BYTE);
void delayTMR4(int);
void set_RGB(unsigned long);
void set_RGB_now(unsigned long);
void PrepareTrigger(void);
void initADC10(void);
void initADCDMA(BYTE);
void initADC12bit_scope(void);
void initADC12(void);
void initADCCTMU(void);
void initADC12_averaging16();

void setADCMode(BYTE,BYTE,BYTE);
unsigned int get_voltage_summed(BYTE channel);
unsigned int get_voltage(BYTE channel);
void setupADC10();
void configureADC();
void disableADCDMA();
void enableADCDMA();


void enableLogicAnalyser(void);
void disableLogicAnalyser(void);



void initUART(unsigned int);
bool hasChar();
void sendChar(BYTE val);
void sendInt(unsigned int val);
void sendLong(unsigned int lsb,unsigned int msb);
char getChar();
unsigned int getInt();
void ack(BYTE);


void configUART2(unsigned int BAUD);
bool hasChar2(void);
char getChar2(void);
unsigned int getInt2(void);
void sendAddress2(char address) ;
void initUART2(void);
void sendChar2(char val);
void sendInt2(unsigned int val);
void initUART2_passthrough(unsigned int);

void setSPIMode(BYTE);
void initSPI();
BYTE spi_write8(BYTE);
unsigned int spi_write16(unsigned int value);
void start_spi();
void stop_spi();

void sqr1(unsigned int,unsigned int,BYTE);
void sqr2(unsigned int,unsigned int,BYTE);

void sqr4(uint16,uint16 ,uint16,uint16,uint16,uint16,uint16,uint16,BYTE);

void mapReferenceOscillator(BYTE ,BYTE );
void Delay_us(unsigned int);
void Delay_with_pulse(unsigned int);

void setPGA(char, char);
void setSensorChannel(char);

void read_all_from_flash(_prog_addressT pointer);
void load_to_flash(_prog_addressT pointer, BYTE location, unsigned int * blk);
void read_flash(_prog_addressT pointer, BYTE location);

void initI2C(void);


void I2CStart();
void I2CStop();
void I2CRestart();
void I2CAck();
void I2CNak();
void I2CWait();
void I2CSend(BYTE dat);
BYTE I2CRead(BYTE ack);



/*Command set for the NRFL01+ radio*/
void nRF_Setup();
void RXMode();
void TXMode();
void PowerDown();
BYTE RXChar();
void TXChar(BYTE ch);
BYTE ReadDataAvailable();
void FlushTXRX();
void WriteRegister(BYTE reg, BYTE val);
void WriteAddress(BYTE reg, BYTE num, BYTE* addr);
BYTE ReadRegister(BYTE reg);
BYTE ReadStatus();
void WriteCommand(BYTE command);
void WritePayload(BYTE,BYTE num, BYTE* data);
void ReadPayload(BYTE num, BYTE* data);


#endif	/* FUNCTIONS_H */

