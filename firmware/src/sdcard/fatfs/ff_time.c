/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/* FAT file timestamps are defined as a 32-bit value with bits corresponding to
   each of the date/time fields:

   YYYYYYYMMMMDDDDDhhhhhmmmmmmsssss

   bits[31-25] = YYYYYYY = Year (number of years since 1980).  Max value allowed
   is 199 (year 2099).
        0b0000000 = 1980
        0b0000001 = 1981
        0b0000010 = 1982
        ...
        0b1110110 = 2098
        0b1110111 = 2099

   bits[24-21] = MMMM = Month (1-12), 0 is not allowed.
        0b0001 = January
        0b0010 = February
        ...
        0b1011 = November
        0b1100 = December
   
   bits[20-16] = DDDDD = Day (1-31), 0 is not allowed.
        0b00001 = 1st of the month
        0b00010 = 2nd
        ...
        0b11110 = 30th
        0b11111 = 31st

   bits[15-11] = hhhhh = hour (0-23)
   bits[10-5] = mmmmmm = minutes (0-59)
   bits[4-0] = sssss = seconds/2 (0-29)
        0b00000 = 0 seconds
        0b00001 = 2 seconds
        ...
        0b11100 = 56 seconds
        0b11101 = 58 seconds
*/

#include "ff.h"
#include <stdint.h>

static DWORD decimalToFatTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t seconds)
{
    DWORD fatTime;

    fatTime = (seconds >> 1);
    fatTime |= ( ((DWORD)minute) << 5 );
    fatTime |= ( ((DWORD)hour) << 11 );
    fatTime |= ( ((DWORD)day) << 16 );
    fatTime |= ( ((DWORD)month) << 21 );
    fatTime |= ( ((DWORD)(year - 1980)) << 25 );

    return fatTime;
}

DWORD get_fattime (void)
{
    return decimalToFatTime(2018, 6, 31, 5, 10, 30);
}