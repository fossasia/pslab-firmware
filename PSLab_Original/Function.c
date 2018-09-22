#include "COMMANDS.h"
#include "Common_Functions.h"
#include "Function.h"
#include "PSLAB_ADC.h"
#include "PSLAB_SPI.h"
#include "Measurements.h"

int *endbuff;
int *buffpointer, *endpointer, dma_channel_length, I2CSamples;
BYTE *bytebuff1, *bytebuff2;
//__eds__ unsigned int ADCbuffer[BUFFER_SIZE] __attribute__((space(eds))); 
int dma_channel_length = 10, I2CSamples = 0;
__prog__ unsigned int __attribute__((section("CALIBS"), space(prog), aligned(_FLASH_PAGE * 2))) dat1[15][_FLASH_PAGE];
unsigned int dest[_FLASH_ROW * 8];
unsigned int blk[8];

int ulsb, umsb;

/*------UART VARIABLES-----*/
unsigned int TCD = 1000;
unsigned int lsb;
_prog_addressT p, pProg;
/*------LOGIC ANALYZER VARIABLES-----*/
BYTE INITIAL_DIGITAL_STATES_ERR = 0;
BYTE DIGITAL_TRIGGER_CHANNEL = 32, DIGITAL_TRIGGER_STATE = 0, b1, b2;
BYTE COMPARATOR_CONFIG = 7 | (3 << 4), I2CConvDone = 1;

/*------LOGIC ANALYZER VARIABLES-----*/
unsigned int lsb, msb, tmp_int2, tmp_int3, tmp_int4, tmp_int5, tmp_int6;

unsigned int LAFinished = 1;
unsigned int I2CTotalSamples = BUFFER_SIZE;

/*-----OSCILLOSCOPE VARIABLES-------*/
BYTE SH = 5, ICG = 15, I2C_TRIGGER_CHANNEL = 0, I2C_TRIGGERED = 0;
BYTE I2C_TRIGGER_READY = 0, I2C_SCOPE_LOCATION = 0x00, I2C_SCOPE_ADDRESS = 0x00;
BYTE I2C_SCOPE_BYTES = 0;

BYTE frequency_scaling = 1, frequency_ready = FALSE;

/*--------Stepper Motor--------*/
BYTE motor_phases[] = {12, 6, 3, 9}, current_motor_phase = 0;

BYTE chan = 1;
long addr_count = 0xAAAA01;

unsigned int LASamples;
unsigned long val, l1, l2;

/*-----OSCILLOSCOPE VARIABLES-------*/
unsigned int freq_lsb, freq_msb, freq2_lsb, freq2_msb;

BYTE data[32];
BYTE cb, cc;

void __attribute__((__interrupt__, no_auto_psv)) _DMA0Interrupt(void) {
    IC1CON2bits.TRIGSTAT = 0;
    IC1CON1bits.ICM = 0;
    DMA0CONbits.CHEN = 0;
    _DMA0IF = 0;
    _DMA0IE = 0; // Clear the DMA0 Interrupt Flag
    if (DMA_MODE == DMA_LA_TWO_CHAN || DMA_MODE == DMA_LA_ONE_CHAN) {
        IC2CON1bits.ICM = 0;
        DMA1CONbits.CHEN = 0;
        _DMA1IF = 0;
        _DMA1IE = 0;
    }//disable DMA1 too
}

void __attribute__((__interrupt__, no_auto_psv)) _DMA1Interrupt(void) {
    IC2CON2bits.TRIGSTAT = 0;
    IC2CON1bits.ICM = 0;
    DMA1CONbits.CHEN = 0;
    _DMA1IF = 0;
    _DMA1IE = 0; // Clear the DMA0 Interrupt Flag
}

void __attribute__((__interrupt__, no_auto_psv)) _DMA2Interrupt(void) {
    IC3CON2bits.TRIGSTAT = 0;
    IC3CON1bits.ICM = 0;
    _DMA2IF = 0; // Clear the DMA0 Interrupt Flag
}

