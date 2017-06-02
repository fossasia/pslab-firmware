/* 
 * File:   PSLAB_UART.h
 * Author: padmal
 *
 * Created on May 28, 2017, 7:25 PM
 */

#ifndef PSLAB_UART_H
#define	PSLAB_UART_H

#include <xc.h>
#include "Common_Methods.h"
#include "COMMANDS.h"

void initUART(unsigned int);
bool hasChar();
void sendChar(bool val);
void sendInt(unsigned int val);
void sendLong(unsigned int lsb, unsigned int msb);
char getChar();
unsigned int getInt();
void ack(unsigned char);

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
