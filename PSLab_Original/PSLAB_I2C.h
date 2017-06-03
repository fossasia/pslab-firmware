/* 
 * File:   PSLAB_I2C.h
 * Author: padmal
 *
 * Created on June 3, 2017, 7:40 AM
 */

#ifndef PSLAB_I2C_H
#define	PSLAB_I2C_H

extern unsigned int I2C_BRGVAL;
extern unsigned int tmp_int1;
extern unsigned char MULTIFUNC_PORT;

extern void initI2C(void);
extern void I2CStart();
extern void I2CStop();
extern void I2CRestart();
extern void I2CAck();
extern void I2CNak();
extern void I2CWait();
extern void I2CSend(unsigned char dat);
extern unsigned char I2CRead(unsigned char ack);
extern void setMultiFuncPortMode(unsigned char mode);

#endif	/* PSLAB_I2C_H */