void __attribute__((__interrupt__, no_auto_psv)) _DMA3Interrupt(void) {
    IC4CON2bits.TRIGSTAT = 0;
    IC4CON1bits.ICM = 0;
    _DMA3IF = 0; // Clear the DMA0 Interrupt Flag
}

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void) // For four channel Logic analyzer
{
    if ((((PORTB >> 10) & DIGITAL_TRIGGER_CHANNEL) > 0) == DIGITAL_TRIGGER_STATE) {
        _CNIEB10 = 0;
        _CNIEB11 = 0;
        _CNIEB12 = 0;
        _CNIEB13 = 0;
        _CNIF = 0;
        _CNIE = 0;
        INITIAL_DIGITAL_STATES = ((PORTB >> 10)&0xF) | (_C4OUT << 4);

        IC1CON2bits.TRIGSTAT = 1;
        IC2CON2bits.TRIGSTAT = 1;
        IC3CON2bits.TRIGSTAT = 1;
        IC4CON2bits.TRIGSTAT = 1;
        T2CONbits.TON = 1;
        INITIAL_DIGITAL_STATES_ERR = ((PORTB >> 10)&0xF) | (_C4OUT << 4); //=(PORTB>>10)&0xF;


    }
    _CNIF = 0;
    // Clear CN interrupt
}

void __attribute__((__interrupt__, no_auto_psv)) _INT2Interrupt(void) {
    IC1CON1bits.ICM = LAM1;
    IC2CON1bits.ICM = LAM2;
    IC3CON1bits.ICM = LAM3;
    IC4CON1bits.ICM = LAM4;
    INITIAL_DIGITAL_STATES = ((PORTB >> 10)&0xF) | (_C4OUT << 4); //INITIAL_DIGITAL_STATES =(PORTB>>10)&0xF;
    IC1CON2bits.TRIGSTAT = 1;
    IC3CON2bits.TRIGSTAT = 1;
    IC2CON2bits.TRIGSTAT = 1;
    IC4CON2bits.TRIGSTAT = 1;
    INITIAL_DIGITAL_STATES_ERR = ((PORTB >> 10)&0xF) | (_C4OUT << 4); // =(PORTB>>10)&0xF;

    _INT2IF = 0;
    _INT2IE = 0;

}

void __attribute__((__interrupt__, no_auto_psv)) _IC4Interrupt(void) {
    INITIAL_DIGITAL_STATES = ((PORTB >> 10)&0xF) | (_C4OUT << 4); //INITIAL_DIGITAL_STATES =(PORTB>>10)&0xF;
    IC4CON1bits.ICM = 0; //Disable IC4 interrupt
    _IC4IF = 0;
    _IC4IE = 0; //disable input capture interrupt
    INITIAL_DIGITAL_STATES_ERR = ((PORTB >> 10)&0xF) | (_C4OUT << 4); //=(PORTB>>10)&0xF;
}

void set_CS(BYTE channel, BYTE status) {

    if (channel == CSNUM_A1)CSCH1 = status;
    else if (channel == CSNUM_A2)CSCH2 = status;
    else if (channel == CSNUM_CS1)CS1 = status;
    else if (channel == CSNUM_CS2)CS2 = status;
    else if (channel == CSNUM_OUT1)CSOUT1 = status;
    else if (channel == CSNUM_OUT2)CSOUT2 = status;

}

void setPGA(char PGAnum, char gain) {
    set_CS(PGAnum, 0);
    spi_write16(0x4000 | gain);
    set_CS(PGAnum, 1);
}

void setSensorChannel(char channel) {
    //  Sensor inputs are located on PGA 5
    set_CS(CSNUM_A1, 0);
    spi_write16(0x4100 | channel);
    set_CS(CSNUM_A1, 1);
}

