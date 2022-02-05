#ifndef _IC4_H
#define _IC4_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "ic_params.h"

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

    void IC4_Initialize(void);

    void IC4_InitializeCON1(void);
    void IC4_InitializeCON2(void);
    
    /**
      @Summary
        Enables the IC module with the corresponding operation mode.

      @Description
        This routine enables the IC module with the corresponding operation mode.

      @Preconditions
        IC4_Initialize function should have been called 

      @Param
        IC_PARAMS_CAPTURE_MODE
 
      @Returns
        None.

      @Example 
        Refer to IC4_Initialize() for an example	
 
     */
    void IC4_Start(IC_PARAMS_CAPTURE_MODE mode);

    /** 
    @Summary
      Disables the IC module.

    @Description
      This routine disables the IC module.

    @Preconditions
      IC4_Initialize function should have been called 

    @Param
      None
 
    @Returns
      None.

    @Example 
      Refer to IC4_Initialize() for an example	
  	
     */
    void IC4_Stop(void);

    /**
      @Summary
        Reads the captured data from buffer.

      @Description
        This routine reads the captured data from buffer.

      @Preconditions
        IC4_Initialize function should have been called 

      @Param
        None
 
      @Returns
        Read data from buffer.

      @Example 
        Refer to IC4_Initialize() for an example	
  	
     */
    uint16_t IC4_CaptureDataRead(void);

    /**
      @Summary
        Sets the manual trigger

      @Description
        This routine sets the manual trigger
	
      @Preconditions
        IC4_Initialize function should have been called 

      @Param
        None
 
      @Returns
        None.
	
      @Example 
        Refer to IC4_TriggerStatusGet() for an example	
 
     */
    void IC4_ManualTriggerSet(void);

    /**
      @Summary
        Gets the status of the timer trigger.

      @Description
        This routine gets the status of the timer trigger source if it has been triggered.
	
      @Preconditions
        IC4_Initialize function should have been called
	
      @Param
        None
 
      @Returns
        Boolean value describing the timer trigger status.
        true  : When the timer source has triggered and is running 
        false : When the timer has not triggered and being held clear 

      @Example 
        <\code>	
        if(IC4_TriggerStatusGet())
        {
            IC4_TriggerStatusClear();
        }
        <\code>	
     */
    bool IC4_TriggerStatusGet(void);

    /**
      @Summary
        Clears the status of the timer trigger.

      @Description
        This routine clears the status of the timer trigger.
	
      @Preconditions
        IC4_Initialize function should have been called 

      @Param
        None
 
      @Returns
        None.
	
      @Example 
        Refer to IC4_TriggerStatusGet() for an example	
 
     */
    void IC4_TriggerStatusClear(void);

    /**
      @Summary
        Gets the buffer overflow status.

      @Description
        This routine gets the buffer overflow status.
	
      @Preconditions
        IC4_Initialize function should have been called 
	
      @Param
        None
 
      @Returns
        Boolean value describing the buffer overflow status.
        true  : When the capture buffer has overflowed
        false : When the capture buffer has not overflowed

       @Example 
        <\code>	
        bool status;

        status = IC4_HasCaptureBufferOverflowed();
        <\code>	
     */
    bool IC4_HasCaptureBufferOverflowed(void);

    /**  
    @Summary
        Gets the buffer empty status.

      @Description
        This routine gets the buffer empty status.
	
      @Preconditions
        IC4_Initialize function should have been called 
	
      @Param
        None

      @Returns
        Boolean value describing the buffer empty status.
        True- If buffer empty
        False-If buffer not empty

       @Example 
        Refer to IC4_Initialize() for an example	
     */
    bool IC4_IsCaptureBufferEmpty(void);

    /**  
    @Summary
        Sets the timer source

      @Description
        This routine sets the timer source for input capture module.
	
      @Preconditions
        IC4_Initialize function should have been called 
	
      @Param
        IC_PARAMS_CAPTURE_TIMER

      @Returns
        None
     */
    inline static void IC4_SetCaptureTimer(IC_PARAMS_CAPTURE_TIMER timer) {
        IC4CON1bits.ICTSEL = timer;
    }

    /**  
    @Summary
        Sets the capture source

      @Description
        This routine sets the capture source for input capture module.
	
      @Preconditions
        IC4_Initialize function should have been called 
	
      @Param
        IC_PARAMS_CAPTURE_SOURCE

      @Returns
        None
     */
    inline static void IC4_SetCaptureSource(IC_PARAMS_CAPTURE_SOURCE source) {
        IC4CON2bits.SYNCSEL = source;
    }
    
    /**  
    @Summary
        Sets the capture mode

      @Description
        This routine sets the capture mode for input capture module.
	
      @Preconditions
        IC4_Initialize function should have been called 
	
      @Param
        IC_PARAMS_CAPTURE_MODE

      @Returns
        None
     */
    inline static void IC4_SetCaptureMode(IC_PARAMS_CAPTURE_MODE mode) {
        IC4CON1bits.ICM = mode;
    }
    
    /**  
    @Summary
        Combine IC modules

      @Description
        This routine combines odd and even input capture modules to form a 32 bit
        capture module. Need to set this for both odd and even IC modules
	
      @Preconditions
        IC4_Initialize function should have been called 
	
      @Param
        None

      @Returns
        None
     */
    inline static void IC4_CombineOddEvenICModules(void) {
        IC4CON2bits.IC32 = 1;
    }
    
    inline static void IC4_UseSourceTo(IC_PARAMS_SOURCE_TASK t) {
        IC4CON2bits.ICTRIG = t;
    }

    inline static void IC4_InterruptFlagClear(void) {
        IFS2bits.IC4IF = 0;
    }
    
    inline static void IC4_InterruptEnable(void) {
        IEC2bits.IC4IE = 1;
    }
    
    inline static void IC4_InputCaptureInterruptOn(IC_PARAMS_CAPTURE_INTERRUPT i) {
        IC4CON1bits.ICI = i;
    }
    
    inline static void IC4_InterruptDisable(void) {
        IEC2bits.IC4IE = 0;
    }
    
    inline static void IC4_InterruptHighPriority(void) {
        IPC9bits.IC4IP = 7;
    }
    
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif //_IC4_H
