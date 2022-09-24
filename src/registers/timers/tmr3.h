#ifndef _TMR3_H
#define _TMR3_H

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
    void TMR3_Initialize(void);

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
    void TMR3_Period16BitSet(uint16_t value);

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
    uint16_t TMR3_Period16BitGet(void);

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
    void TMR3_Counter16BitSet(uint16_t value);

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
    uint16_t TMR3_Counter16BitGet(void);
    
    /**
      @Summary
        Provides 16-bit hold carry counter value

      @Description
        When TMR2 and TMR3 are cascaded to form a single 32-bit timer,
        TMR2 hold the Least Significant Word (LSW) and TMR3 holds the
        Most Significant Word (MSW). When the LSW is read from TMR2,
        the current value of TMR3 is moved to the TMR3HLD register.
        The MSW can then be read from TMR3HLD without worrying about
        the delay between reads.

      @Param
        None.

      @Returns
        16-bit current hold carry value
     */
    uint16_t TMR3_Carry16BitGet(void);

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
    void TMR3_Start(void);

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
    void TMR3_Stop(void);

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif //_TMR3_H
