//PSLab  Firmware
//Author  : Jithin B.P, jithinbp@gmail.com
//License : Apache
//Date : 30-March-2017



#include "xc.h"
#include"functions.h"




void read_all_from_flash(_prog_addressT pointer) {
    unsigned int bytes_to_read = _FLASH_ROW * 16; // _FLASH_ROW*8 integers = twice as many bytes
    _prog_addressT p1, p2;
    p1 = pointer;
    for (i = 0; i < bytes_to_read / 2; i++)dest[i] = 0; //clear buffer
    p2 = _memcpy_p2d16(&dest, pointer, bytes_to_read);
    Nop();
    Nop();

}

void load_to_flash(_prog_addressT pointer, BYTE location, unsigned int * blk) {
    /*Write to locations of 16bytes each (store 8 integers as a string, or 16 BYTES ...)*/
    char bytes_to_write = 16;
    _prog_addressT p;
    //row_p = pointer;
    /*The storage architecture only allows erasing a whole page(1024) at a time. So we must make
     a copy of the data in the RAM, change the locations we need to access, and write the whole page back*/

    /*------fetch a copy of the rows into RAM ( &DEST )------*/
    read_all_from_flash(pointer);

    /*-----------fetch bytes_to_write characters----------*/
    for (i = 0; i < bytes_to_write / 2; i++) {
        dest[location * 8 + i] = blk[i];
    }

    /*------write the copy back into the FLASH MEMORY------*/
    unsigned int dat1, dat2;
    _erase_flash(pointer); /* erase a page */
    for (i = 0; i < _FLASH_ROW * 4; i += 1) /*combine two ints each for each word32 write*/ {
        dat1 = dest[2 * i];
        dat2 = dest[2 * i + 1];
        p = pointer + (4 * i);
        _write_flash_word32(p, dat1, dat2);
    }


}

void read_flash(_prog_addressT pointer, BYTE location) {
    read_all_from_flash(pointer);

    for (i = 0; i < 8; i++) {
        blk[i] = dest[location * 8 + i];
        /*while (U1STAbits.UTXBF); //wait for transmit buffer empty
        U1TXREG = dest[location * 8 + i]&0xff;
        while (U1STAbits.UTXBF); //wait for transmit buffer empty
        U1TXREG = (dest[location * 8 + i] >> 8)&0xff;
*/
    }
}

void init() {

    /*Switching clock to external crystal, and enabling PLL*/
    PLLFBD = 62; // prescale by a factor of 64
    CLKDIVbits.PLLPOST = 0; // postscale by 2
    CLKDIVbits.PLLPRE = 1; //divide by 3

    // Initiate Clock Switch to Primary Oscillator with PLL (NOSC=0b011)
    __builtin_write_OSCCONH(0x03); 
    __builtin_write_OSCCONL(OSCCON | 0x01);
    while (OSCCONbits.COSC != 0b011); // Wait for Clock switch to occur
    while (OSCCONbits.LOCK != 1); //// Wait for PLL to lock
    /*----Clock switching complete. Fosc=128MHz . Fcy = 64MHz------*/
    

    PTGCONbits.PTGWDT = 0; //disable peripheral trigger generator watchdog timer

    //change to I2C DAC
    CCS_TRIS = 0; CCS_OUT=1; //1 means off. 0 is on.
    
    //Digital outputs . Define SQR1 to SQR4
    SQR1_TRIS = 0; //SQR1 RP41
    OD1_TRIS = 0; //OD1  RP54
    PDC1_TRIS = 0; //PDC1 pwm dac
    PDC2_TRIS = 0; //PDC2 pwm dac
    SQR2_TRIS = 0; //SQ2


    // digital inputs. Define ID1 to ID4
    _TRISB2=1; _ANSB2=0;           //SET B2 as an input pin in digital mode . ID1

    
    //ADC inputs. 4 channels
    _TRISA0 = 1; _TRISA1 = 1;
    _TRISB1 = 1;_TRISB0=1;

    _TRISB15 = 1; _CNPUB15 = 0; _CNPDB15 = 0;       //SET B15 as an input pin . Charging capacitors in constant voltage mode

    //------------Initialize SPI pins for PGAs -------------------------
    
    _TRISA10 = 0;    _LATA10 = 1;                //CS-1 kept high.
    _TRISA7 = 0;     _LATA7 = 1;                 //CS-2 kept high.

    LED_TRIS = 0; LED_ANS = 0; LED_OUT=1;                   //LED_OUT

    _TRISA4 = 0; //SDO1
    _ANSA4 = 0; //make A4 digital out
    _TRISA9 = 1; //SDI
    _TRISC3 = 0; //SCK1 output

    //-------CTMU input PIN RP35 (C1IN1-)--------
    // Output pin for constant voltage mode capacitance measurement
    _TRISB3=1; _ANSB3=1;           //SET B3 as an input pin in analog mode . CTMU
    _TRISC0=1; _ANSC0=1;           //CCS monitoring
    _TRISC1=1; _ANSC1=1;           //SEN monitoring


     

    //initI2C();
    //initSPI();
    //initDAC();


    
}

