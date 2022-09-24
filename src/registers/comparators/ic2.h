#ifndef _IC2_H
#define _IC2_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "ic_params.h"

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

    void IC2_Initialize(void);

    void IC2_InitializeCON1(void);
    void IC2_InitializeCON2(void);
    
    /**
      @Summary
        Enables the IC module with the corresponding operation mode.

      @Description
        This routine enables the IC module with the corresponding operation mode.

      @Preconditions
        IC2_Initialize function should have been called 

      @Param
        None
 
      @Returns
        None.

      @Example 
        Refer to IC2_Initialize() for an example	
 
     */
    void IC2_Start(IC_PARAMS_CAPTURE_MODE mode);
    
    /** 
    @Summary
      Disables the IC module.

    @Description
      This routine disables the IC module.

    @Preconditions
      IC2_Initialize function should have been called 

    @Param
      None
 
    @Returns
      None.

    @Example 
      Refer to IC2_Initialize() for an example	
  	
     */
    void IC2_Stop(void);
    
    /**
      @Summary
        Reads the captured data from buffer.

      @Description
        This routine reads the captured data from buffer.

      @Preconditions
        IC2_Initialize function should have been called 

      @Param
        None
 
      @Returns
        Read data from buffer.

      @Example 
        Refer to IC2_Initialize() for an example	
  	
     */
    uint16_t IC2_CaptureDataRead(void);

    /**
      @Summary
        Sets the manual trigger

      @Description
        This routine sets the manual trigger
	
      @Preconditions
        IC2_Initialize function should have been called 

      @Param
        None
 
      @Returns
        None.
	
      @Example 
        Refer to IC2_TriggerStatusGet() for an example	
 
     */
    void IC2_ManualTriggerSet(void);

    /**
      @Summary
        Gets the status of the timer trigger.

      @Description
        This routine gets the status of the timer trigger source if it has been triggered.
	
      @Preconditions
        IC2_Initialize function should have been called
	
      @Param
        None
 
      @Returns
        Boolean value describing the timer trigger status.
        true  : When the timer source has triggered and is running 
        false : When the timer has not triggered and being held clear 

      @Example 
        <\code>	
        if(IC2_TriggerStatusGet())
        {
            IC2_TriggerStatusClear();
        }
        <\code>	
     */
    bool IC2_TriggerStatusGet(void);
    
    /**
      @Summary
        Clears the status of the timer trigger.

      @Description
        This routine clears the status of the timer trigger.
	
      @Preconditions
        IC2_Initialize function should have been called 

      @Param
        None
 
      @Returns
        None.
	
      @Example 
        Refer to IC2_TriggerStatusGet() for an example	
 
     */
    void IC2_TriggerStatusClear(void);
    
    /**
      @Summary
        Gets the buffer overflow status.

      @Description
        This routine gets the buffer overflow status.
	
      @Preconditions
        IC2_Initialize function should have been called 
	
      @Param
        None
 
      @Returns
        Boolean value describing the buffer overflow status.
        true  : When the capture buffer has overflowed
        false : When the capture buffer has not overflowed

       @Example 
        <\code>	
        bool status;

        status = IC2_HasCaptureBufferOverflowed();
        <\code>	
     */
    bool IC2_HasCaptureBufferOverflowed(void);
    
    /**  
    @Summary
        Gets the buffer empty status.

      @Description
        This routine gets the buffer empty status.
	
      @Preconditions
        IC2_Initialize function should have been called 
	
      @Param
        None

      @Returns
        Boolean value describing the buffer empty status.
        True- If buffer empty
        False-If buffer not empty

       @Example 
        Refer to IC2_Initialize() for an example	
     */
    bool IC2_IsCaptureBufferEmpty(void);
    
    /**  
    @Summary
        Sets the timer source

      @Description
        This routine sets the timer source for input capture module.
	
      @Preconditions
        IC2_Initialize function should have been called 
	
      @Param
        IC_PARAMS_CAPTURE_TIMER

      @Returns
        None
     */
    inline static void IC2_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER timer) {
        IC2CON1bits.ICTSEL = timer;
    }
    
    /**  
    @Summary
        Sets the capture source

      @Description
        This routine sets the capture source for input capture module.
	
      @Preconditions
        IC2_Initialize function should have been called 
	
      @Param
        IC_PARAMS_CAPTURE_SOURCE

      @Returns
        None
     */
    inline static void IC2_SetCaptureSource(IC_PARAMS_CAPTURE_SOURCE source) {
        IC2CON2bits.SYNCSEL = source;
    }
    
    /**  
    @Summary
        Sets the capture mode

      @Description
        This routine sets the capture mode for input capture module.
	
      @Preconditions
        IC2_Initialize function should have been called 
	
      @Param
        IC_PARAMS_CAPTURE_MODE

      @Returns
        None
     */
    inline static void IC2_SetCaptureMode(IC_PARAMS_CAPTURE_MODE mode) {
        IC2CON1bits.ICM = mode;
    }
    
    /**  
    @Summary
        Combine IC modules

      @Description
        This routine combines odd and even input capture modules to form a 32 bit
        capture module. Need to set this for both odd and even IC modules
	
      @Preconditions
        IC2_Initialize function should have been called 
	
      @Param
        None

      @Returns
        None
     */
    inline static void IC2_CombineOddEvenICModules(void) {
        IC2CON2bits.IC32 = 1;
    }

    /**
    @Summary
        Sets the trigger event for capture complete interrupt

      @Description
        This routine sets the trigger event for the input capture module to
        notify the ISR that a capture event is complete

      @Preconditions
        IC2_Initialize function should have been called

      @Param
        IC_PARAMS_CAPTURE_INTERRUPT

      @Returns
        None
     */
    inline static void IC2_InputCaptureInterruptOn(IC_PARAMS_CAPTURE_INTERRUPT i) {
        IC2CON1bits.ICI = i;
    }

    /**
    @Summary
        Defines the purpose of input capture pin source

    @Description
        This routine defines if the pin attached to IC module is used either to
        trigger the IC2TMR or to synchronize the timer with another timer module.

    @Preconditions
        IC2_Initialize function should have been called

    @Param
        IC_PARAMS_SOURCE_TASK

    @Returns
        None
    */
    inline static void IC2_UseSourceTo(IC_PARAMS_SOURCE_TASK t) {
        IC2CON2bits.ICTRIG = t;
    }

    /**
    @Summary
        Clears the set interrupt flag of IC2 capture interrupt

      @Description
        This routine will clear the interrupt flag of the IC2 capture
        module.

      @Preconditions
        IC2_Initialize function should have been called

      @Param
        None

      @Returns
        None
     */
    inline static void IC2_InterruptFlagClear(void) {
        IFS0bits.IC2IF = 0;
    }

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif //_IC2_H
