#ifndef _TMR2_H
#define _TMR2_H

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
    void TMR2_Initialize(void);

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
    void TMR2_Period16BitSet(uint16_t value);

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
    uint16_t TMR2_Period16BitGet(void);

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
    void TMR2_Counter16BitSet(uint16_t value);

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
    uint16_t TMR2_Counter16BitGet(void);


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
    void TMR2_Start(void);

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
    void TMR2_Stop(void);

    /**
      @Summary
        Combines Timer 2 and Timer 3 modules

      @Description
        This routine combines Timer 2 and Timer 3 modules to form
        a 32-bit timer module

      @Param
        None

      @Returns
        None.
     */
    inline static void TMR2_CombineWithTimer3(void) {
        T2CONbits.T32 = 1;
    }

    /**
      @Summary
        Enable an external pin as timer clock source

      @Description
        This routine enables one of the available external pins
        to become the clock source for the Timer 2 module

      @Param
        None

      @Returns
        None.
     */
    inline static void TMR2_SetExternalClockAsSource(void) {
        T2CONbits.TCS = 1;
    }

    /**
      @Summary
        Sets pre-scaler for Timer 2 module

      @Description
        This routine defines the down sample rate for the operating frequency
        of Timer 2 module

      @Param
        scale: pre-scaler ratio (TIMER_PARAMS_PRESCALER)

      @Returns
        None.
     */
    inline static void TMR2_SetPrescaler(TIMER_PARAMS_PRESCALER scale) {
        T2CONbits.TCKPS = scale;
    }

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif //_TMR2_H
