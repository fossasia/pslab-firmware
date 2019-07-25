/**
  ADC1 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    adc1.h

  @Summary
    This is the generated header file for the ADC1 driver using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description
    This header file provides APIs for driver for ADC1.
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

#ifndef _ADC1_H
#define _ADC1_H

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
  Section: ISR Helper Macros
*/

#define ADC1_ISR_FUNCTION_HEADER    void __attribute__((interrupt, no_auto_psv)) _AD1Interrupt

/**
  Section: Data Types
*/

/** ADC Channel Definition
 
 @Summary 
   Defines the channels available for conversion
 
 @Description
   This routine defines the channels that are available conversion.
 
 Remarks:
   None
 */
typedef enum 
{
    ADC1_CHANNEL_CTMU =  0x1f,
    ADC1_MAX_CHANNEL_COUNT = 5
} ADC1_CHANNEL;

/** ADC Positive 123 Channels Definition
 
 @Summary 
   Defines the positive 123 channels available for conversion
 
 @Description
   This routine defines the positive 123 channels that are available for the 
   module to use.
 
 Remarks:
   None
 */
typedef enum 
{
    ADC1_POS_123_CHANNEL_0 = 0x0,
    ADC1_POS_123_CHANNEL_1 = 0x1
} ADC1_POS_123_CHANNEL;

/** ADC Negative 123 Channels Definition
 
 @Summary 
   Defines the negative 123 channels available for conversion
 
 @Description
   This routine defines the negative 123 channels that are available for the 
   module to use.
 
 Remarks:
   None
 */
typedef enum 
{
   ADC1_NEG_123_CHANNEL_0 = 0x0,
   ADC1_NEG_123_CHANNEL_1 = 0x2,
   ADC1_NEG_123_CHANNEL_2 = 0x3
} ADC1_NEG_123_CHANNEL;

/** ADC Data Format Type Definition
 
 @Summary 
   Defines the data format types available
 
 @Description
   This routine defines the data format types that are available for the module 
   to use.
 
 Remarks:
   None
 */
typedef enum 
{
    ADC1_FORM_UNSIGNED_INT   = 0, /* Unsigned Integer */
    ADC1_FORM_SIGNED_INT     = 1, /* Signed Integer */
    ADC1_FORM_UNSIGNED_FRACT = 2, /* Unsigned Fraction */
    ADC1_FORM_SIGNED_FRACT   = 3  /* Signed Integer */
} ADC1_FORM_TYPE;

/** ADC Resolution Type Definition
 
 @Summary 
   Defines the resolution types available
 
 @Description
   This routine defines the resolution types that are available for the module 
   to use.
 
 Remarks:
   None
 */
typedef enum 
{
    ADC1_RESOLUTION_10_BIT   = 0, /* 10-bit, 4-channel ADC operation */
    ADC1_RESOLUTION_12_BIT   = 1  /* 12-bit, 1-channel ADC operation */
} ADC1_RESOLUTION_TYPE;

/** ADC Sampling Source Definition
 
 @Summary 
   Defines the sampling sources available
 
 @Description
   This routine defines the sampling sources that are available for the module 
   to use.
 
 Remarks:
   None
 */
typedef enum 
{
    ADC1_SAMPLING_SOURCE_AUTO  =  0x7,
    ADC1_SAMPLING_SOURCE_PTGO12  =  0x3,
    ADC1_SAMPLING_SOURCE_TMR3  =  0x2,
    ADC1_SAMPLING_SOURCE_INT0  =  0x1,
    ADC1_SAMPLING_SOURCE_PTGO13  =  0x4,
    ADC1_SAMPLING_SOURCE_CTMU  =  0x6,
    ADC1_SAMPLING_SOURCE_TMR5  =  0x4,
    ADC1_SAMPLING_SOURCE_PTGO15  =  0x6,
    ADC1_SAMPLING_SOURCE_MANUAL  =  0x0,
    ADC1_SAMPLING_SOURCE_PTGO14  =  0x5,
} ADC1_SAMPLING_SOURCE;

/** ADC Conversion Channel Type Definition
 
 @Summary 
   Defines the conversion channel types available
 
 @Description
   This routine defines the conversion channels types that are available for the 
   module to use.
 
 Remarks:
   None
 */
typedef enum 
{
    ADC1_CONVERSION_CHANNELS_CH0    = 0, /* Converts only CH0 */
    ADC1_CONVERSION_CHANNELS_CH01   = 1, /* Converts CH0 and CH1 */
    ADC1_CONVERSION_CHANNELS_CH0123 = 2  /* Converts CH0, CH1, CH2 and CH3 */
} ADC1_CONVERSION_CHANNELS_TYPE;

