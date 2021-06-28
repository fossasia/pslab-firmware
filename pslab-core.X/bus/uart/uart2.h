#ifndef _UART2_H
#define _UART2_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

    /**
      @Summary
        Initialization routine that takes inputs from the UART2 GUI.

      @Description
        This routine initializes the UART2 driver.
        This routine must be called before any other UART2 routine is called.

      @Preconditions
        None

      @Param
        None

      @Returns
        None

      @Example
        None.
    
     */
    void UART2_Initialize(void);

    /**
      @Summary
        Read a byte of data from the UART2.

      @Description
        This routine reads a byte of data from the UART2.

      @Preconditions
        UART2_Initialize() function should have been called
        before calling this function. This is a blocking function.
        So the receive status should be checked to see
        if the receiver is not empty before calling this function.

      @Param
        None

      @Returns
        A data byte received by the driver.
     */
    uint8_t UART2_Read(void);

    /**
      @Summary
        Writes a byte of data to the UART2.

      @Description
        This routine writes a byte of data to the UART2.

      @Preconditions
        UART2_Initialize() function should have been called
        before calling this function. The transfer status should be checked to see
        if transmitter is not busy before calling this function.

      @Param
        txData  - Data byte to write to the UART2

      @Returns
        None
     */
    void UART2_Write(uint8_t txData);

    /**
      @Description
        Indicates of there is data available to read.

      @Returns
        true if byte can be read.
        false if byte can't be read right now.
     */
    bool UART2_IsRxReady(void);

    /**
      @Description
        Indicates if a byte can be written.
 
     @Returns
        true if byte can be written.
        false if byte can't be written right now.
     */
    bool UART2_IsTxReady(void);

    /**
      @Description
        Indicates if all bytes have been transferred.
 
     @Returns
        true if all bytes transfered.
        false if there is still data pending to transfer.
     */
    bool UART2_IsTxDone(void);

    /** UART2 Driver Hardware Flags

      @Summary
        Specifies the status of the hardware receive or transmit

      @Description
        This type specifies the status of the hardware receive or transmit.
        More than one of these values may be OR'd together to create a complete
        status value.  To test a value of this type, the bit of interest must be
        AND'ed with value and checked to see if the result is non-zero.
     */
    typedef enum {
        /* Indicates that Receive buffer has data, at least one more character can be read */
        UART2_RX_DATA_AVAILABLE = (1 << 0),
        /* Indicates that Receive buffer has overflowed */
        UART2_RX_OVERRUN_ERROR = (1 << 1),
        /* Indicates that Framing error has been detected for the current character */
        UART2_FRAMING_ERROR = (1 << 2),
        /* Indicates that Parity error has been detected for the current character */
        UART2_PARITY_ERROR = (1 << 3),
        /* Indicates that Receiver is Idle */
        UART2_RECEIVER_IDLE = (1 << 4),
        /* Indicates that the last transmission has completed */
        UART2_TX_COMPLETE = (1 << 8),
        /* Indicates that Transmit buffer is full */
        UART2_TX_FULL = (1 << 9)
    } UART2_STATUS;

    /**
      @Summary
        Allows setting of a the enable bit for the UART2 mode

      @Description
        This routine is used to enable the UART2
  
      @Preconditions
        UART2_Initialize() function should have been 
        called before calling this function.
 
      @Returns
        None

      @Param
        None
  
      @Example
        Refer to UART2_Initialize(); for an example
     */
    void UART2_Enable(void);

    /**
      @Summary
        Allows setting of a the disable bit for the UART2 mode

      @Description
        This routine is used to disable the UART2
  
      @Preconditions
        UART2_Initialize() function should have been 
        called before calling this function.
 
      @Returns
        None

      @Param
        None
  
      @Example
        Refer to UART2_Initialize(); for an example
     */
    void UART2_Disable(void);

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif  // _UART2_H
