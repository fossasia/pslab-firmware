
/*
 * File:   newmain.c
 * Author: jithin
 *
 * Created on 28 october, 2014
 *
 * This revision has the following changes
 *
 * ---All ADC channels are stored in a single buffer[3200]
 *
 * ---ADC capture commands do not immediately return the resultant
 * buffer.  They need to be separately fetched using the RETRIEVE_CHANNEL
 * command.
 *
 * ---The RETRIEVE_CHANNEL option allows fetching a smaller section of
 * the buffer at a time. Useful if communication issues arise in low
 * performance clients.
 *
 * ---logic analyser.
 * Input capture module works as a 1 channel, 2 channel, 4 channel LA
 * 15nS time resolution. 16-bit, 32-bit modes
 */

#include"functions.h"

void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt(void) {
    _AD1IF = 0;
    if (conversion_done) {
        return;
    }
    LEDPIN=1;
    if(TRIGGERED){
        *(buff0++) = (ADC1BUF0 );//&0x3ff;
        if (ADC_CHANNELS >= 1){
            *(buff1++) = (ADC1BUF1 );//&0x3ff;
            if (ADC_CHANNELS >= 2) {
                *buff2++ = (ADC1BUF2 );//&0x3ff;
                if (ADC_CHANNELS >= 3)*buff3++ = (ADC1BUF3 );//&0x3ff;
            }
        }
        samples++;
        LEDPIN=0;
        if(samples==samples_to_fetch){
            _AD1IF = 0;   _AD1IE = 0;   //disable any further interrupts until required
            conversion_done = 1;
            LEDPIN=1;
            }
        }
    else{
        if(TRIGGER_CHANNEL&1)adval=ADC1BUF0;
        else if(TRIGGER_CHANNEL&2)adval=ADC1BUF1;
        else if(TRIGGER_CHANNEL&4)adval=ADC1BUF2;
        else if(TRIGGER_CHANNEL&8)adval=ADC1BUF3;

        //-------If the trigger hasn't timed out yet ... ----------
        if(TRIGGER_WAITING<TRIGGER_TIMEOUT){
            TRIGGER_WAITING+=(ADC_DELAY>>TRIGGER_PRESCALER);
            if(!TRIGGER_READY && adval>TRIGGER_LEVEL+10)TRIGGER_READY=1;
            else if(adval<=TRIGGER_LEVEL && TRIGGER_READY){TRIGGERED=1;}
            }
       //-------If the trigger has timed out, then proceed to data acquisition ----------
        else {TRIGGERED=1;}
    }
    
}



void __attribute__((__interrupt__, no_auto_psv)) _DMA0Interrupt(void)
{
IC1CON2bits.TRIGSTAT = 0;IC1CON1bits.ICM=0;DMA0CONbits.CHEN = 0;
_DMA0IF = 0;_DMA0IE = 0; // Clear the DMA0 Interrupt Flag
if(DMA_MODE == DMA_LA_TWO_CHAN || DMA_MODE == DMA_LA_ONE_CHAN){IC2CON1bits.ICM=0; DMA1CONbits.CHEN = 0;_DMA1IF = 0;_DMA1IE = 0;}//disable DMA1 too
}
void __attribute__((__interrupt__, no_auto_psv)) _DMA1Interrupt(void)
{
IC2CON2bits.TRIGSTAT = 0;IC2CON1bits.ICM=0; DMA1CONbits.CHEN = 0;
_DMA1IF = 0;_DMA1IE = 0; // Clear the DMA0 Interrupt Flag
}
void __attribute__((__interrupt__, no_auto_psv)) _DMA2Interrupt(void){
IC3CON2bits.TRIGSTAT = 0;IC3CON1bits.ICM=0;
_DMA2IF = 0; // Clear the DMA0 Interrupt Flag
}

void __attribute__((__interrupt__, no_auto_psv)) _DMA3Interrupt(void)
{
IC4CON2bits.TRIGSTAT = 0;IC4CON1bits.ICM=0;
_DMA3IF = 0; // Clear the DMA0 Interrupt Flag
}

void __attribute__ ((__interrupt__, no_auto_psv)) _CNInterrupt(void) // For four channel Logic analyzer
{
    if((((PORTB>>10)&DIGITAL_TRIGGER_CHANNEL)>0)==DIGITAL_TRIGGER_STATE){
             _CNIEB10=0;_CNIEB11=0;_CNIEB12=0;_CNIEB13=0;_CNIF=0;_CNIE=0;
            INITIAL_DIGITAL_STATES =((PORTB>>10)&0xF)|(_C4OUT<<4);

            IC1CON2bits.TRIGSTAT = 1;  IC2CON2bits.TRIGSTAT =1 ;IC3CON2bits.TRIGSTAT = 1;IC4CON2bits.TRIGSTAT = 1;
            T2CONbits.TON = 1;
            INITIAL_DIGITAL_STATES_ERR =((PORTB>>10)&0xF)|(_C4OUT<<4); //=(PORTB>>10)&0xF;

    
    }
_CNIF = 0;
// Clear CN interrupt
}

void __attribute__((__interrupt__, no_auto_psv)) _INT2Interrupt(void)
{
    IC1CON1bits.ICM=LAM1;IC2CON1bits.ICM=LAM2;IC3CON1bits.ICM=LAM3;IC4CON1bits.ICM=LAM4;
    INITIAL_DIGITAL_STATES =((PORTB>>10)&0xF)|(_C4OUT<<4); //INITIAL_DIGITAL_STATES =(PORTB>>10)&0xF;
    IC1CON2bits.TRIGSTAT = 1;IC3CON2bits.TRIGSTAT = 1;
    IC2CON2bits.TRIGSTAT = 1;IC4CON2bits.TRIGSTAT = 1;
    INITIAL_DIGITAL_STATES_ERR=((PORTB>>10)&0xF)|(_C4OUT<<4); // =(PORTB>>10)&0xF;

    _INT2IF=0;_INT2IE=0;

}


void __attribute__((__interrupt__, no_auto_psv)) _U2RXInterrupt(void)
{
    asm("CLRWDT");
//while (U1STAbits.UTXBF); //wait for transmit buffer empty
U1TXREG = U2RXREG;
_U2RXIF=0;
}
void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void)
{
    asm("CLRWDT");
while (U2STAbits.UTXBF); //wait for transmit buffer empty
U2TXREG = U1RXREG;
_U1RXIF=0;

}


void configUART2(unsigned int BAUD){
    _TRISB5=0;_TRISB6=1;
    RPOR1bits.RP37R=0x03;RPINR19bits.U2RXR = 38;

    U2MODEbits.STSEL = 0; //1 stop bit
    U2MODEbits.PDSEL = 0; //no parity, 8-data bits
    U2MODEbits.ABAUD = 0; //disable auto-baud
    U2MODEbits.BRGH = 1; //high speed mode

    U2BRG = BAUD;
    
    U2MODEbits.UEN = 0;
    U2MODEbits.RTSMD = 1;

    U2STAbits.URXISEL = 0; //interrupt on 1 char recv

    U2MODEbits.UARTEN = 1; //enable UART
    U2STAbits.UTXEN = 1; //enable UART TX

    U2MODEbits.URXINV = 0;
    while(U2STAbits.URXDA)U2RXREG;
    
}

bool hasChar2(void) {
    return U2STAbits.URXDA;
}

char getChar2(void) {
    if (!hasChar2())return 0;
    if (U2STAbits.FERR == 1) {
        U2RXREG;
        U2STAbits.OERR = 0;
        U2STAbits.FERR = 0;
        return 0;
    }
    return U2RXREG;
}

unsigned int getInt2(void) {
    c1 = getChar2()&0xFF;
    c2 = getChar2()&0xFF;
    return (c2 << 8) | c1;
}

void sendChar2(char val) {
    while (U2STAbits.UTXBF); //wait for transmit buffer empty
    U2TXREG = val;
}

void sendInt2(unsigned int val) {
    while (U2STAbits.UTXBF); //wait for transmit buffer empty
    U2TXREG = val & 0xff;
    while (U2STAbits.UTXBF); //wait for transmit buffer empty
    U2TXREG = (val >> 8)&0xff;
}

void sendAddress2(char address) {   //9-bit mode only
    while (U2STAbits.UTXBF); //wait for transmit buffer empty
    U2TXREG = 0x100 + address; // send the address with the 9th bit set
}


/*----UART 2 on SCL, SDA----------------*/
void initUART2_passthrough(unsigned int BAUD) {
    /*---------UART2 passthrough------------*/
    configUART2(BAUD);
    _U1RXIE =1;  //enable receive interrupt for uart1
    _U2RXIE =1;  //enable receive interrupt for uart2
    
    DELAY_105uS

}

void initADCCTMU(void){
    _AD1IF = 0; _AD1IE = 0;                                             //disable ADC interrupts
    disableADCDMA();DisableComparator();
    AD1CON1bits.ADON = 0;                                               //turn off ADC
    AD1CON2 = 0;
    AD1CON4 = 0x0000;
    AD1CSSH = 0x0000;
    AD1CSSL = 0x0000;
    AD1CON1bits.AD12B = 1;
    /* Assign MUXA inputs for block read */
    AD1CHS0bits.CH0SA = CHOSA;
    AD1CON3bits.ADRC = 0; //do not use internal clock
    AD1CON1bits.SSRCG = 0;
    AD1CON1bits.SSRC = 0b000; //Clearing SAMP bit stops sampling and triggers conversion
    AD1CON1bits.SIMSAM = 0; //simultaneous sampling.
    AD1CON1bits.ASAM = 0; //no auto sampling
    AD1CON3bits.SAMC = 0x10; // Sample for (x+1)*Tad before triggering conversion
    AD1CON2bits.SMPI = 0;
    AD1CON3bits.ADCS = 0xA; // Conversion clock x*Tp
    
}

void set_cap_voltage(BYTE v,unsigned int time){
        _TRISC0=0; _ANSC0=0;
        if(v)_LATC0=1;
        else _LATC0=0; 
        
        //_TRISB3=0;_ANSB3=0;_LATB3=0;
        Delay_us(time);
        _LATC0=0;_TRISC0=1; _ANSC0=1;  Nop();           //Return C0 to high impedance
        //_TRISB3=1;_ANSB3=1;_LATB3=1;

}
void disableCTMUSource(void){
    CTMUCON1bits.CTMUEN = 0;CTMUCON2bits.EDG1STAT = 0;CTMUCON2bits.EDG2STAT = 0; CTMUCON1bits.TGEN = 0; // Stop current source and disable CTMU
}
unsigned int get_cc_capacitance(BYTE current_range,BYTE trimval,unsigned int ChargeTime){
    unsigned int sum=0;
    setADCMode(ADC_CTMU,5,0);   // 5 is the CAP channel
    CTMUCON1bits.TGEN = 1;
    CTMUICONbits.ITRIM = trimval;
    CTMUICONbits.IRNG = current_range;    // 01->Base Range .53uA, 10->base*10, 11->base*100, 00->base*1000
    T5CONbits.TON = 0;  T5CONbits.TGATE = 0;  T5CONbits.TCKPS = 2;   PR5 = ChargeTime;
    set_cap_voltage(0,50000);
    AD1CON1bits.ADON=1;Delay_us(20);
    AD1CON1bits.SAMP = 1; //start sampling
    CTMUCON1bits.CTMUEN = 1;Delay_us(1000);
    CTMUCON1bits.IDISSEN = 1; //Ground the charge PUMP
    Delay_us(1500);  //Grounding the ADC S&H
    
    TMR5 = 0x0000;_T5IF=0;_T5IE=0;
    CTMUCON1bits.IDISSEN = 0; //stop draining the circuit
    CTMUCON2bits.EDG1STAT = 1; //start charging    //Delay_us(ChargeTime); //wait for charge
    T5CONbits.TON = 1;
    while(!_T5IF); _T5IF=0;_AD1IF = 0;
    AD1CON1bits.SAMP = 0;       // Begin analog-to-digital conversion
    CTMUCON2bits.EDG1STAT = 0;  // Stop charging circuit
    
    while(!_AD1IF); _AD1IF = 0;
    while (!AD1CON1bits.DONE);
    sum=(ADC1BUF0)&0xFFF;
    disableCTMUSource();
    AD1CON1bits.ADON=0;
    return sum;

}

