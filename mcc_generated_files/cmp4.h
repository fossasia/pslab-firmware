/**
  CMP4 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    cmp4.h

  @Summary
    This is the generated header file for the CMP4 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for CMP4.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.95-b-SNAPSHOT
        Device            :  PIC24EP256GP204
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36
        MPLAB 	          :  MPLAB X v5.10
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
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

#ifndef _CMP4_H
#define _CMP4_H

/**
  Section: Included Files
*/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif


/**
  @Summary
    This function initializes Comparator instance : 4

  @Description
    This routine initializes the Comparator driver instance for : 4, 
    making it ready for clients to open and use it. It also initializes any
    internal data structures.

  @Param
    None.

  @Returns 
    None
 
  @Comment
    
*/
void CMP4_Initialize(void);
/**
    void CMP4_Initialize(void)
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
bool CMP4_OutputStatusGet(void);
/**
    bool CMP4_OutputStatusGet(void)
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
bool CMP4_EventStatusGet(void);
/**
    bool CMP4_EventStatusGet(void)
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
void CMP4_EventStatusReset(void);
/**
    void CMP4_EventStatusReset(void)
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
void CMP4_ComparatorDisable(void);
/**
    void CMP4_ComparatorDisable(void)
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
void CMP4_ComparatorEnable(void);
/**
    void CMP4_ComparatorEnable(void)
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
bool CMP4_HLMSStatusGet(void);
/**
    bool CMP4_HLMSStatusGet(void)
*/

/**
  @Summary
    Callback for CMP4.

  @Description
    This routine is callback for CMP4

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to CMP4_Initialize(); for an example
*/
void CMP4_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    CMP4_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to CMP4_Initialize(); for an example
    
*/
void CMP4_Tasks(void);

#ifdef __cplusplus  // Provide C++ Compatibility

#endif
   
#endif //_CMP4_H

/**
  End of File
*/
