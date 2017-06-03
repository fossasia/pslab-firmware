/* 
 * File:   PSLAB_UART.h
 * Author: padmal
 *
 * Created on June 2, 2017, 11:50 PM
 */

#ifndef PSLAB_UART_H
#define	PSLAB_UART_H

#define DELAY_105uS asm volatile ("REPEAT, #6721"); Nop(); // 105uS delay

extern unsigned char c1;
extern unsigned char c2;

void initUART(unsigned int);
bool hasChar();
void sendChar(BYTE val);
void sendInt(unsigned int val);
void sendLong(unsigned int lsb, unsigned int msb);
char getChar();
unsigned int getInt();
void ack(BYTE);


void configUART2(unsigned int BAUD);
bool hasChar2(void);
char getChar2(void);
unsigned int getInt2(void);
void sendAddress2(char address);
void initUART2(void);
void sendChar2(char val);
void sendInt2(unsigned int val);
void initUART2_passthrough(unsigned int);

#endif	/* PSLAB_UART_H */