unsigned int get_cap_range(unsigned int time){
    unsigned int sum=0;
    set_cap_voltage(0,50000);
    set_cap_voltage(0,50000);
    setADCMode(ADC_12BIT_AVERAGING,5,0);
    T5CONbits.TON = 0; TMR5 = 0x0000; _T5IF=0; PR5 = time;T5CONbits.TCKPS = 2;T5CONbits.TON = 1;
    _TRISC0=0;_LATC0=1; //START CHARGING VIA C0,10K resistor
    while(!_T5IF); _T5IF=0;
    _TRISC0=1;_LATC0=0; //STOP CHARGING.
    sum=get_voltage_summed(5); // This function will turn on the ADC, and turn it off when finished.
    return sum;
}

unsigned int get_ctmu_voltage(BYTE channel,BYTE range,BYTE tgen){
    unsigned int temp=0;
    CTMUCON1bits.TGEN = tgen;                       //(channel==5)?1:0;
    CTMUICONbits.ITRIM = 0;
    CTMUICONbits.IRNG = range;                      // 01->Base Range .53uA, 10->base*10, 11->base*100, 00->base*1000
    CTMUCON2bits.EDG1STAT = 0;
    CTMUCON1bits.CTTRIG = 0; //do not trigger the ADC
    if(channel!=30)CTMUCON2bits.EDG2STAT = 0; 
    else CTMUCON2bits.EDG2STAT = 1;     //30 -> internal temperature.
    CTMUCON1bits.CTMUEN = 1;Delay_us(1000);
    
    CTMUCON1bits.IDISSEN = 1; //Ground the charge PUMP
    Delay_us(1500);  //Grounding the ADC S&H
    CTMUCON1bits.IDISSEN = 0; //stop draining the circuit
    
    CTMUCON1bits.CTMUSIDL = 0; //0=enable operation in idle mode
    CTMUCON2bits.EDG1STAT = 1;  // Start current source

    /*setADCMode(ADC_12BIT_AVERAGING,channel,0);
    AD1CON1bits.ADON = 1; Delay_us(20);   //Turn on the ADC
    AD1CON1bits.ASAM = 1; // auto sampling
    _AD1IF=0;while(!_AD1IF);_AD1IF=0;
    while(!AD1CON1bits.DONE);//wait for conversion
    AD1CON1bits.ASAM = 0; //stop auto sampling
    AD1CON1bits.ADON = 0;*/
    
    temp = get_voltage_summed(channel);
    
    //temp = (ADC1BUF0)+(ADC1BUF1)+(ADC1BUF2)+(ADC1BUF3)+(ADC1BUF4)+(ADC1BUF5)+(ADC1BUF6)+(ADC1BUF7)
    //        +(ADC1BUF8)+(ADC1BUF9)+(ADC1BUFA)+(ADC1BUFB)+(ADC1BUFC)+(ADC1BUFD)+(ADC1BUFE);
    //ADC1BUFF; //ignore this buffer. Gives an occasional error during temperature measurement (TGEN=0);
    //sendInt(ADC1BUF0);sendInt(ADC1BUF1);sendInt(ADC1BUF2);sendInt(ADC1BUF3);sendInt(ADC1BUF4);sendInt(ADC1BUF5);sendInt(ADC1BUF6);sendInt(ADC1BUF7);
    //sendInt(ADC1BUF8);sendInt(ADC1BUF9);sendInt(ADC1BUFA);sendInt(ADC1BUFB);sendInt(ADC1BUFC);sendInt(ADC1BUFD);sendInt(ADC1BUFE);sendInt(ADC1BUFF);

    disableCTMUSource();
    return temp;
    /*
   unsigned int temp=0;
   setADCMode(ADC_12BIT,channel,0);
   AD1CON1bits.ADON=1;Delay_us(20);

    CTMUCON1bits.TGEN = tgen;						//(channel==5)?1:0;
    CTMUICONbits.ITRIM = 0;
    CTMUICONbits.IRNG = range;   					 // 01->Base Range .53uA, 10->base*10, 11->base*100, 00->base*1000
    CTMUCON2bits.EDG1STAT = 0;
    if(channel!=30)CTMUCON2bits.EDG2STAT = 0; 				//30 -> internal temperature.
    else CTMUCON2bits.EDG2STAT = 1;
    CTMUCON1bits.CTMUEN = 1; Delay_us(1000);
    CTMUCON1bits.IDISSEN = 1; 						//Ground the charge PUMP
    Delay_us(1500); 							 //Grounding the ADC S&H
    CTMUCON1bits.IDISSEN = 0; 						 //stop draining the circuit
    AD1CON2bits.SMPI = 15;						  //generate interrupt after x+1 conversions
    CTMUCON1bits.CTMUSIDL = 0; 						 //0=enable operation in idle mode
    CTMUCON2bits.EDG1STAT = 1; 					 	 // Start current source
    _AD1IF=0;
    AD1CON1bits.ASAM = 1; 					 	//start auto sampling
    while(!_AD1IF);_AD1IF=0;
    CTMUCON2bits.EDG1STAT = 0; 					 	 // Stop current source
    AD1CON1bits.ASAM = 0; 					 	//stop auto sampling
    AD1CON2bits.SMPI = 0; 					 	//return to default setting
    ADC1BUF0;
    temp= ADC1BUF1 +ADC1BUF2 +ADC1BUF3 +ADC1BUF4 +ADC1BUF5 +ADC1BUF6 +ADC1BUF7 +
           ADC1BUF8 +ADC1BUF9 +ADC1BUFA +ADC1BUFB +ADC1BUFC +ADC1BUFD +ADC1BUFE +ADC1BUFF ;
    CTMUCON1bits.CTMUEN = 0;
    AD1CON1bits.ADON=0;
    CTMUCON2bits.EDG2STAT = 0;
    CTMUCON1bits.TGEN = 0;
    return temp;
      */ // Old technique
}

void init_IC_for_frequency(BYTE capture_pin,BYTE capture_mode,BYTE captures_per_interrupt){

    RPINR7bits.IC1R = DIN_REMAPS[capture_pin];
    
    IC1CON1bits.ICM=0; //disable the module
    IC2CON1bits.ICM=0; //disable the module
    IC1CON1bits.ICOV=0; //reset overflow flag
    IC2CON1bits.ICOV=0; //reset overflow flag

    IC1CON1bits.ICTSEL = 0b111; //peripheral clock
    IC2CON1bits.ICTSEL = 0b111; //peripheral clock
    IC1CON1bits.ICI = captures_per_interrupt-1; //
    IC2CON1bits.ICI = captures_per_interrupt-1;
    IC1CON2bits.IC32 = 1;
    IC2CON2bits.IC32 = 1;
    IC1CON2bits.TRIGSTAT = 0;
    IC2CON2bits.TRIGSTAT = 0;
    IC1CON2bits.SYNCSEL =0; // no trigger or synchronization
    IC2CON2bits.SYNCSEL =0; // no trigger or synchronization
    IC1CON2bits.ICTRIG = 1;
    IC2CON2bits.ICTRIG = 1;
    INITIAL_DIGITAL_STATES =((PORTB>>10)&0xF)|(_C4OUT<<4); //INITIAL_DIGITAL_STATES =(PORTB>>10)&0xF;
    IC1CON1bits.ICM=capture_mode;
    IC2CON1bits.ICM=capture_mode;

    IC1CON2bits.TRIGSTAT = 1;    IC2CON2bits.TRIGSTAT = 1;

}

void Interval(BYTE capture_pin1,BYTE capture_pin2,BYTE pin1_edge,BYTE pin2_edge){

    RPINR7bits.IC1R = DIN_REMAPS[capture_pin1];
    RPINR8bits.IC3R = DIN_REMAPS[capture_pin2];


    IC1CON1bits.ICM=0;IC3CON1bits.ICM=0; //disable the module
    IC2CON1bits.ICM=0;IC4CON1bits.ICM=0; //disable the module
    IC1CON1bits.ICOV=0;IC3CON1bits.ICOV=0; //reset overflow flag
    IC2CON1bits.ICOV=0;IC4CON1bits.ICOV=0; //reset overflow flag

    IC1CON1bits.ICTSEL = 0b111;IC3CON1bits.ICTSEL = 0b111; //Peripheral clock
    IC2CON1bits.ICTSEL = 0b111;IC4CON1bits.ICTSEL = 0b111; //Peripheral clock
    IC1CON1bits.ICI = 0;IC3CON1bits.ICI = 0; //interrupt after ICI+1 events
    IC1CON2bits.IC32 = 1;IC3CON2bits.IC32 = 1;
    IC2CON2bits.IC32 = 1;IC4CON2bits.IC32 = 1;
    IC1CON2bits.ICTRIG = 0;IC1CON2bits.SYNCSEL =0;// no trigger or synchronization
    IC2CON2bits.ICTRIG = 0;IC2CON2bits.SYNCSEL =0;// no trigger or synchronization

    IC3CON2bits.ICTRIG = 0;IC3CON2bits.SYNCSEL =0;
    IC4CON2bits.ICTRIG = 0;IC4CON2bits.SYNCSEL = 0;
    IC1CON2bits.TRIGSTAT = 0;IC2CON2bits.TRIGSTAT = 0;
    IC3CON2bits.TRIGSTAT = 0;IC4CON2bits.TRIGSTAT = 0;

    //1,2 will capture one type of edge. 3,4 will capture another.
    _IC1IF=0;_IC3IF=0;
    IC1CON1bits.ICM=pin1_edge;IC2CON1bits.ICM=pin1_edge;
    IC3CON1bits.ICM=pin2_edge;IC4CON1bits.ICM=pin2_edge;
    IC1CON2bits.TRIGSTAT = 1;    IC2CON2bits.TRIGSTAT = 1;
    IC3CON2bits.TRIGSTAT = 1;    IC4CON2bits.TRIGSTAT = 1;

}

