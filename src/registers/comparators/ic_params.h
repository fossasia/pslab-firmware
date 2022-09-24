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
        IC_PARAMS_CAPTURE_INTERRUPT_EVERY_EVENT         = 0b00,
        IC_PARAMS_CAPTURE_INTERRUPT_EVERY_SECOND        = 0b01,
        IC_PARAMS_CAPTURE_INTERRUPT_EVERY_THIRD         = 0b10,
        IC_PARAMS_CAPTURE_INTERRUPT_EVERY_FOURTH        = 0b11
    } IC_PARAMS_CAPTURE_INTERRUPT;
    
    typedef enum {
        IC_PARAMS_SOURCE_TASK_SYNC                      = 0b0,
        IC_PARAMS_SOURCE_TASK_TRIGGER                   = 0b1,
    } IC_PARAMS_SOURCE_TASK;

    /***************************************************************************
     * Common functions used altogether by all ICx registers
     ***************************************************************************/
     void IC_PARAMS_InitiateAll(void);     

     void IC_PARAMS_StopAllModules(void);

     void IC_PARAMS_ClearTriggerStatus(void);

     void IC_PARAMS_ClearBufferOverflow(void);

     void IC_PARAMS_DisableAllModules(void);

     void IC_PARAMS_ManualTriggerAll(void);
     
     void IC_PARAMS_UseSourceTo(IC_PARAMS_SOURCE_TASK);
     
     void IC_PARAMS_CombineOddEvenModules(void);
     
     void IC_PARAMS_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER);

    /**
      @Summary
        Combines IC1 and IC2 modules to enable concatenated operation

      @Description
        This routine combines both IC1 and IC2 capture modules to act
        them as 32-bit counters

      @Preconditions
        None

      @Param
        pin: source pin for the input capture clock module (PIN_MANAGER_DIGITAL_PINS)
        timer: timer to run the capture modules (IC_PARAMS_CAPTURE_TIMER)
        interrupt: interrupt trigger event (IC_PARAMS_CAPTURE_INTERRUPT)
        mode: capture event (IC_PARAMS_CAPTURE_MODE)

      @Returns
        None.
     */
     void IC_PARAMS_ConfigureIntervalCaptureWithIC1AndIC2(uint8_t pin, 
        IC_PARAMS_CAPTURE_TIMER timer, 
        IC_PARAMS_CAPTURE_INTERRUPT interrupt,
        IC_PARAMS_CAPTURE_MODE mode);

    /**
      @Summary
        Combines IC3 and IC4 modules to enable concatenated operation

      @Description
        This routine combines both IC3 and IC4 capture modules to act
        them as 32-bit counters

      @Preconditions
        None

      @Param
        pin: source pin for the input capture clock module (PIN_MANAGER_DIGITAL_PINS)
        timer: timer to run the capture modules (IC_PARAMS_CAPTURE_TIMER)
        interrupt: interrupt trigger event (IC_PARAMS_CAPTURE_INTERRUPT)
        mode: capture event (IC_PARAMS_CAPTURE_MODE)

      @Returns
        None.
     */
     void IC_PARAMS_ConfigureIntervalCaptureWithIC3AndIC4(uint8_t pin, 
        IC_PARAMS_CAPTURE_TIMER timer, 
        IC_PARAMS_CAPTURE_INTERRUPT interrupt,
        IC_PARAMS_CAPTURE_MODE mode);
    
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* IC_PARAMS_H */
