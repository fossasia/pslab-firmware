/* 
 * File:   Wave_Generator.h
 * Author: padmal
 *
 * Created on June 11, 2017, 3:49 AM
 */

#ifndef WAVE_GENERATOR_H
#define	WAVE_GENERATOR_H

extern int __attribute__((section("sine_table1"))) sineTable1[];
extern int __attribute__((section("sine_table2"))) sineTable2[];
extern int __attribute__((section("sine_table1_short"))) sineTable1_short[];
extern int __attribute__((section("sine_table2_short"))) sineTable2_short[];

extern void sqr1(unsigned int, unsigned int, BYTE);
extern void sqr2(unsigned int, unsigned int, BYTE);
extern void sqr4(uint16 w,uint16 R0,uint16 R1,uint16 RS1,uint16 R2,uint16 RS2,uint16 R3,uint16 RS3,BYTE scaling);
extern void sineWave1(unsigned int wavelength,BYTE highres);
extern void sineWave2(unsigned int wavelength,BYTE highres);
extern void setSineWaves(unsigned int wavelength1,unsigned int wavelength2,unsigned int pos,unsigned int tmr_delay,BYTE highres);

#endif	/* WAVE_GENERATOR_H */

