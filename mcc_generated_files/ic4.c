/**
  IC4 Generated Driver API Source File

  @Company
    Microchip Technology Inc.

  @File Name
    ic4.c

  @Summary
    This is the generated source file for the IC4 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This source file provides APIs for driver for IC4.
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
#include "ic4.h"

/**
  IC Mode.

  @Summary
    Defines the IC Mode.

  @Description
    This data type defines the IC Mode of operation.

*/

static uint16_t         gIC4Mode;

/**
  Section: Driver Interface
*/

void IC4_Initialize (void)
{
    // ICSIDL disabled; ICM Off; ICTSEL TMR3; ICI Every; 
    IC4CON1 = 0x00;
    // SYNCSEL TMR3; TRIGSTAT disabled; IC32 disabled; ICTRIG Sync; 
    IC4CON2 = 0x0D;
    
    gIC4Mode = IC4CON1bits.ICM;
    
}


void __attribute__ ((weak)) IC4_CallBack(void)
{
    // Add your custom callback code here
}

void IC4_Tasks( void )
{	
    if(IFS2bits.IC4IF)
    {
		// IC4 callback function 
		IC4_CallBack();
        IFS2bits.IC4IF = 0;
    }
}
void IC4_Start( void )
{
    IC4CON1bits.ICM = gIC4Mode;
}

void IC4_Stop( void )
{
    IC4CON1bits.ICM = 0;
}

uint16_t IC4_CaptureDataRead( void )
{
    return(IC4BUF);
}

void IC4_ManualTriggerSet( void )
{
    IC4CON2bits.TRIGSTAT= true; 
}

bool IC4_TriggerStatusGet( void )
{
    return( IC4CON2bits.TRIGSTAT );
}


void IC4_TriggerStatusClear( void )
{
    /* Clears the trigger status */
    IC4CON2bits.TRIGSTAT = 0;
}
bool IC4_HasCaptureBufferOverflowed( void )
{
    return( IC4CON1bits.ICOV );
}


bool IC4_IsCaptureBufferEmpty( void )
{
    return( ! IC4CON1bits.ICBNE );
}

/**
 End of File
*/
