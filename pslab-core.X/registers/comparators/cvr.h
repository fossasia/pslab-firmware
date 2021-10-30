#ifndef _CVR_H
#define _CVR_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

    /**
      @Summary
        This function initializes CVR 

      @Description
        This routine initializes the CVR driver index, making it 
        ready for clients to open and use it. It also initializes any internal data 
        structures.

      @Param
        None.

      @Returns 
        None
 
      @Comment

     */
    void CVR_Initialize(void);

    /**
      @Summary
        Comparator Voltage Reference 1 Output Enable bit
       
      @Description
        Voltage level is output on the CVREF1O pin or is disconnected from the
        CVREF1O pin
     
      @Param
        bool: connect
      
      @Returns
        None
     */
    void CVR_ComparatorReference1Connection(bool connect);

    /**
      @Summary
        Comparator Voltage Reference 2 Output Enable bit
       
      @Description
        (AVDD - AVSS)/2 is connected to or disconnected from the CVREF2O pin
     
      @Param
        bool: connect
      
      @Returns
        None
     */
    void CVR_ComparatorReference2Connection(bool connect);

    inline static void CVR_ComparatorPowerUp(void) {
      CVRCONbits.CVREN = 1;
    }

    inline static void CVR_SetReferenceVoltage(uint8_t V) {
      CVRCONbits.CVR = V;
    }
 
#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif 

#endif //_CVR_H