/**
  Section: Interface Routines
*/


/**
  @Summary
    This function initializes ADC instance : 1

  @Description
    This routine initializes the ADC driver instance for : 1
    index, making it ready for clients to open and use it. It also initializes any
    internal data structures.
    This routine must be called before any other ADC routine is called. 

  @Preconditions
    None.

  @Param
    None.

  @Returns
    None.

  @Comment
    
 
  @Example
    <code>
        int conversion;
        ADC1_Initialize();
        ADC1_ChannelSelect(AN1_Channel);
        ADC1_SamplingStart();
        //Provide Delay
        for(int i=0;i <1000;i++)
        {
        }
        ADC1_SamplingStop();
        while(!ADC1_IsConversionComplete())
        {
            ADC1_Tasks();   
        }
        conversion = ADC1_ConversionResultGet();
    </code>

*/

void ADC1_Initialize (void);

/**
  @Summary
    Clears interrupt flag

  @Description
    This routine is used to clear the interrupt flag manually.
 
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None.

  @Example
    Refer to ADC1_Initialize() for an example

*/

inline static void ADC1_InterruptFlagClear(void)
{
   IFS0bits.AD1IF = 0;
}

/**
  @Summary
    Enables interrupts.

  @Description
    This routine is used to enable the ADC1 interrupt manually.
 
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None.

  @Example
    Refer to ADC1_Initialize() for an example

*/

inline static void ADC1_InterruptEnable(void)
{  
   IEC0bits.AD1IE = 1;
}

/**
  @Summary
    Disables interrupts

  @Description
    This routine is used to disable the ADC1 interrupt manually.
 
  @Preconditions
    None.

  @Param
    None.

  @Returns
    None.

  @Example
    Refer to ADC1_Initialize() for an example

*/

inline static void ADC1_InterruptDisable(void)
{
   IEC0bits.AD1IE = 0;
}

/**
  @Summary
    Starts sampling manually.

  @Description
    This routine is used to start the sampling manually.
 
  @Preconditions
    ADC1_Initialize() function should have been called 
    before calling this function.

  @Param
    None.

  @Returns
    None.

  @Example
    <code>
        int conversion;
        ADC1__Initialize();
        ADC1_ChannelSelect(AN1_Channel);
        ADC1_SamplingStart();
        //Provide Delay
        for(int i=0;i <1000;i++)
        {
        }
        ADC1_SamplingStop();
        while(!ADC1_IsConversionComplete())
        {
            ADC1_Tasks();   
        }
        conversion = ADC1_ConversionResultGet();
    </code>

*/

inline static void ADC1_SamplingStart(void)
{
   AD1CON1bits.SAMP = 1;
}

/**
  @Summary
    Stops sampling manually.

  @Description
    This routine is used to stop the sampling manually before conversion
    is triggered.
 
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.

  @Param
    None.

  @Returns
    None.

  @Example
    <code>
        int conversion;
        ADC1__Initialize();
        ADC1_ChannelSelect(AN1_Channel);
        ADC1_SamplingStart();
        //Provide Delay
        for(int i=0;i <1000;i++)
        {
        }
        ADC1_SamplingStop();
        while(!ADC1_IsConversionComplete())
        {
            ADC1_Tasks();   
        }
        conversion = ADC1_ConversionResultGet();
    </code>
*/

inline static void ADC1_SamplingStop(void)
{
   AD1CON1bits.SAMP = 0;
}

/**
  @Summary
    Gets the buffer loaded with conversion results.

  @Description
    This routine is used to get the analog to digital converted values in a
    buffer. This routine gets converted values from multiple channels.
 
  @Preconditions
    This routine returns the buffer containing the conversion values only after 
    the conversion is complete. Completion status conversion can be checked using 
    ADC1_IsConversionComplete() routine.
 
  @Param
    None.

  @Returns
    Returns the count of the buffer containing the conversion values.

  @Example
    <code>
        int count;
        //Initialize for channel scanning
        ADC1_Initialize();
        ADC1_SamplingStart();
        //Provide Delay
        for(int i=0;i <1000;i++)
        {
        }
        ADC1_SamplingStop();
        while(!ADC1_IsConversionComplete())
        {
            count = ADC1_ConversionResultBufferGet();
        }
    </code>
*/

uint16_t ADC1_ConversionResultBufferGet(uint16_t *buffer);

