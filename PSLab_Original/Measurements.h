/* 
 * File:   Measurements.h
 * Author: padmal
 *
 * Created on June 21, 2017, 8:09 PM
 */

#ifndef MEASUREMENTS_H
#define	MEASUREMENTS_H

extern BYTE DIN_REMAPS[], LAM1, LAM2, LAM3, LAM4;
extern BYTE INITIAL_DIGITAL_STATES;

extern void set_cap_voltage(BYTE v, unsigned int time);
extern unsigned int get_cc_capacitance(BYTE current_range, BYTE trimval, unsigned int ChargeTime);
extern unsigned int get_cap_range(unsigned int);
extern unsigned int get_ctmu_voltage(BYTE, BYTE, BYTE);
extern void get_high_frequency(BYTE, BYTE);
extern void init_IC_for_frequency(BYTE capture_pin, BYTE capture_mode, BYTE captures_per_interrupt);
extern void startCounting(BYTE channel);
extern void TimingMeasurements(BYTE, BYTE, BYTE, BYTE, BYTE, BYTE);
extern void Interval(BYTE, BYTE, BYTE, BYTE);
extern void disableCTMUSource();
extern void start_1chan_LA(unsigned int, BYTE, BYTE, BYTE);
extern void start_2chan_LA(unsigned int, BYTE, BYTE);
extern void start_3chan_LA(unsigned int, unsigned int, BYTE);
extern void start_4chan_LA(unsigned int, unsigned int, BYTE);
extern void disable_input_capture();
extern void alternate_get_high_frequency(BYTE channel, BYTE scale);
// Todo : Implement Logic analyser functions
extern void enableLogicAnalyser();
extern void disableLogicAnalyser();

#endif	/* MEASUREMENTS_H */

