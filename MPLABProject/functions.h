//PSLab  Firmware
//Author  : Jithin B.P, jithinbp@gmail.com
//License : Apache
//Date : 30-March-2017

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
#include"commands.h"

#define FP 64000000
#define BAUDRATE 1000000    //1M
#define BAUDRATE2 10000    //10K
//If BRGH=0 (16 clocks per BAUD), replace '/4' with '/16'
#define BRGVAL ((FP/BAUDRATE)/4)-1
#define BRGVAL38400 ((FP/38400)/4)-1
#define BRGVAL115200 ((FP/115200)/4)-1
#define BRGVAL500000 ((FP/500000)/4)-1
#define BRGVAL1000000 ((FP/1000000)/4)-1
#define BRGVAL2 ((FP/BAUDRATE2)/16)-1


#include "xc.h"
#include"functions.h"

#define Fs   		4000
#define SAMPPRD		(FP/Fs)-1
#define BYTE unsigned char

typedef BYTE bool;
typedef unsigned int uint16;
#define true 1
#define false 0

#define ERROR_BUFFLEN 1500

/*-----PPS and PIN DEFINITIONS-----*/
// suffixes and their meanings :
// _PPS : Remappable pin number. This is used to assign the pin to outputs such as OCR1 , PWMOUT etc
// _OUT : The output latch used to write a state to the pin
// _TRIS : Used to set the state of the pin (In / output / tristate)
// _ANS : Analog feature. Only available on select pins
// _IN : Digital input. Used for reading the state of the port

#define CSCH1 _LATA10
#define CSCH2 _LATA7
#define SCL_OUT _LATB4
#define SDA_OUT _RA8


//W1 C9 RP57
#define W1_PPS  _RP57R
#define W1_OUT _LATC9
#define W1_TRIS _TRISC9

//W2 C8 RP56
#define W2_PPS  _RP56R
#define W2_OUT _LATC8
#define W2_TRIS _TRISC8



//status LED B15 / RPI47
#define LED_TRIS _TRISB15
#define LED_OUT _LATB15  
#define LED_ANS _ANSB15  


//Make the following compatible with PSLab schematic

//sqr1 RP54, C6
#define SQR1_PPS _RP54R
#define SQR1_OUT _LATC6
#define SQR1_TRIS _TRISC6
#define SQR1_IN _RC6
#define SQR1_PPS_IN 0x36
//sqr2 RP55, C7
#define SQR2_PPS _RP55R
#define SQR2_OUT _LATC7
#define SQR2_TRIS _TRISC7
#define SQR2_IN _RC7
#define SQR2_PPS_IN 0x37
//sqr3 RP56, C8
#define SQR3_PPS _RP56R
#define SQR3_OUT _LATC8
#define SQR3_TRIS _TRISC8
#define SQR3_IN _RC8
#define SQR3_PPS_IN 0x38
//sqr4 RP57, C9
#define SQR4_PPS _RP57R
#define SQR4_OUT _LATC9
#define SQR4_TRIS _TRISC9
#define SQR4_IN _RC9
#define SQR4_PPS_IN 0x39




//OD1 readback
#define OD1_READBACK_TRIS _TRISB10
#define OD1_READBACK_PPS 42


//OD1 RP54, C6
#define OD1_PPS _RP54R
#define OD1_OUT _LATC6
#define OD1_TRIS _TRISC6
#define OD1_IN _RC6
#define OD1_PPS_IN 0x36

#define CCS_PPS _RPI46R
#define CCS_OUT _LATB14
#define CCS_TRIS _TRISB14
#define CCS_IN  _RB14

#define RX_TRIS _TRISB8
#define RX_PPS_IN 0x28

#define TX_TRIS _TRISB7
#define TX_PPS _RP39R

#define CAP_CHARGE_TRIS _TRISB15      
#define CAP_CHARGE_OUT _LATB15
#define CAP_CHARGE_PULLUP _CNPUB15
#define CAP_CHARGE_PULLDOWN _CNPDB15

//ID1
#define ID1_TRIS _TRISB2
#define ID1_IN _RB2
#define ID1_ANS _ANSB2
#define ID1_PULLDOWN _CNPDB2
#define ID1_PPS_IN 0x22


