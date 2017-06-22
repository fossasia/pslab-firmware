#include "COMMANDS.h"
#include "Common_Functions.h"
#include "PSLAB_ADC.h"
#include "PSLAB_SPI.h"
#include "Measurements.h"

BYTE DIN_REMAPS[] ={ID1_REMAP, ID2_REMAP, ID3_REMAP, ID4_REMAP, COMP4_REMAP, RP41_REMAP, FREQ_REMAP};
BYTE INITIAL_DIGITAL_STATES = 0;
BYTE LAM1 = 0, LAM2 = 0, LAM3 = 0, LAM4 = 0;

void set_cap_voltage(BYTE v, unsigned int time) {
    _TRISC0 = 0;
    _ANSC0 = 0;
    if (v)_LATC0 = 1;
    else _LATC0 = 0;

    //_TRISB3=0;_ANSB3=0;_LATB3=0;
    Delay_us(time);
    _LATC0 = 0;
    _TRISC0 = 1;
    _ANSC0 = 1;
    Nop(); //Return C0 to high impedance
    //_TRISB3=1;_ANSB3=1;_LATB3=1;

}

unsigned int get_cc_capacitance(BYTE current_range, BYTE trimval, unsigned int ChargeTime) {
    unsigned int sum = 0;
    setADCMode(ADC_CTMU, 5, 0); // 5 is the CAP channel
    CTMUCON1bits.TGEN = 1;
    CTMUICONbits.ITRIM = trimval;
    CTMUICONbits.IRNG = current_range; // 01->Base Range .53uA, 10->base*10, 11->base*100, 00->base*1000
    T5CONbits.TON = 0;
    T5CONbits.TGATE = 0;
    T5CONbits.TCKPS = 2;
    PR5 = ChargeTime;
    set_cap_voltage(0, 50000);
    AD1CON1bits.ADON = 1;
    Delay_us(20);
    AD1CON1bits.SAMP = 1; //start sampling
    CTMUCON1bits.CTMUEN = 1;
    Delay_us(1000);
    CTMUCON1bits.IDISSEN = 1; //Ground the charge PUMP
    Delay_us(1500); //Grounding the ADC S&H

    TMR5 = 0x0000;
    _T5IF = 0;
    _T5IE = 0;
    CTMUCON1bits.IDISSEN = 0; //stop draining the circuit
    CTMUCON2bits.EDG1STAT = 1; //start charging    //Delay_us(ChargeTime); //wait for charge
    T5CONbits.TON = 1;
    while (!_T5IF);
    _T5IF = 0;
    _AD1IF = 0;
    AD1CON1bits.SAMP = 0; // Begin analog-to-digital conversion
    CTMUCON2bits.EDG1STAT = 0; // Stop charging circuit

    while (!_AD1IF);
    _AD1IF = 0;
    while (!AD1CON1bits.DONE);
    sum = (ADC1BUF0)&0xFFF;
    disableCTMUSource();
    AD1CON1bits.ADON = 0;
    return sum;

}

void disableCTMUSource() {
    CTMUCON1bits.CTMUEN = 0;
    CTMUCON2bits.EDG1STAT = 0;
    CTMUCON2bits.EDG2STAT = 0;
    CTMUCON1bits.TGEN = 0; // Stop current source and disable CTMU
}

unsigned int get_cap_range(unsigned int time) {
    unsigned int sum = 0;
    set_cap_voltage(0, 50000);
    set_cap_voltage(0, 50000);
    setADCMode(ADC_12BIT_AVERAGING, 5, 0);
    T5CONbits.TON = 0;
    TMR5 = 0x0000;
    _T5IF = 0;
    PR5 = time;
    T5CONbits.TCKPS = 2;
    T5CONbits.TON = 1;
    _TRISC0 = 0;
    _LATC0 = 1; //START CHARGING VIA C0,10K resistor
    while (!_T5IF);
    _T5IF = 0;
    _TRISC0 = 1;
    _LATC0 = 0; //STOP CHARGING.
    sum = get_voltage_summed(5); // This function will turn on the ADC, and turn it off when finished.
    return sum;
}

