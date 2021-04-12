/**
  RESET Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    reset.c

  @Summary
    This is the generated driver implementation file for the RESET driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides implementations for driver APIs for RESET.
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

#include <stdbool.h>
#include <stdint.h>
#include "xc.h"
#include "reset.h"

/**
 Section: Local Variables
*/

/**
 Section: Function prototypes
*/
static bool RESET_CauseFromSoftware(uint16_t resetCause);
static bool RESET_CauseFromWatchdogTimer(uint16_t resetCause);
static bool RESET_CauseFromConfigurationMismatch(uint16_t resetCause);
static bool RESET_CauseFromIllegalOpcode(uint16_t resetCause);
static bool RESET_CauseFromExternal(uint16_t resetCause);
static bool RESET_CauseFromTrap(uint16_t resetCause);
static void RESET_CauseClear(RESET_MASKS resetFlagMask);

uint16_t RESET_GetCause(void)
{
    return RCON;
}

void __attribute__ ((weak)) RESET_CauseHandler(void)
{
    uint16_t resetCause = RESET_GetCause();
    if(RESET_CauseFromTrap(resetCause))
    { 
      RESET_CauseClear(RESET_MASK_TRAPR); 
      //Do something 
    }
    if(RESET_CauseFromIllegalOpcode(resetCause))
    { 
      RESET_CauseClear(RESET_MASK_IOPUWR); 
      //Do something 
    }
    if(RESET_CauseFromConfigurationMismatch(resetCause))
    { 
      RESET_CauseClear(RESET_MASK_CM); 
      //Do something 
    }
    if(RESET_CauseFromExternal(resetCause))
    { 
      RESET_CauseClear(RESET_MASK_EXTR); 
      //Do something 
    }
    if(RESET_CauseFromSoftware(resetCause))
    { 
      RESET_CauseClear(RESET_MASK_SWR); 
      //Do something 
    }
    if(RESET_CauseFromWatchdogTimer(resetCause))
    { 
      RESET_CauseClear(RESET_MASK_WDTO); 
      //Do something 
    }
}

static bool RESET_CauseFromTrap(uint16_t resetCause)
{
    bool resetStatus = false;
    if(resetCause & RESET_MASK_TRAPR)
    { 
      resetStatus = true; 
    }
    return resetStatus;
}

static bool RESET_CauseFromIllegalOpcode(uint16_t resetCause)
{
    bool resetStatus = false;
    if(resetCause & RESET_MASK_IOPUWR)
    { 
      resetStatus = true; 
    }
    return resetStatus;
}

static bool RESET_CauseFromConfigurationMismatch(uint16_t resetCause)
{
    bool resetStatus = false;
    if(resetCause & RESET_MASK_CM)
    { 
      resetStatus = true; 
    }
    return resetStatus;
}

static bool RESET_CauseFromExternal(uint16_t resetCause)
{
    bool resetStatus = false;
    if(resetCause & RESET_MASK_EXTR)
    { 
      resetStatus = true; 
    }
    return resetStatus;
}

static bool RESET_CauseFromSoftware(uint16_t resetCause)
{
    bool resetStatus = false;
    if(resetCause & RESET_MASK_SWR)
    { 
      resetStatus = true; 
    }
    return resetStatus;
}

static bool RESET_CauseFromWatchdogTimer(uint16_t resetCause)
{
    bool resetStatus = false;
    if(resetCause & RESET_MASK_WDTO)
    { 
      resetStatus = true;
    }
    return resetStatus;
}

static void RESET_CauseClear(RESET_MASKS resetFlagMask)
{ 
     RCON = RCON & (~resetFlagMask); 
} 

void RESET_CauseClearAll()
{ 
    RCON = 0x00; 
}
/**
 End of File
*/