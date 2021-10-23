#ifndef TIMER_PARAMS_H
#define	TIMER_PARAMS_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    /** Timer Prescaler Definition
 
     @Summary 
       Defines the prescaler setting for timer registers
 
     @Description
     Upon selecting the source for a timer, prescaler defines the number of ticks
     required to increment the counter by 1.
 
     Remarks:
       None
     */
    typedef enum {
        TMR_PRESCALER_1 = 0b00,
        TMR_PRESCALER_8 = 0b01,
        TMR_PRESCALER_64 = 0b10,
        TMR_PRESCALER_256 = 0b11
    } TIMER_PARAMS_PRESCALER;

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* TIMER_PARAMS_H */

