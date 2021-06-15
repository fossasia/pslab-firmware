#ifndef _TMR4_H
#define _TMR4_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

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
    void TMR4_Initialize(void);

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
    void TMR4_Period16BitSet(uint16_t value);

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
    uint16_t TMR4_Period16BitGet(void);

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
    void TMR4_Counter16BitSet(uint16_t value);

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
    uint16_t TMR4_Counter16BitGet(void);


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
    void TMR4_Start(void);

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
    void TMR4_Stop(void);

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif //_TMR4_H
