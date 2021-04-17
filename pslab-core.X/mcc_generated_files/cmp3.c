
/**
  CMP3 Generated Driver File 

  @Company
    Microchip Technology Inc.

  @File Name
    cmp3.c

  @Summary
    This is the generated driver implementation file for the CMP3 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides implementations for driver APIs for CMP3. 
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

#include "cmp3.h"

/**
  Section: Driver Interface
*/

void CMP3_Initialize(void)
{   
    
    // PSIDL disabled; 
    CMSTAT = 0x00;
    // CON enabled; CPOL Not Inverted; OPMODE Comparator; EVPOL Disabled; COE disabled; CCH C3IN1-; CREF C3IN1+; CEVT disabled; 
    CM3CON = 0x8000 & ~(0x8000);  //disabling Comparator CON bit
    // SELSRCC PWM1L; SELSRCB PWM1L; SELSRCA PWM1L; 
    CM3MSKSRC = 0x00;
    // AANEN disabled; ABNEN disabled; ACNEN disabled; HLMS disabled; OANEN disabled; ABEN disabled; ACEN disabled; AAEN disabled; PAGS disabled; OBEN disabled; OCEN disabled; NAGS disabled; OCNEN disabled; OBNEN disabled; OAEN disabled; 
    CM3MSKCON = 0x00;
    // CFSEL FOSC/2; CFLTREN disabled; CFDIV 1:1; 
    CM3FLTR = 0x00;
    
    CM3CONbits.CON = 1; //enabling Comparator CON bit
}
bool CMP3_OutputStatusGet(void)
{
    return (CM3CONbits.COUT);
}
bool CMP3_EventStatusGet(void)
{   
    return (CM3CONbits.CEVT);      
}
void CMP3_EventStatusReset(void)
{
    CM3CONbits.CEVT = 0;
}
bool CMP3_HLMSStatusGet(void)
{
    return (CM3MSKCONbits.HLMS);
}

void CMP3_ComparatorDisable(void)
{
    CM3CONbits.CON = 0;
}

void CMP3_ComparatorEnable(void)
{
    CM3CONbits.CON = 1;
}

void __attribute__ ((weak)) CMP3_CallBack(void)
{
    // Add your custom callback code here
}

void CMP3_Tasks ( void )
{
	if(IFS1bits.CMIF)
	{
		// CMP3 callback function 
		CMP3_CallBack();
		
		// Clear the CEVT bit to enable further interrupts
		CMP3_EventStatusReset();
		
		// clear the CMP3 interrupt flag
		IFS1bits.CMIF = 0;
	}
}

/**
  End of File
*/
