/* 
 * File:   PSLAB_RTC.h
 * Author: Dilushi
 *
 * Created on April 12, 2019, 7:38 PM
 */

#ifndef PSLAB_RTC_H
#define	PSLAB_RTC_H

#include "COMMANDS.h"

#define DS1307_ADDRESS 0x68

bool beginRTC(void);
uint8_t isrunning(void);
void adjust(uint16_t year, uint8_t month, uint8_t day,
        uint8_t hour, uint8_t minute, uint8_t seconds);
void getTime(void);

#endif	/* PSLAB_RTC_H */

