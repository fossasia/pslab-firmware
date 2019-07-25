
/**
  PTG Generated Driver API Source File 

  @Company
    Microchip Technology Inc.

  @File Name
    ptg.c

  @Summary
    This is the generated source file for the PTG driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for driver for PTG. 
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

/**
  Section: Included Files
*/

#include <xc.h>
#include "ptg.h"

/**
  Section: PTG Module APIs
*/

void PTG_Initialize (void)
{
    // PTGEN disabled; PTGSSEN disabled; PTGSWT disabled; PTGSIDL disabled; PTGTOGL disabled; PTGITM Continuous edge detect with Step delay executed on exit of command; PTGWTO disabled; PTGIVIS disabled; PTGSTRT disabled; 
    PTGCST = (0x00 & 0x77FF);  // disabling PTGEN bit and PTGSTRT bit  
    // Clock Frequency = 60.0 MHz; PTGWDT disabled; PTGDIV 1; PTGCLK FOSC/2; PTGPWD 1; 
    PTGCON = 0x00;
    // IC4TSS Does not generate Trigger/Synchronization; OC1TSS Does not generate Trigger/Synchronization; OC2TSS Does not generate Trigger/Synchronization; IC3TSS Does not generate Trigger/Synchronization; OC3TSS Does not generate Trigger/Synchronization; OC4TSS Does not generate Trigger/Synchronization; IC1TSS Does not generate Trigger/Synchronization; IC2TSS Does not generate Trigger/Synchronization; OC4CS Does not generate clock pulse; OC1CS Does not generate clock pulse; OC2CS Does not generate clock pulse; OC3CS Does not generate clock pulse; ADCTS1 Does not generate Trigger/Synchronization; ADCTS2 Does not generate Trigger/Synchronization; ADCTS3 Does not generate Trigger/Synchronization; ADCTS4 Does not generate Trigger/Synchronization; 
    PTGBTE = 0x00;
    // PTGHOLD 0; 
    PTGHOLD = 0x00;
    // Timer0 delay = 0.0 ns; PTGT0LIM 0; 
    PTGT0LIM = 0x00;    
    // Timer1 delay = 0.0 ns; PTGT1LIM 0; 
    PTGT1LIM = 0x00;    
    // Step delay = 0.0 ns; PTGSDLIM 0; 
    PTGSDLIM = 0x00;
    // PTGC0LIM 0; 
    PTGC0LIM = 0x00;
    // PTGC1LIM 0; 
    PTGC1LIM = 0x00;
    // PTGADJ 0; 
    PTGADJ = 0x00;
    // PTGL0 0; 
    PTGL0 = 0x00;
    // PTGQPTR 0; 
    PTGQPTR = 0x00;

  /** 
   Step Commands 
  */

    


}

void PTG_Enable (void)
{
    PTGCSTbits.PTGEN = 1;
}

void PTG_StartStepSequence (void)
{
    PTGCSTbits.PTGSTRT = 1;
}

void PTG_SoftwareTriggerSet (void)
{
    PTGCSTbits.PTGSWT = 1;    
}

bool PTG_WatchdogTimeoutStatusGet (void)
{
    return( PTGCSTbits.PTGWTO);
}

void PTG_StopStepSequence (void)
{
    PTGCSTbits.PTGSTRT = 0;
}   

void PTG_Disable (void)
{
    PTGCSTbits.PTGEN = 0;
}


void __attribute__ ((weak)) PTG_Trigger0_CallBack(void)
{
    // Add your custom callback code here
}

void PTG_Trigger0_Tasks ( void )
{
	if(IFS9bits.PTG0IF)
	{
		// PTG Trigger0 callback function 
		PTG_Trigger0_CallBack();
		
		// clear the PTG Trigger0 interrupt flag
		IFS9bits.PTG0IF = 0;
	}
}

void __attribute__ ((weak)) PTG_Trigger1_CallBack(void)
{
    // Add your custom callback code here
}

void PTG_Trigger1_Tasks ( void )
{
	if(IFS9bits.PTG1IF)
	{
		// PTG Trigger1 callback function 
		PTG_Trigger1_CallBack();
		
		// clear the PTG Trigger1 interrupt flag
		IFS9bits.PTG1IF = 0;
	}
}

void __attribute__ ((weak)) PTG_Trigger2_CallBack(void)
{
    // Add your custom callback code here
}

void PTG_Trigger2_Tasks ( void )
{
	if(IFS9bits.PTG2IF)
	{
		// PTG Trigger2 callback function 
		PTG_Trigger2_CallBack();
		
		// clear the PTG Trigger2 interrupt flag
		IFS9bits.PTG2IF = 0;
	}
}

void __attribute__ ((weak)) PTG_Trigger3_CallBack(void)
{
    // Add your custom callback code here
}

void PTG_Trigger3_Tasks ( void )
{
	if(IFS9bits.PTG3IF)
	{
		// PTG Trigger3 callback function 
		PTG_Trigger3_CallBack();
		
		// clear the PTG Trigger3 interrupt flag
		IFS9bits.PTG3IF = 0;
	}
}

void __attribute__ ((weak)) PTG_WatchDogTimer_CallBack(void)
{
    // Add your custom callback code here
}

void PTG_WatchDogTimer_Tasks ( void )
{
	if(IFS9bits.PTGWDTIF)
	{
		// PTG WatchDogTimer callback function 
		PTG_WatchDogTimer_CallBack();
		
		// clear the PTG WatchDogTimer interrupt flag
		IFS9bits.PTGWDTIF = 0;
	}
}
