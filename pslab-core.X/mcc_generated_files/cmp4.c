
/**
  CMP4 Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    cmp4.c

  @Summary
    This is the generated driver implementation file for the CMP4 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides implementations for driver APIs for CMP4. 
    Generation Information : 
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC24EP256GP204
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
        MPLAB             :  MPLAB X v5.45
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

/**
  Section: Included Files
*/

#include "cmp4.h"

/**
  Section: Driver Interface
*/

void CMP4_Initialize(void)
{   
    
    // PSIDL disabled; 
    CMSTAT = 0x00;
    // CON enabled; CPOL Not Inverted; EVPOL Disabled; COE disabled; CCH C4IN1-; CREF C4IN1+; CEVT disabled; 
    CM4CON = 0x8000 & ~(0x8000);  //disabling Comparator CON bit
    // SELSRCC PWM1L; SELSRCB PWM1L; SELSRCA PWM1L; 
    CM4MSKSRC = 0x00;
    // AANEN disabled; ABNEN disabled; ACNEN disabled; HLMS disabled; OANEN disabled; ABEN disabled; ACEN disabled; AAEN disabled; PAGS disabled; OBEN disabled; OCEN disabled; NAGS disabled; OCNEN disabled; OBNEN disabled; OAEN disabled; 
    CM4MSKCON = 0x00;
    // CFSEL FOSC/2; CFLTREN disabled; CFDIV 1:1; 
    CM4FLTR = 0x00;
    
    CM4CONbits.CON = 1; //enabling Comparator CON bit
}
bool CMP4_OutputStatusGet(void)
{
    return (CM4CONbits.COUT);
}
bool CMP4_EventStatusGet(void)
{   
    return (CM4CONbits.CEVT);      
}
void CMP4_EventStatusReset(void)
{
    CM4CONbits.CEVT = 0;
}
bool CMP4_HLMSStatusGet(void)
{
    return (CM4MSKCONbits.HLMS);
}

void CMP4_ComparatorDisable(void)
{
    CM4CONbits.CON = 0;
}

void CMP4_ComparatorEnable(void)
{
    CM4CONbits.CON = 1;
}

void __attribute__ ((weak)) CMP4_CallBack(void)
{
    // Add your custom callback code here
}

void CMP4_Tasks ( void )
{
	if(IFS1bits.CMIF)
	{
		// CMP4 callback function 
		CMP4_CallBack();
		
		// Clear the CEVT bit to enable further interrupts
		CMP4_EventStatusReset();
		
		// clear the CMP4 interrupt flag
		IFS1bits.CMIF = 0;
	}
}

/**
  End of File
*/