void TimingMeasurements(BYTE capture_pin1,BYTE capture_pin2,BYTE pin1_edge,BYTE pin2_edge,BYTE interrupts1,BYTE interrupts2){
    _IC1IF=0;_IC3IF=0;
    if(capture_pin1==4) EnableComparator();
    RPINR7bits.IC1R = DIN_REMAPS[capture_pin1];

    if(capture_pin2==4) EnableComparator();
    RPINR8bits.IC3R = DIN_REMAPS[capture_pin2];
    
    IC1CON1bits.ICM=0;IC3CON1bits.ICM=0; //disable the module
    IC2CON1bits.ICM=0;IC4CON1bits.ICM=0; //disable the module
    IC1CON1bits.ICOV=0;IC3CON1bits.ICOV=0; //reset overflow flag
    IC2CON1bits.ICOV=0;IC4CON1bits.ICOV=0; //reset overflow flag

    T2CONbits.TON = 0;                                                          // Stop any 16/32-bit Timer2 operation
    T2CONbits.T32 = 0; T2CONbits.TCS = 0;                                       // Select External clock
    T2CONbits.TCKPS = 0;                                                        // Select Prescaler
    TMR2 = 0x0000; // Clear 16-bit Timer

    IC1CON1bits.ICTSEL = 0b1;IC3CON1bits.ICTSEL = 0b1;                          //Timer2 clock
    IC2CON1bits.ICTSEL = 0b1;IC4CON1bits.ICTSEL = 0b1;                          //Timer2 clock
    IC1CON1bits.ICI = interrupts1-1;IC3CON1bits.ICI = interrupts2-1;              //interrupt after ICI+1 events
    IC1CON2bits.IC32 = 1;IC3CON2bits.IC32 = 1;
    IC2CON2bits.IC32 = 1;IC4CON2bits.IC32 = 1;
    IC1CON2bits.SYNCSEL =0;IC3CON2bits.SYNCSEL =0;                              // no trigger or synchronization
    IC2CON2bits.SYNCSEL =0;IC4CON2bits.SYNCSEL =0;                              // no trigger or synchronization
    IC1CON2bits.ICTRIG = 0;IC3CON2bits.ICTRIG = 0;
    IC2CON2bits.ICTRIG = 0;IC4CON2bits.ICTRIG = 0;
    INITIAL_DIGITAL_STATES =((PORTB>>10)&0xF)|(_C4OUT<<4); //INITIAL_DIGITAL_STATES =(PORTB>>10)&0xF;

    //1,2 will capture one type of edge. 3,4 will capture another.
    IC1CON1bits.ICM=pin1_edge;IC3CON1bits.ICM=pin2_edge;
    IC2CON1bits.ICM=pin1_edge;IC4CON1bits.ICM=pin2_edge;

    IC1CON2bits.TRIGSTAT = 1;    IC2CON2bits.TRIGSTAT = 1;
    IC3CON2bits.TRIGSTAT = 1;    IC4CON2bits.TRIGSTAT = 1;

    T2CONbits.TON = 1; // Start the timer

}

void DisableComparator(){
    CM4CONbits.CON = 0; PMD3bits.CMPMD = 1;                 
}

void EnableComparator(){
        /*----setup comparator---*/
    CVRCONbits.VREFSEL = 0;				// Voltage reference is generated by resistor network
    CVRCONbits.CVREN = 1;				// Enable comparator reference source
    CVRCONbits.CVRR = 0;				// Step size is CVRSRC/32
    CVRCONbits.CVR = 7;		// CVREFIN = (1/4)*(3.3) + (7/32)*(3.3) = 1.54V

    CM4CONbits.CCH = 0;					// VIN - input of comparator connects to C4IN1-
    CM4CONbits.CREF = 1;				// VIN+ input connected to CVRefin voltage source
    CM4CONbits.EVPOL = 1;				// Trigger/Event/Interrupt generated on high to low
    CM4CONbits.CPOL = 0;				// Comparator output is not inverted
    CM4CONbits.CEVT=0;
    CM4CONbits.COE = 1;					// Comparator output is present on CxOUT pin
    CM4FLTRbits.CFSEL = 0;				// Choose CPU instruction clock (TCY)
    CM4FLTRbits.CFDIV = 4;	// Choose comparator filter clock
    CM4FLTRbits.CFLTREN = 1;    			// Digital filter enabled
    CM4MSKSRCbits.SELSRCA = 1;          		// PWM1H1 is the source for Mask A input
    CM4MSKCONbits.HLMS = 1;				// Mask input will prevent comparator output
    CM4MSKCONbits.OAEN = 1;				// OR Gate A input enabled
    CM4CONbits.CON = 1;					// Comparator is enabled
    PMD3bits.CMPMD = 0;                                 // Enable Comparator
}

void __attribute__ ((__interrupt__, no_auto_psv)) _IC4Interrupt(void){
INITIAL_DIGITAL_STATES =((PORTB>>10)&0xF)|(_C4OUT<<4); //INITIAL_DIGITAL_STATES =(PORTB>>10)&0xF;
IC4CON1bits.ICM=0;     //Disable IC4 interrupt
_IC4IF=0;_IC4IE=0;  //disable input capture interrupt
INITIAL_DIGITAL_STATES_ERR =((PORTB>>10)&0xF)|(_C4OUT<<4); //=(PORTB>>10)&0xF;
}

void start_1chan_LA(unsigned int data_points, BYTE channel,BYTE mode,BYTE trigchan){
    _CNIF=0;_CNIE=0;
    DMA0CONbits.CHEN = 0; DMA1CONbits.CHEN = 0;
    IC1CON1bits.ICM=0;  IC2CON1bits.ICM=0; //disable the module
    IC1CON1bits.ICOV=0; IC2CON1bits.ICOV=0; //reset overflow flag
    DMA_MODE=DMA_LA_ONE_CHAN;    
    if(channel==4) EnableComparator();
    RPINR7bits.IC1R = DIN_REMAPS[channel];

    IC1CON1bits.ICTSEL = 0b111;  IC2CON1bits.ICTSEL = 0b111; //peripheral clock
    IC1CON1bits.ICI = 0;       IC2CON1bits.ICI = 0;
    IC1CON2bits.IC32 = 1;      IC2CON2bits.IC32 = 1;
    IC1CON2bits.TRIGSTAT = 0;  IC2CON2bits.TRIGSTAT = 0;
    IC1CON2bits.ICTRIG = 1;    IC2CON2bits.ICTRIG = 1;
    if(trigchan&7){
        if((trigchan>>4)==4) EnableComparator();
        RPINR8bits.IC4R = DIN_REMAPS[(trigchan>>4)&0xF];

        IC4CON1bits.ICTSEL = 0b111;  //peripheral clock
        IC4CON1bits.ICI = 0;
        IC4CON2bits.ICTRIG = 0;
        IC4CON2bits.SYNCSEL =0;
        IC4CON1bits.ICOV=0; //reset overflow flag
        IC1CON2bits.SYNCSEL =0x13;    IC2CON2bits.SYNCSEL =0x13; // trigger using IC4
        }
    else{IC1CON2bits.SYNCSEL =0;    IC2CON2bits.SYNCSEL =0;}  // no trigger or synchronization

    DMA0CONbits.AMODE = 0b00;DMA1CONbits.AMODE = 0b00; // Register indirect with post increment
    DMA0CONbits.MODE = 0b01;DMA1CONbits.MODE = 0b01; // One Shot, Ping-Pong mode Disabled
    DMA0CONbits.DIR = 0;DMA1CONbits.DIR = 0; // Peripheral to RAM

    DMA0PAD = (int)&IC1BUF; DMA1PAD = (int)&IC2BUF; // Address of the capture buffer register
    DMA0REQ = 1;DMA1REQ = 1; // Select IC1 module as DMA request source
    DMA0CNT = data_points-1;DMA1CNT = data_points-1; // Number of words to buffer
    DMA0STAH = __builtin_dmapage (&ADCbuffer);
    DMA0STAL = __builtin_dmaoffset (&ADCbuffer);
    DMA1STAH = __builtin_dmapage ((int*)(ADCbuffer+BUFFER_SIZE/4));
    DMA1STAL = __builtin_dmaoffset ((int*)(ADCbuffer+BUFFER_SIZE/4));

    IC1CON1bits.ICOV=0; IC2CON1bits.ICOV=0; //reset overflow flag
    IC1CON1bits.ICM=mode;IC2CON1bits.ICM=mode;
    _DMA0IF = 0; // Clear the DMA interrupt flag bit
    _DMA0IE = 1; // Enable DMA interrupt enable bit
    DMA0CONbits.CHEN = 1;
    DMA1CONbits.CHEN = 1;
    IC4CON1bits.ICM=trigchan&7;     //mode
    
}

void start_2chan_LA(unsigned int data_points,BYTE modes,BYTE locations){
    DMA0CONbits.CHEN = 0; DMA1CONbits.CHEN = 0;DMA2CONbits.CHEN = 0; DMA3CONbits.CHEN = 0;
    IC1CON1bits.ICM=0;  IC2CON1bits.ICM=0;IC3CON1bits.ICM=0;  IC4CON1bits.ICM=0; //disable the module
    IC1CON1bits.ICOV=0; IC2CON1bits.ICOV=0;IC3CON1bits.ICOV=0; IC4CON1bits.ICOV=0; //reset overflow flag
    DMA_MODE=DMA_LA_TWO_CHAN;
    RPINR7bits.IC1R = DIN_REMAPS[locations&0xF]; //remapping IC1
    RPINR8bits.IC3R = DIN_REMAPS[(locations>>4)&0xF]; //remapping IC2
    IC1CON1bits.ICM=0;  IC2CON1bits.ICM=0;IC3CON1bits.ICM=0;  IC4CON1bits.ICM=0; //disable the module
    IC1CON1bits.ICOV=0; IC2CON1bits.ICOV=0;IC3CON1bits.ICOV=0; IC4CON1bits.ICOV=0; //reset overflow flag

    IC1CON1bits.ICTSEL = 7;IC2CON1bits.ICTSEL = 7;IC3CON1bits.ICTSEL = 7;IC4CON1bits.ICTSEL = 7; //Peripheral. setting for all 4 is important!
    IC1CON1bits.ICI = 0;       IC2CON1bits.ICI = 0; IC3CON1bits.ICI = 0;       IC4CON1bits.ICI = 0;
    IC1CON2bits.IC32 = 1;      IC2CON2bits.IC32 = 1; IC3CON2bits.IC32 = 1;      IC4CON2bits.IC32 = 1;
    IC1CON2bits.TRIGSTAT = 0;  IC2CON2bits.TRIGSTAT = 0;IC3CON2bits.TRIGSTAT = 0;  IC4CON2bits.TRIGSTAT = 0;
    IC1CON2bits.SYNCSEL =0;    IC2CON2bits.SYNCSEL =0;IC3CON2bits.SYNCSEL =0;    IC4CON2bits.SYNCSEL =0;
    IC1CON2bits.ICTRIG = 1;    IC2CON2bits.ICTRIG = 1;IC3CON2bits.ICTRIG = 1;    IC4CON2bits.ICTRIG = 1;

    DMA0CONbits.AMODE = 0b00;DMA1CONbits.AMODE = 0b00;DMA2CONbits.AMODE = 0b00;DMA3CONbits.AMODE = 0b00; // Register indirect with post increment
    DMA0CONbits.MODE = 0b01;DMA1CONbits.MODE = 0b01;DMA2CONbits.MODE = 0b01;DMA3CONbits.MODE = 0b01; // One Shot, Ping-Pong mode Disabled
    DMA0CONbits.DIR = 0;DMA1CONbits.DIR = 0;DMA2CONbits.DIR = 0;DMA3CONbits.DIR = 0; // Peripheral to RAM

    DMA0PAD = (int)&IC1BUF; DMA1PAD = (int)&IC2BUF;DMA2PAD = (int)&IC3BUF; DMA3PAD = (int)&IC4BUF; // Address of the capture buffer register
    DMA0REQ = 1;DMA1REQ = 1; DMA2REQ = 37;DMA3REQ = 37; // Select IC1+IC2, IC3+IC4(00100101) modules as DMA request source
    DMA0CNT = data_points-1;DMA1CNT = data_points-1;DMA2CNT = data_points-1;DMA3CNT = data_points-1; // Number of words to buffer
    DMA0STAH = __builtin_dmapage (&ADCbuffer);      DMA2STAH = __builtin_dmapage ((int*)(ADCbuffer+BUFFER_SIZE/2));
    DMA0STAL = __builtin_dmaoffset (&ADCbuffer);    DMA2STAL = __builtin_dmaoffset ((int*)(ADCbuffer+BUFFER_SIZE/2));
    DMA1STAH = __builtin_dmapage ((int*)(ADCbuffer+BUFFER_SIZE/4)); DMA3STAH = __builtin_dmapage ((int*)(ADCbuffer+3*BUFFER_SIZE/4));
    DMA1STAL = __builtin_dmaoffset ((int*)(ADCbuffer+BUFFER_SIZE/4)); DMA3STAL = __builtin_dmaoffset ((int*)(ADCbuffer+3*BUFFER_SIZE/4));

    _DMA0IF = 0; // Clear the DMA interrupt flag bit
    _DMA0IE = 1; // Enable DMA interrupt enable bit
    DMA0CONbits.CHEN = 1;DMA2CONbits.CHEN = 1;
    DMA1CONbits.CHEN = 1;DMA3CONbits.CHEN = 1;
    //IC1CON1bits.ICM=modes&0xF;IC2CON1bits.ICM=modes&0xF;IC3CON1bits.ICM=(modes>>4)&0xF;IC4CON1bits.ICM=(modes>>4)&0xF;
    LAM1=modes&0xF;LAM2=modes&0xF;LAM3=(modes>>4)&0xF;LAM4=(modes>>4)&0xF;   //TESTING  A NEW  APPROACH

}

