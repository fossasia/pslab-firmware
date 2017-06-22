/* 
 * File:   Function.h
 * Author: padmal
 *
 * Created on June 18, 2017, 8:45 AM
 */

#ifndef FUNCTION_H
#define	FUNCTION_H

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

//__eds__ unsigned int ADCbuffer[BUFFER_SIZE] __attribute__((space(eds))); 
extern int *endbuff;
extern int *buffpointer, *endpointer, dma_channel_length, I2CSamples;
extern BYTE *bytebuff1, *bytebuff2;

extern unsigned int dest[_FLASH_ROW * 8];
extern int ulsb, umsb; //DAC_OFFSETS[4],

/*------UART VARIABLES-----*/
extern unsigned int TCD;

/*------LOGIC ANALYZER VARIABLES-----*/
extern BYTE INITIAL_DIGITAL_STATES_ERR, DIGITAL_TRIGGER_CHANNEL;
extern BYTE DIGITAL_TRIGGER_STATE, b1, b2, COMPARATOR_CONFIG, I2CConvDone;
extern unsigned int lsb, msb, blk[8], tmp_int2, tmp_int3, tmp_int4, tmp_int5, tmp_int6;

extern unsigned int LAFinished, LASamples;
extern unsigned int I2CTotalSamples;
extern unsigned long val, l1, l2;

/*-----OSCILLOSCOPE VARIABLES-------*/
extern BYTE SH, ICG, I2C_TRIGGER_CHANNEL, I2C_TRIGGERED, I2C_TRIGGER_READY;
extern BYTE I2C_SCOPE_LOCATION, I2C_SCOPE_ADDRESS, I2C_SCOPE_BYTES;

extern BYTE frequency_scaling, frequency_ready;
extern unsigned int freq_lsb, freq_msb, freq2_lsb, freq2_msb;
extern _prog_addressT p, pProg;

/*--------Stepper Motor--------*/
extern BYTE motor_phases[], current_motor_phase;

extern BYTE chan;
extern BYTE data[32];
extern BYTE cb, cc;
extern long addr_count;

extern __prog__ unsigned int __attribute__((section("CALIBS"), space(prog), aligned(_FLASH_PAGE * 2))) dat1[15][_FLASH_PAGE];
extern void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void);
extern void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt(void); //For frequency counter

extern void init(void);
extern void setFlashPointer(BYTE);
extern void delayTMR4(int);
extern void set_RGB(unsigned long);
extern void set_RGB_now(unsigned long);
extern void PrepareTrigger(void);

extern void mapReferenceOscillator(BYTE, BYTE);
extern void Delay_with_pulse(unsigned int);

extern void setPGA(char, char);
extern void setSensorChannel(char);

extern void read_all_from_flash(_prog_addressT pointer);
extern void load_to_flash(_prog_addressT pointer, BYTE location, unsigned int * blk);
extern void read_flash(_prog_addressT pointer, BYTE location);

extern void preciseDelay(int t);
extern void set_CS(BYTE channel,BYTE status);
extern void alternate_get_high_frequency(BYTE channel,BYTE scale);

#endif	/* FUNCTION_H */
