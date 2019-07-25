
/**
  OC2 Generated Driver API Source File

  @Company
    Microchip Technology Inc.

  @File Name
    oc2.c

  @Summary
    This is the generated source file for the OC2 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for driver for OC2.
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
#include "oc2.h"

/** OC Mode.

  @Summary
    Defines the OC Mode.

  @Description
    This data type defines the OC Mode of operation.

*/

static uint16_t         gOC2Mode;

/**
  Section: Driver Interface
*/


void OC2_Initialize (void)
{
    // ENFLT0 disabled; ENFLT1 disabled; OCSIDL disabled; OCM Off; OCFLT1 disabled; OCFLT0 disabled; OCTSEL TMR2; TRIGMODE Only Software; 
    OC2CON1 = 0x00;
    // SYNCSEL TMR2; TRIGSTAT disabled; OCINV disabled; OCTRIG Sync; OC32 disabled; FLTOUT disabled; OCTRIS disabled; FLTMD Cycle; FLTTRIEN disabled; 
    OC2CON2 = 0x0C;
    // OC2RS 0; 
    OC2RS = 0x00;
    // OC2R 0; 
    OC2R = 0x00;
	
    gOC2Mode = OC2CON1bits.OCM;
}

void __attribute__ ((weak)) OC2_CallBack(void)
{
    // Add your custom callback code here
}

void OC2_Tasks( void )
{	
    if(IFS0bits.OC2IF)
    {
		// OC2 callback function 
		OC2_CallBack();
        IFS0bits.OC2IF = 0;
    }
}



void OC2_Start( void )
{
    OC2CON1bits.OCM = gOC2Mode;
}


void OC2_Stop( void )
{
    OC2CON1bits.OCM = 0;
}


void OC2_SingleCompareValueSet( uint16_t value )
{
    OC2R = value;
}


void OC2_DualCompareValueSet( uint16_t priVal, uint16_t secVal )
{
    OC2R = priVal;
	
    OC2RS = secVal;
}


void OC2_CentreAlignedPWMConfig( uint16_t priVal, uint16_t secVal )
{
    OC2R = priVal;
	
    OC2RS = secVal;
}

void OC2_EdgeAlignedPWMConfig( uint16_t priVal, uint16_t secVal )
{
    OC2R = priVal;
	
    OC2RS = secVal;
}

void OC2_SecondaryValueSet( uint16_t secVal )
{
   
    OC2RS = secVal;
}


void OC2_PrimaryValueSet( uint16_t priVal )
{
   
    OC2R = priVal;
}

bool OC2_IsCompareCycleComplete( void )
{
    return(IFS0bits.OC2IF);
}


bool OC2_FaultStatusGet( OC2_FAULTS faultNum )
{
    bool status;
    /* Return the status of the fault condition */
   
    switch(faultNum)
    { 
        case OC2_FAULT0:
			status = OC2CON1bits.OCFLT0;
            break;
        case OC2_FAULT1:
			status = OC2CON1bits.OCFLT1;
            break;
        default :
            break;

    }
    return(status);
}


void OC2_FaultStatusClear( OC2_FAULTS faultNum )
{
    
    switch(faultNum)
    { 
        case OC2_FAULT0:
			OC2CON1bits.OCFLT0 = 0;
                break;
        case OC2_FAULT1:
			OC2CON1bits.OCFLT1 = 0;
                break;
        default :
                break;
    }    
}


void OC2_ManualTriggerSet( void )
{
    OC2CON2bits.TRIGSTAT= true; 
}

bool OC2_TriggerStatusGet( void )
{
    return( OC2CON2bits.TRIGSTAT );
}


void OC2_TriggerStatusClear( void )
{
    /* Clears the trigger status */
    OC2CON2bits.TRIGSTAT = 0;
}

/**
 End of File
*/
