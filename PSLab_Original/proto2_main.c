/*
 * File:   proto2_main.c
 * Author: jithin
 *
 *
 */

// FICD
//#define POCKET

#pragma config ICS = NONE               // ICD Communication Channel Select bits (Reserved, do not use)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

#include<xc.h>
#include <p24EP256GP204.h>
#include <stdlib.h>
#include<libpic30.h>
#include"../PSLab_Original_Library/functions.c"
//_FICD(ICS_PGD2 & JTAGEN_OFF) //Programmming pins  ..PGED2


/* PLL using external oscillator. */

_FOSCSEL(FNOSC_FRC & IESO_OFF); //Start up using internal oscillator
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT ); // enable failsafe osc, use external crystal
//_FOSC(FCKSM_CSECMD & OSCIOFNC_ON & POSCMD_EC ); // For Non-passive , driving clocks.

_FUID0(0x1000); // One way to set USER ID.  preferably use IPE + SQTP? for sequentially setting unique UID
_FUID1(0x0000); // This approach was abandoned in ExpEYES17 due to its time consuming nature. Instead , unique timestamps are writting into flash by the calibration code
_FUID2(0x0000); 
_FUID3(0x00FF); //4B

// FWDT
#pragma config WDTPOST = PS512          // Watchdog Timer Postscaler bits (1:512)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF              // Watchdog Timer Enable bit (Watchdog timer always enabled)

#pragma config ALTI2C2 = OFF            // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)

const BYTE version[] = "CSpark-SE.P.1.0"; //Change to 'PSLab' after communicating with android and PC app developers


