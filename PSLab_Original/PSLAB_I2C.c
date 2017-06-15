/******************************************************************************/
/******** This file contains I2C control modules of function.c file ***********/
/******************************************************************************/
#include "COMMANDS.h"
#include "PSLAB_I2C.h"
#include "Common_Functions.h"


uint16 I2C_BRGVAL = 0x272;
BYTE MULTIFUNC_PORT = 0;
uint16 tmp_int1 = 0;

void initI2C(void) {

    _TRISB4 = 1; // set SCL and SDA pins as inputs.
    _TRISA8 = 1;
    ODCBbits.ODCB4=1;
    ODCAbits.ODCA8=1;
    CNPUBbits.CNPUB4 = 1;
    CNPUAbits.CNPUA8 = 1;
    I2C2CON=0;
    
    Delay_us(1000);
    I2C2CONbits.I2CEN = 0;
    //I2C bus clock => I2CxBRG = ( Fcy/Fscl - Fcy/10.000.000 ) - 1
    //I2C bus clock => Fscl = 1/( (I2CxBRG+1)/Fcy + (1/10.000.000) )
    //I2C2BRG=0x0092;     //392kHz @ 60MHz // 1/((0x92+1.0)/fcy+1.0/1e7)
    //I2C2BRG=0x00ff;     //229kHz @ 60MHz // 1/((0xff+1.0)/fcy+1.0/1e7)
    I2C2BRG = I2C_BRGVAL;

    I2C2STAT = 0b0000000000000000;
    //Clear BCL: Master Bus Collision Detect bit
    //Clear IWCOL: Write Collision Detect bit
    //Clear I2CPOV: Receive Overflow Flag bit

    I2C2CONbits.DISSLW = 0; //disable slew rate
    I2C2CONbits.I2CEN = 1; //enable. configure SDA, SCL as serial
    Delay_us(1000);

}

void I2CStart() {
    I2C2CONbits.SEN = 1; /* Start condition enabled */
    tmp_int1=1000;
    while (I2C2CONbits.SEN && tmp_int1--)Delay_us(1); /* wait for ack data to send on bus */
    /* wait for start condition to finish */
}

void I2CStop() {
    I2C2CONbits.PEN = 1; /* Stop condition enabled */
    tmp_int1=1000;
    while (I2C2CONbits.PEN && tmp_int1--)Delay_us(1); /* wait for stop cond to finish */

    /* PEN automatically cleared by hardware */
}

void I2CRestart() {
    I2C2CONbits.RSEN = 1; /* Repeated start enabled */
    tmp_int1=1000;
    while (I2C2CONbits.RSEN && tmp_int1--)Delay_us(1); /* wait for condition to finish */

}

void I2CAck() {
    I2C2CONbits.ACKDT = 0; /* Acknowledge data bit, 0 = ACK */
    I2C2CONbits.ACKEN = 1; /* Ack data enabled */

    tmp_int1=1000;
    while (I2C2CONbits.ACKEN && tmp_int1--)Delay_us(1); /* wait for ack data to send on bus */
}

void I2CNak() {
    I2C2CONbits.ACKDT = 1; /* Acknowledge data bit, 1 = NAK */
    I2C2CONbits.ACKEN = 1; /* Ack data enabled */
    tmp_int1=1000;
    while (I2C2CONbits.ACKEN && tmp_int1--)Delay_us(1); /* wait for ack data to send on bus */

}

void I2CWait() {
    tmp_int1=1000;
    while (I2C2STATbits.TBF && tmp_int1--)Delay_us(1);
    /* wait for any pending transfer */
}

void I2CSend(BYTE dat) {
    I2C2TRN = dat; /* Move data to SSPBUF */
    tmp_int1=1000;
    while (I2C2STATbits.TRSTAT && tmp_int1--)Delay_us(1);/* wait till complete data is sent from buffer */

    I2CWait(); /* wait for any pending transfer */
}

BYTE I2CRead(BYTE ack) {
    BYTE retval;
    I2CWait();
    I2C2CONbits.RCEN=1;

    tmp_int1=1000;
    while (I2C2CONbits.RCEN && tmp_int1--)Delay_us(1);
    while ((!I2C2STATbits.RBF) && tmp_int1--)Delay_us(1);

    retval = I2C2RCV;
    if(ack)I2CAck();
    else I2CNak();
    return retval;
    
}

void setMultiFuncPortMode(BYTE mode) {
    if(MULTIFUNC_PORT == mode)return;
    else{
        MULTIFUNC_PORT = mode;
        if(mode == MULTIFUNC_I2C){initI2C();}
        else if(mode == MULTIFUNC_UART){}
        else if(mode == MULTIFUNC_NONSTANDARD){_TRISB4=0;_TRISA8=1;RPINR8bits.IC3R = 0;RPINR8bits.IC4R = 0;}


    }
}
