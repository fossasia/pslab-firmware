
/**
  CMP1 Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    cmp1.c

  @Summary
    This is the generated driver implementation file for the CMP1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides implementations for driver APIs for CMP1. 
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
#include "cmp1.h"
/**
  Section: Driver Interface
*/

void CMP1_Initialize(void)
{   
    
    // PSIDL disabled; 
    CMSTAT = 0x00;
    // CON enabled; CPOL Not Inverted; OPMODE Comparator; EVPOL Disabled; COE disabled; CCH C1IN1-; CREF C1IN1+; CEVT disabled; 
    CM1CON = 0x8000 & ~(0x8000);  //disabling Comparator CON bit
    // SELSRCC PWM1L; SELSRCB PWM1L; SELSRCA PWM1L; 
    CM1MSKSRC = 0x00;
    // AANEN disabled; ABNEN disabled; ACNEN disabled; HLMS disabled; OANEN disabled; ABEN disabled; ACEN disabled; AAEN disabled; PAGS disabled; OBEN disabled; OCEN disabled; NAGS disabled; OCNEN disabled; OBNEN disabled; OAEN disabled; 
    CM1MSKCON = 0x00;
    // CFSEL FOSC/2; CFLTREN disabled; CFDIV 1:1; 
    CM1FLTR = 0x00;
    
    CM1CONbits.CON = 1; //enabling Comparator CON bit
}
bool CMP1_OutputStatusGet(void)
{
    return (CM1CONbits.COUT);
}
bool CMP1_EventStatusGet(void)
{   
    return (CM1CONbits.CEVT);      
}
void CMP1_EventStatusReset(void)
{
    CM1CONbits.CEVT = 0;
}
bool CMP1_HLMSStatusGet(void)
{
    return (CM1MSKCONbits.HLMS);
}

void CMP1_ComparatorDisable(void)
{
    CM1CONbits.CON = 0;
}

void CMP1_ComparatorEnable(void)
{
    CM1CONbits.CON = 1;
}

void __attribute__ ((weak)) CMP1_CallBack(void)
{
    // Add your custom callback code here
}

void CMP1_Tasks ( void )
{
	if(IFS1bits.CMIF)
	{
		// CMP1 callback function 
		CMP1_CallBack();
		
		// Clear the CEVT bit to enable further interrupts
		CMP1_EventStatusReset();
		
		// clear the CMP1 interrupt flag
		IFS1bits.CMIF = 0;
	}
}

/**
  End of File
*/
