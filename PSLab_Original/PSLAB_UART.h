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

void initUART(uint16);
bool hasChar();
void sendChar(BYTE val);
void sendInt(uint16 val);
void sendLong(uint16 lsb, uint16 msb);
char getChar();
uint16 getInt();
void ack(BYTE);


void configUART2(uint16 BAUD);
bool hasChar2(void);
char getChar2(void);
uint16 getInt2(void);
void sendAddress2(char address);
void initUART2(void);
void sendChar2(char val);
void sendInt2(uint16 val);
void initUART2_passthrough(uint16);

#endif	/* PSLAB_UART_H */
