#ifndef _CMP2_H
#define _CMP2_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

    /**
      @Summary
        This function initializes Comparator instance : 2

      @Description
        This routine initializes the Comparator driver instance for : 2, 
        making it ready for clients to open and use it. It also initializes any
        internal data structures.

      @Param
        None.

      @Returns 
        None
 
      @Comment
    
     */
    void CMP2_Initialize(void);
    /**
        void CMP2_Initialize(void)
     */

    /**
      @Summary
        This function returns the output of the comparator

      @Description
        This routine returns the output of the comparator. It returns a bool which 
        is the output of the comparator.

      @Param
        None.

      @Returns 
        None
 
     */
    bool CMP2_OutputStatusGet(void);
    /**
        bool CMP2_OutputStatusGet(void)
     */

    /**
      @Summary
        This function returns the if the comparator event has occurred or not

      @Description
        This routine returns true if the comparator event has occurred. It returns false 
        if the comparator event has not occurred yet.

      @Param
        None.

      @Returns 
        bool.
 
     */
    bool CMP2_EventStatusGet(void);
    /**
        bool CMP2_EventStatusGet(void)
     */

    /**
      @Summary
        This function resets the event status bit of the comparator.

      @Description
        This routine resets the event status bit of the comparator after the comparator
        event has occurred.

      @Param
        None.

      @Returns 
        bool.
 
     */
    void CMP2_EventStatusReset(void);
    /**
        void CMP2_EventStatusReset(void)
     */
    /**
      @Summary
        This function get the status of the HLMS bit of the blanking function of the comparator.

      @Description
        This routine will prevent any asserted ('0' or '1') comparator signal from propagating

      @Param
        None.

      @Returns 
        bool.
     */
    void CMP2_ComparatorDisable(void);
    /**
        void CMP2_ComparatorDisable(void)
     */
    /**
      @Summary
        Disables the comparator module.

      @Description
        This routine disables the comparator module.

      @Param
        None.

      @Returns 
        bool.
     */
    void CMP2_ComparatorEnable(void);
    /**
        void CMP2_ComparatorEnable(void)
     */
    /**
      @Summary
         Enables the comparator module.

      @Description
        This routine enables the comparator module..

      @Param
        None.

      @Returns 
        bool.
     */
    bool CMP2_HLMSStatusGet(void);
    /**
        bool CMP2_HLMSStatusGet(void)
     */

    /**
      @Summary
        Callback for CMP2.

      @Description
        This routine is callback for CMP2

      @Param
        None.

      @Returns
        None
 
      @Example 
        Refer to CMP2_Initialize(); for an example
     */
    void CMP2_CallBack(void);

    /**
      @Summary
        Polled implementation

      @Description
        This routine is used to implement the tasks for polled implementations.
  
      @Preconditions
        CMP2_Initialize() function should have been 
        called before calling this function.
 
      @Returns 
        None
 
      @Param
        None
 
      @Example
        Refer to CMP2_Initialize(); for an example
    
     */
    void CMP2_Tasks(void);

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif //_CMP2_H