int main() {
    LEDPIN=0;
    RCONbits.SWDTEN=1;
    unsigned char main_command,sub_command,RESPONSE;
    unsigned int i = 0, n = 0;
    unsigned int ADLOC;
    pProg=0x0;
    unsigned int *pData;
    init();
    initUART(BRGVAL1000000);
    setMultiFuncPortMode(MULTIFUNC_I2C);
    
    nRF_Setup();
    LEDPIN=1;//set_RGB(0x000503); //new colour. bluish with a hint of red.
    
    // The state machine that handles and executes commands.
    while (1) {
        while (!hasChar()){asm("CLRWDT");}
        main_command = getChar();
        sub_command = getChar();
        RESPONSE = SUCCESS;
        switch (main_command) {
            case FLASH:
                switch(sub_command){
                    case WRITE_FLASH:
                        value = getChar(); //fetch the page[0-19]
                        location = getChar(); //fetch the address(0-31)
                        /*-----------fetch 16 characters----------*/
                        for (i = 0; i < 8; i++) {    blk[i] = getInt();       } //two per loop
                        setFlashPointer(value);
                        load_to_flash(p, location, &blk[0]);
                        break;

                    case READ_FLASH:
                        value = getChar(); //fetch the page[0-19]
                        location = getChar();
                        setFlashPointer(value);
                        read_flash(p, location);
                        for (i = 0; i < 8; i++) {
                            sendInt(blk[i]);
                            }
                        break;
                    
                    case READ_BULK_FLASH:
                        lsb = getInt();
                        location = getChar();
                        setFlashPointer(location);
                        read_all_from_flash(p);
                        for (i = 0; i < lsb/2; i++) {
                            sendInt(dest[i]);
                        }
                        break;

                    case WRITE_BULK_FLASH:
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
                    case CAPTURE_12BIT:
                        disable_input_capture();
                        value = getChar();  //channel number
                        samples_to_fetch = getInt();
                        ADC_DELAY = getInt();
                        ADC_CHANNELS = 0; //capture one channel
                        disableADCDMA();
                        setADCMode(ADC_12BIT_SCOPE,value&0x7F,0);

                        if(value&0x80)PrepareTrigger(); //bit 6 of value.
                        else TRIGGERED=true;
                        conversion_done = 0;samples=0;
                        buff0 = &ADCbuffer[0];
                        endbuff = &ADCbuffer[samples_to_fetch];
                        setupADC10();
                        _AD1IF = 0;   _AD1IE = 1;
                        LEDPIN=0;//set_RGB(0x060606);
                        break;

                    case CAPTURE_ONE:
                        //disable_input_capture();
                        value = getChar();  //channel number
                        samples_to_fetch = getInt();
                        ADC_DELAY = getInt();

                        ADC_CHANNELS = 0; //capture one channel
                        AD1CON2bits.CHPS = 0;
                        setADCMode(ADC_10BIT_SIMULTANEOUS,value&0x7F,0);
                        AD1CON2bits.CHPS = 0;

                        if(value&0x80)PrepareTrigger(); //bit 6 of value.
                        else TRIGGERED=true;
                        conversion_done = 0;samples=0;
                        buff0 = &ADCbuffer[0];
                        endbuff = &ADCbuffer[samples_to_fetch];
                        setupADC10();
                        _AD1IF = 0;   _AD1IE = 1;
                        LEDPIN=0;//set_RGB_now(0x000A06);
                        break;

                    case CAPTURE_TWO:
                        value = getChar();  //channel number
                        samples_to_fetch = getInt();
                        ADC_DELAY = getInt();
                        ADC_CHANNELS = 1; //capture two channels
                        AD1CON2bits.CHPS = 1;
                        setADCMode(ADC_10BIT_SIMULTANEOUS,value&0x7F,0);
                        AD1CON2bits.CHPS = 1;
                        buff0 = &ADCbuffer[0];buff1 = &ADCbuffer[samples_to_fetch];
                        endbuff = &ADCbuffer[samples_to_fetch];
                        if(value&0x80)PrepareTrigger(); //bit 6 of value.
                        else TRIGGERED=true;
                        conversion_done = 0;samples=0;
                        setupADC10();
                        _AD1IF = 0;   _AD1IE = 1;
                        LEDPIN=0;//set_RGB_now(0x0A0000);
                        break;

                    case CAPTURE_FOUR:
                        value = getChar();  //channel number for SH0
                        samples_to_fetch = getInt();
                        ADC_DELAY = getInt();
                        ADC_CHANNELS = 3;   //capture all four channels
                        AD1CON2bits.CHPS = ADC_CHANNELS;
                        setADCMode(ADC_10BIT_SIMULTANEOUS,(value&0xF),(value>>4)&0x1);
                        AD1CON2bits.CHPS = ADC_CHANNELS;
                        buff0 = &ADCbuffer[0];buff1 = &ADCbuffer[samples_to_fetch];
                        buff2 = &ADCbuffer[2*samples_to_fetch];buff3 = &ADCbuffer[3*samples_to_fetch];
                        endbuff = &ADCbuffer[samples_to_fetch];
                        if(value&0x80)PrepareTrigger(); //bit 8 of value.
                        else TRIGGERED=true;
                        conversion_done = 0;samples=0;
                        setupADC10();
                        _AD1IF = 0;   _AD1IE = 1;
                        LEDPIN=0;//set_RGB_now(0x060006);
                        break;

                    case SET_HI_CAPTURE: //Set High , SET_LO change the state of SQR1 right before capturing data. Used for RC discharge, and other time critical experiments
                    case SET_LO_CAPTURE:
                        RPOR5bits.RP54R = 0; //SQR1(RC6) mappings disabled
                        //fall through
                    case MULTIPOINT_CAPACITANCE: // Used to estimate capacitance via RC
                    case PULSE_TRAIN:            //Output a preset frequency square wave for a fixed interval before capture.
                    case CAPTURE_DMASPEED:
                        //disable_input_capture();
                        value = getChar();  //channel number
                        
                        samples_to_fetch = getInt();
                        ADC_DELAY = getInt();
                        ADC_CHANNELS = 0; //capture one channel
                        AD1CON2bits.CHPS = 0;
                        _AD1IF = 0;   _AD1IE = 0; // Do not enable ADC interrupt. We're using DMA.
                        conversion_done = 1;samples=samples_to_fetch; //assume it's all over already
                        if(value&0x80){setADCMode(ADC_12BIT_DMA,value&0x7F,0); } 
                        else {setADCMode(ADC_10BIT_DMA,value&0x7F,0); }

                        DMA0STAH = __builtin_dmapage (&ADCbuffer[0]);
                        DMA0STAL = __builtin_dmaoffset (&ADCbuffer[0]);
                        DMA0PAD = (int)&ADC1BUF0; // Address of the capture buffer register
                        DMA0CNT = samples_to_fetch-1; // Number of words to buffer


                        if(sub_command==PULSE_TRAIN){     //Temporarily attach SQR1 to square wave. For speed of sound. RC6 to OC1
                            lsb = getInt();
                            OC1TMR=0; RPOR5bits.RP54R = 0x10;
                            preciseDelay(lsb);
                            RPOR5bits.RP54R =0;
                        }
                        _DMA0IF = 0; _DMA0IE = 1; // Enable DMA interrupt enable bit
                        DMA0CONbits.CHEN = 1;
                        if(sub_command==SET_LO_CAPTURE)_LATC6=0; 
                        else if(sub_command==SET_HI_CAPTURE)_LATC6=1; 
                        else if(sub_command==MULTIPOINT_CAPACITANCE){ _TRISC0=0; _LATC0=0;}    //Prepare 20K impedance voltage source, and connect it to 0V.


                        setupADC10();
                        LEDPIN=0;
                        break;

                    case SET_CAP:
                        value = getChar();
                        lsb = getInt();         //Delay uS
                        set_cap_voltage(value,lsb);
                        break;

                    case CONFIGURE_TRIGGER:
                        value = getChar();
                        TRIGGER_CHANNEL=value&0x0F;
                        TRIGGER_LEVEL=getInt();
                        TRIGGER_TIMEOUT=50000;
                        TRIGGER_PRESCALER = (value>>4)&0xF;
                        break;

                    case GET_CAPTURE_STATUS:
                        sendChar(conversion_done);
                        sendInt(samples);
                        break;

                    case SET_PGA_GAIN:
                        location = getChar();
                        value = getChar();
                        setPGA(location, value);
                        break;

                    case SELECT_PGA_CHANNEL:
                        location = getChar();
                        setSensorChannel(location);
                        break;

                    case GET_VOLTAGE:
                        location = getChar();
                        setADCMode(ADC_12BIT,3,0);
                        i=get_voltage(location);
                        sendInt(i);
                        break;

                    case GET_VOLTAGE_SUMMED:
                        location = getChar();
                        i=get_voltage_summed(location);
                        sendInt(i);
                        //sendInt(ADC1BUF0);sendInt(ADC1BUF1);sendInt(ADC1BUF2);sendInt(ADC1BUF3);sendInt(ADC1BUF4);sendInt(ADC1BUF5);sendInt(ADC1BUF6);sendInt(ADC1BUF7);
                        //sendInt(ADC1BUF8);sendInt(ADC1BUF9);sendInt(ADC1BUFA);sendInt(ADC1BUFB);sendInt(ADC1BUFC);sendInt(ADC1BUFD);sendInt(ADC1BUFE);sendInt(ADC1BUFF);
                        break;


                    case GET_CAPTURE_CHANNEL:
                        //disable_input_capture();
                        _LATC0=0;_TRISC0=1; //set 20K CAP voltage source to high impedance mode.

                        value = getChar();      //channel number
                        lsb = getInt();   //number of bytes
                        msb = getInt();           //offset / starting position
                        for (i = msb; i < msb+lsb; i++) sendInt(ADCbuffer[i+samples_to_fetch*value]);
                        break;

                }
                break;
            case SPI:
                switch(sub_command){
                    case START_SPI:
                        location = getChar();
                        set_CS(location,0);
                        RESPONSE = DO_NOT_BOTHER;
                        break;

                    case STOP_SPI:
                        location = getChar();
                        set_CS(location,1);
                        RESPONSE = DO_NOT_BOTHER;
                        break;

                    case SEND_SPI8:
                        value = getChar();
                        sendChar(spi_write8(value));
                        break;

                    case SEND_SPI16:
                        msb = getInt();
                        sendInt(spi_write16(msb));
                        break;

                    case SET_SPI_PARAMETERS:
                        value=getChar();        //frequency,cke,ckp
                        SPI_SPRE = (value)&0x7;
                        SPI_PPRE = (value>>3)&0x3;
                        SPI_CKE =  (value>>5)&1;
                        SPI_CKP =  (value>>6)&1;
                        SPI_SMP =  (value>>7)&1;
                        initSPI();
                        break;


                }
                break;
            case I2C:
                switch(sub_command){
                    case I2C_START:             //Initialize I2C and select device address
                        setMultiFuncPortMode(MULTIFUNC_I2C);
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
                        MULTIFUNC_PORT=NOT_READY;
                        setMultiFuncPortMode(MULTIFUNC_I2C);
                        break;
                        
                    case I2C_STATUS:
                        sendInt(I2C2STAT);
                        break;

                    case I2C_READ_BULK:   //USE THIS
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

                    case I2C_WRITE_BULK:  //AND THIS. The rest are primitive I2C calls.
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


                }
                break;
            case UART_2:
                switch(sub_command){
                    case SEND_CHAR:
                        value=getChar();
                        sendChar2(value);
                    break;

                    case SEND_INT:
                        msb=getInt();
                        sendInt2(msb);
                    break;

                    case READ_BYTE:
                        sendChar(getChar2());
                        RESPONSE = DO_NOT_BOTHER;
                    break;

                    case READ_INT:
                        sendInt(getInt2());
                        RESPONSE = DO_NOT_BOTHER;
                    break;

                    case READ_UART2_STATUS:
                        sendChar(U2STAbits.URXDA);
                        RESPONSE = DO_NOT_BOTHER;
                    break;
                    
                    case SET_BAUD:
                        lsb=getInt();
                        configUART2(lsb);
                    break;

                }
                break;
            case DAC:
                switch(sub_command){

                case SET_DAC:
                    I2C2BRG = 0x90;
                    setMultiFuncPortMode(MULTIFUNC_I2C);
                    location = getChar(); //=address<<1 | R/W   [r=1,w=0]
                    value = getChar(); //=DAC channel 0-3
                    lsb = getInt();
                    I2CStart();
                    I2CSend(location);
                    I2CSend(64 | (value << 1)); //use 64, not 88(writes to EEPROM also. uses 30mS extra)
                    I2CSend((lsb>>8)&0xFF );
                    I2CSend(lsb&0xFF);
                    I2CStop();
                    Delay_us(6);
                    break;
                case SET_CALIBRATED_DAC:   // removed. All calibration is applied on the Python Side, and this is unnecessary
                    location = getChar(); //=address << 1 | R/W   [r=1,w=0]
                    value = getChar(); //=DAC channel 0-3
                    lsb = getInt();
                    sendInt(lsb);
                    break;
                    
                }
                break;
            case WAVEGEN:
                switch(sub_command){
                    case SET_SINE1:
                        value = getChar();
                        lsb = getInt();
                        sineWave1(lsb,value);
                        break;

                    case SET_SINE2:
                        value = getChar();
                        lsb = getInt();
                        sineWave2(lsb,value);
                        break;

                    case SET_WG_PHASE:
                        tmp_int1 = getInt(); //wavelength
                        tmp_int2 = getInt(); //wavelength
                        lsb = getInt();     //pos
                        msb=getInt();       //timer delay
                        value = getChar();  //(prescaler1<<4)|(prescaler2<<2)|(HIGHRES2<<1)|(HIGHRES)
                        setSineWaves(tmp_int1,tmp_int2,lsb,msb,value);
                        break;
                        
                    case LOAD_WAVEFORM1:
                        for(lsb=0;lsb<WAVE_TABLE_FULL_LENGTH;lsb++)sineTable1[lsb]=getInt();
                        for(lsb=0;lsb<WAVE_TABLE_SHORT_LENGTH;lsb++)sineTable1_short[lsb]=getChar();                            
                        break;

                    case LOAD_WAVEFORM2:
                        for(lsb=0;lsb<WAVE_TABLE_FULL_LENGTH;lsb++)sineTable2[lsb]=getInt();
                        for(lsb=0;lsb<WAVE_TABLE_SHORT_LENGTH;lsb++)sineTable2_short[lsb]=getChar();                            
                        break;



                    case SET_SQR1:
                        lsb = getInt(); //wavelength
                        msb = getInt(); //high time
                        value = getChar(); //prescaler
                        sqr1(lsb,msb,value);
                        break;

                    case SET_SQR2:
                        lsb = getInt();         //wavelength
                        msb = getInt();         //high time
                        value = getChar();      //prescaler
                        sqr2(lsb,msb,value);
                        break;

                    case SQR4:
                        lsb = getInt();          //wavelength
                        msb = getInt();          //high time 0
                        tmp_int1 = getInt();     //high time 1
                        tmp_int2 = getInt();     //phase1
                        tmp_int3 = getInt();     //high time 2
                        tmp_int4 = getInt();     //phase2
                        tmp_int5 = getInt();     //high time 3
                        tmp_int6 = getInt();     //phase3
                        value = getChar();       //prescaler
                        sqr4(lsb,msb,tmp_int1,tmp_int2,tmp_int3,tmp_int4,tmp_int5,tmp_int6,value);
                        break;

                    case MAP_REFERENCE:
                        location = getChar();
                        value = getChar();
                        mapReferenceOscillator(location,value);
                        break;

                    
                }
                break;
            case DOUT:
                switch(sub_command){
                    case SET_STATE:
                        lsb = getChar();
                        //SQR1 - 4
                        if(lsb&0x10)RPOR5bits.RP54R = 0;
                        if(lsb&0x20)RPOR5bits.RP55R = 0;
                        if(lsb&0x40)RPOR6bits.RP56R = 0; //output1 pin(RC8)disconnected
                        if(lsb&0x80)RPOR6bits.RP57R = 0;  //output2(RC9) disconnected

                        LATC&=~((lsb&0x00F0)<<2); //clear the bits specified in the mask(4MSB of data)
                        LATC|=((lsb&0x000F)<<6); //set the bits specified in the data(4LSB of data)
                        break;

                }
                break;
            case DIN:
                switch(sub_command){
                    case GET_STATES:
                        sendChar((PORTB>>10)&0xF);
                        break;
                }
                break;
            case TIMING:
                switch(sub_command){
                    case GET_TIMING:        //Using input capture
                        _IC1IF=0;
                        lsb = getInt(); //timeout. [t(s)*64e6>>16]
                        value = getChar();
                        location = getChar();
                        init_IC_for_frequency(location&0xF,(value>>2)&0x7,value&0x3);
                        while((!_IC1IF) && (IC2TMR<lsb))asm("CLRWDT"); _IC1IF=0;
                        sendInt(IC2TMR);
                        for(n=0;n<(value&0x3);n++){
                        sendInt(IC1BUF);sendInt(IC2BUF); //read from FIFO
                        }
                        disable_input_capture();
                    break;


                    case TIMING_MEASUREMENTS:        //Using two input captures
                        lsb = getInt(); //timeout. [t(s)*64e6>>16]
                        location = getChar();
                        value = getChar();
                        cb = getChar();
                        TimingMeasurements(location&0xF,(location>>4)&0xF,value&0x7,(value>>3)&0x7,cb&0xF,(cb>>4)&0xF);
                        if((value>>6)&1){RPOR5bits.RP54R = 0;_LATC6=(value>>7)&1;}
                        while((!_IC1IF || !_IC3IF) && (IC2TMR<lsb))asm("CLRWDT");;
                        for(n=0;n<((cb)&0xF);n++){sendInt(IC1BUF);sendInt(IC2BUF);}   //send data from PIN 1
                        for(n=0;n<((cb>>4)&0xF);n++){sendInt(IC3BUF);sendInt(IC4BUF);}//send data from PIN 2
                        _IC3IF=0;_IC1IF=0;
                        sendInt(IC2TMR);
                        disable_input_capture();
                        T2CONbits.TON = 0;
                    break;


                    case INTERVAL_MEASUREMENTS:        //Using two input captures
                        lsb = getInt(); //timeout. [t(s)*64e6>>16]
                        location = getChar();
                        value = getChar();
                        Interval(location&0xF,(location>>4)&0xF,value&0x7,(value>>3)&0x7);
                        while((!_IC1IF) && (IC2TMR<lsb))asm("CLRWDT");
                        sendInt(IC1BUF);sendInt(IC2BUF);
                        while((!_IC3IF) && (IC2TMR<lsb))asm("CLRWDT");
                        sendInt(IC3BUF);sendInt(IC4BUF);
                        sendInt(IC2TMR);
                        disable_input_capture();
                    break;


                    case CONFIGURE_COMPARATOR:
                        COMPARATOR_CONFIG = getChar();
                        break;
                        
                    case START_ONE_CHAN_LA:        //Using input capture
                        lsb=getInt();
                        value = getChar(); //trigger or not
                        ca = getChar();
                        if(value&1){
                            if(value&2)INTCON2bits.INT2EP=1; //falling edge interrupt
                            else INTCON2bits.INT2EP=0;
                            DIGITAL_TRIGGER_CHANNEL=0;

                            RPINR1bits.INT2R = DIN_REMAPS[(value>>4)&0xF];
                            start_1chan_LA(lsb,(ca>>4)&0xF,ca&0xF,0);
                            _INT2IF=0;_INT2IE=1;

                        }else{
                            start_1chan_LA(lsb,(ca>>4)&0xF,ca&0xF,0);
                            INITIAL_DIGITAL_STATES =((PORTB>>10)&0xF)|(_C4OUT<<4); //INITIAL_DIGITAL_STATES =(PORTB>>10)&0xF;
                            IC1CON2bits.TRIGSTAT = 1;  IC2CON2bits.TRIGSTAT =1 ;
                        }

                        break;

                    // Logic analyzer requires a lot of work in the 'DETECT EVERY EDGE' mode. It does not work well if signal starts within <100nS of initialization
                    case START_ALTERNATE_ONE_CHAN_LA:        //Using input capture
                        _IC4IF=0;_IC4IP=7;_IC4IE=0;
                        lsb=getInt();
                        value = getChar(); //channel,mode
                        location = getChar(); //trigger_channel,trigger_mode
                        if(location&7){
                            start_1chan_LA(lsb,(value>>4)&0xF,(value)&0xF,location);
                            _IC4IF=0;_IC4IP=7;_IC4IE=1;  //enable input capture interrupt. highest priority
                        }else{
                            start_1chan_LA(lsb,(value>>4)&0xF,(value)&0xF,0);
                            INITIAL_DIGITAL_STATES =((PORTB>>10)&0xF)|(_C4OUT<<4); //INITIAL_DIGITAL_STATES =(PORTB>>10)&0xF;
                            IC2CON2bits.TRIGSTAT =1 ;IC1CON2bits.TRIGSTAT = 1;  
                        }

                        break;


                    case START_TWO_CHAN_LA:        //Using input capture
                        lsb=getInt();   //points to capture
                        value = getChar(); //trigger or not
                        ca = getChar(); //modes.  four bits each. compressed into a char
                        location = getChar(); //Channels.  four bits each. compressed into a char
                        start_2chan_LA(lsb,ca,location);
                        if(value&1){
                            if(value&2)INTCON2bits.INT2EP=1; //falling edge interrupt
                            else INTCON2bits.INT2EP=0;
                            RPINR1bits.INT2R = DIN_REMAPS[(value>>4)&0xF];
                            start_2chan_LA(lsb,ca,location);
                            _INT2IF=0;_INT2IE=1;

                        }else{
                            start_2chan_LA(lsb,ca,location);
                            IC1CON1bits.ICM=LAM1;IC2CON1bits.ICM=LAM2;IC3CON1bits.ICM=LAM3;IC4CON1bits.ICM=LAM4;
                            INITIAL_DIGITAL_STATES =((PORTB>>10)&0xF)|(_C4OUT<<4); //INITIAL_DIGITAL_STATES =(PORTB>>10)&0xF;
                            IC1CON2bits.TRIGSTAT = 1;IC3CON2bits.TRIGSTAT = 1;
                            IC2CON2bits.TRIGSTAT = 1;IC4CON2bits.TRIGSTAT = 1;
                            INITIAL_DIGITAL_STATES_ERR =((PORTB>>10)&0xF)|(_C4OUT<<4); //=(PORTB>>10)&0xF;
                        }

                        break;

                    case START_THREE_CHAN_LA:        //Using input capture
                        lsb=getInt();   //DMACNT
                        msb = getInt(); //modes.  four bits each. compressed into an INT
                        location = getChar(); //trigger_channel[4msb],trigger_mode[4lsb]
                        if(location&7){     //If trigger mode is enabled
                            start_3chan_LA(lsb,msb&0x0FFF,location);
                            _IC4IF=0;_IC4IP=7;_IC4IE=1;  //enable input capture interrupt. highest priority
                        }else{
                            start_3chan_LA(lsb,msb&0x0FFF,0);
                            b1=PORTB;
                            INITIAL_DIGITAL_STATES =((PORTB>>10)&0xF)|(_C4OUT<<4); //INITIAL_DIGITAL_STATES =(PORTB>>10)&0xF;
                            IC1CON2bits.TRIGSTAT = 1;IC2CON2bits.TRIGSTAT = 1;IC3CON2bits.TRIGSTAT = 1;
                            INITIAL_DIGITAL_STATES_ERR =((PORTB>>10)&0xF)|(_C4OUT<<4); //=(PORTB>>10)&0xF;
                            }

                        break;

                    case START_FOUR_CHAN_LA:        //Using input capture
                        lsb=getInt();
                        msb = getInt();                                         //modes.  four bits each. compressed into an INT
                        location=getChar();                                     //timer2 prescaler
                        value = getChar(); //trigger or not
                        INITIAL_DIGITAL_STATES =0;
                        start_4chan_LA(lsb,msb,location);                       //Number of bytes, IC modes, prescaler of TIMER
                        if(value&1){                                            //[7,6,5,4-B4,3-B6,2-B5,1-edge(rise/fall),0-enable]
                            if(value&2)DIGITAL_TRIGGER_STATE=1;
                            else DIGITAL_TRIGGER_STATE=0;
                            DIGITAL_TRIGGER_CHANNEL=0;
                            if(value&4) {_CNIEB10=1;DIGITAL_TRIGGER_CHANNEL|=1;} //enable level change interrupt on B10(ID1)
                            if(value&8) {_CNIEB11=1;DIGITAL_TRIGGER_CHANNEL|=2;} //enable level change interrupt on B11(ID2)
                            if(value&16) {_CNIEB12=1;DIGITAL_TRIGGER_CHANNEL|=4;}//enable level change interrupt on B12(ID3)
                            _CNIF=0;_CNIE=1;
                        }else{
                            T2CONbits.TON = 1; // Start Timer
                            INITIAL_DIGITAL_STATES =((PORTB>>10)&0xF)|(_C4OUT<<4); //INITIAL_DIGITAL_STATES =(PORTB>>10)&0xF;
                            IC1CON2bits.TRIGSTAT = 1;IC2CON2bits.TRIGSTAT = 1;IC3CON2bits.TRIGSTAT = 1;IC4CON2bits.TRIGSTAT = 1;
                            INITIAL_DIGITAL_STATES_ERR =((PORTB>>10)&0xF)|(_C4OUT<<4); //=(PORTB>>10)&0xF;
                        }
                        break;

                    case STOP_LA:        //
                        disable_input_capture();
                        break;

                    case GET_INITIAL_DIGITAL_STATES:                            //Using input capture
                        sendInt(__builtin_dmaoffset (&ADCbuffer));
                        sendInt(DMA0STAL);sendInt(DMA1STAL);sendInt(DMA2STAL);sendInt(DMA3STAL);
                        sendChar(INITIAL_DIGITAL_STATES);
                        sendChar(INITIAL_DIGITAL_STATES_ERR);
                        break;

                    case FETCH_LONG_DMA_DATA:                                   //Using input capture
                        lsb = getInt();                                         //Bytes to get
                        value = getChar();                                      //channel number
                        for(n=0;n<lsb;n++){sendInt(ADCbuffer[n+2*value*(BUFFER_SIZE/4)]);sendInt(ADCbuffer[n+(2*value+1)*(BUFFER_SIZE/4)]);}//{sendInt(BufferA1[n]);sendInt(BufferA2[n]);}
                        LEDPIN=1;
                        break;

                    case FETCH_INT_DMA_DATA:                                    //Using input capture
                        lsb = getInt();                                         //Bytes to get
                        value = getChar();                                      //channel number
                        for(n=0;n<lsb;n++){sendInt(ADCbuffer[n+value*(BUFFER_SIZE/4)]);}//{sendInt(BufferA1[n]);sendInt(BufferA2[n]);}
                        LEDPIN=1;
                        break;

                }
                break;
            case COMMON:
                switch(sub_command){
                    case GET_CTMU_VOLTAGE: // Can be used to measure knee voltages of diodes on the fly.
                        value=getChar();                                        //bits<0-4> = channel,bits <5-6> current range
                        msb = get_ctmu_voltage(value&0x1F,(value>>5)&0x3,(value>>7)&0x1);
                        //sendInt(ADC1BUF0);sendInt(ADC1BUF1);sendInt(ADC1BUF2);sendInt(ADC1BUF3);sendInt(ADC1BUF4);sendInt(ADC1BUF5);sendInt(ADC1BUF6);sendInt(ADC1BUF7);
                        //sendInt(ADC1BUF8);sendInt(ADC1BUF9);sendInt(ADC1BUFA);sendInt(ADC1BUFB);sendInt(ADC1BUFC);sendInt(ADC1BUFD);sendInt(ADC1BUFE);sendInt(ADC1BUFF);
                        sendInt(msb);
                    break;

                    case GET_CAP_RANGE:
                        msb=getInt();               //Charge time.  microseconds
                        sendInt(get_cap_range(msb));
                    break;

                    case START_CTMU:
                        value=getChar();    //bits<0-6> =  current range, bit 7=TGEN
                        location=getChar();    // current trim
                        CTMUCON1bits.CTMUEN = 0;
                        CTMUCON1bits.TGEN = (value>>7)&0x1;                     //(channel==5)?1:0;
                        CTMUICONbits.ITRIM = location;
                        CTMUICONbits.IRNG = (value)&0x7F;                       // 01->Base Range .53uA, 10->base*10, 11->base*100, 00->base*1000
                        CTMUCON1bits.CTTRIG = 0; //do not trigger the ADC
                        CTMUCON1bits.CTMUEN = 1;Delay_us(1000);
                        CTMUCON2bits.EDG1STAT = 1;  // Start current source
                        break;

                    case STOP_CTMU:
                        disableCTMUSource();
                        break;
                    
                    
                    case GET_CAPACITANCE:
                        value=getChar();            //current range for CTMU
                        location=getChar();         //current trimming bits
                        msb=getInt();               //Charge time.  microseconds
                        LEDPIN=0;
                        sendInt(get_cc_capacitance(value,location,msb));
                        LEDPIN=1;
                    break;

                    case START_COUNTING:           // Need to count the number of Skittles in a packet? Make a light barrier, connect the output to the digital input , and start pouring them throught the barrier!
                        location=getChar();         //Channel
                        startCounting(location);
                    break;
                    case FETCH_COUNT:
                        sendInt(TMR2);              //Count
                    break;

                    case GET_HIGH_FREQUENCY:        //This one shares TIMER5 with the ADC!
                        LEDPIN=0;
                        value=getChar();
                        get_high_frequency(value&0xF,(value>>4)&0x4); //0=> 1:1 scaling
                        while(!_T5IF);_T5IF=0;
                        LEDPIN=1;
                        freq_lsb=TMR2;
                        freq_msb=TMR3HLD;
                        sendChar(1);  //scaling factor
                        sendLong(freq_lsb,freq_msb);
                        break;

                    case GET_ALTERNATE_HIGH_FREQUENCY:    //This one shares TIMER5 with the ADC and uses also uses timers from the input capture module. We're running out of timers and modules :/
                        LEDPIN=0;
                        value=getChar();
                        alternate_get_high_frequency(value&0xF,(value>>4)&0x4); //0=> 1:1 scaling
                        while(!_T5IF);_T5IF=0;
                        LEDPIN=1;
                        freq2_lsb=IC1TMR;
                        freq2_msb=IC2TMR;
                        sendChar(1); //scaling factor
                        sendLong(freq2_lsb,freq2_msb);
                        break;

                    case GET_FREQUENCY:        //Using input capture
                        _IC1IF=0;
                        lsb = getInt(); //timeout. [t(s)*64e6>>16]
                        value = getChar();
                        init_IC_for_frequency(value,EVERY_SIXTEENTH_RISING_EDGE,2);
                        while((IC2TMR<lsb) && (!_IC1IF)); _IC1IF=0; RPINR7bits.IC1R =0; //disconnect
                        if((IC2TMR>=lsb) || (IC2CON1bits.ICOV))sendChar(1); //in case of buffer overflow/timeout
                        else sendChar(0);
                        sendInt(IC1BUF);sendInt(IC2BUF);
                        sendInt(IC1BUF);sendInt(IC2BUF);
                        disable_input_capture();
                        break;

                    case GET_VERSION:
                        for (i = 0; i < sizeof(version)-1; i++) sendChar(version[i]);
                        sendChar('\n');
                        RESPONSE = DO_NOT_BOTHER;
                        break;

                    case RETRIEVE_BUFFER:
                        lsb = getInt();   //starting point
                        msb = getInt();   //number of bytes
                        for (i = lsb; i < msb+lsb; i++) sendInt(ADCbuffer[i]);
                        LEDPIN=1;
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

                    case SETRGB:
                        value = getChar();
                        for(ca=0;ca<value;ca++)data[ca]=getChar();
                        _GIE=0;
                        CSOUT1 = 0;Delay_us(50); //reset LED
                        for(location=0;location<value;location++){
                            cb=data[location];
                            for(ca = 0; ca < 8; ca++) {
                                if (cb&0x80) {
                                    CSOUT1 = 1;
                                    __asm__ volatile ("repeat #45");__asm__ volatile ("nop");
                                    CSOUT1 = 0;
                                    __asm__ volatile ("repeat #38");__asm__ volatile ("nop");
                                    } else {
                                    CSOUT1 = 1;
                                    __asm__ volatile ("repeat #22");__asm__ volatile ("nop");
                                    CSOUT1 = 0;
                                    __asm__ volatile ("repeat #51");__asm__ volatile ("nop");
                                    }
                                    cb = cb << 1;
                               }
                        }
                        _GIE=1;

                        break;
                        
                    case SETRGB2:
                        value = getChar();
                        for(ca=0;ca<value;ca++)data[ca]=getChar();
                        _GIE=0;
                        CSOUT2 = 0;Delay_us(50); //reset LED
                        for(location=0;location<value;location++){
                            cb=data[location];
                            for(ca = 0; ca < 8; ca++) {
                                if (cb&0x80) {
                                    CSOUT2 = 1;
                                    __asm__ volatile ("repeat #45");__asm__ volatile ("nop");
                                    CSOUT2 = 0;
                                    __asm__ volatile ("repeat #38");__asm__ volatile ("nop");
                                    } else {
                                    CSOUT2 = 1;
                                    __asm__ volatile ("repeat #22");__asm__ volatile ("nop");
                                    CSOUT2 = 0;
                                    __asm__ volatile ("repeat #51");__asm__ volatile ("nop");
                                    }
                                    cb = cb << 1;
                               }

                        }
                        _GIE=1;
                        break;


                    case SETRGB3:
                        RPOR5bits.RP54R = 0; //disconnect square wave
                        value = getChar();
                        _GIE=0;
                        _LATC6 = 0;Delay_us(50); //reset LED
                        for(ca=0;ca<value;ca++)data[ca] =getChar();

                        for(location=0;location<value;location++){
                            cb=data[location];
                            for(ca = 0; ca < 8; ca++) {
                                if (cb&0x80) {
                                    _LATC6 = 1;
                                    __asm__ volatile ("repeat #45");__asm__ volatile ("nop");
                                    _LATC6 = 0;
                                    __asm__ volatile ("repeat #38");__asm__ volatile ("nop");
                                    } else {
                                    _LATC6 = 1;
                                    __asm__ volatile ("repeat #22");__asm__ volatile ("nop");
                                    _LATC6 = 0;
                                    __asm__ volatile ("repeat #51");__asm__ volatile ("nop");
                                    }
                                    cb = cb << 1;
                               }

                        }
                        _GIE=1;
                        break;


                    case READ_PROGRAM_ADDRESS:
                        pProg=0x0;
                        l1=getInt()&0xFFFF;
                        l2=getInt()&0xFFFF;
                        _memcpy_p2d16(&ADLOC, pProg+(l1|(l2<<16)),sizeof(unsigned int));
                        sendInt(ADLOC);
                        break;


                    case WRITE_PROGRAM_ADDRESS: // Doesn't work . To be fix. I also can't really think of any use case where modifying the flash code will be necessary. flash writes are available as a separate command
                        pProg=0x0;
                        l1=getInt()&0xFFFF;
                        l2=getInt()&0xFFFF;
                        ADLOC=getInt();
                        //__builtin_tbladdress(p,(l1|(l2<<16)) );  //initialize flash pointer
                        //load_to_flash(p, location, &blk[0]);
                        //_memcpy_p2d16(p,ADLOC ,sizeof(unsigned int));
                        break;

                    case READ_DATA_ADDRESS:  //read SFRs from python
                        lsb=getInt()&0xFFFF;
                        pData=lsb;
                        sendInt(*pData);
                        break;

                    case WRITE_DATA_ADDRESS: //write SFRs from python. Forgot to add a pull-up? Push a software update instead of a firmware update. phew.
                        msb=getInt();
                        lsb=getInt();
                        pData=msb;
                        *pData=lsb;
                        break;

                    case READ_LOG:          // Not really used extensively. Can be incorporated
                        while(error_readpos!=error_writepos){
                            sendChar(*error_readpos++);
                            if(error_readpos==&errors[ERROR_BUFFLEN])error_readpos=&errors[0];
                        }
                        sendChar('\n');
                        break;
                    case RESTORE_STANDALONE: //Reset the device
                        asm("goto 0x0000");

                        break;
                }
                break;

            case NRFL01:                     //Wireless transceiver. Can be used to exchange data between wireless sensor nodes, as well as other PSLabs
                switch(sub_command){
                    case NRF_SETUP:
                        nRF_Setup();
                        break;
                    case NRF_RXMODE:
                        RXMode();
                        break;
                    case NRF_TXMODE:
                        TXMode();
                        break;
                    case NRF_POWER_DOWN:
                        PowerDown();
                        break;
                    case NRF_RXCHAR:
                        sendChar(RXChar());
                        break;
                    case NRF_TXCHAR:
                        value=getChar();
                        TXChar(value); //transmit if buffer is ready
                        if (!rfCardPresent)RESPONSE |=0x20;
                        break;
                    case NRF_HASDATA:
                        sendChar(ReadDataAvailable());
                        break;
                    case NRF_FLUSH:
                        FlushTXRX();
                        break;
                    case NRF_WRITEREG:
                        location=getChar();
                        value=getChar();
                        WriteRegister(location,value);
                        break;
                    case NRF_READREG:
                        location=getChar();
                        sendChar(ReadRegister(location));
                        break;
                    case NRF_GETSTATUS:
                        sendChar(ReadStatus());
                        break;
                    case NRF_WRITECOMMAND:
                        value = getChar();
                        WriteCommand(value);
                        break;

                    case NRF_WRITEADDRESSES:
                        for(i=0;i<3;i++)data[i]=getChar();
                        WriteAddress(RX_ADDR_P0, 3, &data[0]);
                        WriteAddress(TX_ADDR, 3, &data[0]);
                        break;
                        
                    case NRF_WRITEADDRESS:
                        value = getChar(); //address register
                        for(i=0;i<3;i++)data[i]=getChar();
                        if(value==RX_ADDR_P0){
                            CSN_LOW;
                            spi_write8(W_REG | RX_ADDR_P0);
                            for (i=0; i<3; i++)spi_write8(data[i]);
                            CSN_HIGH;
                        }else if (value==TX_ADDR){
                            CSN_LOW;
                            spi_write8(W_REG | TX_ADDR);
                            for (i=0; i<3; i++)spi_write8(data[i]);
                            CSN_HIGH;

                        }else{
                            CSN_LOW;
                            spi_write8(W_REG | RX_ADDR_P1);
                            if(value==RX_ADDR_P1)spi_write8(data[0]);
                            else spi_write8(0xFF);
                            spi_write8(data[1]);spi_write8(data[2]);
                            CSN_HIGH;
                            if(value!=RX_ADDR_P1){
                                    Delay_us(1);
                                    CSN_LOW;
                                    spi_write8(W_REG | value);
                                    spi_write8(data[0]);
                                    CSN_HIGH;
                                    }
                        }
                        break;

                    case NRF_WRITEPAYLOAD:
                        value = getChar();
                        location = getChar();
                        for(i=0;i<(value&0x3F);i++)data[i]=getChar();
                        WritePayload(location,value, &data[0]);//transmit if buffer is ready
                        while((ReadStatus() & 0x30)==0) Delay_us(100);  //Wait for ACK / MAX_restransmits
                        location=ReadStatus();
                        if(location & 0x10)RESPONSE |=0x10;
                        if(!rfCardPresent)RESPONSE |=0x20;
                        if(location&0x10)WriteRegister(NRF_STATUS, 0x30); //clear data_sent, max_rt
                        if(value&0x40)RXMode(); //Switch to RXMode immediately
                        break;


                    case NRF_TRANSACTION:
                        _GIE=0;
                        value = getChar();
                        lsb = getInt();
                        for(i=0;i<value;i++)data[i]=getChar();
                        if(ReadStatus() & 0x40){WriteCommand(FLUSH_RX);logit("Stray Packet[0],");} //In case a stray packet is found on pipe 0, flush it!

                        TXMode();Delay_us(130);
                        WriteRegister(NRF_STATUS, 0x70); //clear data_sent, max_rt
                        WritePayload(TX_PAYLOAD,value|0x80, &data[0]);//transmit if buffer is ready
                        while((ReadStatus() & 0x30)==0){Delay_us(100);}  //Wait for ACK / MAX_restransmits
                        asm("CLRWDT");
                        value=ReadStatus();
                        if(value&0x10)RESPONSE |=0x10; //Max retransmits!
                        if(!rfCardPresent)RESPONSE |=0x20; //Radio not present
                        WriteRegister(NRF_STATUS, 0x30); //clear data_sent, max_rt
                        
                        RXMode();//Switch to RX mode , and wait for reply
                        
                        if((value&0x10) == 0 && data[0]&0x80){ //Command was sent to the node succesfully
                            asm("CLRWDT");
                            while((ReadStatus() & 0x40)==0 && lsb){ //Wait for RX_DR
                                    lsb-=1;
                                    Delay_us(100);
                                }
                            asm("CLRWDT");

                            if(lsb && ((ReadStatus()&14) == 0) ){  //Reply received before timeout. RX FIFO has packet from pipe 0
                                location = ReadRegister(R_RX_PL_WID);
                                ReadPayload(location, &data[0]);
                                sendChar(location);        //Number of characters coming through
                                for(i=0;i<location;i++)sendChar(data[i]);
                                }
                            else{
                                RESPONSE|=0x40; //reply timeout
                                sendChar(0); //no characters coming through
                                if(ReadStatus()&14)WriteCommand(FLUSH_RX);
                                }
                        }else{
                            sendChar(0); //no characters coming through
                        }
                        _GIE=1;
                        break;


                    case NRF_READPAYLOAD:
                        value = getChar();
                        ReadPayload(value, &data[0]);
                        for(i=0;i<value;i++)sendChar(data[i]);
                        break;

                    case NRF_START_TOKEN_MANAGER:
                        WriteRegister(EN_AA, 0x03); //enable auto ack
                        WriteRegister(DYNPD, 0x03); //enable dynamic payload
                        WriteRegister(EN_RXADDR, 0x03); //Enable data pipe 2
                        _INT1IF=0;_INT1IE=1;
                        break;

                    case NRF_STOP_TOKEN_MANAGER:
                        WriteRegister(EN_AA, 0x01);
                        WriteRegister(DYNPD, 0x01);
                        WriteRegister(EN_RXADDR, 0x01);
                        _INT1IF=0;_INT1IE=0;
                        break;

                    case NRF_TOTAL_TOKENS:
                        sendChar(nodecount);
                        break;
                    case NRF_REPORTS:
                        value = getChar();
                        for(i=0;i<NRF_ROW_LENGTH;i++)sendChar(i2c_list[value][i]);
                        break;
                    case NRF_WRITE_REPORT:
                        value = getChar();
                        for(i=0;i<NRF_ROW_LENGTH;i++)i2c_list[value][i]=getChar();
                        break;
                    case NRF_DELETE_REPORT_ROW:
                        value = getChar();
                        for(n=value;n<NRF_REPORT_ROWS-1;n++)
                            for(i=0;i<NRF_ROW_LENGTH;i++)
                                i2c_list[n][i]=i2c_list[n+1][i];
                        for(i=0;i<NRF_ROW_LENGTH;i++)
                            i2c_list[NRF_REPORT_ROWS-1][i]=0;
                        if(nodecount)nodecount--;
                        break;
                }
                break;


            case SETBAUD: //This is a hack. you shouldn't have to use it unless you are on a severely underpowered system that keeps dropping packets.
                    initUART(sub_command);
                    break;

            case NONSTANDARD_IO:
                switch(sub_command){


                    case HCSR04:            //distance sensor
                        RPOR5bits.RP54R = 0;
                        _DMA2IF = 0;    _DMA2IE = 0; _DMA3IF = 0;    _DMA3IE = 0;
                        DMA2CONbits.CHEN = 0;DMA3CONbits.CHEN = 0;

                        lsb = getInt(); //timeout. [t(s)*64e6>>16]
                        _LATC6 = 1;  //SQR1  high
                        Interval(0,0,3,2);
                        Delay_us(10);
                        _LATC6 =0;   //SQR1 low
                        //msb=50000;
                        while((!_IC1IF) && (IC2TMR<lsb));// &&msb){Delay_us(1);msb-=1;}
                        sendInt(IC1BUF);sendInt(IC2BUF);
                        while((!_IC3IF) && (IC4TMR<lsb));// &&msb){Delay_us(1);msb-=1;}
                        sendInt(IC3BUF);sendInt(IC4BUF);
                        sendInt(IC2TMR);
                        disable_input_capture();
                        break;

                    case AM2302:          //humidity and temperature sensor
                        setMultiFuncPortMode(MULTIFUNC_NONSTANDARD);
                        _LATB4=0;
                        Delay_us(10000);
                        
                        _IC4IF=0;_IC4IP=7;_IC4IE=0;
                        start_1chan_LA(200,2,1,0); //200 points, channel ID3, EVERY_EDGE, no trigger
                        INITIAL_DIGITAL_STATES =_RB10;
                        IC2CON2bits.TRIGSTAT =1 ;IC1CON2bits.TRIGSTAT = 1;
                        
                        break;

                }
                break;

            case PASSTHROUGHS:
                switch(sub_command){
                    case PASS_UART:
                        value = getChar();
                        lsb = getInt();
                        initUART2_passthrough(lsb);
                        if(value)RCONbits.SWDTEN=0;
                        while(1);
                        break;
                }
                break;
         }
        if(RESPONSE)ack(RESPONSE);


    }


    return (EXIT_SUCCESS);
}