void start_3chan_LA(unsigned int data_points,unsigned int modes , BYTE trigchan){
    DMA0CONbits.CHEN = 0; DMA1CONbits.CHEN = 0;DMA2CONbits.CHEN = 0; DMA3CONbits.CHEN = 0;
    IC1CON1bits.ICM=0;  IC2CON1bits.ICM=0;IC3CON1bits.ICM=0;  IC4CON1bits.ICM=0; //disable the module
    IC1CON1bits.ICOV=0; IC2CON1bits.ICOV=0;IC3CON1bits.ICOV=0; IC4CON1bits.ICOV=0; //reset overflow flag

    DMA_MODE=DMA_LA_FOUR_CHAN;
    RPINR7bits.IC1R = ID1_REMAP; //remapping IC1 to ID1;
    RPINR7bits.IC2R = ID2_REMAP; //remapping IC1 to ID2;
    RPINR8bits.IC3R = ID3_REMAP;


    IC1CON1bits.ICM=0;  IC2CON1bits.ICM=0;IC3CON1bits.ICM=0;  IC4CON1bits.ICM=0; //disable the module
    IC1CON1bits.ICOV=0; IC2CON1bits.ICOV=0;IC3CON1bits.ICOV=0; IC4CON1bits.ICOV=0; //reset overflow flag

    IC1CON1bits.ICTSEL = 0b111;  IC2CON1bits.ICTSEL = 0b111; IC3CON1bits.ICTSEL = 0b111;  IC4CON1bits.ICTSEL = 0b111; //Fp
    IC1CON1bits.ICI = 0;       IC2CON1bits.ICI = 0; IC3CON1bits.ICI = 0;       IC4CON1bits.ICI = 0;
    IC1CON2bits.IC32 = 0;      IC2CON2bits.IC32 = 0; IC3CON2bits.IC32 = 0;      IC4CON2bits.IC32 = 0;
    IC1CON2bits.TRIGSTAT = 0;  IC2CON2bits.TRIGSTAT = 0;IC3CON2bits.TRIGSTAT = 0;  IC4CON2bits.TRIGSTAT = 0;
    IC1CON2bits.ICTRIG = 1;    IC2CON2bits.ICTRIG = 1;IC3CON2bits.ICTRIG = 1; 

    if(trigchan==0){IC1CON2bits.SYNCSEL =0;    IC2CON2bits.SYNCSEL =0; IC3CON2bits.SYNCSEL =0;}  // no trigger or synchronization
    else if(trigchan&7){
        RPINR8bits.IC4R = DIN_REMAPS[(trigchan>>4)&0xF];

        IC4CON2bits.ICTRIG = 0;
        IC4CON2bits.SYNCSEL =0;
        IC1CON2bits.SYNCSEL =0x13;    IC2CON2bits.SYNCSEL =0x13; IC3CON2bits.SYNCSEL =0x13; // trigger using IC4
        }



    DMA0CONbits.AMODE = 0b00;DMA1CONbits.AMODE = 0b00;DMA2CONbits.AMODE = 0b00; // Register indirect with post increment
    DMA0CONbits.MODE = 0b01;DMA1CONbits.MODE = 0b01;DMA2CONbits.MODE = 0b01; // One Shot, Ping-Pong mode Disabled
    DMA0CONbits.DIR = 0;DMA1CONbits.DIR = 0;DMA2CONbits.DIR = 0; // Peripheral to RAM

    DMA0PAD = (int)&IC1BUF; DMA1PAD = (int)&IC2BUF;DMA2PAD = (int)&IC3BUF;  // Address of the capture buffer register
    DMA0REQ = 1;DMA1REQ = 5; DMA2REQ = 37; // Select IC1,IC2, IC3 modules as DMA request source
    DMA0CNT = data_points-1;DMA1CNT = data_points-1;DMA2CNT = data_points-1; // Number of words to buffer
    DMA0STAH = __builtin_dmapage (&ADCbuffer);      DMA1STAH = __builtin_dmapage ((int*)(ADCbuffer+BUFFER_SIZE/4));
    DMA0STAL = __builtin_dmaoffset (&ADCbuffer);    DMA1STAL = __builtin_dmaoffset ((int*)(ADCbuffer+BUFFER_SIZE/4));
    DMA2STAH = __builtin_dmapage ((int*)(ADCbuffer+BUFFER_SIZE/2)); 
    DMA2STAL = __builtin_dmaoffset ((int*)(ADCbuffer+BUFFER_SIZE/2));

    _DMA0IF = 0; // Clear the DMA interrupt flag bit
    _DMA0IE = 1; // Enable DMA interrupt enable bit
    DMA0CONbits.CHEN = 1;DMA2CONbits.CHEN = 1;
    DMA1CONbits.CHEN = 1;
    IC1CON1bits.ICM=modes&0xF;IC2CON1bits.ICM=(modes>>4)&0xF;IC3CON1bits.ICM=(modes>>8)&0xF;
    IC4CON1bits.ICM=trigchan&7;     //trigger via IC4

}

void start_4chan_LA(unsigned int data_points,unsigned int modes , BYTE scale){
    DMA0CONbits.CHEN = 0; DMA1CONbits.CHEN = 0;DMA2CONbits.CHEN = 0; DMA3CONbits.CHEN = 0;
    IC1CON1bits.ICM=0;  IC2CON1bits.ICM=0;IC3CON1bits.ICM=0;  IC4CON1bits.ICM=0; //disable the module
    IC1CON1bits.ICOV=0; IC2CON1bits.ICOV=0;IC3CON1bits.ICOV=0; IC4CON1bits.ICOV=0; //reset overflow flag

    DMA_MODE=DMA_LA_FOUR_CHAN;
    RPINR7bits.IC1R = ID1_REMAP; //remapping IC1 to ID1;
    RPINR7bits.IC2R = ID2_REMAP; //remapping IC1 to ID2;
    RPINR8bits.IC3R = ID3_REMAP;
    RPINR8bits.IC4R = ID4_REMAP;
    T2CONbits.TON = 0; // Stop any 16/32-bit Timer2 operation
    T2CONbits.T32 = 0; T2CONbits.TCS = 0; // Select External clock
    T2CONbits.TCKPS = scale&0xF; // Select Prescaler
    TMR2 = 0x0001; // Init 16-bit Timer
    

    IC1CON1bits.ICM=0;  IC2CON1bits.ICM=0;IC3CON1bits.ICM=0;  IC4CON1bits.ICM=0; //disable the module
    IC1CON1bits.ICOV=0; IC2CON1bits.ICOV=0;IC3CON1bits.ICOV=0; IC4CON1bits.ICOV=0; //reset overflow flag

    IC1CON1bits.ICTSEL = 0b1;  IC2CON1bits.ICTSEL = 0b1; IC3CON1bits.ICTSEL = 0b1;  IC4CON1bits.ICTSEL = 0b1; //TIMER2
    IC1CON1bits.ICI = 0;       IC2CON1bits.ICI = 0; IC3CON1bits.ICI = 0;       IC4CON1bits.ICI = 0;
    IC1CON2bits.IC32 = 0;      IC2CON2bits.IC32 = 0; IC3CON2bits.IC32 = 0;      IC4CON2bits.IC32 = 0;
    IC1CON2bits.TRIGSTAT = 0;  IC2CON2bits.TRIGSTAT = 0;IC3CON2bits.TRIGSTAT = 0;  IC4CON2bits.TRIGSTAT = 0;
    IC1CON2bits.SYNCSEL =0;    IC2CON2bits.SYNCSEL =0;IC3CON2bits.SYNCSEL =0;    IC4CON2bits.SYNCSEL =0;
    IC1CON2bits.ICTRIG = 1;    IC2CON2bits.ICTRIG = 1;IC3CON2bits.ICTRIG = 1;    IC4CON2bits.ICTRIG = 1;

    DMA0CONbits.AMODE = 0b00;DMA1CONbits.AMODE = 0b00;DMA2CONbits.AMODE = 0b00;DMA3CONbits.AMODE = 0b00; // Register indirect with post increment
    DMA0CONbits.MODE = 0b01;DMA1CONbits.MODE = 0b01;DMA2CONbits.MODE = 0b01;DMA3CONbits.MODE = 0b01; // One Shot, Ping-Pong mode Disabled
    DMA0CONbits.DIR = 0;DMA1CONbits.DIR = 0;DMA2CONbits.DIR = 0;DMA3CONbits.DIR = 0; // Peripheral to RAM

    DMA0PAD = (int)&IC1BUF; DMA1PAD = (int)&IC2BUF;DMA2PAD = (int)&IC3BUF; DMA3PAD = (int)&IC4BUF; // Address of the capture buffer register
    DMA0REQ = 1;DMA1REQ = 5; DMA2REQ = 37;DMA3REQ = 38; // Select IC1,IC2, IC3,IC4 modules as DMA request source
    DMA0CNT = data_points-1;DMA1CNT = data_points-1;DMA2CNT = data_points-1;DMA3CNT = data_points-1; // Number of words to buffer
    DMA0STAH = __builtin_dmapage (&ADCbuffer);      DMA1STAH = __builtin_dmapage ((int*)(ADCbuffer+BUFFER_SIZE/4));
    DMA0STAL = __builtin_dmaoffset (&ADCbuffer);    DMA1STAL = __builtin_dmaoffset ((int*)(ADCbuffer+BUFFER_SIZE/4));
    DMA2STAH = __builtin_dmapage ((int*)(ADCbuffer+BUFFER_SIZE/2)); DMA3STAH = __builtin_dmapage ((int*)(ADCbuffer+3*BUFFER_SIZE/4));
    DMA2STAL = __builtin_dmaoffset ((int*)(ADCbuffer+BUFFER_SIZE/2)); DMA3STAL = __builtin_dmaoffset ((int*)(ADCbuffer+3*BUFFER_SIZE/4));

    _DMA0IF = 0; // Clear the DMA interrupt flag bit
    _DMA0IE = 1; // Enable DMA interrupt enable bit
    DMA0CONbits.CHEN = 1;DMA2CONbits.CHEN = 1;
    DMA1CONbits.CHEN = 1;DMA3CONbits.CHEN = 1;
    IC1CON1bits.ICM=modes&0xF;IC2CON1bits.ICM=(modes>>4)&0xF;IC3CON1bits.ICM=(modes>>8)&0xF;IC4CON1bits.ICM=(modes>>12)&0xF;
    
}

