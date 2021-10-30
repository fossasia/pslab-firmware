#ifndef CMP_PARAMS_H
#define	CMP_PARAMS_H

#include <xc.h> 

#ifdef	__cplusplus
extern "C" {
#endif

    typedef enum {
        CMP_MASK_PWM1L              = 0b0000,
        CMP_MASK_PWM1H              = 0b0001,
        CMP_MASK_PWM2L              = 0b0010,
        CMP_MASK_PWM2H              = 0b0011,
        CMP_MASK_PWM3L              = 0b0100,
        CMP_MASK_PWM3H              = 0b0101,
        CMP_MASK_PTGO18             = 0b1100,
        CMP_MASK_PTGO19             = 0b1101,
        CMP_MASK_FLT2               = 0b1110,
        CMP_MASK_FLT4               = 0b1111
    } CMP_MASK;
    
    
    typedef enum {
        CMP_FILTER_CLOCK_FP         = 0b000,
        CMP_FILTER_CLOCK_FOSC       = 0b001,
        CMP_FILTER_CLOCK_SYNCO1     = 0b010,
        CMP_FILTER_CLOCK_TIMER2     = 0b100,
        CMP_FILTER_CLOCK_TIMER3     = 0b101,
        CMP_FILTER_CLOCK_TIMER4     = 0b110,
        CMP_FILTER_CLOCK_TIMER5     = 0b111
    } CMP_FILTER_CLOCK;
    
    
    typedef enum {
      CMP_CLOCK_PRESCALER_1         = 0b000,
      CMP_CLOCK_PRESCALER_2         = 0b001,
      CMP_CLOCK_PRESCALER_4         = 0b010,
      CMP_CLOCK_PRESCALER_8         = 0b011,
      CMP_CLOCK_PRESCALER_16        = 0b100,
      CMP_CLOCK_PRESCALER_32        = 0b101,
      CMP_CLOCK_PRESCALER_64        = 0b110,
      CMP_CLOCK_PRESCALER_128       = 0b111
    } CMP_CLOCK_PRESCALER;
    

    typedef enum {
      CMP_TRIGGER_DISABLED          = 0b00,
      CMP_TRIGGER_LOW_TO_HIGH       = 0b01,
      CMP_TRIGGER_HIGH_TO_LOW       = 0b10,
      CMP_TRIGGER_EVERY_EVENT       = 0b11
    } CMP_TRIGGER;

#ifdef	__cplusplus
}
#endif

#endif	/* CMP_PARAMS_H */

