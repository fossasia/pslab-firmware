/* 
 * File:   PSLAB_NRF.h
 * Author: padmal
 *
 * Created on June 1, 2017, 12:42 PM
 */

#ifndef PSLAB_NRF_H
#define	PSLAB_NRF_H

#define CE_HIGH _LATC5=1
#define CE_LOW  _LATC5=0

#define CSN_HIGH _LATC4=1;Nop();
#define CSN_LOW  _LATC4=0;Nop();

static BYTE RXTX_ADDR[3] = { 0x01, 0xAA, 0xAA }; //Randomly chosen address
static BYTE TOKEN_ADDR[3] = { 0xFF, 0xAA, 0xAA }; //Fixed address on pipe 2.
static BYTE i2c_list[NRF_REPORT_ROWS][NRF_ROW_LENGTH];
static BYTE rfCardPresent = 0;
static BYTE ca;
static BYTE nodecount;

static char tmpstr[25];

static unsigned int n;

#define ERROR_BUFFLEN 1500

/*--------Error handling definitions------*/
static char errors[ERROR_BUFFLEN];
static char *error_readpos, *error_writepos;

/*Command set for the NRFL01+ radio*/
void nRF_Setup();
void RXMode();
void TXMode();
void PowerDown();
unsigned char RXChar();
void TXChar(unsigned char ch);
unsigned char ReadDataAvailable();
void FlushTXRX();
void WriteRegister(unsigned char reg, unsigned char val);
void WriteAddress(unsigned char reg, unsigned char num, unsigned char* addr);
unsigned char ReadRegister(unsigned char reg);
unsigned char ReadStatus();
void WriteCommand(unsigned char command);
void WritePayload(unsigned char addr, unsigned char num, unsigned char* data);
void ReadPayload(unsigned char num, unsigned char* data);
void logit(char *str);

#endif	/* PSLAB_NRF_H */