void setMultiFuncPortMode(BYTE mode){
    if(MULTIFUNC_PORT == mode)return;
    else{
        MULTIFUNC_PORT = mode;
        if(mode == MULTIFUNC_I2C){initI2C();}
        else if(mode == MULTIFUNC_UART){}
        else if(mode == MULTIFUNC_NONSTANDARD){_TRISB4=0;_TRISA8=1;RPINR8bits.IC3R = 0;RPINR8bits.IC4R = 0;}


    }
}

void disable_input_capture(){
    IC1CON2bits.TRIGSTAT = 0;  IC2CON2bits.TRIGSTAT = 0;IC3CON2bits.TRIGSTAT = 0;  IC4CON2bits.TRIGSTAT = 0;
    IC1CON1bits.ICM=0;  IC2CON1bits.ICM=0;IC3CON1bits.ICM=0;  IC4CON1bits.ICM=0; //disable the module
    IC1CON1bits.ICOV=0; IC2CON1bits.ICOV=0;IC3CON1bits.ICOV=0; IC4CON1bits.ICOV=0; //reset overflow flag
    /*
    _DMA0IF = 0; // Clear the DMA interrupt flag bit
    _DMA0IE = 0; // Disable DMA interrupt enable bit
    DMA0CONbits.CHEN = 0;
    DMA1CONbits.CHEN = 0;
    DMA2CONbits.CHEN = 0;
    DMA3CONbits.CHEN = 0;
     * */
}

void alternate_get_high_frequency(BYTE channel,BYTE scale){ //Measure freq using only input captures. Timer 3 not available

    RPINR7bits.IC1R = 0;    RPINR7bits.IC2R = 0;
    T2CONbits.TON=0;
    T2CONbits.T32=0;
    T2CONbits.TCS = 1; // Select External clock
    T2CONbits.TCKPS = scale; // Select Prescaler

    if(channel==4) EnableComparator();
    RPINR3bits.T2CKR = DIN_REMAPS[channel];


    IC1CON1bits.ICM=0;  IC2CON1bits.ICM=0; //disable the module
    IC1CON1bits.ICTSEL = 1;  IC2CON1bits.ICTSEL = 1; //timer 2
    IC1CON1bits.ICI = 1;       IC2CON1bits.ICI = 1;
    IC1CON1bits.ICOV=0; IC2CON1bits.ICOV=0; //reset overflow flag
    IC1CON2bits.IC32 = 1;      IC2CON2bits.IC32 = 1;
    IC1CON2bits.TRIGSTAT = 0;  IC2CON2bits.TRIGSTAT = 0;
    IC1CON2bits.ICTRIG = 1;    IC2CON2bits.ICTRIG = 1;
    IC1CON2bits.SYNCSEL =0;    IC2CON2bits.SYNCSEL = 0;  //no sync

    
    T5CONbits.TON = 0; // Stop any 16/32-bit Timer5 operation
    T5CONbits.TCKPS = 3; //1:256 , 1.0/8 MHz
    PR5=25000;   //100mS sampling
    TMR5=0x0000;

    _T5IP = 0x01;// Set Timer5 Interrupt Priority Level
    _T5IF = 0; // Clear Timer5 Interrupt Flag
    TMR2 = 0x0000;
    T2CONbits.TON=1;

    IC2CON1bits.ICM=EVERY_SIXTEENTH_RISING_EDGE;  IC1CON1bits.ICM=EVERY_SIXTEENTH_RISING_EDGE;
    T5CONbits.TON = 1; // Start 16-bit Timer
    IC2CON2bits.TRIGSTAT = 1;IC1CON2bits.TRIGSTAT = 1;  
}

void startCounting(BYTE channel){
    T2CONbits.TON=0;T2CONbits.T32=0;
    T2CONbits.TCS = 1; // Select External clock
    if(channel==4) EnableComparator();
    RPINR3bits.T2CKR = DIN_REMAPS[channel];
    TMR2 = 0x0000;
    T2CONbits.TON=1;    
}
void get_high_frequency(BYTE channel,BYTE scale){ //T2CK is tied to ID1. Using timer 3/2

    if(channel==4) EnableComparator();
    RPINR3bits.T2CKR = DIN_REMAPS[channel];

    T3CONbits.TON = 0; // Stop any 16-bit Timer3 operation
    T2CONbits.TON = 0; // Stop any 16/32-bit Timer2 operation
    T5CONbits.TON = 0; // Stop any 16/32-bit Timer5 operation

    T2CONbits.T32 = 1;      // 32 bit mode T2 and T3
    T2CONbits.TCS = 1; // Select External clock
    T2CONbits.TCKPS = scale; // Select Prescaler
    IFS0bits.T3IF = 0;
    T5CONbits.TCKPS = 3; //1:256 , 1.0/8 MHz
    PR5=25000;   //100mS sampling
    TMR3HLD=0;
    TMR3 = 0x0000; // Clear 32-bit Timer (msw)
    TMR2 = 0x0000; // Clear 32-bit Timer (lsw)
    TMR5=0x0000;


    _T5IP = 0x01; // Set Timer5 Interrupt Priority Level
    _T5IF = 0; // Clear Timer5 Interrupt Flag
    //_T5IE = 1; // Enable Timer5 interrupt
    T2CONbits.TON = 1; // Start 32-bit Timer
    T5CONbits.TON = 1; // Start 16-bit Timer

}

void setSPIMode(BYTE mode){
    if(SPI_MODE == mode)return;
    else{
        SPI_MODE = mode;
        initSPI();
    }
}

void initSPI(){

    SPI1STAT = 0; //disable SPI
    _TRISB5 = 0;     _LATB5=1;                   //CSOUT-1
    _TRISB6 = 0;     _LATB6=1;                   //CSOUT-2  
    IFS0bits.SPI1IF = 0; // Clear the Interrupt flag
    IEC0bits.SPI1IE = 0; // Disable the interrupt

    SPI1CON1bits.PPRE = SPI_PPRE; //primary prescale 64:1
    SPI1CON1bits.SPRE = SPI_SPRE; //secondary prescale 7:1
    SPI1CON1bits.DISSCK = 0; //enable internal clock

    SPI1CON1bits.DISSDO = 0; //SDO controlled by module.
    SPI1CON1bits.SSEN = 0; // CS not used
    if(SPI_MODE==SPI_8)SPI1CON1bits.MODE16 = 0; //0=8,1=16 bits wide data
    else SPI1CON1bits.MODE16 = 1; //0=8,1=16 bits wide data
    SPI1CON1bits.CKE = SPI_CKE; //  Read data on rising edge of SCK; //SDO does not keep state
    SPI1CON1bits.CKP = SPI_CKP; //Clock idle state
    SPI1CON1bits.MSTEN = 1; //enable master mode

    SPI1CON1bits.SMP = SPI_SMP;

    SPI1STATbits.SPIEN = 1; //enable SPI1
}

BYTE spi_write8(BYTE value) {
    setSPIMode(SPI_8);
    SPI1STATbits.SPIROV = 0;
    SPI1BUF = value;
    while (SPI1STATbits.SPITBF); // wait for the data to be sent out
    while (!SPI1STATbits.SPIRBF); // wait for dummy byte to clock in
    return SPI1BUF&0xFF;
}

unsigned int spi_write16(unsigned int value) {
    setSPIMode(SPI_16);
    SPI1STATbits.SPIROV = 0;
    SPI1BUF = value;
    while (SPI1STATbits.SPITBF); // wait for the data to be sent out
    while (!SPI1STATbits.SPIRBF); // wait for dummy byte to clock in
    return SPI1BUF; // dummy read of the SPI1BUF register to clear the SPIRBF flag
}

void set_CS(BYTE channel,BYTE status){

    if (channel == CSNUM_A1)CSCH1 = status;
    else if (channel == CSNUM_A2)CSCH2  =   status;
    else if (channel == CSNUM_CS1)CS1   =   status;
    else if (channel == CSNUM_CS2)CS2   =   status;
    else if (channel == CSNUM_OUT1)CSOUT1   =   status;
    else if (channel == CSNUM_OUT2)CSOUT2   =   status;

}

void setPGA(char PGAnum, char gain) {
    set_CS(PGAnum,0);
    spi_write16(0x4000 | gain);
    set_CS(PGAnum,1);
}

void setSensorChannel(char channel) {
    //  Sensor inputs are located on PGA 5
    set_CS(CSNUM_A1,0);
    spi_write16(0x4100 | channel);
    set_CS(CSNUM_A1,1);
}

