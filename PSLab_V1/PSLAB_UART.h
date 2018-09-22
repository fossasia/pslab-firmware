/* 
 * File:   PSLAB_UART.h
 * Author: padmal
 *
 * Created on June 2, 2017, 11:50 PM
 */

#ifndef PSLAB_UART_H
#define	PSLAB_UART_H

extern BYTE c1;
extern BYTE c2;

extern void initUART(uint16);
extern bool hasChar();
extern void sendChar(BYTE val);
extern void sendInt(uint16 val);
extern void sendLong(uint16 lsb, uint16 msb);
extern char getChar();
extern uint16 getInt();
extern void ack(BYTE);

extern void configUART2(uint16 BAUD);
extern bool hasChar2(void);
extern char getChar2(void);
extern uint16 getInt2(void);
extern void sendAddress2(char address);
extern void initUART2(void);
extern void sendChar2(char val);
extern void sendInt2(uint16 val);
extern void initUART2_passthrough(uint16);

#endif	/* PSLAB_UART_H */
