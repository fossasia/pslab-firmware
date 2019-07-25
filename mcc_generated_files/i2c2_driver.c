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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "mcc.h"
#include "i2c2_driver.h"
#include <libpic30.h>

void (*i2c2_driver_busCollisionISR)(void);
void (*i2c2_driver_Masteri2cISR)(void);
void (*i2c2_driver_Slavei2cISR)(void);

inline void i2c2_driver_close(void) {
    I2C2CONbits.I2CEN = 0;
}

/* Interrupt Control */
inline void i2c2_enableIRQ(void) {
    IEC3bits.MI2C2IE = 1;
    IEC3bits.SI2C2IE = 1;
}

inline bool i2c2_IRQisEnabled(void) {
    return IEC3bits.MI2C2IE || IEC3bits.SI2C2IE;
}

inline void i2c2_disableIRQ(void) {
    IEC3bits.MI2C2IE = 0;
    IEC3bits.SI2C2IE = 0;
}

inline void i2c2_clearIRQ(void) {
    IFS3bits.MI2C2IF = 0;
    IFS3bits.SI2C2IF = 0;
}

inline void i2c2_setIRQ(void) {
    IFS3bits.MI2C2IF = 1;
    IFS3bits.SI2C2IF = 1;
}

inline void i2c2_waitForEvent(uint16_t *timeout) {
    //uint16_t to = (timeout!=NULL)?*timeout:100;
    //to <<= 8;
    if ((IFS3bits.MI2C2IF == 0) && (IFS3bits.SI2C2IF == 0)) {
        while (1)// to--)
        {
            if (IFS3bits.MI2C2IF || IFS3bits.SI2C2IF) break;
            __delay_us(100);
        }
    }
}

bool i2c2_driver_driver_open(void) {
    if (!I2C2CONbits.I2CEN) {
        // initialize the hardware
        // STAT Setting 
        I2C2STAT = 0x0;

        // CON Setting
        I2C2CON = 0x8000;

        // Baud Rate Generator Value: I2CBRG 100000;   
        I2C2BRG = 593;

        return true;
    } else
        return false;
}

bool i2c2_driver_initSlaveHardware(void) {
    if (!I2C2CONbits.I2CEN) {


        /* NOTE on SEN:
         * SEN will be set enabling clock stretching.  This is because we don't know how
         * long the user will take to process data bytes in their callbacks.  If they are fast,
         * we may not need to stretch the clock.  If they are slow, we need to stretch the clock.
         * If we ALWAYS stretch the clock, we will release the clock when the ISR is complete.
         */

        /* NOTE on PCIE:
         * PCIE will be set to enable interrupts on STOP.  This will allow us know when
         * the master is finished
         */

        /* NOTE on SCIE:
         * SCIE will be set to enable interrupts on START.  This will allow us to detect
         * both a START and a RESTART event and prepare to restart communications.
         */
        I2C2CONbits.STREN = 1;

        I2C2CONbits.I2CEN = 1;
        return true;
    }
    return false;
}

inline void i2c2_driver_resetBus(void) {

}

inline void i2c2_driver_start(void) {
    I2C2CONbits.SEN = 1;
}

inline void i2c2_driver_restart(void) {
    I2C2CONbits.RSEN = 1;
}

inline void i2c2_driver_stop(void) {
    I2C2CONbits.PEN = 1;
}

inline bool i2c2_driver_isNACK(void) {
    return I2C2STATbits.ACKSTAT;
}

inline void i2c2_driver_startRX(void) {
    I2C2CONbits.RCEN = 1;
}

inline char i2c2_driver_getRXData(void) {
    return I2C2RCV;
}

inline void i2c2_driver_TXData(uint8_t d) {
    I2C2TRN = d;
}

inline void i2c2_driver_sendACK(void) {
    I2C2CONbits.ACKDT = 0;
    I2C2CONbits.ACKEN = 1; // start the ACK/NACK
}

inline void i2c2_driver_sendNACK(void) {
    I2C2CONbits.ACKDT = 1;
    I2C2CONbits.ACKEN = 1; // start the ACK/NACK
}

inline void i2c2_driver_releaseClock(void) {
    I2C2CONbits.SCLREL = 1;
}

inline bool i2c2_driver_isBuferFull(void) {
    return I2C2STATbits.RBF || I2C2STATbits.TBF;
}

inline bool i2c2_driver_isStart(void) {
    return I2C2STATbits.S;
}

inline bool i2c2_driver_isAddress(void) {
    return !I2C2STATbits.D_A;
}

inline bool i2c2_driver_isStop(void) {
    return I2C2STATbits.P;
}

inline bool i2c2_driver_isData(void) {
    return I2C2STATbits.D_A;
}

inline bool i2c2_driver_isRead(void) {
    return I2C2STATbits.R_W;
}

inline void i2c2_driver_clearBusCollision(void) {
    I2C2STATbits.BCL = 0; // clear the bus collision.
}

inline void i2c2_driver_enableStartIRQ(void) {

}

inline void i2c2_driver_disableStartIRQ(void) {

}

inline void i2c2_driver_enableStopIRQ(void) {

}

inline void i2c2_driver_disableStopIRQ(void) {

}

inline void i2c2_driver_setBusCollisionISR(void *f) {
    i2c2_driver_busCollisionISR = f;
}

inline void i2c2_driver_setMasterI2cISR(void *f) {
    i2c2_driver_Masteri2cISR = f;
}

inline void i2c2_driver_setSlaveI2cISR(void *f) {
    i2c2_driver_Slavei2cISR = f;
}

void __attribute__((interrupt, no_auto_psv)) _MI2C2Interrupt(void) {
    (*i2c2_driver_Masteri2cISR)();
}

void __attribute__((interrupt, no_auto_psv)) _SI2C2Interrupt(void) {
    (*i2c2_driver_Slavei2cISR)();
}