void delayTMR4(int n){
    _T4IF=0;
    PR4=n;
    while(!_T4IF);
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
    _TRISC6 = 0;    _TRISC7 = 0;    _TRISC8 = 0;    _TRISC9 = 0;


    //ADC inputs.
    _TRISA0 = 1; _TRISA1 = 1;
    _TRISB1 = 1;_TRISB0=1; 
    _TRISC1=1;_TRISC2=1;

    
    
    _TRISB10 = 1; //ID1
    _TRISB11 = 1; //ID2
    _TRISB12 = 1; //ID3
    _TRISB13 = 1; //ID4
    _TRISB14 = 1; //Frequency input
    _TRISB9 = 1;  //Expansion slot digital input

    _TRISB2 = 1; _ANSB2=0; //RB2/ NRF interrupt AN4
    _INT1IF=0;_INT1IE=0; //disable this interrupt unless something comes up
    //------------Initialize SPI pins for DAC, PGAs -------------------------
    
    _TRISA10 = 0;    _LATA10 = 1;                //CS-1 kept high.
    _TRISA7 = 0;     _LATA7 = 1;                 //CS-2 kept high.

    _TRISB5 = 0;     _LATB5=1;                   //CSOUT-1
    _TRISB6 = 0;     _LATB6=1;                   //CSOUT-2  
    _TRISB15 = 0;    _LATB15 = 0;                // LEDPIN
    
    _TRISC4 = 0;     _LATC4 = 1;                 //CS1 kept high.
    _TRISC5 = 0;     _LATC5 = 1;                 //CS2 kept high.
    

    _TRISA4 = 0; //SDO1
    _ANSA4 = 0; //make A4 digital out
    _TRISC3 = 0; //SCK1 output
    _TRISA9 = 1; //SDI input (RA9)

    setSPIMode(SPI_16);
    //-------CTMU input PIN RP35 (C1IN1-)--------
    // Output pin for constant voltage mode capacitance measurement
    _TRISC0=1; _ANSC0=0;_LATC0=0;
     _TRISB3=1; _ANSB3=1; //SET B3 as an input pin in analog mode

    _U1RXIF=0;_U1RXIE =0;  //disable receive interrupt for uart1
    _U2RXIF=0;_U2RXIE =0;  //disable receive interrupt for uart2

    disableCTMUSource();
    configureADC();
    
}

void setFlashPointer(BYTE location){
    if(location<20){
        _init_prog_address(p, dat1[0]);
        p+=0x800*location;
    }

}

void sqr1(unsigned int wavelength,unsigned int high_time,BYTE scaling) {
    /*-----------------------square wave output-----------------*/
    OC1R = high_time-1;
    PR1 = wavelength-1;

    OC1CON1 = 6; //Edge aligned PWM
    OC1CON1bits.OCTSEL = 4;
    T1CONbits.TCKPS = scaling&0x3;
    OC1CON2 = 11; //01011 = Timer1 synchronizes or triggers OCx (default)
    T1CONbits.TON = 1;

    if((scaling&0x4)==0)RPOR5bits.RP54R = 0x10; //square wave pin(RC6) mapped to 0b010001 (output compare 1 )
    /*-----------------------square wave output-----------------*/

}

void sineWave1(unsigned int wavelength,BYTE highres){
    /*-----------------------sine wave output-----------------*/
    T4CONbits.TON= 0;
    _DMA2IF = 0;    _DMA2IE = 0;
    DMA2CONbits.CHEN = 0;
    if(highres&1){
        OC3R =HIGH_RES_WAVE>>1;
        OC3RS = HIGH_RES_WAVE;
    }else{
        OC3R =LOW_RES_WAVE>>1;
        OC3RS = LOW_RES_WAVE;        
    }
    OC3CON2 = 0;
    OC3CON1 = 6; //Edge aligned PWM
    OC3CON2bits.SYNCSEL = 0x1F;  //OCRS compare used for sync
    OC3CON1bits.OCTSEL = 7;     //Fp used as clock

    
    DMA2CONbits.AMODE = 0; // Configure DMA for Register Indirect mode with post-increment
    DMA2CONbits.SIZE = 0;  //word transfer
    DMA2CONbits.MODE = 0; // Configure DMA for Continuous mode
    DMA2CONbits.DIR = 1; // RAM-to-Peripheral data transfers
    DMA2PAD = (volatile unsigned int)&OC3R; // Point DMA to OC3R
    DMA2REQ = 0b11011;    //timer 4 triggers DMA


    if(highres&1){
        DMA2STAH = __builtin_dmapage (&sineTable1);
        DMA2STAL = __builtin_dmaoffset (&sineTable1);
        DMA2CNT = WAVE_TABLE_FULL_LENGTH-1; // total table size -1/  DMA requests
    }else{
        DMA2STAH = __builtin_dmapage (&sineTable1_short);
        DMA2STAL = __builtin_dmaoffset (&sineTable1_short);
        DMA2CNT = WAVE_TABLE_SHORT_LENGTH-1; // total table size -1/  DMA requests
    }


    _DMA2IF = 0;    _DMA2IE = 1;
    DMA2CONbits.CHEN = 1;

    T4CONbits.TCKPS = (highres>>1)&3;
    PR4 = wavelength;
    T4CONbits.TON= 1;
    
    RPOR6bits.RP57R = 0x12;  //Sine 1 Mapping output to square wave 4 SQR4
}
void sineWave2(unsigned int wavelength,BYTE highres){
    /*-----------------------sine wave output-----------------*/
    T3CONbits.TON= 0; T2CONbits.T32=0;
    _DMA3IF = 0;    _DMA3IE = 0;
    DMA3CONbits.CHEN = 0;
    
    if(highres&1){
        OC4R =HIGH_RES_WAVE>>1;
        OC4RS = HIGH_RES_WAVE;
    }else{
        OC4R =LOW_RES_WAVE>>1;
        OC4RS = LOW_RES_WAVE;        
    }

    OC4CON2 = 0;
    OC4CON1 = 6; //Edge aligned PWM
    OC4CON2bits.SYNCSEL = 0x1F;  //OCRS compare used for sync
    OC4CON1bits.OCTSEL = 7;     //Fp used as clock

    
    DMA3CONbits.AMODE = 0; // Configure DMA for Register Indirect mode with post-increment
    DMA3CONbits.SIZE = 0;  //word transfer
    DMA3CONbits.MODE = 0; // Configure DMA for Continuous mode
    DMA3CONbits.DIR = 1; // RAM-to-Peripheral data transfers
    DMA3PAD = (volatile unsigned int)&OC4R; // Point DMA to OC4R
    DMA3REQ = 0b1000;    //timer 3 triggers DMA

    if(highres&1){
        DMA3CNT = WAVE_TABLE_FULL_LENGTH-1; // total table size -1/  DMA requests
        DMA3STAH = __builtin_dmapage (&sineTable2);
        DMA3STAL = __builtin_dmaoffset (&sineTable2);
    }else{
        DMA3CNT = WAVE_TABLE_SHORT_LENGTH-1; // total table size -1/  DMA requests
        DMA3STAH = __builtin_dmapage (&sineTable2_short);
        DMA3STAL = __builtin_dmaoffset (&sineTable2_short);
    }

    _DMA3IF = 0;    _DMA3IE = 1;
    DMA3CONbits.CHEN = 1;

    T3CONbits.TCKPS = (highres>>1)&3;
    PR3 = wavelength;
    T3CONbits.TON= 1;
    
    RPOR6bits.RP56R = 0x13;  //Sine 2 Mapping output to square wave 3 SQR3
}

void setSineWaves(unsigned int wavelength1,unsigned int wavelength2,unsigned int pos,unsigned int tmr_delay,BYTE highres){
    /*-----------------------sine wave output-----------------*/
    T3CONbits.TON= 0;T2CONbits.T32= 0; T4CONbits.TON= 0;
    _DMA3IF = 0;    _DMA3IE = 0;_DMA2IF = 0;    _DMA2IE = 0;
    DMA2CONbits.CHEN = 0;DMA3CONbits.CHEN = 0;
    
    if(highres&1){        //highres = (prescaler1<<4)|(prescaler2<<2)|(HIGHRES2<<1)|(HIGHRES)
        OC3R =HIGH_RES_WAVE>>1;
        OC3RS = HIGH_RES_WAVE;
    }else{
        OC3R =LOW_RES_WAVE>>1;
        OC3RS = LOW_RES_WAVE;        
    }

    
    if(highres&2){
        OC4R =HIGH_RES_WAVE>>1;
        OC4RS = HIGH_RES_WAVE;
    }else{
        OC4R =LOW_RES_WAVE>>1;
        OC4RS = LOW_RES_WAVE;        
    }
    
    
    
    OC3CON2 = 0;
    OC3CON1 = 6; //Edge aligned PWM
    OC4CON2 = 0;
    OC4CON1 = 6; //Edge aligned PWM
    OC3CON2bits.SYNCSEL = 0x1F;  //OCRS compare used for sync
    OC3CON1bits.OCTSEL = 7;     //Fp used as clock
    OC4CON2bits.SYNCSEL = 0x1F;  //OCRS compare used for sync
    OC4CON1bits.OCTSEL = 7;     //Fp used as clock

    

    DMA2CONbits.AMODE = 0; // Configure DMA for Register Indirect mode with post-increment
    DMA2CONbits.SIZE = 0;  //word transfer
    DMA2CONbits.MODE = 0; // Configure DMA for Continuous mode
    DMA2CONbits.DIR = 1; // RAM-to-Peripheral data transfers
    DMA2PAD = (volatile unsigned int)&OC3R; // Point DMA2 to OC3R
    DMA2REQ = 0b11011;    //Timer 4 requests DMA
    
    
    DMA3CONbits.AMODE = 0; // Configure DMA for Register Indirect mode with post-increment
    DMA3CONbits.SIZE = 0;  //word transfer
    DMA3CONbits.MODE = 0; // Configure DMA for Continuous mode
    DMA3CONbits.DIR = 1; // RAM-to-Peripheral data transfers
    DMA3PAD = (volatile unsigned int)&OC4R; // Point DMA3 to OC4R
    DMA3REQ = 0b1000;    //Timer 3 requests DMA

    if(highres&1){
        DMA2CNT = WAVE_TABLE_FULL_LENGTH-1; // total table size -1/  DMA requests
        DMA2STAH = __builtin_dmapage (&sineTable1);
        DMA2STAL = __builtin_dmaoffset (&sineTable1);
    }else{
        DMA2CNT = WAVE_TABLE_SHORT_LENGTH-1; // total table size -1 [  DMA requests]
        DMA2STAH = __builtin_dmapage (&sineTable1_short);
        DMA2STAL = __builtin_dmaoffset (&sineTable1_short);
    }

    
    if(highres&2){
        DMA3CNT = WAVE_TABLE_FULL_LENGTH-1; // total table size -1/  DMA requests
        DMA3STAH = __builtin_dmapage (&sineTable2);
        DMA3STAL = __builtin_dmaoffset (&sineTable2);
    }else{
        DMA3CNT = WAVE_TABLE_SHORT_LENGTH-1; // total table size -1 [  DMA requests]
        DMA3STAH = __builtin_dmapage (&sineTable2_short);
        DMA3STAL = __builtin_dmaoffset (&sineTable2_short);
    }
    

    _DMA2IF = 0;    _DMA2IE = 1;_DMA3IF = 0;    _DMA3IE = 1;
    DMA2CONbits.CHEN = 1;DMA3CONbits.CHEN = 1;

    for(i=0;i<pos;i++){DMA3REQbits.FORCE=1;while(DMA3REQbits.FORCE);} //skip these many points for wave #2
    T3CONbits.TCKPS = (highres>>2)&3;T4CONbits.TCKPS = (highres>>4)&3;
    PR4 = wavelength1;PR3 = wavelength2;
    TMR4 = 0;
    TMR3=tmr_delay;

    RPOR6bits.RP56R = 0x13;  //Sine 2 Mapping output to square wave 3 SQR3
    RPOR6bits.RP57R = 0x12;  //Sine 1 Mapping output to square wave 4 SQR4

    T3CONbits.TON= 1;T4CONbits.TON= 1;
    
}

