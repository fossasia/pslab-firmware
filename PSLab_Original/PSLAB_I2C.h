/* 
 * File:   PSLAB_I2C.h
 * Author: padmal
 *
 * Created on June 2, 2017, 10:29 PM
 */

#ifndef PSLAB_I2C_H
#define	PSLAB_I2C_H

#include <xc.h>
#include "Common_Methods.h"
#include "COMMANDS.h"

extern unsigned int tmp_int1;
extern unsigned char MULTIFUNC_PORT;
extern unsigned int I2C_BRGVAL;

void initI2C(void);
void I2CStart();
void I2CStop();
void I2CRestart();
void I2CAck();
void I2CNak();
void I2CWait();
void I2CSend(BYTE dat);
BYTE I2CRead(BYTE ack);
void setMultiFuncPortMode(BYTE mode);

#endif	/* PSLAB_I2C_H */