void setFlashPointer(BYTE location){
    if(location<20){
        _init_prog_address(p, dat1[0]);
        p+=0x800*location;
    }

}

/*-----------------------DAC output---------------*/
void initDAC() {
   //setup I2C DAC
}


void initUART(unsigned int BAUD) {
    /*---------UART------------*/
    TRISBbits.TRISB8 = 1; // B8 set as input(RX). connected to TX of MCP2200
    ANSELBbits.ANSB8 = 0; // set B8 as digital input.
    TRISBbits.TRISB7 = 0; // set as output. connected to RX of MCP2200

    RPOR2bits.RP39R = 0x01; //Map B7(RP39) to UART TX
    RPINR18bits.U1RXR = 0x28; //Map B8(RP40) to UART1 RX

    U1MODEbits.STSEL = 0; //1 stop bit
    U1MODEbits.PDSEL = 0; //no parity, 8-data bits
    U1MODEbits.ABAUD = 0; //disable auto-baud
    U1MODEbits.BRGH = 1; //high speed mode
    U1BRG = BAUD;
    U1MODEbits.UEN = 0;
    U1MODEbits.RTSMD = 1;

    U1STAbits.URXISEL = 0; //interrupt on 1 char recv

    //IEC0bits.U1TXIE = 1; //enable TX interrupt

    U1MODEbits.UARTEN = 1; //enable UART
    U1STAbits.UTXEN = 1; //enable UART TX

    U1MODEbits.URXINV = 0;

    DELAY_105uS
    while(hasChar())getChar(); //clear buffer

}

void Delay_us(unsigned int delay) {
    unsigned int i;
    for ( i = 0; i < delay; i++) {
        __asm__ volatile ("repeat #63");
        __asm__ volatile ("nop");
    }
}

void Delay_ms(unsigned int delay) {
    unsigned int i,i2;
    for ( i2 = 0; i2 < delay; i2++){
        for ( i = 0; i < 860; i++) {
            __asm__ volatile ("repeat #63");
            __asm__ volatile ("nop");
        }
    __asm__ volatile ("CLRWDT");
    }
}


bool hasChar() {
    return U1STAbits.URXDA;
}

void sendChar(BYTE val) {
    while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = val;
}

void sendInt(unsigned int val) {
    while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = val & 0xff;
    while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = (val >> 8)&0xff;
}

void sendLong(unsigned int lsb,unsigned int msb) {
    while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = lsb & 0xff;
    while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = (lsb >> 8)&0xff;
    while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = msb & 0xff;
    while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = (msb >> 8)&0xff;
}

void ack(BYTE response) {
    while (U1STAbits.UTXBF); //wait for transmit buffer empty
    U1TXREG = response;
}

char getChar() {
    while (!hasChar());
    if (U1STAbits.FERR == 1) {
        U1RXREG;
        U1STAbits.OERR = 0;
        return 0;
    }
    return U1RXREG;
}

unsigned int getInt() {
    c1 = getChar()&0xFF;
    c2 = getChar()&0xFF;
    return (c2 << 8) | c1;
}

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
    I2C2BRG = I2C_BRGVAL;

    I2C2STAT = 0b0000000000000000;

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

BYTE I2CRead(BYTE ack){
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



void setSPIMode(BYTE mode){
    if(SPI_MODE == mode)return;
    else{
        SPI_MODE = mode;
        //initSPI();
    }
}

