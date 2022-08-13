#ifndef ADC_MODULE_FEATURES
#define ADC_MODULE_FEATURES

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

/*******************************************************************************
********************************************************************************
******************************************************************************** 
                            ********IMPORTANT********

This file defines the ADC module feature C-macros available 
on the selected device. The macros defined in this file provides the 
flexibility to easily migrate the user application to other device which might 
have varied feature list.

The file has to be manually included in main.c, if the user intends to migrate the
application to another device which might have varied feature list.
  
  
 ***************The content in this file is strictly "read only" and should 
 not be altered. Failing to do so, the migration is not guaranteed.*************

********************************************************************************
********************************************************************************         
********************************************************************************/

/*******************************************************************************
            Macros defined for features supported in the device
*******************************************************************************/

//None

/*******************************************************************************
            Macros defined for features not supported in the device
*******************************************************************************/

    /** ADC POWER ENABLE FEATURE
 
     @Summary 
       Defines the macro associated with ADC core power enable.

     @APIs Supported:
       None
  
     @Remarks:
       None
     */
#define ADC_POWER_ENABLE_FEATURE_AVAILABLE  0

    /** ADC CALIBRATION FEATURE
 
     @Summary 
       Defines the macro associated with ADC core calibration.

     @APIs Supported:
       None

       Refer driver header file for detailed description of the APIs.
  
     @Remarks:
       None
     */
#define ADC_CALIBRATION_FEATURE_AVAILABLE  0

    /** ADC COMPARATOR FEATURE
 
     @Summary 
       Defines the macro associated with ADC comparator feature.

     @APIs Supported:
       None
  
     @Remarks:
       None
     */
#define ADC_COMPARATOR_FEATURE_AVAILABLE   0

    /** ADC INDIVIDUAL CHANNEL INTERRUPT FEATURE
 
     @Summary 
       Defines the macro associated with individual channel interrupts.

     @APIs Supported:
       None
  
     @Remarks:
       None
     */
#define ADC_INDIVIDUAL_CHANNEL_INTERRUPT_FEATURE_AVAILABLE   0

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif //ADC_MODULE_FEATURES
