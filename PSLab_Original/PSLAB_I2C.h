/* 
 * File:   PSLAB_I2C.h
 * Author: padmal
 *
 * Created on June 1, 2017, 7:41 AM
 */

#ifndef PSLAB_I2C_H
#define	PSLAB_I2C_H

static unsigned int I2C_BRGVAL = 0x272, tmp_int1;

void initI2C(void);
void I2CStart();
void I2CStop();
void I2CRestart();
void I2CAck();
void I2CNak();
void I2CWait();
void I2CSend(unsigned char dat);
unsigned char I2CRead(unsigned char ack);

#endif	/* PSLAB_I2C_H */

