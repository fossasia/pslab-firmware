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



//sqr1 RP54, C6
#define SQR1_PPS _RP54R
#define SQR1_OUT _LATC6
#define SQR1_TRIS _TRISC6
#define SQR1_IN _RC6
#define SQR1_PPS_IN 54
//sqr2 RP55, C7
#define SQR2_PPS _RP55R
#define SQR2_OUT _LATC7
#define SQR2_TRIS _TRISC7
#define SQR2_IN _RC7
#define SQR2_PPS_IN 55
//sqr3 RP56, C8
#define SQR3_PPS _RP56R
#define SQR3_OUT _LATC8
#define SQR3_TRIS _TRISC8
#define SQR3_IN _RC8
#define SQR3_PPS_IN 56
//sqr4 RP57, C9
#define SQR4_PPS _RP57R
#define SQR4_OUT _LATC9
#define SQR4_TRIS _TRISC9
#define SQR4_IN _RC9
#define SQR4_PPS_IN 57


//ID1 B10 , RP42
#define ID1_TRIS _TRISB10
#define ID1_IN _RB10
#define ID1_PULLDOWN _CNPDB10
#define ID1_PPS_IN 0x2A
//ID1 B11 , RP43
#define ID2_TRIS _TRISB11
#define ID2_IN _RB11
#define ID2_PULLDOWN _CNPDB11
#define ID2_PPS_IN 0x2B
//ID1 B12 , RP44
#define ID3_TRIS _TRISB12
#define ID3_IN _RB12
#define ID3_PULLDOWN _CNPDB12
#define ID3_PPS_IN 0x2C
//ID1 B13 , RP45
#define ID4_TRIS _TRISB13
#define ID4_IN _RB13
#define ID4_PULLDOWN _CNPDB13
#define ID4_PPS_IN 0x2D

//COMPARATOR4 on SEN
#define COMP4_PPS_IN 0x4
//Additional Input states for reading (Comparator 4 on SEN)
#define COMP4_READ _C4OUT



#define RX_TRIS _TRISB8
#define RX_PPS_IN 0x28

#define TX_TRIS _TRISB7
#define TX_PPS _RP39R


//Capacitance measurement in constant voltage mode. C0, AN6 charges via 20K resistor
#define CAP_CHARGE_TRIS _TRISC0      
#define CAP_CHARGE_OUT _LATC0
#define CAP_CHARGE_ANS _ANSC0
#define CAP_CHARGE_PULLUP _CNPUC0
#define CAP_CHARGE_PULLDOWN _CNPDC0


#define NO_REMAP_USE_FP 254

//Definitions for ADC & PGAs
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