/**
  @Summary
    Returns the ADC1 conversion value for Channel 0.

  @Description
    This routine is used to get the analog to digital converted value. This
    routine gets converted values from the channel specified.
 
  @Preconditions
    The channel required must be selected before calling this routine using
    ADC1_ChannelSelect(channel). This routine returns the 
    conversion value only after the conversion is complete. Completion status 
    conversion can be checked using ADC1_IsConversionComplete()
    routine.
   
  @Returns
    Returns the buffer containing the conversion value.

  @Param
    Buffer address
  
  @Example
    Refer to ADC1_Initialize(); for an example
 */

inline static uint16_t ADC1_Channel0ConversionResultGet(void) 
{
    return ADC1BUF0;
}

/**
  @Summary
    Returns the ADC1 conversion value from Channel 1.

  @Description
    This routine is used to get the analog to digital converted value. This
    routine gets converted values from the channel specified.
 
  @Preconditions
    The channel required must be selected before calling this routine using
    ADC1_ChannelSelect(channel). This routine returns the 
    conversion value only after the conversion is complete. Completion status 
    conversion can be checked using ADC1_IsConversionComplete()
    routine.
   
  @Returns
    Returns the buffer containing the conversion value.

  @Param
    Buffer address
  
  @Example
    Refer to ADC1_Initialize(); for an example
 */

inline static uint16_t ADC1_Channel1ConversionResultGet(void) 
{
    return ADC1BUF1;
}

/**
  @Summary
    Returns the ADC1 conversion value from Channel 2.

  @Description
    This routine is used to get the analog to digital converted value. This
    routine gets converted values from the channel specified.
 
  @Preconditions
    The channel required must be selected before calling this routine using
    ADC1_ChannelSelect(channel). This routine returns the 
    conversion value only after the conversion is complete. Completion status 
    conversion can be checked using ADC1_IsConversionComplete()
    routine.
   
  @Returns
    Returns the buffer containing the conversion value.

  @Param
    Buffer address
  
  @Example
    Refer to ADC1_Initialize(); for an example
 */

inline static uint16_t ADC1_Channel2ConversionResultGet(void) 
{
    return ADC1BUF2;
}

/**
  @Summary
    Returns the ADC1 conversion value from Channel 3.

  @Description
    This routine is used to get the analog to digital converted value. This
    routine gets converted values from the channel specified.
 
  @Preconditions
    The channel required must be selected before calling this routine using
    ADC1_ChannelSelect(channel). This routine returns the 
    conversion value only after the conversion is complete. Completion status 
    conversion can be checked using ADC1_IsConversionComplete()
    routine.
   
  @Returns
    Returns the buffer containing the conversion value.

  @Param
    Buffer address
  
  @Example
    Refer to ADC1_Initialize(); for an example
 */

inline static uint16_t ADC1_Channel3ConversionResultGet(void) 
{
    return ADC1BUF3;
}

/**
  @Summary
    Returns true when the conversion is completed

  @Description
    This routine is used to determine if conversion is completed. This routine
    returns the value of the DONE bit. When conversion is complete the routine
    returns 1. It returns 0 otherwise.
 
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    Returns true if conversion is completed

  @Param
    None
  
  @Example
    Refer to ADC1_Initialize(); for an example
 */

inline static bool ADC1_IsConversionComplete( void )
{
    return AD1CON1bits.DONE; //Wait for conversion to complete   
}

/**
  @Summary
    Allows selection of a channel for conversion

  @Description
    This routine is used to select desired channel for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    Pass in required channel from the ADC1_CHANNEL list
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static void ADC1_ChannelSelectSet( ADC1_CHANNEL channel )
{
    AD1CHS0bits.CH0SA = channel;
}

/**
  @Summary
    Returns the channel selected for conversion

  @Description
    This routine is used to return the channel selected for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    The value of the Channel Conversion register

  @Param
    None
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static uint16_t ADC1_ChannelSelectGet( void )
{
    return AD1CHS0bits.CH0SA ;
}

/**
  @Summary
    Allows selection of a data format type for conversion

  @Description
    This routine is used to select desired data format for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    Pass in required data format type from the ADC1_FORM_TYPE list
  
  @Example
    Refer to ADC1_Initialize(); for an example
*/

inline static void ADC1_FormatDataSet( ADC1_FORM_TYPE form )
{
    AD1CON1bits.FORM = form;
}

