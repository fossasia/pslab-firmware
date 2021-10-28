#ifndef IC_PARAMS_H
#define	IC_PARAMS_H

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    /** Input Capture Parameter Definition
  
     @Summary 
       Defines the capture modes for IC modules
  
     @Description
       Input Capture will response to multiple trigger events and they are listed
       below.
     */
    typedef enum {
        IC_PARAMS_CAPTURE_MODE_OFF                      = 0b000,
        IC_PARAMS_CAPTURE_MODE_EVERY_EDGE               = 0b001,
        IC_PARAMS_CAPTURE_MODE_EVERY_FALLING_EDGE       = 0b010,
        IC_PARAMS_CAPTURE_MODE_EVERY_RISING_EDGE        = 0b011,
        IC_PARAMS_CAPTURE_MODE_EVERY_4TH_RISING_EDGE    = 0b100,
        IC_PARAMS_CAPTURE_MODE_EVERY_16TH_RISING_EDGE   = 0b101,
        IC_PARAMS_CAPTURE_MODE_AS_INTERRUPT             = 0b111
    } IC_PARAMS_CAPTURE_MODE;

    typedef enum {
        IC_PARAMS_CAPTURE_SOURCE_NONE                   = 0b00000,
        IC_PARAMS_CAPTURE_SOURCE_OC1                    = 0b00001,
        IC_PARAMS_CAPTURE_SOURCE_OC2                    = 0b00010,
        IC_PARAMS_CAPTURE_SOURCE_OC3                    = 0b00011,
        IC_PARAMS_CAPTURE_SOURCE_OC4                    = 0b00100,
        IC_PARAMS_CAPTURE_SOURCE_PTGO                   = 0b01010,
        IC_PARAMS_CAPTURE_SOURCE_TMR1                   = 0b01011,
        IC_PARAMS_CAPTURE_SOURCE_TMR2                   = 0b01100,
        IC_PARAMS_CAPTURE_SOURCE_TMR3                   = 0b01101,
        IC_PARAMS_CAPTURE_SOURCE_TMR4                   = 0b01110,
        IC_PARAMS_CAPTURE_SOURCE_TMR5                   = 0b01111,
        IC_PARAMS_CAPTURE_SOURCE_IC1                    = 0b10000,
        IC_PARAMS_CAPTURE_SOURCE_IC2                    = 0b10001,
        IC_PARAMS_CAPTURE_SOURCE_IC3                    = 0b10010,
        IC_PARAMS_CAPTURE_SOURCE_IC4                    = 0b10011,
        IC_PARAMS_CAPTURE_SOURCE_CMP1                   = 0b11000,
        IC_PARAMS_CAPTURE_SOURCE_CMP2                   = 0b11001,
        IC_PARAMS_CAPTURE_SOURCE_CMP3                   = 0b11010,
        IC_PARAMS_CAPTURE_SOURCE_ADC1                   = 0b11011,
        IC_PARAMS_CAPTURE_SOURCE_CTMU                   = 0b11100,
    } IC_PARAMS_CAPTURE_SOURCE;
    
    typedef enum {
        IC_PARAMS_CAPTURE_TIMER3                        = 0b000,
        IC_PARAMS_CAPTURE_TIMER2                        = 0b001,
        IC_PARAMS_CAPTURE_TIMER4                        = 0b010,
        IC_PARAMS_CAPTURE_TIMER5                        = 0b011,
        IC_PARAMS_CAPTURE_TIMER1                        = 0b100,
        IC_PARAMS_CAPTURE_TIMER_PERIPHERAL              = 0b111
    } IC_PARAMS_CAPTURE_TIMER;

    typedef enum {
      IC_PARAMS_CAPTURE_INTERRUPT_EVERY_EVENT           = 0b00,
      IC_PARAMS_CAPTURE_INTERRUPT_EVERY_SECOND          = 0b01,
      IC_PARAMS_CAPTURE_INTERRUPT_EVERY_THIRD           = 0b10,
      IC_PARAMS_CAPTURE_INTERRUPT_EVERY_FOURTH          = 0b11
    } IC_PARAMS_CAPTURE_INTERRUPT;
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* IC_PARAMS_H */
