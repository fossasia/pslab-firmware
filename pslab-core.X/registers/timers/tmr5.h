#ifndef _TMR5_H
#define _TMR5_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "timer_params.h"

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

    /**
      @Summary
        Initializes hardware and data for the given instance of the TMR module

      @Description
        This routine initializes hardware for the instance of the TMR module,
        using the hardware initialization given data.  It also initializes all
        necessary internal data.

      @Param
        None.

      @Returns
        None
     */
    void TMR5_Initialize(void);

    /**
      @Summary
        Updates 16-bit timer value

      @Description
        This routine updates 16-bit timer value

      @Param
        None.

      @Returns
        None
     */
    void TMR5_Period16BitSet(uint16_t value);

    /**

      @Summary
        Provides the timer 16-bit period value

      @Description
        This routine provides the timer 16-bit period value

      @Param
        None.

      @Returns
        Timer 16-bit period value
     */

    uint16_t TMR5_Period16BitGet(void);

    /**
      @Summary
        Updates the timer's 16-bit value

      @Description
        This routine updates the timer's 16-bit value

      @Param
        None.

      @Returns
        None
     */
    void TMR5_Counter16BitSet(uint16_t value);

    /**
      @Summary
        Provides 16-bit current counter value

      @Description
        This routine provides 16-bit current counter value

      @Param
        None.

      @Returns
        16-bit current counter value
     */
    uint16_t TMR5_Counter16BitGet(void);


    /**
      @Summary
        Starts the TMR

      @Description
        This routine starts the TMR

      @Param
        None.

      @Returns
        None
     */
    void TMR5_Start(void);

    /**
      @Summary
        Stops the TMR

      @Description
        This routine stops the TMR

      @Param
        None.

      @Returns
        None
     */
    void TMR5_Stop(void);
    
    /**
      @Summary
        Stops the TMR when MCU idles.

      @Description
        This routine stops the TMR when the MCU is in idle mode.

      @Param
        None.

      @Returns
        None
     */
    inline static void TMR5_StopWhenIdle(void) {
        T5CONbits.TSIDL = 1;
    }
    
    /**
      @Summary
       Slows down the TMR.

      @Description
        This routine sets a prescaler which slows down the TMR by a factor.

      @Param
        Pass the desired prescaler from the TIMER_PARAMS_PRESCALER list.

      @Returns
        None
     */
    inline static void TMR5_SetPrescaler(TIMER_PARAMS_PRESCALER prescaler) {
        T5CONbits.TCKPS = prescaler;
    }

    /**
      @Summary
        Disables the TMR interrupt.

      @Description
        This routine disables the TMR interrupt.

      @Param
        None.

      @Returns
        None
     */
    inline static void TMR5_InterruptDisable(void) {
        IEC1bits.T5IE = 0;
    }

    /**
      @Summary
        Clears the TMR interrupt flag.

      @Description
        This routine clears the TMR interrupt flag.

      @Param
        None.

      @Returns
        None
     */   
    inline static void TMR5_InterruptFlagClear(void) {
        IFS1bits.T5IF = 0;
    }
    
    void TMR5_PrescalerSet(TIMER_PARAMS_PRESCALER scale);
    
    void TMR5_WaitForInterruptEvent(void);

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif //_TMR5_H