void delayTMR4(int n) {
    _T4IF = 0;
    PR4 = n;
    while (!_T4IF);
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
    //RCONbits.SWDTEN=0;  //disable software watchdog

    //SQR outputs(2)//Digital outputs
    _TRISC6 = 0;
    _TRISC7 = 0;
    _TRISC8 = 0;
    _TRISC9 = 0;


    //ADC inputs.
    _TRISA0 = 1;
    _TRISA1 = 1;
    _TRISB1 = 1;
    _TRISB0 = 1;
    _TRISC1 = 1;
    _TRISC2 = 1;



    _TRISB10 = 1; //ID1
    _TRISB11 = 1; //ID2
    _TRISB12 = 1; //ID3
    _TRISB13 = 1; //ID4
    _TRISB14 = 1; //Frequency input
    _TRISB9 = 1; //Expansion slot digital input

    _TRISB2 = 1;
    _ANSB2 = 0; //RB2/ NRF interrupt AN4
    _INT1IF = 0;
    _INT1IE = 0; //disable this interrupt unless something comes up
    //------------Initialize SPI pins for DAC, PGAs -------------------------

    _TRISA10 = 0;
    _LATA10 = 1; //CS-1 kept high.
    _TRISA7 = 0;
    _LATA7 = 1; //CS-2 kept high.

    _TRISB5 = 0;
    _LATB5 = 1; //CSOUT-1
    _TRISB6 = 0;
    _LATB6 = 1; //CSOUT-2  
    _TRISB15 = 0;
    _LATB15 = 0; // LEDPIN

    _TRISC4 = 0;
    _LATC4 = 1; //CS1 kept high.
    _TRISC5 = 0;
    _LATC5 = 1; //CS2 kept high.


    _TRISA4 = 0; //SDO1
    _ANSA4 = 0; //make A4 digital out
    _TRISC3 = 0; //SCK1 output
    _TRISA9 = 1; //SDI input (RA9)

    setSPIMode(SPI_16);
    //-------CTMU input PIN RP35 (C1IN1-)--------
    // Output pin for constant voltage mode capacitance measurement
    _TRISC0 = 1;
    _ANSC0 = 0;
    _LATC0 = 0;
    _TRISB3 = 1;
    _ANSB3 = 1; //SET B3 as an input pin in analog mode

    _U1RXIF = 0;
    _U1RXIE = 0; //disable receive interrupt for uart1
    _U2RXIF = 0;
    _U2RXIE = 0; //disable receive interrupt for uart2

    disableCTMUSource();
    configureADC();

}

void setFlashPointer(BYTE location) {
    if (location < 20) {
        _init_prog_address(p, dat1[0]);
        p += 0x800 * location;
    }

}

void mapReferenceOscillator(BYTE channel, BYTE scaler) {
    /*--------------External clock monitoring, RP55/RC7  ----------*/
    REFOCONbits.ROON = 0;
    REFOCONbits.ROSEL = 0;
    REFOCONbits.RODIV = scaler;

    if (channel & 1)RPOR5bits.RP54R = 0x31;
    if (channel & 2)RPOR5bits.RP55R = 0x31;
    if (channel & 4)RPOR6bits.RP56R = 0x31;
    if (channel & 8)RPOR6bits.RP57R = 0x31;
    if (channel & 16) {
        _LATB9 = 0; //set B9 as output. 
        RPOR3bits.RP41R = 0x31;
    }
    REFOCONbits.ROON = 1;
}

void preciseDelay(int t) {
    T5CONbits.TON = 0;
    T5CONbits.TCKPS = 2;
    PR5 = t - 1;
    TMR5 = 0x0000;
    _T5IF = 0;
    T5CONbits.TON = 1;
    while (!_T5IF);
    T5CONbits.TON = 0;

}

void PrepareTrigger(void) {
    TRIGGER_WAITING = 0;
    TRIGGER_READY = 0;
    TRIGGERED = 0;
}

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
