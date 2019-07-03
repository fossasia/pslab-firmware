/******************************************************************************/
/********** This file contains RTC control functions and commands *************/
/******************************************************************************/
#include "COMMANDS.h"
#include "PSLAB_RTC.h"
#include "PSLAB_I2C.h"
#include "PSLAB_UART.h"
#include "Common_Functions.h"

/*******************************************************************************
 * Supportive functions for Binary Coded Decimal operations
 ******************************************************************************/
static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }

/**
 * Initiates DS1307 IC with I2C connection
 * @return status of initiation
 */
bool beginRTC(void) {
    initI2C();
    return TRUE;
}

/**
 * Checks if RTC has time set already
 * @return status of time in RTC
 */
uint8_t isrunning(void) {
    I2CStart();
    // Set I2C Slave
    I2CSend(DS1307_ADDRESS << 1);
    // Set I2C slave's address
    I2CSend(DS1307_ADDRESS);
    I2CRestart();
    I2CSend(DS1307_ADDRESS << 1 | 1);
    uint8_t stat = I2CRead(1);
    I2CRead(0);
    I2CStop();
    return !(stat >> 7);
}

/**
 * Sets initial time
 */
void adjust(uint16_t year, uint8_t month, uint8_t day,
        uint8_t hour, uint8_t minute, uint8_t seconds) {
    I2CStart();
    // Set I2C Slave
    I2CSend(DS1307_ADDRESS << 1);
    I2CSend(0);
    I2CSend(bin2bcd(seconds));
    I2CSend(bin2bcd(minute));
    I2CSend(bin2bcd(hour));
    I2CSend(bin2bcd(0));
    I2CSend(bin2bcd(day));
    I2CSend(bin2bcd(month));
    I2CSend(bin2bcd(year - 2000));
    I2CStop();
}

void getTime() {
    I2CStart();
    // Set I2C Slave
    I2CSend(DS1307_ADDRESS << 1);
    // Set I2C slave's address
    I2CSend(DS1307_ADDRESS);
    I2CRestart();
    I2CSend(DS1307_ADDRESS << 1 | 1);
    for (i = 0; i < 7; i++) sendChar(bcd2bin(I2CRead(1)));
    I2CRead(0);
    I2CStop();
}
