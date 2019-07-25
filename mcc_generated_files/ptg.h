/*
  PTG Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    ptg.h

  @Summary
    This is the generated header file for the PTG driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for PTG.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.95-b-SNAPSHOT
        Device            :  PIC24EP256GP204
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36
        MPLAB             :  MPLAB X v5.10
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

#ifndef _PTG_H
#define _PTG_H

/**
  Section: Included Files
*/

#include <stdint.h>
#include <stdbool.h>

/**
  Section: Helper Macros
*/ 



#define PTGCTRL  (0x0 << 4 )    //PTGCTRL command
#define PTGCOPY  (0x1 << 4 )    //PTGCOPY command
#define PTGADD  (0x1 << 4 )    //PTGADD command
#define PTGSTRB  (0x2 << 4 )    //PTGSTRB command
#define PTGWHI  (0x4 << 4 )    //PTGWHI command
#define PTGWLO  (0x5 << 4 )    //PTGWLO command
#define PTGIRQ  (0x7 << 4 )    //PTGIRQ command
#define PTGTRIG  (0x8 << 4 )    //PTGTRIG command
#define PTGJMP  (0xa << 4 )    //PTGJMP command
#define PTGJMPC0  (0xc << 4 )    //PTGJMPC0 command
#define PTGJMPC1  (0xe << 4 )    //PTGJMPC1 command


#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/**
  Section: PTG Module APIs
*/

/**
  @Summary
    Initializes the PTG

  @Description
    This routine initializes the PTG.
    This routine must be called before any other PTG routine is called.
    This routine should only be called once during system initialization.
    This routine does not enable PTG module and start the step sequence. 
    Functions PTG_Enable() and PTG_StartStepSequence() need to be called for
    enabling PTG module and for starting step sequence execution, respectively.
    
  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Example
    <code>
    void main(void)
    {
    // initialize the device
    
    SYSTEM_Initialize();

    PTG_Enable();
    PTG_StartStepSequence();

      while (1)
      {      
          PTG_SoftwareTriggerSet();  
          bool wdtBit = PTG_WatchdogTimeoutStatusGet();
              if(wdtBit){          
                 PTG_StopStepSequence();
                 PTG_Disable();
                }
      }
    }
    </code> 
*/
void PTG_Initialize(void);

/**
  @Summary
    Enables the PTG module.

  @Description
    This routine enables the PTG module.
  
  @Preconditions
    PTG_Initializer function should have been called 
    before calling this function.
  
  @Param
    None.

  @Returns
    None
 
  @Example 
    Refer to the example of PTG_Initialize();
*/

void PTG_Enable (void);

/**
  @Summary
    Start the step sequence execution.

  @Description
    This routine starts the step sequence execution.

  
  @Preconditions
    PTG_Initializer and PTG_Enable functions should have been called 
    before calling this function.

  @Param
    None.

  @Returns
    None
 
  @Example 
    Refer to the example of PTG_Initialize();
*/

void PTG_StartStepSequence (void);

/**
  @Summary
    Software trigger for PTG module

  @Description
    This routine provides a software trigger for PTG module
  
  @Preconditions
    None. 

  @Param
    None.

  @Returns
    None
 
  @Example 
    Refer to the example of PTG_Initialize();
*/

void PTG_SoftwareTriggerSet (void);

/**
  @Summary
    Status of PTG watchdog time out bit.

  @Description
    This routine returns the status of PTG watchdog time out bit.
    
  @Preconditions
    None.

  @Param
    None.

  @Returns
    Returns the status of PTG watchdog time out bit.
 
  @Example 
    Refer to the example of PTG_Initialize();
*/

bool PTG_WatchdogTimeoutStatusGet (void);

/**
  @Summary
    Stops the step sequence execution.

  @Description
    This routine stops the step sequence execution.

  @Preconditions
    None.

  @Param
    None.

  @Returns
    None
 
  @Example 
    Refer to the example of PTG_Initialize();
*/

void PTG_StopStepSequence (void);

/**
  @Summary
   Disable the PTG module. 

  @Description
   This routine disables the PTG module. 

  @Preconditions
    None.

  @Param
    None.

  @Returns
    None
 
  @Example 
    Refer to the example of PTG_Initialize();
*/

void PTG_Disable (void);

/**
  @Summary
    Callback for PTG Trigger0.

  @Description
    This routine is callback for PTG Trigger0

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to PTG_Initialize(); for an example
*/
void PTG_Trigger0_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    PTG_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to PTG_Initialize(); for an example
    
*/
void PTG_Trigger0_Tasks(void);
/**
  @Summary
    Callback for PTG Trigger1.

  @Description
    This routine is callback for PTG Trigger1

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to PTG_Initialize(); for an example
*/
void PTG_Trigger1_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    PTG_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to PTG_Initialize(); for an example
    
*/
void PTG_Trigger1_Tasks(void);
/**
  @Summary
    Callback for PTG Trigger2.

  @Description
    This routine is callback for PTG Trigger2

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to PTG_Initialize(); for an example
*/
void PTG_Trigger2_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    PTG_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to PTG_Initialize(); for an example
    
*/
void PTG_Trigger2_Tasks(void);
/**
  @Summary
    Callback for PTG Trigger3.

  @Description
    This routine is callback for PTG Trigger3

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to PTG_Initialize(); for an example
*/
void PTG_Trigger3_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    PTG_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to PTG_Initialize(); for an example
    
*/
void PTG_Trigger3_Tasks(void);
/**
  @Summary
    Callback for PTG WatchDogTimer.

  @Description
    This routine is callback for PTG WatchDogTimer

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to PTG_Initialize(); for an example
*/
void PTG_WatchDogTimer_CallBack(void);

/**
  @Summary
    Polled implementation

  @Description
    This routine is used to implement the tasks for polled implementations.
  
  @Preconditions
    PTG_Initialize() function should have been 
    called before calling this function.
 
  @Returns 
    None
 
  @Param
    None
 
  @Example
    Refer to PTG_Initialize(); for an example
    
*/
void PTG_WatchDogTimer_Tasks(void);

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif //_PTG_H
    
/**
 End of File
*/
