/* 
 * File:   Common_Functions.h
 * Author: padmal
 *
 * Created on June 3, 2017, 7:41 AM
 */

#ifndef COMMON_FUNCTIONS_H
#define	COMMON_FUNCTIONS_H

#define ERROR_BUFFLEN 1500
extern char errors[ERROR_BUFFLEN];
extern char *error_readpos, *error_writepos;
extern unsigned int i;

extern void Delay_us(unsigned int);
extern void Delay_us_by8(unsigned int delay);
extern void Delay_ms(unsigned int delay);
extern void logit(char *str);

#endif	/* COMMON_FUNCTIONS_H */

