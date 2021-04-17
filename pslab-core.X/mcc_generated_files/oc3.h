/**
  OC3 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    oc3.h

  @Summary
    This is the generated header file for the OC3 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for OC3.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC24EP256GP204
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB 	          :  MPLAB X v5.45
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#ifndef _OC3_H
#define _OC3_H

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
  Section: Data Types
*/

/** OC3 Fault Number

  @Summary
    Defines the fault number

  @Description
    This type defines the various faults supported.

  Remarks:
    None
*/

typedef enum
{   
    /* Fault 0 */
    OC3_FAULT0        /*DOM-IGNORE-BEGIN*/  = 0,/*DOM-IGNORE-END*/
    /* Fault 1 */
    OC3_FAULT1        /*DOM-IGNORE-BEGIN*/  = 1,/*DOM-IGNORE-END*/
} OC3_FAULTS;

/**
  Section: Interface Routines
*/


/**
  @Summary
    This function initializes OC instance : 3

  @Description
    This routine initializes the OC3 driver instance for : 3
    index, making it ready for clients to open and use it.
    This routine must be called before any other OC3 routine is called.
	
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
    OC3_FAULTS faultNum = OC3_FAULT0;

    OC3_Initialize();
    
    OC3_CentreAlignedPWMConfig( priVal, secVal );
  
    OC3_Start();

    while(1)
    {
        faultStat =  OC3_FaultStatusGet( faultNum );

        if(faultStat)
        {
            OC3_FaultStatusClear( faultNum );
        }

        completeCycle = OC3_IsCompareCycleComplete( void );
        if(completeCycle)
        {
            OC3_Stop();
        }
    }
    </code>

*/

void OC3_Initialize (void);

/**
  @Summary
    Callback for OC3.

  @Description
    This routine is callback for OC3

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to OC3_Initialize(); for an example
*/
void OC3_CallBack(void);

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
        OC3_Tasks();

        // Do other tasks
    }
    </code>
    
*/
void OC3_Tasks( void );

/**
  @Summary
    Enables the OC module with the corresponding operation mode.

  @Description
    This routine enables the OC module with the corresponding operation mode.

  @Preconditions
    OC3_Initialize function should have been called 

  @Param
    None.

  @Returns
    None.

  @Example 
    Refer to OC3_Initialize() for an example	
 
*/
void OC3_Start( void );

/**
  @Summary
    Disables the OC module.

  @Description
    This routine disables the OC module.

  @Preconditions
    OC3_Initialize function should have been called 

  @Param
    None.

  @Returns
    None.

*/
void OC3_Stop( void );

/**
  @Summary
    Sets the primary compare value of respective OC.
	
  @Description
    This routine sets the primary compare value for respective OC.

  @Preconditions
    OC3_Initialize function should have been called 

  @Param
    priVal - 16 bit primary compare value.
    

  @Returns
    None.

  @Example 
    <code>
        uint16_t priVal = 0x1000;
        OC3_PrimaryValueSet( uint16_t priVal);
    <code> 
 	
*/
void OC3_PrimaryValueSet( uint16_t priVal);

/**
  @Summary
    Sets the secondary compare value of respective OC.
	
  @Description
    This routine sets the Secondary compare value for respective OC.

  @Preconditions
    OC3_Initialize function should have been called 

  @Param
    secVal - 16 bit secondary compare value.
    

  @Returns
    None.

  @Example 
    <code>
        uint16_t secVal = 0x1000;
        OC3_SecondaryValueSet( uint16_t secVal);
    <code> 
 	
*/
void OC3_SecondaryValueSet( uint16_t secVal);


/**
  @Summary
    Gets the status of the compare cycle completion.

  @Description
    This routine gets the status of the compare cycle completion.

  @Preconditions
    OC3_Initialize function should have been called 

  @Param
    None.

  @Returns
    Boolean value describing the current status of the cycle completion. Returns
    true  : When the compare cycle has completed. 
    false : When the compare cycle has not completed. 

  @Example 
    Refer to OC3_Initialize() for an example
	

*/
bool OC3_IsCompareCycleComplete( void );

/**
  @Summary
    Gets the status of the PWM fault condition occurrence.

  @Description
    This routine gets the status of the PWM fault condition occurrence.
  
  @Preconditions
    OC3_Initialize function should have been called 

  @Param
    faultNum - The fault number

  @Returns
    boolean value describing the occurrence of the fault condition.
    true  : When the specified fault has occurred.
    false : When the specified fault has not occurred.
	
  @Example 
    Refer to OC3_Initialize() for an example 
 

*/
bool OC3_FaultStatusGet( OC3_FAULTS faultNum );

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
    Refer to OC3_Initialize() for an example 
  	
*/
void OC3_FaultStatusClear( OC3_FAULTS faultNum );


/**
  @Summary
    Sets the manual trigger

  @Description
    This routine sets the manual trigger
	
  @Preconditions
    OC3_Initialize function should have been called 

  @Param
    None
 
  @Returns
    None.
	
  @Example 
    Refer to OC3_TriggerStatusGet() for an example	
 
*/
void OC3_ManualTriggerSet( void );

/**
  @Summary
    Gets the status of the timer trigger.

  @Description
    This routine gets the status of the timer trigger source if it has been triggered.
	
  @Preconditions
    OC3_Initialize function should have been called 
	
  @Param
    None
	
  @Returns
    Boolean value describing the timer trigger status.
    true  : When the timer source has triggered and is running 
    false : When the timer has not triggered and being held clear 

  @Example 
    <\code>	
    if(OC3_TriggerStatusGet())
    {
        OC3_TriggerStatusClear();
    }
    <\code>	
*/
bool OC3_TriggerStatusGet( void );

/**
  @Summary
    Clears the status of the timer trigger.

  @Description
    This routine clears the status of the timer trigger.
	
  @Preconditions
    OC3_Initialize function should have been called

  @Param
    None
	
  @Returns
    None.
	
  @Example 
    Refer to OC3_TriggerStatusGet() for an example	

*/
void OC3_TriggerStatusClear( void );

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif //_OC3_H
    
/**
 End of File
*/