void sqr2(unsigned int wavelength,unsigned int high_time,BYTE scaling){
    /*-----------------------square wave output-----------------*/
    OC4R = high_time-1;
    PR3 = wavelength-1;
    OC4CON1 = 6; //Edge aligned PWM
    OC4CON1bits.OCTSEL = 1;
    T4CONbits.TCKPS = scaling;
    OC4CON2 = 13; //01101 = Timer3 synchronizes or triggers OCx (default)
    T4CONbits.TON= 1;

    RPOR5bits.RP55R = 0x13;  //square wave pin(RC7) mapped to 0x010011 (output compare 4 )
    /*-----------------------square wave output-----------------*/

}


void mapReferenceOscillator(BYTE channel,BYTE scaler){
    /*--------------External clock monitoring, RP55/RC7  ----------*/
    REFOCONbits.ROON = 0;
    REFOCONbits.ROSEL = 0;
    REFOCONbits.RODIV = scaler;

    if(channel&1)RPOR5bits.RP54R = 0x31;
    if(channel&2)RPOR5bits.RP55R = 0x31;
    if(channel&4)RPOR6bits.RP56R = 0x31;
    if(channel&8)RPOR6bits.RP57R = 0x31;
    if(channel&16){
        _LATB9=0 ; //set B9 as output. 
        RPOR3bits.RP41R = 0x31; 
    }
    REFOCONbits.ROON = 1;   
}

