/**
 * @file delay.h
 * @brief Delay functions.
 *
 * This file contains wrapper functions for the compiler built in delay
 * functions. The reason for this is that other files which need to use delays
 * can include just a single file, delay.h, instead of needing to both define
 * FCY and import <libpic30.h>.
 */

#ifndef DELAY_H
#define	DELAY_H

#ifdef	__cplusplus
extern "C" {
#endif

void DELAY_ms(unsigned long d);
void DELAY_us(unsigned long d);

#ifdef	__cplusplus
}
#endif

#endif	/* DELAY_H */

