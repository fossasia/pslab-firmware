//PSLab  Firmware
//Author  : Jithin B.P, jithinbp@gmail.com
//License : Apache
//Date : 30-March-2017

#pragma config ICS = NONE               // ICD Communication Channel Select bits (Reserved, do not use)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

//Contains all byte header definitions
#include"commands.h"

#include<xc.h>
#include <p24EP256GP204.h>
// Used for flash R/W functions. For storing calibration constants etc
#include <stdlib.h>
#include<libpic30.h>

//Contains all function declarations for accessing the hardware
#include"functions.h"


const BYTE version[] = "PSLab-2.0"; //Python communication library will check first few letters to confirm

/* PLL using external oscillator. */
_FOSCSEL(FNOSC_FRC & IESO_OFF); //Start up using internal oscillator
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT ); // enable failsafe osc, use external crystal

// Why do we need a watchdog timer?
// Watchdog timers are free running timers that automatically reset the processor if left untouched
// until they reach a preset count. They need to be intermittently cleared. The following code
// Sets a timeout of around 1.5 Seconds. This is used to prevent CPU freeze up in unexpected scenarios

#pragma config WDTPOST = PS512          // Watchdog Timer Postscaler bits (1:512)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF              // Watchdog Timer Enable bit (Watchdog timer always enabled)

#pragma config ALTI2C2 = OFF            // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)