void sqr4(uint16 w,uint16 R0,uint16 R1,uint16 RS1,uint16 R2,uint16 RS2,uint16 R3,uint16 RS3,BYTE scaling){
    /*-----------------------square wave output-----------------*/
    _DMA2IF = 0;    _DMA2IE = 0; _DMA3IF = 0;    _DMA3IE = 0;
    DMA2CONbits.CHEN = 0;DMA3CONbits.CHEN = 0;

    T1CONbits.TON = 0;
    if((scaling>>6)&1){
        EnableComparator();
    OC1CON2 = 0x19; OC1CON2bits.OCTRIG = 1; //COMP2 triggers
    OC2CON2 = 0x19; OC2CON2bits.OCTRIG = 1;
    OC3CON2 = 0x19; OC3CON2bits.OCTRIG = 1;
    OC4CON2 = 0x19; OC4CON2bits.OCTRIG = 1;

    }else{
    OC1CON2 = 11; //01011 = Timer1 synchronizes or triggers OCx (default)
    OC2CON2 = 11; //01011 = Timer1 synchronizes or triggers OCx (default)
    OC3CON2 = 11; //01011 = Timer1 synchronizes or triggers OCx (default)
    OC4CON2 = 11; //01011 = Timer1 synchronizes or triggers OCx (default)
    }
    PR1 = w;
    OC1R = 0;
    OC1RS = R0;
    OC2R = R1;
    OC2RS = RS1;
    OC3R = R2;
    OC3RS = RS2;
    OC4R = R3;
    OC4RS = RS3;

    if((scaling>>5)&1){
        OC1CON1 = 7;  OC2CON1 = 7;   OC3CON1 = 7;   OC4CON1 = 7; //Continuous center aligned PWM. high when R, low when RS.
        OC1CON2bits.OCINV = 0;
        OC2CON2bits.OCINV = 0;
        OC3CON2bits.OCINV = 0;
        OC4CON2bits.OCINV = 0;
    }
    else{
        OC1CON1 = 4;  OC2CON1 = 4;   OC3CON1 = 4;   OC4CON1 = 4; //double compare mode. one shot toggle on alternate matches of OCR, OCRS
        OC1CON2bits.OCINV = 0;
        OC2CON2bits.OCINV = (scaling>>2)&1;
        OC3CON2bits.OCINV = (scaling>>3)&1;
        OC4CON2bits.OCINV = (scaling>>4)&1;
    }
    OC1CON1bits.OCTSEL = 4;
    OC2CON1bits.OCTSEL = 4;
    OC3CON1bits.OCTSEL = 4;
    OC4CON1bits.OCTSEL = 4;

    T1CONbits.TCKPS = scaling&0x3;

    TMR1=0;
    T1CONbits.TON = 1;

    RPOR5bits.RP54R = 0x10; //SQR1(RC6) mapped to (output compare 1 )
    RPOR5bits.RP55R = 0x11; //SQR2(RC7) mapped to (output compare 2 )
    RPOR6bits.RP56R = 0x12; //SQR3(RC8) mapped to (output compare 3 )
    RPOR6bits.RP57R = 0x13; //SQR4(RC9) mapped to (output compare 4 )
    /*-----------------------square wave output-----------------*/

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

void setADCMode(BYTE mode,BYTE chosa,BYTE ch123sa){
    if(ADC_MODE == mode && chosa==CHOSA && ch123sa == CH123SA)return;
    else{
        if(CHOSA==7 || CHOSA == 5)DisableComparator();
        ADC_MODE = mode;CHOSA=chosa;CH123SA=ch123sa;
        if(mode == ADC_10BIT_SIMULTANEOUS)initADC10();
        else if(mode == ADC_10BIT_DMA)initADCDMA(0);  //12 bit mode disabled
        else if(mode == ADC_12BIT_DMA)initADCDMA(1);  //12 bit mode
        else if(mode == ADC_12BIT)initADC12();
        else if(mode == ADC_12BIT_SCOPE)initADC12bit_scope();
        else if(mode == ADC_12BIT_AVERAGING)initADC12_averaging16();
        else if(mode == ADC_CTMU)initADCCTMU();


    }
}

void setupADC10() {
    T5CONbits.TCKPS = 1;
    PR5 = ADC_DELAY-1;
    TMR5 = 0x0000;
    T5CONbits.TON = 1;

}
void preciseDelay(int t){
    T5CONbits.TON = 0;
    T5CONbits.TCKPS = 2;
    PR5 = t-1;
    TMR5 = 0x0000;
    _T5IF=0;
    T5CONbits.TON = 1;
    while(!_T5IF);
    T5CONbits.TON = 0;
    
}    
void enableADCDMA(){
    AD1CON1bits.ADDMABM=1;AD1CON4bits.ADDMAEN=1;
}
void disableADCDMA(){
    AD1CON1bits.ADDMABM=0;AD1CON4bits.ADDMAEN=0;DMA0CONbits.CHEN = 0; 
}

void PrepareTrigger(void) {
    TRIGGER_WAITING=0; TRIGGER_READY=0;TRIGGERED=0;
}

void initADC10(void) {
    /* Set port configuration */
    AD1CON1 = 0;
    disableADCDMA();
   /* Initialize ADC module */
    AD1CON1bits.AD12B = 0;
    AD1CON1bits.SSRCG = 0;
    AD1CON1bits.SSRC = 4; //Timer5 compare starts conversion
    AD1CON1bits.ASAM = 1; //auto sampling

    AD1CON1bits.SIMSAM = 1; //simultaneous sampling
    AD1CHS0bits.CH0SA = CHOSA; //AN3 - CH0
    AD1CHS0bits.CH0NA = 0;
    AD1CHS123bits.CH123SA = CH123SA; //AN0 -> CH1 , AN1 -> ch2, AN2 -> ch3
    AD1CHS123bits.CH123NA = 0; //-ve of CH1,2,3 to -vref
    AD1CON2bits.SMPI = 0; //generate interrupt after converting all chans

    AD1CON4 = 0x0000;
    AD1CSSH = 0x0000;
    AD1CSSL = 0x0000;

    /* Assign MUXA inputs for block read */
    _AD1IF = 0;  _AD1IE = 0; //disable ADC interrupt until required
    //AD1CON3bits.SAMC = 0; // SAMC - Sample for (x+1)*Tad before triggering conversion (TMR5 will decide this here)
    AD1CON3bits.ADCS = 1; // Conversion clock x*Tp
    AD1CON3bits.ADRC = 0; //use clock derived from system clock

    AD1CON1bits.ADON = 1;
    Delay_us(20);

    T5CONbits.TON = 0;
    T5CONbits.TSIDL = 1;
    T5CONbits.TCKPS = 1;
    TMR5 = 0x0000;
    T5CONbits.TON = 1;
    _T5IF = 0;
    _T5IE = 0;
}

void initADCDMA(BYTE bits) {
    /* Set port configuration */
    AD1CON1 = 0;
    /* Initialize ADC module */
    AD1CON1bits.AD12B = bits;
    AD1CON1bits.SSRCG = 0;
    AD1CON1bits.SSRC = 4; //Timer5 compare starts conversion
    AD1CON1bits.ASAM = 1; //auto sampling

    AD1CON1bits.SIMSAM = 1; //simultaneous sampling
    AD1CHS0bits.CH0SA = CHOSA; 
    AD1CHS0bits.CH0NA = 0;
    AD1CHS123bits.CH123SA = CH123SA; //AN0 -> CH1 , AN1 -> ch2, AN2 -> ch3
    AD1CHS123bits.CH123NA = 0; //-ve of CH1,2,3 to -vref
    AD1CON2bits.SMPI = 0; //generate interrupt after converting all chans

    AD1CON4 = 0x0000;
    AD1CSSH = 0x0000;
    AD1CSSL = 0x0000;

    /* Assign MUXA inputs for block read */
    _AD1IF = 0;  _AD1IE = 0; //disable ADC interrupt until required
    AD1CON3bits.ADCS = 1; // Conversion clock x*Tp
    AD1CON3bits.ADRC = 0; //use clock derived from system clock

    AD1CON1bits.ADON = 1;
    Delay_us(20);

    T5CONbits.TON = 0;
    T5CONbits.TSIDL = 1;
    T5CONbits.TCKPS = 1;
    TMR5 = 0x0000;
    _T5IF = 0; _T5IE = 0;

    DMA0CONbits.CHEN = 0; 

    DMA0CONbits.AMODE = 0b00; // Register indirect with post increment
    DMA0CONbits.MODE = 0b01;// One Shot, Ping-Pong mode Disabled
    DMA0CONbits.DIR = 0;// Peripheral to RAM
    DMA0REQ = 0b1101; // Select ADC module as DMA request source


    DMA_MODE = DMA_LA_ONE_CHAN;
    enableADCDMA();

}


void initADC12bit_scope(void) {
    /* Set port configuration */
    disableADCDMA();

    AD1CON1bits.ADON = 0;
    /* Initialize ADC module */
    AD1CON1bits.AD12B = 1;
    AD1CON1bits.SSRCG = 0;
    AD1CON1bits.SSRC = 4; //Timer5 compare starts conversion
    AD1CON1bits.ASAM = 1; //auto sampling

    AD1CHS0bits.CH0SA = CHOSA; //AN3 - CH0
    AD1CHS0bits.CH0NA = 0;
    AD1CON2bits.SMPI = 0; //generate interrupt after converting all chans

    AD1CON4 = 0x0000;
    AD1CSSH = 0x0000;
    AD1CSSL = 0x0000;

    /* Assign MUXA inputs for block read */
    _AD1IF = 0;
    _AD1IE = 0; //disable ADC interrupt until required
    AD1CON3bits.ADCS = 9; // Conversion clock x*Tp
    AD1CON3bits.ADRC = 0; //use clock derived from system clock

    AD1CON1bits.ADON = 1;
    Delay_us(20);

    T5CONbits.TON = 0;
    T5CONbits.TSIDL = 1;
    T5CONbits.TCKPS = 1;
    TMR5 = 0x0000;
    T5CONbits.TON = 1;
    _T5IF = 0;
    _T5IE = 0;
}

void initADC12(void) {
    _AD1IF = 0;  _AD1IE = 0;
    disableADCDMA();

    AD1CON2 = 0;
    AD1CON4 = 0x0000;
    AD1CSSH = 0x0000;
    AD1CSSL = 0x0000;

    AD1CON1bits.ADON = 0;

    AD1CON1bits.AD12B = 1;
    AD1CON1bits.ADSIDL = 0; 
    AD1CON3bits.ADRC = 1; //use internal clock
    AD1CON1bits.SSRCG = 0;
    AD1CON2bits.CHPS =0;
    AD1CHS0bits.CH0SA = CHOSA; //AN3 - CH0
    AD1CHS0bits.CH0NA = 0;
    AD1CON1bits.SSRC = 7; //Internal counter ends sampling, starts conversion
    //AD1CON1bits.SIMSAM = 0; //simultaneous sampling  .  Not applicable for single channel sampling
    AD1CON1bits.ASAM = 0; // No auto sampling
    AD1CON2bits.SMPI = 0; //generate interrupt after argument+1 conversion

    /* Assign MUXA inputs for block read */
    //AD1CHS0bits.CH0SA = channel; //AN<channel> connected to CH0
    AD1CON3bits.SAMC = 0x1f; // Sample for (x+1)*Tad before triggering conversion
    AD1CON3bits.ADCS = 9; // Conversion clock x*Tp

    //AD1CON2bits.CHPS = 0; //unimplemented in 12 bit mode. read as 0
    AD1CON1bits.ADON = 1;
    Delay_us(20);
}

void initADC12_averaging16() {
            _AD1IF = 0; _AD1IE = 0;                                             //disable ADC interrupts
            disableADCDMA();

            AD1CON1bits.ADON = 0;                                               //turn off ADC.
            AD1CON2 = 0;
            AD1CON4 = 0x0000;
            AD1CSSH = 0x0000;
            AD1CSSL = 0x0000;
            AD1CON1bits.AD12B = 1;                                              //12 bit mode
            AD1CON1bits.ADSIDL = 0;                                             //continue operation in idle
            AD1CON3bits.ADRC = 0;                                               //do not use internal clock
            AD1CON1bits.SSRCG = 0;                                              
            AD1CON2bits.CHPS =0;
            /* Assign MUXA inputs for block read */
            AD1CHS0bits.CH0SA = CHOSA;
            AD1CHS0bits.CH0NA = 0;
            AD1CON1bits.SSRC = 7;                                               //Internal counter ends sampling, starts conversion (SSRCG=0)

            AD1CON3bits.SAMC = 0x10; // Sample for (x+1)*Tad before triggering conversion
            AD1CON3bits.ADCS = 0xA; // Conversion clock Tad = ADCS*Tp(15nS))
            AD1CON2bits.SMPI = 15;   //generate interrupt after argument+1 conversions
}

unsigned int get_voltage_summed(BYTE channel){
            setADCMode(ADC_12BIT_AVERAGING,channel,0);
            AD1CON1bits.ADON = 1; Delay_us(20);   //Turn on the ADC
            AD1CON1bits.ASAM = 1; // auto sampling
            _AD1IF=0;while(!_AD1IF);_AD1IF=0;
            while(!AD1CON1bits.DONE);//wait for conversion
            AD1CON1bits.ASAM = 0; //stop auto sampling
            AD1CON1bits.ADON = 0;

            return (ADC1BUF0)+(ADC1BUF1)+(ADC1BUF2)+(ADC1BUF3)+(ADC1BUF4)+(ADC1BUF5)+(ADC1BUF6)+(ADC1BUF7)
                    +(ADC1BUF8)+(ADC1BUF9)+(ADC1BUFA)+(ADC1BUFB)+(ADC1BUFC)+(ADC1BUFD)+(ADC1BUFE)+(ADC1BUFF);

}

unsigned int get_voltage(BYTE channel){
            AD1CHS0bits.CH0SA = channel; //AN<channel> connected to CH0
            AD1CON1bits.SAMP = 1; //start sampling
            while (!AD1CON1bits.DONE);
            return ADC1BUF0;
}


void configureADC() {
    ANSELB = ANSELC = 0x0000;
    ANSELAbits.ANSA0 = 1; // Ensure AN0 is analog
    ANSELAbits.ANSA1 = 1; // Ensure AN1 is analog
    ANSELBbits.ANSB0 = 1;
    ANSELBbits.ANSB1 = 1;
    ANSELBbits.ANSB3 = 1;
    ANSELCbits.ANSC1 = 1;
    ANSELCbits.ANSC2 = 1;  //URGENT!
}

void Delay_us(unsigned int delay) {
    unsigned int i;
    for ( i = 0; i < delay; i++) {
        __asm__ volatile ("repeat #63");
        __asm__ volatile ("nop");
    }
}

void Delay_us_by8(unsigned int delay) {
    unsigned int i;
    for ( i = 0; i < delay; i++) {
        __asm__ volatile ("repeat #7");
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


void logit(char *str){
    while(*str!='\0'){
        *error_writepos++=*str++;
        if(error_writepos==&errors[ERROR_BUFFLEN])
            error_writepos=&errors[0];
    }
}

void __attribute__((__interrupt__, no_auto_psv)) _INT1Interrupt(void)
{
    if((ReadStatus()&0x42)==0x42){
        logit("Remote:");
        ca=ReadRegister(R_RX_PL_WID);
        ReadPayload(ca, &i2c_list[nodecount][0]);  //Whatever the node said. Assuming it'll send I2C sensor list
        WriteRegister(NRF_STATUS, 0x30);
        tmpstr[2]=(char)(nodecount%10 + 48);
        tmpstr[1]=(char)((nodecount/10)%10 + 48);
        tmpstr[0]=(char)((nodecount/100)%10 + 48);
        tmpstr[3]='\0';
        logit(&tmpstr[0]);
        nodecount+=1;
        if(nodecount==15){nodecount=0;logit("Nodecount exceeded. index reset to 0.");}
    }
    _INT1IF=0;
}

/*Command set for the NRFL01+ radio*/
void nRF_Setup()
{
    CSN_HIGH;  CE_LOW;
    WriteRegister(NRF_CONFIG, 0x3F); //MASK max_rt TX_DS, 1 BYTE CRC, POWER UP, PRX
    WriteRegister(EN_AA, 0x01); //enable auto ack
    WriteRegister(DYNPD, 0x01); //enable dynamic payload
    WriteRegister(EN_RXADDR, 0x01); //Enable data pipe 0
    WriteRegister(SETUP_AW, 0x01); //3 BYTE address
    WriteRegister(SETUP_RETR, 0x25); //Retransmit enabled
    WriteRegister(FEATURE, 0x04); //enable dynamic payload length
    WriteRegister(RF_CH, 100); //Randomly chosen RF channel
    WriteRegister(RF_SETUP, 0x26); //250kbps, 0dBm(0x26) , 2MbPS ( 0x0E)
    //WriteRegister(RX_PW_P0, 0x01); //RX payload = 1 BYTE
    WriteAddress(RX_ADDR_P0, 3, RXTX_ADDR);
    WriteAddress(RX_ADDR_P1, 3, TOKEN_ADDR);
    WriteAddress(TX_ADDR, 3, RXTX_ADDR);

    FlushTXRX();
    if ((ReadRegister(NRF_CONFIG) & 0x08) != 0){
        rfCardPresent = TRUE;
    }

    /*Configure falling edge interrupt on RPI34 /AN4/RB2*/
    INTCON2bits.INT1EP=1;  //negative interrupt on AN4
    RPINR0bits.INT1R = 34; //remap to RPI34

}
void RXMode()
    {
    WriteRegister(NRF_CONFIG, 0x3F); //1 BYTE CRC, POWER UP, PRX
    CE_HIGH;
    Delay_us(4); //Tpece2csn
    }
void TXMode()
    {
    CE_LOW;
    WriteRegister(NRF_CONFIG, 0x3E); //1 BYTE CRC, POWER UP, PTX
    }
void PowerDown()
    {
    CE_LOW;
    WriteRegister(NRF_CONFIG, 0);
    }
BYTE RXChar()
    {
    BYTE data;
    ReadPayload(1, &data);
    WriteRegister(NRF_STATUS, 0x30);
    return data;
    }
void TXChar(BYTE ch)
    {
    WritePayload(TX_PAYLOAD,1, &ch);
    if (rfCardPresent)
        {
        //Wait for char to be sent
        BYTE stat;
        do
            {
            stat = ReadStatus();
            } while ((stat & 0x20) == 0);
        }
    //Clear status bit
    WriteRegister(NRF_STATUS, 0x20);
    }

BYTE ReadDataAvailable()
    {
    BYTE stat = ReadStatus();
    return (stat & 0x40) != 0;
    }
void FlushTXRX()
    {
    WriteRegister(NRF_STATUS, 0x30); //Clear: data RX ready, data sent TX, Max TX retransmits
    WriteCommand(FLUSH_RX);
    WriteCommand(FLUSH_TX);
    }

// *************** Helper Methods for NRF ***************
void WriteRegister(BYTE reg, BYTE val)
    {
    CSN_LOW;
    spi_write8(W_REG | reg);
    spi_write8(val);
    CSN_HIGH;
    }
//Address is 3-5 bytes, LSB first
void WriteAddress(BYTE reg, BYTE num, BYTE* addr)
    {
    CSN_LOW;
    spi_write8(W_REG | reg);
    for (i=0; i<num; i++)
    spi_write8(addr[i]);
    CSN_HIGH;Nop();Nop();Nop();
    }
BYTE ReadRegister(BYTE reg)
    {
    BYTE val;
    CSN_LOW;
    spi_write8(R_REG | reg);
    val = spi_write8(0x00);
    CSN_HIGH;
    return val;
    }
BYTE ReadStatus()
    {
    BYTE val;
    CSN_LOW;
    val = spi_write8(R_STATUS);
    CSN_HIGH;
    return val;
    }
void WriteCommand(BYTE command)
    {
    CSN_LOW;
    spi_write8(command);
    CSN_HIGH;
    }
void WritePayload(BYTE addr,BYTE num, BYTE* data)
    {
    CSN_LOW;
    spi_write8(addr);
    for (i=0; i<(num&0x3F); i++)  spi_write8(data[i]);
    CSN_HIGH;
    if(num&0x80){
    CE_HIGH;
    Delay_us(12); //Thce (10us) + a bit (2us)
    CE_LOW;
    }
}
void ReadPayload(BYTE num, BYTE* data)
    {
    CSN_LOW;
    spi_write8(RX_PAYLOAD);
    for (i=0; i<num; i++)
    data[i] = spi_write8(0);
    CSN_HIGH;
    WriteRegister(NRF_STATUS, 0x40);
}


