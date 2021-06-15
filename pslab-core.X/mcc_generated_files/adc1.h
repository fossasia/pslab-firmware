#ifndef _ADC1_H
#define _ADC1_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

    /**
      Section: Data Types
     */

    /** Scan Selected Macro Definition
 
     @Summary 
       Defines the scan option selection done for the shared channels.
 
     @Description
       This macro defines the scan option selection done for the shared channels.
 
     Remarks:
       None
     */
#define ADC1_SCAN_MODE_SELECTED false

    /** ADC1 Channel Definition
 
     @Summary 
       Defines the channels selected.
 
     @Description
       This routine defines the channels that are available for the module to use.
 
     Remarks:
       None
     */
    typedef enum {
        channel_CTMU_Temp, //Channel Name:CTMU Temp   Assigned to:Shared Channel
        channel_CTMU, //Channel Name:CTMU   Assigned to:Shared Channel
        channel_AN0, //Channel Name:AN0   Assigned to:Dedicated Channel1
        channel_AN1, //Channel Name:AN1   Assigned to:Dedicated Channel2
        channel_AN2, //Channel Name:AN2   Assigned to:Dedicated Channel3
    } ADC1_CHANNEL;

    /**
      Section: Interface Routines
     */

    /**
      @Summary
        Initializes ADC1 module.

      @Description
        This routine initializes ADC1 module, using the given initialization data. 
        This routine must be called before any other ADC routine is called. 

      @Preconditions
        None.

      @Param
        None.

      @Returns
        None

      @Example
        <code>
            int conversion,i=0;
            ADC1_Initialize();

            ADC1_Enable();
            ADC1_ChannelSelect(channel);
            ADC1_SoftwareTriggerEnable();
            //Provide Delay
            for(i=0;i <1000;i++)
            {
            }
            ADC1_SoftwareTriggerDisable();
            while(!ADC1_IsConversionComplete(channel));
            conversion = ADC1_ConversionResultGet(channel);
            ADC1_Disable(); 
        </code>
     */
    void ADC1_Initialize(void);

    void ADC1_InitializeCON1(void);
    void ADC1_InitializeCON2(void);
    void ADC1_InitializeCON3(void);
    void ADC1_InitializeCON4(void);

    /**
      @Summary
        Enables the ADC1 module.

      @Description
        This routine is used to enable the ADC1 module.
 
      @Preconditions
        ADC1_Initialize function should have been called 
        before calling this function.

      @Param
        None.

      @Returns
        None.

      @Example
        <code>
            int conversion,i=0;
            ADC1_Initialize();

            ADC1_Enable();
            ADC1_ChannelSelect(channel);
            ADC1_SoftwareTriggerEnable();
            //Provide Delay
            for(i=0;i <1000;i++)
            {
            }
            ADC1_SoftwareTriggerDisable();
            while(!ADC1_IsConversionComplete(channel));
            conversion = ADC1_ConversionResultGet(channel);
            ADC1_Disable(); 
        </code>
     */
    inline static void ADC1_Enable(void) {
        AD1CON1bits.ADON = 1;
    }

    /**
      @Summary
        Disables the ADC1 module.

      @Description
        This routine is used to disable the ADC1 module.
 
      @Preconditions
        ADC1_Initialize function should have been called 
        before calling this function.

      @Param
        None.

      @Returns
        None.

      @Example
        <code>
            int conversion,i=0;
            ADC1_Initialize();

            ADC1_Enable();
            ADC1_ChannelSelect(channel);
            ADC1_SoftwareTriggerEnable();
            //Provide Delay
            for(i=0;i <1000;i++)
            {
            }
            ADC1_SoftwareTriggerDisable();
            while(!ADC1_IsConversionComplete(channel));
            conversion = ADC1_ConversionResultGet(channel);
            ADC1_Disable(); 
        </code>
     */
    inline static void ADC1_Disable(void) {
        AD1CON1bits.ADON = 0;
    }

    /**
      @Summary
        Starts sampling manually.

      @Description
        This routine is used to start sampling manually.
 
      @Preconditions
        ADC1_Initialize function should have been called 
        before calling this function.

      @Param
        None.

      @Returns
        None.

      @Example
        <code>
            int conversion,i=0;
            ADC1_Initialize();

            ADC1_Enable();
            ADC1_ChannelSelect(channel);
            ADC1_SoftwareTriggerEnable();
            //Provide Delay
            for(i=0;i <1000;i++)
            {
            }
            ADC1_SoftwareTriggerDisable();
            while(!ADC1_IsConversionComplete(channel));
            conversion = ADC1_ConversionResultGet(channel);
            ADC1_Disable(); 
        </code>
     */
    inline static void ADC1_SoftwareTriggerEnable(void) {
        AD1CON1bits.SAMP = 1;
    }

    /**
      @Summary
        Stops sampling manually.

      @Description
        This routine is used to stop the sampling manually.
 
      @Preconditions
        ADC1_Initialize() function should have been 
        called before calling this function.

      @Param
        None.

      @Returns
        None.

      @Example
        <code>
            int conversion,i=0;
            ADC1_Initialize();

            ADC1_Enable();
            ADC1_ChannelSelect(channel);
            ADC1_SoftwareTriggerEnable();
            //Provide Delay
            for(i=0;i <1000;i++)
            {
            }
            ADC1_SoftwareTriggerDisable();
            while(!ADC1_IsConversionComplete(channel));
            conversion = ADC1_ConversionResultGet(channel);
            ADC1_Disable(); 
        </code>
     */
    inline static void ADC1_SoftwareTriggerDisable(void) {
        AD1CON1bits.SAMP = 0;
    }

    /**
      @Summary
        Allows selection of a channel for conversion.

      @Description
        This routine is used to select desired channel for conversion.
  
      @Preconditions
        ADC1_Initialize() function should have been 
        called before calling this function.

      @Param
        channel - Channel for conversion

      @Returns
        None
  
      @Example
        <code>
            int conversion,i=0;
            ADC1_Initialize();

            ADC1_Enable();
            ADC1_ChannelSelect(channel);
            ADC1_SoftwareTriggerEnable();
            //Provide Delay
            for(i=0;i <1000;i++)
            {
            }
            ADC1_SoftwareTriggerDisable();
            while(!ADC1_IsConversionComplete(channel));
            conversion = ADC1_ConversionResultGet(channel);
            ADC1_Disable(); 
        </code>
     */
    inline static void ADC1_ChannelSelect(ADC1_CHANNEL channel) {
        switch (channel) {
            case channel_CTMU_Temp:
                AD1CHS0bits.CH0SA = 0x1E;
                break;
            case channel_CTMU:
                AD1CHS0bits.CH0SA = 0x1F;
                break;
            default:
                break;
        }
    }

    /**
      @Summary
        Returns the conversion value for the channel selected.

      @Description
        This routine is used to get the analog to digital converted value for a 
        specific channel.
 
      @Preconditions
        This routine returns the conversion value only after the conversion is complete. 
        Conversion completion status can be checked using ADC1_IsConversionComplete(channel)
        routine.

      @Param
        channel - Selected channel
   
      @Returns
       Returns the analog to digital converted value
  
      @Example
        <code>
            int conversion,i=0;
            ADC1_Initialize();

            ADC1_Enable();
            ADC1_ChannelSelect(channel);
            ADC1_SoftwareTriggerEnable();
            //Provide Delay
            for(i=0;i <1000;i++)
            {
            }
            ADC1_SoftwareTriggerDisable();
            while(!ADC1_IsConversionComplete(channel));
            conversion = ADC1_ConversionResultGet(channel);
            ADC1_Disable(); 
        </code>
     */
    inline static uint16_t ADC1_ConversionResultGet(ADC1_CHANNEL channel) {
        uint16_t result;

        switch (channel) {
            case channel_CTMU_Temp:
            case channel_CTMU:
                result = ADC1BUF0;
                break;
            case channel_AN0:
                result = ADC1BUF1;
                break;
            case channel_AN1:
                result = ADC1BUF2;
                break;
            case channel_AN2:
                result = ADC1BUF3;
                break;
            default:
                break;
        }

        return result;
    }

    /**
      @Summary
        Returns the status of conversion.

      @Description
        This routine is used to determine if conversion is completed. When conversion
        is complete the routine returns true otherwise false.
 
      @Preconditions
        ADC1_Initialize() function should have been 
        called before calling this function.

      @Param
        channel - Selected channel
 
      @Returns
        true - Conversion is complete.
        false - Conversion is not complete.
  
      @Example
        <code>
            int conversion,i=0;
            ADC1_Initialize();

            ADC1_Enable();
            ADC1_ChannelSelect(channel);
            ADC1_SoftwareTriggerEnable();
            //Provide Delay
            for(i=0;i <1000;i++)
            {
            }
            ADC1_SoftwareTriggerDisable();
            while(!ADC1_IsConversionComplete(channel));
            conversion = ADC1_ConversionResultGet(channel);
            ADC1_Disable(); 
        </code>
     */
    inline static bool ADC1_IsConversionComplete(ADC1_CHANNEL channel) {
        bool status;

        status = AD1CON1bits.DONE;

        return status;
    }

    /**
      @Summary
        Enables interrupts.

      @Description
        This routine is used to enable the ADC1 interrupt.
 
      @Preconditions
        None.

      @Param
        None.

      @Returns
        None.

      @Example
        <code>
            ADC1_InterruptEnable(); 
        </code>
     */
    inline static void ADC1_InterruptEnable(void) {
        IEC0bits.AD1IE = 1;
    }

    /**
      @Summary
        Disables interrupts.

      @Description
        This routine is used to disable the ADC1 interrupt.
 
      @Preconditions
        None.

      @Param
        None.

      @Returns
        None.

      @Example
        <code>
            ADC1_InterruptDisable(); 
        </code>
     */
    inline static void ADC1_InterruptDisable(void) {
        IEC0bits.AD1IE = 0;
    }

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
        <code>
            ADC1_InterruptFlagClear(); 
        </code>
     */
    inline static void ADC1_InterruptFlagClear(void) {
        IFS0bits.AD1IF = 0;
    }

    /**
      @Summary
        Allows selection of priority for interrupt.

      @Description
        This routine is used to select desired priority for interrupt.
  
      @Preconditions
        None.

      @Param
        None.

      @Returns
        None.

      @Example
        <code>
            uint16_t priorityValue;
            priorityValue = 0x002;
 
            ADC1_InterruptPrioritySet(priorityValue); 
        </code>
     */
    inline static void ADC1_InterruptPrioritySet(uint16_t priorityValue) {
        IPC3bits.AD1IP = 0x7 & priorityValue;
    }

    /**
      @Summary
        Callback for ADC1.

      @Description
        This routine is callback for ADC1
  
      @Preconditions
        None.

      @Param
        None.

      @Returns
        None
 
      @Example 
        <code>    
            ADC1_CallBack();
        </code>
     */
    void ADC1_CallBack(void);

    /**
      @Summary
        Assigns a function pointer with a callback address.

      @Description
        This routine assigns a function pointer with a callback address.
  
      @Preconditions
        None.

      @Param
        Address of the callback routine.

      @Returns
        None
 
      @Example 
        <code>
            ADC1_SetInterruptHandler(&ADC1_CallBack);
        </code>
     */
    void ADC1_SetInterruptHandler(void* handler);

    /** ADC Positive 123 Channels Definition
 
     @Summary 
       Defines the positive 123 channels available for conversion
 
     @Description
       This routine defines the positive 123 channels that are available for the 
       module to use.
 
     Remarks:
       None
     */
    typedef enum {
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
    typedef enum {
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
    typedef enum {
        ADC1_FORM_UNSIGNED_INT = 0, /* Unsigned Integer */
        ADC1_FORM_SIGNED_INT = 1, /* Signed Integer */
        ADC1_FORM_UNSIGNED_FRACT = 2, /* Unsigned Fraction */
        ADC1_FORM_SIGNED_FRACT = 3 /* Signed Integer */
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
    typedef enum {
        ADC1_RESOLUTION_10_BIT = 0, /* 10-bit, 4-channel ADC operation */
        ADC1_RESOLUTION_12_BIT = 1 /* 12-bit, 1-channel ADC operation */
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
    typedef enum {
        ADC1_SAMPLING_SOURCE_PTGO12 = 0x3,
        ADC1_SAMPLING_SOURCE_PTGO14 = 0x5,
        ADC1_SAMPLING_SOURCE_CTMU = 0x6,
        ADC1_SAMPLING_SOURCE_PTGO15 = 0x6,
        ADC1_SAMPLING_SOURCE_AUTO = 0x7,
        ADC1_SAMPLING_SOURCE_INT0 = 0x1,
        ADC1_SAMPLING_SOURCE_TMR5 = 0x4,
        ADC1_SAMPLING_SOURCE_TMR3 = 0x2,
        ADC1_SAMPLING_SOURCE_MANUAL = 0x0,
        ADC1_SAMPLING_SOURCE_PTGO13 = 0x4,
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
    typedef enum {
        ADC1_CONVERSION_CHANNELS_CH0 = 0, /* Converts only CH0 */
        ADC1_CONVERSION_CHANNELS_CH01 = 1, /* Converts CH0 and CH1 */
        ADC1_CONVERSION_CHANNELS_CH0123 = 2 /* Converts CH0, CH1, CH2 and CH3 */
    } ADC1_CONVERSION_CHANNELS_TYPE;

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
    inline static void ADC1_ChannelSelectSet(ADC1_CHANNEL channel) {
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
    inline static uint16_t ADC1_ChannelSelectGet(void) {
        return AD1CHS0bits.CH0SA;
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
    inline static void ADC1_FormatDataSet(ADC1_FORM_TYPE form) {
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
    inline static void ADC1_ResolutionModeSet(ADC1_RESOLUTION_TYPE resolution) {
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
    inline static void ADC1_SimultaneousSamplingEnable(void) {
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
    inline static void ADC1_SimultaneousSamplingDisble(void) {
        AD1CON1bits.SIMSAM = 0;
    }

    /**
    @Summary
        Allows automatic sampling to be enabled manually

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
    inline static void ADC1_AutomaticSamplingEnable(void) {
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
    inline static void ADC1_AutomaticSamplingDisable(void) {
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
    inline static void ADC1_ConversionClockPrescalerSet(uint8_t prescaler) {
        AD1CON3bits.ADCS = prescaler - 1;
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
    inline static void ADC1_Positive123ChannelSelect(ADC1_POS_123_CHANNEL channel) {
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
    inline static void ADC1_Negative123ChannelSelect(ADC1_NEG_123_CHANNEL channel) {
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
    inline static void ADC1_ConversionChannelsSet(ADC1_CONVERSION_CHANNELS_TYPE channel) {
        AD1CON2bits.CHPS = channel;
    }

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif //_ADC1_H