unsigned int get_ctmu_voltage(BYTE channel, BYTE range, BYTE tgen) {
    unsigned int temp = 0;
    CTMUCON1bits.TGEN = tgen; //(channel==5)?1:0;
    CTMUICONbits.ITRIM = 0;
    CTMUICONbits.IRNG = range; // 01->Base Range .53uA, 10->base*10, 11->base*100, 00->base*1000
    CTMUCON2bits.EDG1STAT = 0;
    CTMUCON1bits.CTTRIG = 0; //do not trigger the ADC
    if (channel != 30)CTMUCON2bits.EDG2STAT = 0;
    else CTMUCON2bits.EDG2STAT = 1; //30 -> internal temperature.
    CTMUCON1bits.CTMUEN = 1;
    Delay_us(1000);

    CTMUCON1bits.IDISSEN = 1; //Ground the charge PUMP
    Delay_us(1500); //Grounding the ADC S&H
    CTMUCON1bits.IDISSEN = 0; //stop draining the circuit

    CTMUCON1bits.CTMUSIDL = 0; //0=enable operation in idle mode
    CTMUCON2bits.EDG1STAT = 1; // Start current source

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

void get_high_frequency(BYTE channel, BYTE scale) { //T2CK is tied to ID1. Using timer 3/2

    if (channel == 4) EnableComparator();
    RPINR3bits.T2CKR = DIN_REMAPS[channel];

    T3CONbits.TON = 0; // Stop any 16-bit Timer3 operation
    T2CONbits.TON = 0; // Stop any 16/32-bit Timer2 operation
    T5CONbits.TON = 0; // Stop any 16/32-bit Timer5 operation

    T2CONbits.T32 = 1; // 32 bit mode T2 and T3
    T2CONbits.TCS = 1; // Select External clock
    T2CONbits.TCKPS = scale; // Select Prescaler
    IFS0bits.T3IF = 0;
    T5CONbits.TCKPS = 3; //1:256 , 1.0/8 MHz
    PR5 = 25000; //100mS sampling
    TMR3HLD = 0;
    TMR3 = 0x0000; // Clear 32-bit Timer (msw)
    TMR2 = 0x0000; // Clear 32-bit Timer (lsw)
    TMR5 = 0x0000;


    _T5IP = 0x01; // Set Timer5 Interrupt Priority Level
    _T5IF = 0; // Clear Timer5 Interrupt Flag
    //_T5IE = 1; // Enable Timer5 interrupt
    T2CONbits.TON = 1; // Start 32-bit Timer
    T5CONbits.TON = 1; // Start 16-bit Timer

}

void init_IC_for_frequency(BYTE capture_pin, BYTE capture_mode, BYTE captures_per_interrupt) {

    RPINR7bits.IC1R = DIN_REMAPS[capture_pin];

    IC1CON1bits.ICM = 0; //disable the module
    IC2CON1bits.ICM = 0; //disable the module
    IC1CON1bits.ICOV = 0; //reset overflow flag
    IC2CON1bits.ICOV = 0; //reset overflow flag

    IC1CON1bits.ICTSEL = 0b111; //peripheral clock
    IC2CON1bits.ICTSEL = 0b111; //peripheral clock
    IC1CON1bits.ICI = captures_per_interrupt - 1; //
    IC2CON1bits.ICI = captures_per_interrupt - 1;
    IC1CON2bits.IC32 = 1;
    IC2CON2bits.IC32 = 1;
    IC1CON2bits.TRIGSTAT = 0;
    IC2CON2bits.TRIGSTAT = 0;
    IC1CON2bits.SYNCSEL = 0; // no trigger or synchronization
    IC2CON2bits.SYNCSEL = 0; // no trigger or synchronization
    IC1CON2bits.ICTRIG = 1;
    IC2CON2bits.ICTRIG = 1;
    INITIAL_DIGITAL_STATES = ((PORTB >> 10)&0xF) | (_C4OUT << 4); //INITIAL_DIGITAL_STATES =(PORTB>>10)&0xF;
    IC1CON1bits.ICM = capture_mode;
    IC2CON1bits.ICM = capture_mode;

    IC1CON2bits.TRIGSTAT = 1;
    IC2CON2bits.TRIGSTAT = 1;

}

void startCounting(BYTE channel) {
    T2CONbits.TON = 0;
    T2CONbits.T32 = 0;
    T2CONbits.TCS = 1; // Select External clock
    if (channel == 4) EnableComparator();
    RPINR3bits.T2CKR = DIN_REMAPS[channel];
    TMR2 = 0x0000;
    T2CONbits.TON = 1;
}

void TimingMeasurements(BYTE capture_pin1, BYTE capture_pin2, BYTE pin1_edge, BYTE pin2_edge, BYTE interrupts1, BYTE interrupts2) {
    _IC1IF = 0;
    _IC3IF = 0;
    if (capture_pin1 == 4) EnableComparator();
    RPINR7bits.IC1R = DIN_REMAPS[capture_pin1];

    if (capture_pin2 == 4) EnableComparator();
    RPINR8bits.IC3R = DIN_REMAPS[capture_pin2];

    IC1CON1bits.ICM = 0;
    IC3CON1bits.ICM = 0; //disable the module
    IC2CON1bits.ICM = 0;
    IC4CON1bits.ICM = 0; //disable the module
    IC1CON1bits.ICOV = 0;
    IC3CON1bits.ICOV = 0; //reset overflow flag
    IC2CON1bits.ICOV = 0;
    IC4CON1bits.ICOV = 0; //reset overflow flag

    T2CONbits.TON = 0; // Stop any 16/32-bit Timer2 operation
    T2CONbits.T32 = 0;
    T2CONbits.TCS = 0; // Select External clock
    T2CONbits.TCKPS = 0; // Select Prescaler
    TMR2 = 0x0000; // Clear 16-bit Timer

    IC1CON1bits.ICTSEL = 0b1;
    IC3CON1bits.ICTSEL = 0b1; //Timer2 clock
    IC2CON1bits.ICTSEL = 0b1;
    IC4CON1bits.ICTSEL = 0b1; //Timer2 clock
    IC1CON1bits.ICI = interrupts1 - 1;
    IC3CON1bits.ICI = interrupts2 - 1; //interrupt after ICI+1 events
    IC1CON2bits.IC32 = 1;
    IC3CON2bits.IC32 = 1;
    IC2CON2bits.IC32 = 1;
    IC4CON2bits.IC32 = 1;
    IC1CON2bits.SYNCSEL = 0;
    IC3CON2bits.SYNCSEL = 0; // no trigger or synchronization
    IC2CON2bits.SYNCSEL = 0;
    IC4CON2bits.SYNCSEL = 0; // no trigger or synchronization
    IC1CON2bits.ICTRIG = 0;
    IC3CON2bits.ICTRIG = 0;
    IC2CON2bits.ICTRIG = 0;
    IC4CON2bits.ICTRIG = 0;
    INITIAL_DIGITAL_STATES = ((PORTB >> 10)&0xF) | (_C4OUT << 4); //INITIAL_DIGITAL_STATES =(PORTB>>10)&0xF;

    //1,2 will capture one type of edge. 3,4 will capture another.
    IC1CON1bits.ICM = pin1_edge;
    IC3CON1bits.ICM = pin2_edge;
    IC2CON1bits.ICM = pin1_edge;
    IC4CON1bits.ICM = pin2_edge;

    IC1CON2bits.TRIGSTAT = 1;
    IC2CON2bits.TRIGSTAT = 1;
    IC3CON2bits.TRIGSTAT = 1;
    IC4CON2bits.TRIGSTAT = 1;

    T2CONbits.TON = 1; // Start the timer

}

void Interval(BYTE capture_pin1, BYTE capture_pin2, BYTE pin1_edge, BYTE pin2_edge) {

    RPINR7bits.IC1R = DIN_REMAPS[capture_pin1];
    RPINR8bits.IC3R = DIN_REMAPS[capture_pin2];


    IC1CON1bits.ICM = 0;
    IC3CON1bits.ICM = 0; //disable the module
    IC2CON1bits.ICM = 0;
    IC4CON1bits.ICM = 0; //disable the module
    IC1CON1bits.ICOV = 0;
    IC3CON1bits.ICOV = 0; //reset overflow flag
    IC2CON1bits.ICOV = 0;
    IC4CON1bits.ICOV = 0; //reset overflow flag

    IC1CON1bits.ICTSEL = 0b111;
    IC3CON1bits.ICTSEL = 0b111; //Peripheral clock
    IC2CON1bits.ICTSEL = 0b111;
    IC4CON1bits.ICTSEL = 0b111; //Peripheral clock
    IC1CON1bits.ICI = 0;
    IC3CON1bits.ICI = 0; //interrupt after ICI+1 events
    IC1CON2bits.IC32 = 1;
    IC3CON2bits.IC32 = 1;
    IC2CON2bits.IC32 = 1;
    IC4CON2bits.IC32 = 1;
    IC1CON2bits.ICTRIG = 0;
    IC1CON2bits.SYNCSEL = 0; // no trigger or synchronization
    IC2CON2bits.ICTRIG = 0;
    IC2CON2bits.SYNCSEL = 0; // no trigger or synchronization

    IC3CON2bits.ICTRIG = 0;
    IC3CON2bits.SYNCSEL = 0;
    IC4CON2bits.ICTRIG = 0;
    IC4CON2bits.SYNCSEL = 0;
    IC1CON2bits.TRIGSTAT = 0;
    IC2CON2bits.TRIGSTAT = 0;
    IC3CON2bits.TRIGSTAT = 0;
    IC4CON2bits.TRIGSTAT = 0;

    //1,2 will capture one type of edge. 3,4 will capture another.
    _IC1IF = 0;
    _IC3IF = 0;
    IC1CON1bits.ICM = pin1_edge;
    IC2CON1bits.ICM = pin1_edge;
    IC3CON1bits.ICM = pin2_edge;
    IC4CON1bits.ICM = pin2_edge;
    IC1CON2bits.TRIGSTAT = 1;
    IC2CON2bits.TRIGSTAT = 1;
    IC3CON2bits.TRIGSTAT = 1;
    IC4CON2bits.TRIGSTAT = 1;

}

void start_1chan_LA(unsigned int data_points, BYTE channel, BYTE mode, BYTE trigchan) {
    _CNIF = 0;
    _CNIE = 0;
    DMA0CONbits.CHEN = 0;
    DMA1CONbits.CHEN = 0;
    IC1CON1bits.ICM = 0;
    IC2CON1bits.ICM = 0; //disable the module
    IC1CON1bits.ICOV = 0;
    IC2CON1bits.ICOV = 0; //reset overflow flag
    DMA_MODE = DMA_LA_ONE_CHAN;
    if (channel == 4) EnableComparator();
    RPINR7bits.IC1R = DIN_REMAPS[channel];

    IC1CON1bits.ICTSEL = 0b111;
    IC2CON1bits.ICTSEL = 0b111; //peripheral clock
    IC1CON1bits.ICI = 0;
    IC2CON1bits.ICI = 0;
    IC1CON2bits.IC32 = 1;
    IC2CON2bits.IC32 = 1;
    IC1CON2bits.TRIGSTAT = 0;
    IC2CON2bits.TRIGSTAT = 0;
    IC1CON2bits.ICTRIG = 1;
    IC2CON2bits.ICTRIG = 1;
    if (trigchan & 7) {
        if ((trigchan >> 4) == 4) EnableComparator();
        RPINR8bits.IC4R = DIN_REMAPS[(trigchan >> 4)&0xF];

        IC4CON1bits.ICTSEL = 0b111; //peripheral clock
        IC4CON1bits.ICI = 0;
        IC4CON2bits.ICTRIG = 0;
        IC4CON2bits.SYNCSEL = 0;
        IC4CON1bits.ICOV = 0; //reset overflow flag
        IC1CON2bits.SYNCSEL = 0x13;
        IC2CON2bits.SYNCSEL = 0x13; // trigger using IC4
    } else {
        IC1CON2bits.SYNCSEL = 0;
        IC2CON2bits.SYNCSEL = 0;
    } // no trigger or synchronization

    DMA0CONbits.AMODE = 0b00;
    DMA1CONbits.AMODE = 0b00; // Register indirect with post increment
    DMA0CONbits.MODE = 0b01;
    DMA1CONbits.MODE = 0b01; // One Shot, Ping-Pong mode Disabled
    DMA0CONbits.DIR = 0;
    DMA1CONbits.DIR = 0; // Peripheral to RAM

    DMA0PAD = (int) &IC1BUF;
    DMA1PAD = (int) &IC2BUF; // Address of the capture buffer register
    DMA0REQ = 1;
    DMA1REQ = 1; // Select IC1 module as DMA request source
    DMA0CNT = data_points - 1;
    DMA1CNT = data_points - 1; // Number of words to buffer
    DMA0STAH = __builtin_dmapage(&ADCbuffer);
    DMA0STAL = __builtin_dmaoffset(&ADCbuffer);
    DMA1STAH = __builtin_dmapage((int*) (ADCbuffer + BUFFER_SIZE / 4));
    DMA1STAL = __builtin_dmaoffset((int*) (ADCbuffer + BUFFER_SIZE / 4));

    IC1CON1bits.ICOV = 0;
    IC2CON1bits.ICOV = 0; //reset overflow flag
    IC1CON1bits.ICM = mode;
    IC2CON1bits.ICM = mode;
    _DMA0IF = 0; // Clear the DMA interrupt flag bit
    _DMA0IE = 1; // Enable DMA interrupt enable bit
    DMA0CONbits.CHEN = 1;
    DMA1CONbits.CHEN = 1;
    IC4CON1bits.ICM = trigchan & 7; //mode

}

void start_2chan_LA(unsigned int data_points, BYTE modes, BYTE locations) {
    DMA0CONbits.CHEN = 0;
    DMA1CONbits.CHEN = 0;
    DMA2CONbits.CHEN = 0;
    DMA3CONbits.CHEN = 0;
    IC1CON1bits.ICM = 0;
    IC2CON1bits.ICM = 0;
    IC3CON1bits.ICM = 0;
    IC4CON1bits.ICM = 0; //disable the module
    IC1CON1bits.ICOV = 0;
    IC2CON1bits.ICOV = 0;
    IC3CON1bits.ICOV = 0;
    IC4CON1bits.ICOV = 0; //reset overflow flag
    DMA_MODE = DMA_LA_TWO_CHAN;
    RPINR7bits.IC1R = DIN_REMAPS[locations & 0xF]; //remapping IC1
    RPINR8bits.IC3R = DIN_REMAPS[(locations >> 4)&0xF]; //remapping IC2
    IC1CON1bits.ICM = 0;
    IC2CON1bits.ICM = 0;
    IC3CON1bits.ICM = 0;
    IC4CON1bits.ICM = 0; //disable the module
    IC1CON1bits.ICOV = 0;
    IC2CON1bits.ICOV = 0;
    IC3CON1bits.ICOV = 0;
    IC4CON1bits.ICOV = 0; //reset overflow flag

    IC1CON1bits.ICTSEL = 7;
    IC2CON1bits.ICTSEL = 7;
    IC3CON1bits.ICTSEL = 7;
    IC4CON1bits.ICTSEL = 7; //Peripheral. setting for all 4 is important!
    IC1CON1bits.ICI = 0;
    IC2CON1bits.ICI = 0;
    IC3CON1bits.ICI = 0;
    IC4CON1bits.ICI = 0;
    IC1CON2bits.IC32 = 1;
    IC2CON2bits.IC32 = 1;
    IC3CON2bits.IC32 = 1;
    IC4CON2bits.IC32 = 1;
    IC1CON2bits.TRIGSTAT = 0;
    IC2CON2bits.TRIGSTAT = 0;
    IC3CON2bits.TRIGSTAT = 0;
    IC4CON2bits.TRIGSTAT = 0;
    IC1CON2bits.SYNCSEL = 0;
    IC2CON2bits.SYNCSEL = 0;
    IC3CON2bits.SYNCSEL = 0;
    IC4CON2bits.SYNCSEL = 0;
    IC1CON2bits.ICTRIG = 1;
    IC2CON2bits.ICTRIG = 1;
    IC3CON2bits.ICTRIG = 1;
    IC4CON2bits.ICTRIG = 1;

    DMA0CONbits.AMODE = 0b00;
    DMA1CONbits.AMODE = 0b00;
    DMA2CONbits.AMODE = 0b00;
    DMA3CONbits.AMODE = 0b00; // Register indirect with post increment
    DMA0CONbits.MODE = 0b01;
    DMA1CONbits.MODE = 0b01;
    DMA2CONbits.MODE = 0b01;
    DMA3CONbits.MODE = 0b01; // One Shot, Ping-Pong mode Disabled
    DMA0CONbits.DIR = 0;
    DMA1CONbits.DIR = 0;
    DMA2CONbits.DIR = 0;
    DMA3CONbits.DIR = 0; // Peripheral to RAM

    DMA0PAD = (int) &IC1BUF;
    DMA1PAD = (int) &IC2BUF;
    DMA2PAD = (int) &IC3BUF;
    DMA3PAD = (int) &IC4BUF; // Address of the capture buffer register
    DMA0REQ = 1;
    DMA1REQ = 1;
    DMA2REQ = 37;
    DMA3REQ = 37; // Select IC1+IC2, IC3+IC4(00100101) modules as DMA request source
    DMA0CNT = data_points - 1;
    DMA1CNT = data_points - 1;
    DMA2CNT = data_points - 1;
    DMA3CNT = data_points - 1; // Number of words to buffer
    DMA0STAH = __builtin_dmapage(&ADCbuffer);
    DMA2STAH = __builtin_dmapage((int*) (ADCbuffer + BUFFER_SIZE / 2));
    DMA0STAL = __builtin_dmaoffset(&ADCbuffer);
    DMA2STAL = __builtin_dmaoffset((int*) (ADCbuffer + BUFFER_SIZE / 2));
    DMA1STAH = __builtin_dmapage((int*) (ADCbuffer + BUFFER_SIZE / 4));
    DMA3STAH = __builtin_dmapage((int*) (ADCbuffer + 3 * BUFFER_SIZE / 4));
    DMA1STAL = __builtin_dmaoffset((int*) (ADCbuffer + BUFFER_SIZE / 4));
    DMA3STAL = __builtin_dmaoffset((int*) (ADCbuffer + 3 * BUFFER_SIZE / 4));

    _DMA0IF = 0; // Clear the DMA interrupt flag bit
    _DMA0IE = 1; // Enable DMA interrupt enable bit
    DMA0CONbits.CHEN = 1;
    DMA2CONbits.CHEN = 1;
    DMA1CONbits.CHEN = 1;
    DMA3CONbits.CHEN = 1;
    //IC1CON1bits.ICM=modes&0xF;IC2CON1bits.ICM=modes&0xF;IC3CON1bits.ICM=(modes>>4)&0xF;IC4CON1bits.ICM=(modes>>4)&0xF;
    LAM1 = modes & 0xF;
    LAM2 = modes & 0xF;
    LAM3 = (modes >> 4)&0xF;
    LAM4 = (modes >> 4)&0xF; //TESTING  A NEW  APPROACH

}

void start_3chan_LA(unsigned int data_points, unsigned int modes, BYTE trigchan) {
    DMA0CONbits.CHEN = 0;
    DMA1CONbits.CHEN = 0;
    DMA2CONbits.CHEN = 0;
    DMA3CONbits.CHEN = 0;
    IC1CON1bits.ICM = 0;
    IC2CON1bits.ICM = 0;
    IC3CON1bits.ICM = 0;
    IC4CON1bits.ICM = 0; //disable the module
    IC1CON1bits.ICOV = 0;
    IC2CON1bits.ICOV = 0;
    IC3CON1bits.ICOV = 0;
    IC4CON1bits.ICOV = 0; //reset overflow flag

    DMA_MODE = DMA_LA_FOUR_CHAN;
    RPINR7bits.IC1R = ID1_REMAP; //remapping IC1 to ID1;
    RPINR7bits.IC2R = ID2_REMAP; //remapping IC1 to ID2;
    RPINR8bits.IC3R = ID3_REMAP;


    IC1CON1bits.ICM = 0;
    IC2CON1bits.ICM = 0;
    IC3CON1bits.ICM = 0;
    IC4CON1bits.ICM = 0; //disable the module
    IC1CON1bits.ICOV = 0;
    IC2CON1bits.ICOV = 0;
    IC3CON1bits.ICOV = 0;
    IC4CON1bits.ICOV = 0; //reset overflow flag

    IC1CON1bits.ICTSEL = 0b111;
    IC2CON1bits.ICTSEL = 0b111;
    IC3CON1bits.ICTSEL = 0b111;
    IC4CON1bits.ICTSEL = 0b111; //Fp
    IC1CON1bits.ICI = 0;
    IC2CON1bits.ICI = 0;
    IC3CON1bits.ICI = 0;
    IC4CON1bits.ICI = 0;
    IC1CON2bits.IC32 = 0;
    IC2CON2bits.IC32 = 0;
    IC3CON2bits.IC32 = 0;
    IC4CON2bits.IC32 = 0;
    IC1CON2bits.TRIGSTAT = 0;
    IC2CON2bits.TRIGSTAT = 0;
    IC3CON2bits.TRIGSTAT = 0;
    IC4CON2bits.TRIGSTAT = 0;
    IC1CON2bits.ICTRIG = 1;
    IC2CON2bits.ICTRIG = 1;
    IC3CON2bits.ICTRIG = 1;

    if (trigchan == 0) {
        IC1CON2bits.SYNCSEL = 0;
        IC2CON2bits.SYNCSEL = 0;
        IC3CON2bits.SYNCSEL = 0;
    }// no trigger or synchronization
    else if (trigchan & 7) {
        RPINR8bits.IC4R = DIN_REMAPS[(trigchan >> 4)&0xF];

        IC4CON2bits.ICTRIG = 0;
        IC4CON2bits.SYNCSEL = 0;
        IC1CON2bits.SYNCSEL = 0x13;
        IC2CON2bits.SYNCSEL = 0x13;
        IC3CON2bits.SYNCSEL = 0x13; // trigger using IC4
    }



    DMA0CONbits.AMODE = 0b00;
    DMA1CONbits.AMODE = 0b00;
    DMA2CONbits.AMODE = 0b00; // Register indirect with post increment
    DMA0CONbits.MODE = 0b01;
    DMA1CONbits.MODE = 0b01;
    DMA2CONbits.MODE = 0b01; // One Shot, Ping-Pong mode Disabled
    DMA0CONbits.DIR = 0;
    DMA1CONbits.DIR = 0;
    DMA2CONbits.DIR = 0; // Peripheral to RAM

    DMA0PAD = (int) &IC1BUF;
    DMA1PAD = (int) &IC2BUF;
    DMA2PAD = (int) &IC3BUF; // Address of the capture buffer register
    DMA0REQ = 1;
    DMA1REQ = 5;
    DMA2REQ = 37; // Select IC1,IC2, IC3 modules as DMA request source
    DMA0CNT = data_points - 1;
    DMA1CNT = data_points - 1;
    DMA2CNT = data_points - 1; // Number of words to buffer
    DMA0STAH = __builtin_dmapage(&ADCbuffer);
    DMA1STAH = __builtin_dmapage((int*) (ADCbuffer + BUFFER_SIZE / 4));
    DMA0STAL = __builtin_dmaoffset(&ADCbuffer);
    DMA1STAL = __builtin_dmaoffset((int*) (ADCbuffer + BUFFER_SIZE / 4));
    DMA2STAH = __builtin_dmapage((int*) (ADCbuffer + BUFFER_SIZE / 2));
    DMA2STAL = __builtin_dmaoffset((int*) (ADCbuffer + BUFFER_SIZE / 2));

    _DMA0IF = 0; // Clear the DMA interrupt flag bit
    _DMA0IE = 1; // Enable DMA interrupt enable bit
    DMA0CONbits.CHEN = 1;
    DMA2CONbits.CHEN = 1;
    DMA1CONbits.CHEN = 1;
    IC1CON1bits.ICM = modes & 0xF;
    IC2CON1bits.ICM = (modes >> 4)&0xF;
    IC3CON1bits.ICM = (modes >> 8)&0xF;
    IC4CON1bits.ICM = trigchan & 7; //trigger via IC4

}

void start_4chan_LA(unsigned int data_points, unsigned int modes, BYTE scale) {
    DMA0CONbits.CHEN = 0;
    DMA1CONbits.CHEN = 0;
    DMA2CONbits.CHEN = 0;
    DMA3CONbits.CHEN = 0;
    IC1CON1bits.ICM = 0;
    IC2CON1bits.ICM = 0;
    IC3CON1bits.ICM = 0;
    IC4CON1bits.ICM = 0; //disable the module
    IC1CON1bits.ICOV = 0;
    IC2CON1bits.ICOV = 0;
    IC3CON1bits.ICOV = 0;
    IC4CON1bits.ICOV = 0; //reset overflow flag

    DMA_MODE = DMA_LA_FOUR_CHAN;
    RPINR7bits.IC1R = ID1_REMAP; //remapping IC1 to ID1;
    RPINR7bits.IC2R = ID2_REMAP; //remapping IC1 to ID2;
    RPINR8bits.IC3R = ID3_REMAP;
    RPINR8bits.IC4R = ID4_REMAP;
    T2CONbits.TON = 0; // Stop any 16/32-bit Timer2 operation
    T2CONbits.T32 = 0;
    T2CONbits.TCS = 0; // Select External clock
    T2CONbits.TCKPS = scale & 0xF; // Select Prescaler
    TMR2 = 0x0001; // Init 16-bit Timer


    IC1CON1bits.ICM = 0;
    IC2CON1bits.ICM = 0;
    IC3CON1bits.ICM = 0;
    IC4CON1bits.ICM = 0; //disable the module
    IC1CON1bits.ICOV = 0;
    IC2CON1bits.ICOV = 0;
    IC3CON1bits.ICOV = 0;
    IC4CON1bits.ICOV = 0; //reset overflow flag

    IC1CON1bits.ICTSEL = 0b1;
    IC2CON1bits.ICTSEL = 0b1;
    IC3CON1bits.ICTSEL = 0b1;
    IC4CON1bits.ICTSEL = 0b1; //TIMER2
    IC1CON1bits.ICI = 0;
    IC2CON1bits.ICI = 0;
    IC3CON1bits.ICI = 0;
    IC4CON1bits.ICI = 0;
    IC1CON2bits.IC32 = 0;
    IC2CON2bits.IC32 = 0;
    IC3CON2bits.IC32 = 0;
    IC4CON2bits.IC32 = 0;
    IC1CON2bits.TRIGSTAT = 0;
    IC2CON2bits.TRIGSTAT = 0;
    IC3CON2bits.TRIGSTAT = 0;
    IC4CON2bits.TRIGSTAT = 0;
    IC1CON2bits.SYNCSEL = 0;
    IC2CON2bits.SYNCSEL = 0;
    IC3CON2bits.SYNCSEL = 0;
    IC4CON2bits.SYNCSEL = 0;
    IC1CON2bits.ICTRIG = 1;
    IC2CON2bits.ICTRIG = 1;
    IC3CON2bits.ICTRIG = 1;
    IC4CON2bits.ICTRIG = 1;

    DMA0CONbits.AMODE = 0b00;
    DMA1CONbits.AMODE = 0b00;
    DMA2CONbits.AMODE = 0b00;
    DMA3CONbits.AMODE = 0b00; // Register indirect with post increment
    DMA0CONbits.MODE = 0b01;
    DMA1CONbits.MODE = 0b01;
    DMA2CONbits.MODE = 0b01;
    DMA3CONbits.MODE = 0b01; // One Shot, Ping-Pong mode Disabled
    DMA0CONbits.DIR = 0;
    DMA1CONbits.DIR = 0;
    DMA2CONbits.DIR = 0;
    DMA3CONbits.DIR = 0; // Peripheral to RAM

    DMA0PAD = (int) &IC1BUF;
    DMA1PAD = (int) &IC2BUF;
    DMA2PAD = (int) &IC3BUF;
    DMA3PAD = (int) &IC4BUF; // Address of the capture buffer register
    DMA0REQ = 1;
    DMA1REQ = 5;
    DMA2REQ = 37;
    DMA3REQ = 38; // Select IC1,IC2, IC3,IC4 modules as DMA request source
    DMA0CNT = data_points - 1;
    DMA1CNT = data_points - 1;
    DMA2CNT = data_points - 1;
    DMA3CNT = data_points - 1; // Number of words to buffer
    DMA0STAH = __builtin_dmapage(&ADCbuffer);
    DMA1STAH = __builtin_dmapage((int*) (ADCbuffer + BUFFER_SIZE / 4));
    DMA0STAL = __builtin_dmaoffset(&ADCbuffer);
    DMA1STAL = __builtin_dmaoffset((int*) (ADCbuffer + BUFFER_SIZE / 4));
    DMA2STAH = __builtin_dmapage((int*) (ADCbuffer + BUFFER_SIZE / 2));
    DMA3STAH = __builtin_dmapage((int*) (ADCbuffer + 3 * BUFFER_SIZE / 4));
    DMA2STAL = __builtin_dmaoffset((int*) (ADCbuffer + BUFFER_SIZE / 2));
    DMA3STAL = __builtin_dmaoffset((int*) (ADCbuffer + 3 * BUFFER_SIZE / 4));

    _DMA0IF = 0; // Clear the DMA interrupt flag bit
    _DMA0IE = 1; // Enable DMA interrupt enable bit
    DMA0CONbits.CHEN = 1;
    DMA2CONbits.CHEN = 1;
    DMA1CONbits.CHEN = 1;
    DMA3CONbits.CHEN = 1;
    IC1CON1bits.ICM = modes & 0xF;
    IC2CON1bits.ICM = (modes >> 4)&0xF;
    IC3CON1bits.ICM = (modes >> 8)&0xF;
    IC4CON1bits.ICM = (modes >> 12)&0xF;

}

void disable_input_capture() {
    IC1CON2bits.TRIGSTAT = 0;
    IC2CON2bits.TRIGSTAT = 0;
    IC3CON2bits.TRIGSTAT = 0;
    IC4CON2bits.TRIGSTAT = 0;
    IC1CON1bits.ICM = 0;
    IC2CON1bits.ICM = 0;
    IC3CON1bits.ICM = 0;
    IC4CON1bits.ICM = 0; //disable the module
    IC1CON1bits.ICOV = 0;
    IC2CON1bits.ICOV = 0;
    IC3CON1bits.ICOV = 0;
    IC4CON1bits.ICOV = 0; //reset overflow flag
    /*
    _DMA0IF = 0; // Clear the DMA interrupt flag bit
    _DMA0IE = 0; // Disable DMA interrupt enable bit
    DMA0CONbits.CHEN = 0;
    DMA1CONbits.CHEN = 0;
    DMA2CONbits.CHEN = 0;
    DMA3CONbits.CHEN = 0;
     * */
}

void alternate_get_high_frequency(BYTE channel, BYTE scale) { //Measure freq using only input captures. Timer 3 not available

    RPINR7bits.IC1R = 0;
    RPINR7bits.IC2R = 0;
    T2CONbits.TON = 0;
    T2CONbits.T32 = 0;
    T2CONbits.TCS = 1; // Select External clock
    T2CONbits.TCKPS = scale; // Select Prescaler

    if (channel == 4) EnableComparator();
    RPINR3bits.T2CKR = DIN_REMAPS[channel];


    IC1CON1bits.ICM = 0;
    IC2CON1bits.ICM = 0; //disable the module
    IC1CON1bits.ICTSEL = 1;
    IC2CON1bits.ICTSEL = 1; //timer 2
    IC1CON1bits.ICI = 1;
    IC2CON1bits.ICI = 1;
    IC1CON1bits.ICOV = 0;
    IC2CON1bits.ICOV = 0; //reset overflow flag
    IC1CON2bits.IC32 = 1;
    IC2CON2bits.IC32 = 1;
    IC1CON2bits.TRIGSTAT = 0;
    IC2CON2bits.TRIGSTAT = 0;
    IC1CON2bits.ICTRIG = 1;
    IC2CON2bits.ICTRIG = 1;
    IC1CON2bits.SYNCSEL = 0;
    IC2CON2bits.SYNCSEL = 0; //no sync


    T5CONbits.TON = 0; // Stop any 16/32-bit Timer5 operation
    T5CONbits.TCKPS = 3; //1:256 , 1.0/8 MHz
    PR5 = 25000; //100mS sampling
    TMR5 = 0x0000;

    _T5IP = 0x01; // Set Timer5 Interrupt Priority Level
    _T5IF = 0; // Clear Timer5 Interrupt Flag
    TMR2 = 0x0000;
    T2CONbits.TON = 1;

    IC2CON1bits.ICM = EVERY_SIXTEENTH_RISING_EDGE;
    IC1CON1bits.ICM = EVERY_SIXTEENTH_RISING_EDGE;
    T5CONbits.TON = 1; // Start 16-bit Timer
    IC2CON2bits.TRIGSTAT = 1;
    IC1CON2bits.TRIGSTAT = 1;
}
