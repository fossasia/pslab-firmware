/* 
 * File:   PSLAB_NRF.h
 * Author: padmal
 *
 * Created on June 3, 2017, 7:41 AM
 */

#ifndef PSLAB_NRF_H
#define	PSLAB_NRF_H

#define CSN_HIGH _LATC4=1;Nop();
#define CSN_LOW  _LATC4=0;Nop();
#define CE_LOW  _LATC5=0
#define CE_HIGH _LATC5=1

extern BYTE RXTX_ADDR[3]; //Randomly chosen address
extern BYTE TOKEN_ADDR[3]; //Fixed address on pipe 2.
extern BYTE ca;
extern BYTE i2c_list[NRF_REPORT_ROWS][NRF_ROW_LENGTH];
extern BYTE nodecount;
extern char tmpstr[25];
extern BYTE rfCardPresent;

/*Command set for the NRFL01+ radio*/
extern void nRF_Setup();
extern void RXMode();
extern void TXMode();
extern void PowerDown();
extern unsigned char RXChar();
extern void TXChar(unsigned char ch);
extern unsigned char ReadDataAvailable();
extern void FlushTXRX();
extern void WriteRegister(unsigned char reg, unsigned char val);
extern void WriteAddress(unsigned char reg, unsigned char num, unsigned char* addr);
extern unsigned char ReadRegister(unsigned char reg);
extern unsigned char ReadStatus();
extern void WriteCommand(unsigned char command);
extern void WritePayload(unsigned char, unsigned char num, unsigned char* data);
extern void ReadPayload(unsigned char num, unsigned char* data);

#endif	/* PSLAB_NRF_H */