int main() {
    LED_OUT=0;
    RCONbits.SWDTEN=1; // Enable Watchdog
    // The comm lib will send two bytes (Main group, sub group) for all commands. A success/failure response is returned by firmware.
    unsigned char main_command,sub_command,RESPONSE;
    
    unsigned int i = 0, n = 0;
    unsigned int ADLOC;
    pProg=0x0; //initialize the flash writing pointer
    unsigned int *pData;
    
    // Initialize everything from pin mappings to I2C & SPI ports.
    init(); 
    initUART(BRGVAL1000000);

    LED_OUT=1;
    /*
     State machine starts here
     -Wait for data
     -First byte received defines a broad category such as 'ADC' or 'TIMING MEASUREMENTS' [main_command]
     -Second byte received(sub command) determines the specific function to execute within the chosen category
     -  All byte values and their meanings are defined in commands.h
     Once the function call is chosen, the device will read further bytes(arguments) relevant to the chosen function.*/
    while (1) {
        while (!hasChar()){asm("CLRWDT");}  // A watchdog timer prevents the device from freezing up in case the master disconnects midway
        main_command = getChar();
        sub_command = getChar();
        RESPONSE = SUCCESS; //Assume success. This value is modified later on if necessary
        switch (main_command) {
            case FLASH:     // category pertaining to flash memory read and write
                switch(sub_command){
                    case READ_BULK_FLASH: //read an entire page
                        lsb = getInt();
                        location = getChar();
                        setFlashPointer(location);
                        read_all_from_flash(p);
                        for (i = 0; i < lsb/2; i++) {
                            sendInt(dest[i]);
                        }
                        break;

                    case WRITE_BULK_FLASH: //write an entire page
                        lsb = getInt();
                        location = getChar();
                        for (i = 0; i < lsb/2; i += 1)  {
                            dest[i]=getInt();
                        }
                        setFlashPointer(location);
                        _erase_flash(p); /* erase a page */
                        for (i = 0; i < _FLASH_ROW * 4; i += 1) /*combine two ints each for each word32 write*/ {
                            tmp_int1 = dest[2 * i];
                            tmp_int2 = dest[2 * i + 1];
                            pProg = p + (4 * i);
                            _write_flash_word32(pProg, tmp_int1, tmp_int2); Nop(); Nop();
                        }
                        
                        break;
                }
                break;

            case ADC:
                switch(sub_command){
                    case CAPTURE_12BIT: //12-bit oscilloscope. Single channel
                        break;

                    case CAPTURE_ONE:  //Capture 10 bit ADC readings from 1 channel
                        break;

                    case CAPTURE_TWO:  //Capture 10 bit ADC readings from 2 channels (Simultaneous)
                        break;

                    case CAPTURE_FOUR:  //Capture 10 bit ADC readings from 4 channels (Simultaneous)
                        break;
                        
                        
                    case CONFIGURE_TRIGGER:
                        break;

                    case GET_CAPTURE_STATUS:
                        break;

                    case SET_PGA_GAIN:
                        break;

                    case SELECT_PGA_CHANNEL:
                        break;

                    case GET_VOLTAGE_SUMMED:
                        location = getChar();
                        //i=get_voltage_summed(location);
                        //sendInt(i);
                        //sendInt(ADC1BUF0);sendInt(ADC1BUF1);sendInt(ADC1BUF2);sendInt(ADC1BUF3);sendInt(ADC1BUF4);sendInt(ADC1BUF5);sendInt(ADC1BUF6);sendInt(ADC1BUF7);
                        //sendInt(ADC1BUF8);sendInt(ADC1BUF9);sendInt(ADC1BUFA);sendInt(ADC1BUFB);sendInt(ADC1BUFC);sendInt(ADC1BUFD);sendInt(ADC1BUFE);sendInt(ADC1BUFF);
                        break;


                    case GET_CAPTURE_CHANNEL:
                        //CAP_CHARGE_OUT=0;CAP_CHARGE_TRIS=1; //set 20K CAP voltage source to high impedance mode.
                        value = getChar();      //channel number
                        lsb = getInt();   //number of bytes
                        msb = getInt();           //offset / starting position
                        for (i = msb; i < msb+lsb; i++) sendInt(ADCbuffer[i+samples_to_fetch*value]);
                        break;


                }
                break;
            case I2C:
                switch(sub_command){
                    case I2C_START:             //Initialize I2C and select device address
                        I2CStart();
                        location = getChar(); //=address<<1 | R/W   [r=1,w=0]
                        I2CSend(location);
                        RESPONSE|=(I2C2STATbits.ACKSTAT<<4)|(I2C2STATbits.BCL<<5);
                        break;

                    case I2C_STOP:
                        I2CStop();              // send stop condition as transfer finishes
                        break;

                    case I2C_WAIT:
                        I2CWait();              // send stop condition as transfer finishes
                        break;

                    case I2C_SEND:
                        value = getChar();
                        I2CSend(value);
                        RESPONSE|=(I2C2STATbits.ACKSTAT<<4)|(I2C2STATbits.BCL<<5);
                        break;

                    case I2C_SEND_BURST:
                        value = getChar();
                        I2CSend(value);
                        RESPONSE=DO_NOT_BOTHER;
                        break;

                    case I2C_RESTART:
                        I2CRestart();
                        location = getChar(); //=address<<1 | R/W   [r=1,w=0]
                        I2CSend(location);
                        RESPONSE|=(I2C2STATbits.ACKSTAT<<4)|(I2C2STATbits.BCL<<5);
                        break;

                    case I2C_READ_MORE:
                        location = I2CRead(1);
                        sendChar(location);
                        break;
                    case I2C_READ_END:
                        location = I2CRead(0);
                        sendChar(location);
                        break;

                    case I2C_CONFIG:
                        I2C_BRGVAL = getInt();
                        initI2C();
                        break;
                        
                    case I2C_STATUS:
                        sendInt(I2C2STAT);
                        break;

                    case I2C_READ_BULK:
                        location=getChar();
                        ca=getChar();
                        value=getChar();
                        I2CStart();
                        I2CSend(location<<1); //Address of I2C slave. write.
                        I2CSend(ca); //I2C slave Memory Address to read from
                        I2CRestart();
                        I2CSend((location<<1)|1); //Address , read
                        for(i=0;i<value-1;i++)sendChar(I2CRead(1));
                        sendChar(I2CRead(0));
                        I2CStop();
                        break;

                    case I2C_WRITE_BULK:
                        location=getChar();
                        value=getChar();
                        I2CStart();
                        I2CSend(location<<1);
                        for(i=0;i<value;i++)I2CSend(getChar());
                        I2CStop();
                        break;

                    case I2C_ENABLE_SMBUS:
                        I2C2STAT = 0x0000;
                        I2C2CONbits.SMEN=1;
                        I2C2CONbits.I2CEN = 1;
                        break;

                    case I2C_DISABLE_SMBUS:
                        I2C2STAT = 0x0000;
                        I2C2CONbits.SMEN=0;
                        I2C2CONbits.I2CEN = 1;
                        break;

                    case I2C_INIT:
                        initI2C();
                        break;

                    case PULLDOWN_SCL:
                        lsb=getInt();
                        _TRISB4 = 0;
                        _LATB4=0;
                        Delay_us(lsb);
                        _LATB4=1;
                        _TRISB4 = 1;
                        break;


                    case I2C_START_SCOPE:

                        break;


                }
                break;
            case DAC:
                switch(sub_command){

                case SET_DAC:
                    lsb = getInt();
                    //Write to I2C DAC
                    break;
                }
                break;
            case WAVEGEN:
                switch(sub_command){
                    case SET_SINE1:
                        value = getChar();
                        lsb = getInt();
                        //sineWave1(lsb,value);
                        break;

                        
                    case LOAD_WAVEFORM1:
                        for(lsb=0;lsb<WAVE_TABLE_FULL_LENGTH;lsb++)sineTable1[lsb]=getInt();
                        for(lsb=0;lsb<WAVE_TABLE_SHORT_LENGTH;lsb++)sineTable1_short[lsb]=getChar();                            
                        break;

                    case SET_SQR1:  //on OD1
                        lsb = getInt(); //wavelength
                        msb = getInt(); //high time
                        value = getChar(); //prescaler

                        //if(value&0x8){sqrs(lsb,msb,value&0x3);}
                        //else if(value&0x4){sqr2(lsb,msb,value&0x3);}
                        //else{sqr1(lsb,msb,value&0x3);}
                        break;

                }
                break;
            case DOUT:
                switch(sub_command){
                    case SET_STATE:
                        value = getChar();
                        //Set DOUT pin states
                        break;                        
                        
                }
                break;
            case DIN:
                switch(sub_command){
                    case GET_STATE:
                        //['ID1','SQR1_READ','OD1_READ','SEN','SQR1','OD1','SQ2','SQ3']
                        //EnableComparator();
                        //sendChar(ID1_READ|(SQR1_READ<<1)|(OD1_READ<<2)|(COMP4_READ<<3)|(SQR1_IN<<4)|(OD1_IN<<5)|(CCS_IN<<6)|(SQR2_IN<<7));
                        RESPONSE = DO_NOT_BOTHER; //No need to acknowledge
                        break;
                }
                break;
            case TIMING:
                switch(sub_command){
                    case GET_TIMING:        //Using input capture
                    break;


                    case TIMING_MEASUREMENTS:        //Using two input captures
                    break;

                    //not being used.
                    case INTERVAL_MEASUREMENTS:        //Using two input captures

                    break;


                    case SINGLE_PIN_EDGES:

                    break;
                    
                    case DOUBLE_PIN_EDGES:

                    break;
                    
 

                }
                break;
            case COMMON:
                switch(sub_command){
                    case GET_CTMU_VOLTAGE:

                    break;

                    case GET_CAP_RANGE:

                    break;

                    case START_CTMU:

                        break;

                    case STOP_CTMU:
                       break;
                    
                    
                    case GET_CAPACITANCE:

                    break;


                    case GET_HIGH_FREQUENCY:        //This one shares TIMER5 with the ADC! and uses TMR2,3 in cascaded mode

                        break;

                    case GET_ALTERNATE_HIGH_FREQUENCY:   //Discontinued.

                        break;

                    case GET_FREQUENCY:        //Using input capture
                        break;

                    case GET_VERSION:
                        for (i = 0; i < sizeof(version)-1; i++) sendChar(version[i]);
                        value = 0;
                        #ifdef NRF_ENABLED
                        value|=1;
                        #endif

                        #ifdef OLED_ENABLED
                        value|=2;
                        #endif

                        #ifdef HX711_ENABLED
                        value|=4;
                        #endif

                        #ifdef HCSR04_ENABLED
                        value|=8;
                        #endif
                                
                        sendChar(value);
                        sendChar('\n');
                        RESPONSE = DO_NOT_BOTHER;
                        break;

                    case RETRIEVE_BUFFER:
                        lsb = getInt();   //starting point
                        msb = getInt();   //number of bytes
                        for (i = lsb; i < msb+lsb; i++) sendInt(ADCbuffer[i]);
                        LED_OUT=1;
                        break;
                    case CLEAR_BUFFER:
                        lsb = getInt();   //starting point
                        msb = getInt();   //number of bytes
                        for (i = lsb; i < msb+lsb; i++) ADCbuffer[i]=0;
                        break;

                    case FILL_BUFFER:
                        lsb = getInt();   //starting point
                        msb = getInt();   //number of bytes
                        for (i = lsb; i < msb+lsb; i++) ADCbuffer[i]=getInt();
                        break;




                    case READ_PROGRAM_ADDRESS:
                        break;


                    case READ_DATA_ADDRESS:
                        lsb=getInt()&0xFFFF;
                        pData=lsb;
                        sendInt(*pData);
                        break;

                    case WRITE_DATA_ADDRESS:
                        msb=getInt();
                        lsb=getInt();
                        pData=msb;
                        *pData=lsb;
                        break;

                    case READ_LOG:
                        while(error_readpos!=error_writepos){
                            sendChar(*error_readpos++);
                            if(error_readpos==&errors[ERROR_BUFFLEN])error_readpos=&errors[0];
                        }
                        sendChar('\n');
                        break;


                    #ifdef HX711_ENABLED
                    case HX711:
                        break;
                    #endif  

                }
                break;

                

            case SETBAUD:
                    initUART(sub_command);
                    break;

         }
        if(RESPONSE)ack(RESPONSE);


    }


    return (EXIT_SUCCESS);
}

