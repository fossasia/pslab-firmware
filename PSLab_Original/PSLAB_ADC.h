/* 
 * File:   PSLAB_ADC.h
 * Author: padmal
 *
 * Created on June 3, 2017, 6:20 PM
 */

#ifndef PSLAB_ADC_H
#define	PSLAB_ADC_H

extern int __attribute__((section("adcbuff"), far)) ADCbuffer[BUFFER_SIZE];

extern BYTE CHOSA, CH123SA;
extern uint16 ADC_DELAY;
extern BYTE conversion_done;
extern BYTE TRIGGERED;
extern BYTE TRIGGER_READY;
extern BYTE TRIGGER_CHANNEL;
extern int *buff0, *buff1, *endbuff, *buff2, *buff3;
extern BYTE ADC_CHANNELS; // CH1 only
extern int samples;
extern uint16 samples_to_fetch;
extern uint16 adval;
extern uint16 TRIGGER_TIMEOUT, TRIGGER_WAITING, TRIGGER_LEVEL, TRIGGER_PRESCALER;

extern void initADCCTMU(void);
extern void EnableComparator();
extern void DisableComparator();
extern void initADC10(void);
extern void initADCDMA(BYTE);
extern void initADC12bit_scope(void);
extern void initADC12(void);
extern void initADCCTMU(void);
extern void initADC12_averaging16();
extern void setADCMode(BYTE, BYTE, BYTE);
extern uint16 get_voltage_summed(BYTE channel);
extern uint16 get_voltage(BYTE channel);
extern void setupADC10();
extern void configureADC();
extern void enableADCDMA();
extern void disableADCDMA();

#endif	/* PSLAB_ADC_H */
