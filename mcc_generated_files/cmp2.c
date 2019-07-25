
/**
  CMP2 Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    cmp2.c

  @Summary
    This is the generated driver implementation file for the CMP2 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides implementations for driver APIs for CMP2. 
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
/**
  Section: Included Files
*/
#include <xc.h>
#include "cmp2.h"
/**
  Section: Driver Interface
*/

void CMP2_Initialize(void)
{   
    
    // PSIDL disabled; 
    CMSTAT = 0x00;
    // CON enabled; CPOL Not Inverted; OPMODE Comparator; EVPOL Disabled; COE disabled; CCH C2IN1-; CREF C2IN1+; CEVT disabled; 
    CM2CON = 0x8000 & ~(0x8000);  //disabling Comparator CON bit
    // SELSRCC PWM1L; SELSRCB PWM1L; SELSRCA PWM1L; 
    CM2MSKSRC = 0x00;
    // AANEN disabled; ABNEN disabled; ACNEN disabled; HLMS disabled; OANEN disabled; ABEN disabled; ACEN disabled; AAEN disabled; PAGS disabled; OBEN disabled; OCEN disabled; NAGS disabled; OCNEN disabled; OBNEN disabled; OAEN disabled; 
    CM2MSKCON = 0x00;
    // CFSEL FOSC/2; CFLTREN disabled; CFDIV 1:1; 
    CM2FLTR = 0x00;
    
    CM2CONbits.CON = 1; //enabling Comparator CON bit
}
bool CMP2_OutputStatusGet(void)
{
    return (CM2CONbits.COUT);
}
bool CMP2_EventStatusGet(void)
{   
    return (CM2CONbits.CEVT);      
}
void CMP2_EventStatusReset(void)
{
    CM2CONbits.CEVT = 0;
}
bool CMP2_HLMSStatusGet(void)
{
    return (CM2MSKCONbits.HLMS);
}

void CMP2_ComparatorDisable(void)
{
    CM2CONbits.CON = 0;
}

void CMP2_ComparatorEnable(void)
{
    CM2CONbits.CON = 1;
}

void __attribute__ ((weak)) CMP2_CallBack(void)
{
    // Add your custom callback code here
}

void CMP2_Tasks ( void )
{
	if(IFS1bits.CMIF)
	{
		// CMP2 callback function 
		CMP2_CallBack();
		
		// Clear the CEVT bit to enable further interrupts
		CMP2_EventStatusReset();
		
		// clear the CMP2 interrupt flag
		IFS1bits.CMIF = 0;
	}
}

/**
  End of File
*/
