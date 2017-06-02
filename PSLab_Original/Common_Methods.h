/* 
 * File:   Common_Methods.h
 * Author: padmal
 *
 * Created on May 30, 2017, 9:32 AM
 */

#ifndef COMMON_METHODS_H
#define	COMMON_METHODS_H

#include <xc.h>
#include <p24EP256GP204.h>
#include <libpic30.h>

#define DELAY_105uS asm volatile ("REPEAT, #6721"); Nop(); // 105uS delay

void Delay_us(unsigned int delay);
void Delay_us_by8(unsigned int delay);
void Delay_ms(unsigned int delay);

#endif	/* COMMON_METHODS_H */

