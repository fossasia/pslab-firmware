/* 
 * File:   PSLAB_NRF.h
 * Author: padmal
 *
 * Created on June 3, 2017, 7:41 AM
 */

#ifndef PSLAB_NRF_H
#define	PSLAB_NRF_H
#include <xc.h>

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
extern BYTE RXChar();
extern void TXChar(BYTE ch);
extern BYTE ReadDataAvailable();
extern void FlushTXRX();
extern void WriteRegister(BYTE reg, BYTE val);
extern void WriteAddress(BYTE reg, BYTE num, BYTE* addr);
extern BYTE ReadRegister(BYTE reg);
extern BYTE ReadStatus();
extern void WriteCommand(BYTE command);
extern void WritePayload(BYTE, BYTE num, BYTE* data);
extern void ReadPayload(BYTE num, BYTE* data);

#endif	/* PSLAB_NRF_H */
