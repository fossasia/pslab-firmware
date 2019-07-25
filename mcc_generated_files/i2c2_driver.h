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

#ifndef __I2C2_DRIVER_H
#define __I2C2_DRIVER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define FCY _XTAL_FREQ

#define INLINE  inline 

/* arbitration interface */
INLINE void i2c2_driver_close(void);

/* Interrupt interfaces */
INLINE void i2c2_enableIRQ(void);
INLINE bool i2c2_IRQisEnabled(void);
INLINE void i2c2_disableIRQ(void);
INLINE void i2c2_clearIRQ(void);
INLINE void i2c2_setIRQ(void);
INLINE void i2c2_waitForEvent(uint16_t*);

/* I2C interfaces */
bool i2c2_driver_driver_open(void);
INLINE char i2c2_driver_getRXData(void);
INLINE void i2c2_driver_TXData(uint8_t);
INLINE void i2c2_driver_resetBus(void);
INLINE void i2c2_driver_start(void);
INLINE void i2c2_driver_restart(void);
INLINE void i2c2_driver_stop(void);
INLINE bool i2c2_driver_isNACK(void);
INLINE void i2c2_driver_startRX(void);
INLINE void i2c2_driver_sendACK(void);
INLINE void i2c2_driver_sendNACK(void);
INLINE void i2c2_driver_clearBusCollision(void);

bool i2c2_driver_initSlaveHardware(void);
INLINE void i2c2_driver_releaseClock(void);
INLINE bool i2c2_driver_isBuferFull(void);
INLINE bool i2c2_driver_isStart(void);
INLINE bool i2c2_driver_isStop(void);
INLINE bool i2c2_driver_isAddress(void);
INLINE bool i2c2_driver_isData(void);
INLINE bool i2c2_driver_isRead(void);
INLINE void i2c2_driver_enableStartIRQ(void);
INLINE void i2c2_driver_disableStartIRQ(void);
INLINE void i2c2_driver_enableStopIRQ(void);
INLINE void i2c2_driver_disableStopIRQ(void);

INLINE void i2c2_driver_setBusCollisionISR(void *f);
INLINE void i2c2_driver_setMasterI2cISR(void *f);
INLINE void i2c2_driver_setSlaveI2cISR(void *f);

#endif // __I2C2_DRIVER_H
