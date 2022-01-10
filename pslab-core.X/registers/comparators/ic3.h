#ifndef _IC3_H
#define _IC3_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "ic_params.h"

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

    void IC3_Initialize(void);
    
    void IC3_InitializeCON1(void);
    void IC3_InitializeCON2(void);

    /**
      @Summary
        Enables the IC module with the corresponding operation mode.

      @Description
        This routine enables the IC module with the corresponding operation mode.

      @Preconditions
        IC3_Initialize function should have been called 

      @Param
        IC_PARAMS_CAPTURE_MODE
 
      @Returns
        None.

      @Example 
        Refer to IC3_Initialize() for an example	
 
     */
    void IC3_Start(IC_PARAMS_CAPTURE_MODE mode);
    
    /** 
    @Summary
      Disables the IC module.

    @Description
      This routine disables the IC module.

    @Preconditions
      IC3_Initialize function should have been called 

    @Param
      None
 
    @Returns
      None.

    @Example 
      Refer to IC3_Initialize() for an example	
  	
     */
    void IC3_Stop(void);
    
    /**
      @Summary
        Reads the captured data from buffer.

      @Description
        This routine reads the captured data from buffer.

      @Preconditions
        IC3_Initialize function should have been called 

      @Param
        None
 
      @Returns
        Read data from buffer.

      @Example 
        Refer to IC3_Initialize() for an example	
  	
     */
    uint16_t IC3_CaptureDataRead(void);

    /**
      @Summary
        Sets the manual trigger

      @Description
        This routine sets the manual trigger
	
      @Preconditions
        IC3_Initialize function should have been called 

      @Param
        None
 
      @Returns
        None.
	
      @Example 
        Refer to IC3_TriggerStatusGet() for an example	
 
     */
    void IC3_ManualTriggerSet(void);

    /**
      @Summary
        Gets the status of the timer trigger.

      @Description
        This routine gets the status of the timer trigger source if it has been triggered.
	
      @Preconditions
        IC3_Initialize function should have been called
	
      @Param
        None
 
      @Returns
        Boolean value describing the timer trigger status.
        true  : When the timer source has triggered and is running 
        false : When the timer has not triggered and being held clear 

      @Example 
        <\code>	
        if(IC3_TriggerStatusGet())
        {
            IC3_TriggerStatusClear();
        }
        <\code>	
     */
    bool IC3_TriggerStatusGet(void);
    
    /**
      @Summary
        Clears the status of the timer trigger.

      @Description
        This routine clears the status of the timer trigger.
	
      @Preconditions
        IC3_Initialize function should have been called 

      @Param
        None
 
      @Returns
        None.
	
      @Example 
        Refer to IC3_TriggerStatusGet() for an example	
 
     */
    void IC3_TriggerStatusClear(void);
    
    /**
      @Summary
        Gets the buffer overflow status.

      @Description
        This routine gets the buffer overflow status.
	
      @Preconditions
        IC3_Initialize function should have been called 
	
      @Param
        None
 
      @Returns
        Boolean value describing the buffer overflow status.
        true  : When the capture buffer has overflowed
        false : When the capture buffer has not overflowed

       @Example 
        <\code>	
        bool status;

        status = IC3_HasCaptureBufferOverflowed();
        <\code>	
     */
    bool IC3_HasCaptureBufferOverflowed(void);
    
    /**  
    @Summary
        Gets the buffer empty status.

      @Description
        This routine gets the buffer empty status.
	
      @Preconditions
        IC3_Initialize function should have been called 
	
      @Param
        None

      @Returns
        Boolean value describing the buffer empty status.
        True- If buffer empty
        False-If buffer not empty

       @Example 
        Refer to IC3_Initialize() for an example	
     */
    bool IC3_IsCaptureBufferEmpty(void);

        /**  
        @Summary
            Sets the timer source

          @Description
            This routine sets the timer source for input capture module.
	
          @Preconditions
            IC3_Initialize function should have been called 
	
          @Param
            IC_PARAMS_CAPTURE_TIMER

          @Returns
            None
     */
    inline static void IC3_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER timer) {
        IC3CON1bits.ICTSEL = timer;
    }

    /**  
    @Summary
        Sets the capture source

      @Description
        This routine sets the capture source for input capture module.
	
      @Preconditions
        IC3_Initialize function should have been called 
	
      @Param
        IC_PARAMS_CAPTURE_SOURCE

      @Returns
        None
     */
    inline static void IC3_SetCaptureSource(IC_PARAMS_CAPTURE_SOURCE source) {
        IC3CON2bits.SYNCSEL = source;
    }
    
    /**  
    @Summary
        Sets the capture mode

      @Description
        This routine sets the capture mode for input capture module.
	
      @Preconditions
        IC3_Initialize function should have been called 
	
      @Param
        IC_PARAMS_CAPTURE_MODE

      @Returns
        None
     */
    inline static void IC3_SetCaptureMode(IC_PARAMS_CAPTURE_MODE mode) {
        IC3CON1bits.ICM = mode;
    }
    
    /**  
    @Summary
        Combine IC modules

      @Description
        This routine combines odd and even input capture modules to form a 32 bit
        capture module. Need to set this for both odd and even IC modules
	
      @Preconditions
        IC3_Initialize function should have been called 
	
      @Param
        None

      @Returns
        None
     */
    inline static void IC3_CombineOddEvenICModules(void) {
        IC3CON2bits.IC32 = 1;
    }
    
    inline static void IC3_UseSourceTo(IC_PARAMS_SOURCE_TASK t) {
        IC3CON2bits.ICTRIG = t;
    }
    
    inline static void IC3_InterruptFlagClear(void) {
        IFS2bits.IC3IF = 0;
    }

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif //_IC3_H
