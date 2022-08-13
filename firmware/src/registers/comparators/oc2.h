#ifndef _OC2_H
#define _OC2_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

    /** OC2 Fault Number

      @Summary
        Defines the fault number

      @Description
        This type defines the various faults supported.

      Remarks:
        None
     */

    typedef enum {
        /* Fault 0 */
        OC2_FAULT0 /*DOM-IGNORE-BEGIN*/ = 0, /*DOM-IGNORE-END*/
        /* Fault 1 */
        OC2_FAULT1 /*DOM-IGNORE-BEGIN*/ = 1, /*DOM-IGNORE-END*/
    } OC2_FAULTS;

    /**
      Section: Interface Routines
     */


    /**
      @Summary
        This function initializes OC instance : 2

      @Description
        This routine initializes the OC2 driver instance for : 2
        index, making it ready for clients to open and use it.
        This routine must be called before any other OC2 routine is called.
	
      @Preconditions
        None.

      @Param
        None.

      @Returns
        None.

      @Example
        <code>
        uint16_t priVal,secVal;
        bool completeCycle = false;
        priVal = 0x1000;
        secVal = 0x2000;
        OC2_FAULTS faultNum = OC2_FAULT0;

        OC2_Initialize();
    
        OC2_CentreAlignedPWMConfig( priVal, secVal );
  
        OC2_Start();

        while(1)
        {
            faultStat =  OC2_FaultStatusGet( faultNum );

            if(faultStat)
            {
                OC2_FaultStatusClear( faultNum );
            }

            completeCycle = OC2_IsCompareCycleComplete( void );
            if(completeCycle)
            {
                OC2_Stop();
            }
        }
        </code>

     */

    void OC2_Initialize(void);

    void OC2_InitializeCON1(void);
    void OC2_InitializeCON2(void);

    /**
      @Summary
        Callback for OC2.

      @Description
        This routine is callback for OC2

      @Param
        None.

      @Returns
        None
 
      @Example 
        Refer to OC2_Initialize(); for an example
     */
    void OC2_CallBack(void);

    /**
      @Summary
        Maintains the driver's state machine and implements its ISR

      @Description
        This routine is used to maintain the driver's internal state
        machine and implement its ISR for interrupt-driven implementations.
  
      @Preconditions
        None.

      @Param
        None.

      @Returns
        None.

      @Example
        <code>
        while (true)
        {
            OC2_Tasks();

            // Do other tasks
        }
        </code>
    
     */
    void OC2_Tasks(void);

    /**
      @Summary
        Enables the OC module with the corresponding operation mode.

      @Description
        This routine enables the OC module with the corresponding operation mode.

      @Preconditions
        OC2_Initialize function should have been called 

      @Param
        None.

      @Returns
        None.

      @Example 
        Refer to OC2_Initialize() for an example	
 
     */
    void OC2_Start(void);

    /**
      @Summary
        Disables the OC module.

      @Description
        This routine disables the OC module.

      @Preconditions
        OC2_Initialize function should have been called 

      @Param
        None.

      @Returns
        None.

     */
    void OC2_Stop(void);

    /**
      @Summary
        Sets the primary compare value of respective OC.
	
      @Description
        This routine sets the primary compare value for respective OC.

      @Preconditions
        OC2_Initialize function should have been called 

      @Param
        priVal - 16 bit primary compare value.
    

      @Returns
        None.

      @Example 
        <code>
            uint16_t priVal = 0x1000;
            OC2_PrimaryValueSet( uint16_t priVal);
        <code> 
 	
     */
    void OC2_PrimaryValueSet(uint16_t priVal);

    /**
      @Summary
        Sets the secondary compare value of respective OC.
	
      @Description
        This routine sets the Secondary compare value for respective OC.

      @Preconditions
        OC2_Initialize function should have been called 

      @Param
        secVal - 16 bit secondary compare value.
    

      @Returns
        None.

      @Example 
        <code>
            uint16_t secVal = 0x1000;
            OC2_SecondaryValueSet( uint16_t secVal);
        <code> 
 	
     */
    void OC2_SecondaryValueSet(uint16_t secVal);


    /**
      @Summary
        Gets the status of the compare cycle completion.

      @Description
        This routine gets the status of the compare cycle completion.

      @Preconditions
        OC2_Initialize function should have been called 

      @Param
        None.

      @Returns
        Boolean value describing the current status of the cycle completion. Returns
        true  : When the compare cycle has completed. 
        false : When the compare cycle has not completed. 

      @Example 
        Refer to OC2_Initialize() for an example
	

     */
    bool OC2_IsCompareCycleComplete(void);

    /**
      @Summary
        Gets the status of the PWM fault condition occurrence.

      @Description
        This routine gets the status of the PWM fault condition occurrence.
  
      @Preconditions
        OC2_Initialize function should have been called 

      @Param
        faultNum - The fault number

      @Returns
        boolean value describing the occurrence of the fault condition.
        true  : When the specified fault has occurred.
        false : When the specified fault has not occurred.
	
      @Example 
        Refer to OC2_Initialize() for an example 
 

     */
    bool OC2_FaultStatusGet(OC2_FAULTS faultNum);

    /**
      @Summary
        Clears the status of the PWM fault condition occurrence.

      @Description
        This routine clears the status of the PWM fault condition occurrence.
	
      @Preconditions
        None.	

      @Param
        faultNum - The fault number
  
      @Returns
        None.

      @Example 
        Refer to OC2_Initialize() for an example 
  	
     */
    void OC2_FaultStatusClear(OC2_FAULTS faultNum);


    /**
      @Summary
        Sets the manual trigger

      @Description
        This routine sets the manual trigger
	
      @Preconditions
        OC2_Initialize function should have been called 

      @Param
        None
 
      @Returns
        None.
	
      @Example 
        Refer to OC2_TriggerStatusGet() for an example	
 
     */
    void OC2_ManualTriggerSet(void);

    /**
      @Summary
        Gets the status of the timer trigger.

      @Description
        This routine gets the status of the timer trigger source if it has been triggered.
	
      @Preconditions
        OC2_Initialize function should have been called 
	
      @Param
        None
	
      @Returns
        Boolean value describing the timer trigger status.
        true  : When the timer source has triggered and is running 
        false : When the timer has not triggered and being held clear 

      @Example 
        <\code>	
        if(OC2_TriggerStatusGet())
        {
            OC2_TriggerStatusClear();
        }
        <\code>	
     */
    bool OC2_TriggerStatusGet(void);

    /**
      @Summary
        Clears the status of the timer trigger.

      @Description
        This routine clears the status of the timer trigger.
	
      @Preconditions
        OC2_Initialize function should have been called

      @Param
        None
	
      @Returns
        None.
	
      @Example 
        Refer to OC2_TriggerStatusGet() for an example	

     */
    void OC2_TriggerStatusClear(void);

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif //_OC2_H

/**
 End of File
 */
