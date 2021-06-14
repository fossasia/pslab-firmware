#ifndef _TMR5_H
#define _TMR5_H

/**
  Section: Included Files
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif


    /**
      Section: Interface Routines
     */

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
 
      @Example 
        <code>
        bool statusTimer1;
        uint16_t period;
        uint16_t value;

        period = 0x20;

        TMR5_Initialize();

        TMR5_Period16BitSet(period);

        if((value = TMR5_Period16BitGet())== period)
        {
            TMR5_Start();
        }

        while(1)
        {
            TMR5_Tasks();
            if( (statusTimer1 = TMR5_GetElapsedThenClear()) == true)
            {
                TMR5_Stop();
            }
        }
        </code>
     */
    void TMR5_Initialize(void);

    /**
      @Summary
        Used to maintain the driver's state machine and implement its ISR

      @Description
        This routine is used to maintain the driver's internal state machine and
        implement its ISR for interrupt-driven implementations.

      @Param
        None.

      @Returns
        None
 
      @Example 
        Refer to the example of TMR5_Initialize();
     */

    void TMR5_Tasks_16BitOperation(void);

    /**
      @Summary
        Updates 16-bit timer value

      @Description
        This routine updates 16-bit timer value

      @Param
        None.

      @Returns
        None
 
      @Example 
        Refer to the example of TMR5_Initialize();
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
 
      @Example 
        Refer to the example of TMR5_Initialize();
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

      @Example 
        <code>
        uint16_t value=0xF0F0;

        TMR5_Counter16BitSet(value));

        while(1)
        {
            TMR5_Tasks();
            if( (value == TMR5_Counter16BitGet()))
            {
                TMR5_Stop();
            }
        }
        </code>
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
 
      @Example 
        Refer to the example of TMR5_Counter16BitSet();
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
 
      @Example 
        Refer to the example of TMR5_Initialize();
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
 
      @Example 
        Refer to the example of TMR5_Initialize();
     */

    void TMR5_Stop(void);

    /**
      @Summary
        Returns the elapsed status of the timer and clears if flag is set.

      @Description
        This routine returns the elapsed status of the timer and clears 
        flag if its set.

      @Param
        None.

      @Returns
        True - Timer has elapsed.
        False - Timer has not elapsed.
 
      @Example 
        Refer to the example of TMR5_Initialize();
     */

    bool TMR5_GetElapsedThenClear(void);

    /**
      @Summary
        Returns the software counter value.

      @Description
        This routine returns the software counter value.

      @Param
        None.

      @Returns
        Software counter value.
 
      @Example 
        Refer to the example of TMR5_Initialize();
     */

    int TMR5_SoftwareCounterGet(void);

    /**
      @Summary
        Clears the software counter value.

      @Description
        This routine clears the software counter value.

      @Param
        None.

      @Returns
        None
 
      @Example 
        Refer to the example of TMR5_Initialize();
     */

    void TMR5_SoftwareCounterClear(void);

#ifdef __cplusplus  // Provide C++ Compatibility

}

#endif

#endif //_TMR5_H
