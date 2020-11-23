#include "PSLAB_RTC.h"
#include "PSLAB_I2C.h"
#include "Variables.h"

unsigned char time_array[7] = {0, 0, 0, 0, 0, 0, 0};

/**
 * Updates the time in DS1307 controller given a set of timing parameters
 * 
 * @param seconds Clock Halt (1) | 10 Seconds (3) | Seconds (4)
 * @param minutes XX (1) | 10 Minutes (3) | Minutes (4)
 * @param hours XX (1) | 12/24 | AM/PM | 10h | Hours (4)
 * @param day XX (5) | Day (3)
 * @param date XX (2) | 10 Dates (2) | Dates (4)
 * @param month XX (3) | 10 Months (1) | Month (4)
 * @param year 10 Years (4) | Year (4)
 * @param control_reg OUT (1) | XX (2) | SQWE (1) | XX (2) | RS1:0 (2)
 */
unsigned char setRealTime(
    unsigned char seconds,
    unsigned char minutes,
    unsigned char hours,
    unsigned char day,
    unsigned char date,
    unsigned char month,
    unsigned char year,
    unsigned char control_reg) {
    
    initI2C();
    I2CStart();
    I2CSend(0xD0); // Address DS1307
    I2CSend(0x00); // Call register (Seconds)
    I2CSend(seconds); // 0
    I2CSend(minutes); // 1
    I2CSend(hours); // 2
    I2CSend(day); // 3
    I2CSend(date); // 4
    I2CSend(month); // 5
    I2CSend(year); // 6
    I2CSend(control_reg); // 7
    I2CStop();
    
    return 1;    
}

unsigned char* getRealTime(void) {
    
    I2CStart();
    I2CSend(0xD0);
    I2CSend(0x00);
    I2CRestart();
    I2CSend(0xD1);
    for (loop0 = 0; loop0 < 6; loop0++) {
        time_array[loop0] = I2CRead(1);
    }
    time_array[6] = I2CRead(0);
    I2CStop();
    
    return &(time_array[0]);
}