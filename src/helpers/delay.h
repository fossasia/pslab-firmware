#ifndef DELAY_H
#define	DELAY_H

/**
 * @file delay.h
 * @brief Delay functions.
 *
 * This file contains wrapper functions for the compiler built in delay
 * functions. The reason for this is that other files which need to use delays
 * can include just a single file, delay.h, instead of needing to both define
 * FCY and import <libpic30.h>.
 */

#include "../registers/system/clock.h"

#define FCY (_XTAL_FREQ / 2)

void DELAY_ms(unsigned long d);
void DELAY_us(unsigned long d);

#endif	/* DELAY_H */