/**
  @Summary
    Allows selection of a resolution mode for conversion

  @Description
    This routine is used to select desired resolution mode for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    Pass in required resolution mode from the ADC1_RESOLUTION_TYPE list
  
  @Example
    Refer to ADC1_Initialize(); for an example
*/

inline static void ADC1_ResolutionModeSet( ADC1_RESOLUTION_TYPE resolution )
{
    AD1CON1bits.AD12B = resolution;
}

/**
  @Summary
    Allows simultaneous sampling to be enabled manually

  @Description
    This routine is used to enable simultaneous sampling of channels manually
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None.
  
  @Example
    Refer to ADC1_Initialize(); for an example

*/

inline static void ADC1_SimultaneousSamplingEnable(void)
{
    AD1CON1bits.SIMSAM = 1;
}

/**
  @Summary
    Allows simultaneous sampling to be disabled manually

  @Description
    This routine is used to disable simultaneous sampling of channels manually
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None.
  
  @Example
    Refer to ADC1_Initialize(); for an example
*/

inline static void ADC1_SimultaneousSamplingDisble(void)
{
    AD1CON1bits.SIMSAM = 0;
}

/**
@Summary
    Allows sutomatic sampling to be enabled manually

  @Description
    This routine is used to enable automatic sampling of channels manually
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None.
  
  @Example
    Refer to ADC1_Initialize(); for an example
*/

inline static void ADC1_AutomaticSamplingEnable(void)
{
    AD1CON1bits.ASAM = 1;
}

/**
  @Summary
    Allows automatic sampling to be disabled manually

  @Description
    This routine is used to disable automatic sampling of channels manually
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None.
  
  @Example
    Refer to ADC1_Initialize(); for an example
*/

inline static void ADC1_AutomaticSamplingDisable(void)
{
    AD1CON1bits.ASAM = 0;
}

/**
  @Summary
    Allows conversion clock prescaler value to be set

  @Description
    This routine is used to allow conversion clock prescaler value to be set manually
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    Pass in required prescaler integer value
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static void ADC1_ConversionClockPrescalerSet(uint8_t prescaler)
{
    AD1CON3bits.ADCS = prescaler - 1;
}

/**
  @Summary
    Allows module to be enabled manually

  @Description
    This routine is used to enable the ADC1 module manually
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None
  
  @Example
*/

inline static void ADC1_Enable(void)
{
    AD1CON1bits.ADON = 1;
}

/**
  @Summary
    Allows module to be disabled manually

  @Description
    This routine is used to disable the ADC1 module manually
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    None
  
  @Example
*/

inline static void ADC1_Disable(void)
{
    AD1CON1bits.ADON = 0;
}

/**
  @Summary
    Allows selection of a positive 123 channel for conversion

  @Description
    This routine is used to select desired positive 123 channel for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    Pass in required channel from the ADC1_POS_123_CHANNEL list
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static void ADC1_Positive123ChannelSelect( ADC1_POS_123_CHANNEL channel )
{
    AD1CHS123 = (AD1CHS123 & 0xFF06) | channel;
}

/**
  @Summary
    Allows selection of a negative 123 channel for conversion

  @Description
    This routine is used to select desired negative 123 channel for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    Pass in required channel from the ADC1_NEG_123_CHANNEL list
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static void ADC1_Negative123ChannelSelect( ADC1_NEG_123_CHANNEL channel )
{
    AD1CHS123bits.CH123NA = channel;
}

/**
  @Summary
    Allows selection of conversion channels

  @Description
    This routine is used to select conversion channel for conversion.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    Pass in required channel from the ADC1_CONVERSION_CHANNELS_TYPE list
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static void ADC1_ConversionChannelsSet( ADC1_CONVERSION_CHANNELS_TYPE channel )
{
    AD1CON2bits.CHPS = channel;
}

/**
  @Summary
    Allows selection of a priority for interrupt

  @Description
    This routine is used to select desired priority for interrupt.
  
  @Preconditions
    ADC1_Initialize() function should have been 
    called before calling this function.
 
  @Returns
    None

  @Param
    Pass in required integer priority value
  
  @Example
    Refer to ADC1_Initialize(); for an example
 
*/

inline static void ADC1_InterruptPrioritySet( uint16_t priorityValue )
{
    _AD1IP = 0x7 & priorityValue;
}

/**
  @Summary
    Callback for ADC1.

  @Description
    This routine is callback for ADC1

  @Param
    None.

  @Returns
    None
 
  @Example 
	Refer to ADC1_Initialize(); for an example
*/
void ADC1_CallBack(void);

        
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif //_ADC1_H
    
/**
 End of File
*/