//ID2
#define ID1_TRIS _TRISB2
#define ID1_IN _RB2
#define ID1_ANS _ANSB2
#define ID1_PULLDOWN _CNPDB2
#define ID1_PPS_IN 0x22

//ID3
#define ID1_TRIS _TRISB2
#define ID1_IN _RB2
#define ID1_ANS _ANSB2
#define ID1_PULLDOWN _CNPDB2
#define ID1_PPS_IN 0x22

//ID4
#define ID1_TRIS _TRISB2
#define ID1_IN _RB2
#define ID1_ANS _ANSB2
#define ID1_PULLDOWN _CNPDB2
#define ID1_PPS_IN 0x22


//COMPARATOR4 on SEN
#define COMP4_PPS_IN 0x4


#define SQ1_REMAP 41
#define OD1_REMAP 54
#define SQ2_REMAP 55
#define SQ3_REMAP 56


#define ID1_READ _RB2
#define SQR1_READ _RB11
#define OD1_READ  _RB10
#define COMP4_READ _C4OUT


#define NO_REMAP_USE_FP 254

#define CSNUM_A1 1
#define CSNUM_A2 2

#define BUFFER_SIZE 10000

//SPI modes
#define SPI_8 1
#define SPI_16 2

#define TRUE 1
#define FALSE 0

#define DELAY_105uS asm volatile ("REPEAT, #6721"); Nop(); // 105uS delay


/*--------------ENABLE SPECIAL FUNCTIONS-------------*/
//#define HX711_ENABLED
//#define HCSR04_ENABLED



/*------------Sine Table--------------*/

#define WAVE_TABLE_FULL_LENGTH 512
#define WAVE_TABLE_SHORT_LENGTH 32

#define HIGH_RES_WAVE 512
#define LOW_RES_WAVE  64



/*Global Variables*/


extern __prog__ unsigned int __attribute__((section("CALIBS"), space(prog), aligned(_FLASH_PAGE * 2))) dat1[15][_FLASH_PAGE];

//__eds__ unsigned int ADCbuffer[BUFFER_SIZE] __attribute__((space(eds))); 
extern int __attribute__((section("adcbuff"),far)) ADCbuffer[BUFFER_SIZE];
extern int *buff0,*buff1,*endbuff,*buff2,*buff3,samples_to_fetch;
extern int  *buffpointer, *endpointer, dma_channel_length,samples,I2CSamples;
extern BYTE *bytebuff1,*bytebuff2,ca,cb,cc;

extern unsigned int dest[_FLASH_ROW * 8];
extern unsigned int ulsb,umsb; //DAC_OFFSETS[4],

/*-----SPI VARIABLES-------*/
extern BYTE location, value,ADC_MODE ,DMA_MODE;
extern BYTE SPI_PPRE,SPI_SPRE,SPI_CKE,SPI_CKP,SPI_SMP,SPI_MODE;

/*------UART VARIABLES-----*/
extern unsigned int I2C_BRGVAL,TCD ;

/*------LOGIC ANALYZER VARIABLES-----*/
extern unsigned int i, lsb, msb, blk[8], c1, c2,adval,tmp_int1,tmp_int2;

extern unsigned long val,l1,l2;
extern BYTE DIN_REMAPS[] ,LAM1,LAM2,LAM3,LAM4;

/*-----OSCILLOSCOPE VARIABLES-------*/
extern _prog_addressT p,pProg;

/*--------Error handling definitions------*/
extern char errors[ERROR_BUFFLEN],tmpstr[25];
extern char *error_readpos,*error_writepos;


extern int  __attribute__((section("sine_table1"))) sineTable1[];
extern int  __attribute__((section("sine_table1_short"))) sineTable1_short[] ;

void init(void);
void setFlashPointer(BYTE);
void delayTMR4(int);

void initUART(unsigned int);
bool hasChar();
void sendChar(BYTE val);
void sendInt(unsigned int val);
void sendLong(unsigned int lsb,unsigned int msb);
char getChar();
unsigned int getInt();
void ack(BYTE);

void initDAC();

void Delay_us(unsigned int);

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


#endif	/* FUNCTIONS_H */

