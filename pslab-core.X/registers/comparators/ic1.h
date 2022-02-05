#ifndef _IC1_H
#define _IC1_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "ic_params.h"

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

    void IC1_Initialize(void);

    void IC1_InitializeCON1(void);
    void IC1_InitializeCON2(void);

    /**
      @Summary
        Enables the IC module with the corresponding operation mode.

      @Description
        This routine enables the IC module with the corresponding operation mode.

      @Preconditions
        IC1_Initialize function should have been called 

      @Param
        IC_PARAMS_CAPTURE_MODE
 
      @Returns
        None.	
     */
    void IC1_Start(IC_PARAMS_CAPTURE_MODE mode);
    
    /** 
    @Summary
      Disables the IC module.

    @Description
      This routine disables the IC module.

    @Preconditions
      IC1_Initialize function should have been called 

    @Param
      None
 
    @Returns
      None.

    @Example 
      Refer to IC1_Initialize() for an example	
  	
     */
    void IC1_Stop(void);
    
    /**
      @Summary
        Reads the captured data from buffer.

      @Description
        This routine reads the captured data from buffer.

      @Preconditions
        IC1_Initialize function should have been called 

      @Param
        None
 
      @Returns
        Read data from buffer.

      @Example 
        Refer to IC1_Initialize() for an example	
  	
     */
    uint16_t IC1_CaptureDataRead(void);

    /**
      @Summary
        Sets the manual trigger

      @Description
        This routine sets the manual trigger
	
      @Preconditions
        IC1_Initialize function should have been called 

      @Param
        None
 
      @Returns
        None.
	
      @Example 
        Refer to IC1_TriggerStatusGet() for an example	
     */
    void IC1_ManualTriggerSet(void);

    /**
      @Summary
        Gets the status of the timer trigger.

      @Description
        This routine gets the status of the timer trigger source if it has been triggered.
	
      @Preconditions
        IC1_Initialize function should have been called
	
      @Param
        None
 
      @Returns
        Boolean value describing the timer trigger status.
        true  : When the timer source has triggered and is running 
        false : When the timer has not triggered and being held clear 

      @Example 
        <\code>	
        if(IC1_TriggerStatusGet())
        {
            IC1_TriggerStatusClear();
        }
        <\code>	
     */
    bool IC1_TriggerStatusGet(void);
    
    /**
      @Summary
        Clears the status of the timer trigger.

      @Description
        This routine clears the status of the timer trigger.
	
      @Preconditions
        IC1_Initialize function should have been called 

      @Param
        None
 
      @Returns
        None.
	
      @Example 
        Refer to IC1_TriggerStatusGet() for an example	
     */
    void IC1_TriggerStatusClear(void);
    
    /**
      @Summary
        Gets the buffer overflow status.

      @Description
        This routine gets the buffer overflow status.
	
      @Preconditions
        IC1_Initialize function should have been called 
	
      @Param
        None
 
      @Returns
        Boolean value describing the buffer overflow status.
        true  : When the capture buffer has overflowed
        false : When the capture buffer has not overflowed

       @Example 
        <\code>	
        bool status;

        status = IC1_HasCaptureBufferOverflowed();
        <\code>	
     */
    bool IC1_HasCaptureBufferOverflowed(void);
    
    /**  
    @Summary
        Gets the buffer empty status.

      @Description
        This routine gets the buffer empty status.
	
      @Preconditions
        IC1_Initialize function should have been called 
	
      @Param
        None

      @Returns
        Boolean value describing the buffer empty status.
        True- If buffer empty
        False-If buffer not empty

       @Example 
        Refer to IC1_Initialize() for an example	
     */
    bool IC1_IsCaptureBufferEmpty(void);
    
    /**  
    @Summary
        Sets the timer source

      @Description
        This routine sets the timer source for input capture module.
	
      @Preconditions
        IC1_Initialize function should have been called 
	
      @Param
        IC_PARAMS_CAPTURE_TIMER

      @Returns
        None
     */
    inline static void IC1_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER timer) {
        IC1CON1bits.ICTSEL = timer;
    }
    
    /**  
    @Summary
        Sets the capture source

      @Description
        This routine sets the capture source for input capture module.
	
      @Preconditions
        IC1_Initialize function should have been called 
	
      @Param
        IC_PARAMS_CAPTURE_SOURCE

      @Returns
        None
     */
    inline static void IC1_SetCaptureSource(IC_PARAMS_CAPTURE_SOURCE source) {
        IC1CON2bits.SYNCSEL = source;
    }
    
    /**  
    @Summary
        Sets the capture mode

      @Description
        This routine sets the capture mode for input capture module.
	
      @Preconditions
        IC1_Initialize function should have been called 
	
      @Param
        IC_PARAMS_CAPTURE_MODE

      @Returns
        None
     */
    inline static void IC1_SetCaptureMode(IC_PARAMS_CAPTURE_MODE mode) {
        IC1CON1bits.ICM = mode;
    }
    
    /**  
    @Summary
        Combine IC modules

      @Description
        This routine combines odd and even input capture modules to form a 32 bit
        capture module. Need to set this for both odd and even IC modules
	
      @Preconditions
        IC1_Initialize function should have been called 
	
      @Param
        None

      @Returns
        None
     */
    inline static void IC1_CombineOddEvenICModules(void) {
        IC1CON2bits.IC32 = 1;
    }
    
    inline static void IC1_InputCaptureInterruptOn(IC_PARAMS_CAPTURE_INTERRUPT i) {
        IC1CON1bits.ICI = i;
    }
    
    inline static void IC1_UseSourceTo(IC_PARAMS_SOURCE_TASK t) {
        IC1CON2bits.ICTRIG = t;
    }
    
    inline static void IC1_InterruptFlagClear(void) {
        IFS0bits.IC1IF = 0;
    }

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif //_IC1_H
