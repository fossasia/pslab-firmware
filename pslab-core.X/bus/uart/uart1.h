#ifndef _UART1_H
#define _UART1_H

#include <stdbool.h>
#include <stdint.h>
#include <xc.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
      @Summary
        Initialization routine that takes inputs from the UART1 GUI.

      @Description
        This routine initializes the UART1 driver.
        This routine must be called before any other UART1 routine is called.

      @Preconditions
        None

      @Param
        None

      @Returns
        None

      @Example
        None.
    
     */
    void UART1_Initialize(void);

    /**
      @Summary
        Read a byte of data from the UART1.

      @Description
        This routine reads a byte of data from the UART1.

      @Preconditions
        UART1_Initialize() function should have been called
        before calling this function. This is a blocking function.
        So the receive status should be checked to see
        if the receiver is not empty before calling this function.

      @Param
        None

      @Returns
        A data byte received by the driver.
     */
    uint8_t UART1_Read(void);

    /**
      @Summary
        Read two bytes of data from the UART1.

      @Description
        This routine reads two byte of data sequentially from the UART1.

      @Preconditions
        UART1_Initialize() function should have been called
        before calling this function. This is a blocking function.
        So the receive status should be checked to see
        if the receiver is not empty before calling this function.

      @Param
        None

      @Returns
        A 16 bit integer
     */
    uint16_t UART1_ReadInt(void);

    /**
      @Summary
        Writes a byte of data to the UART1.

      @Description
        This routine writes a byte of data to the UART1.

      @Preconditions
        UART1_Initialize() function should have been called
        before calling this function. The transfer status should be checked to see
        if transmitter is not busy before calling this function.

      @Param
        txData  - Data byte to write to the UART1

      @Returns
        None
     */
    void UART1_Write(uint8_t txData);

    /**
      @Summary
        Writes an integer data to the UART1.

      @Description
        This routine sequentially writes two bytes of data to the UART1.

      @Preconditions
        UART1_Initialize() function should have been called
        before calling this function. The transfer status should be checked to see
        if transmitter is not busy before calling this function.

      @Param
        txData  - Data as an integer to write to the UART1

      @Returns
        None
     */
    void UART1_WriteInt(uint16_t txData);

    /**
      @Description
        Indicates of there is data available to read.

      @Returns
        true if byte can be read.
        false if byte can't be read right now.
     */
    bool UART1_IsRxReady(void);

    /**
      @Description
        Indicates if a byte can be written.
 
     @Returns
        true if byte can be written.
        false if byte can't be written right now.
     */
    bool UART1_IsTxReady(void);

    /**
      @Description
        Indicates if all bytes have been transferred.
 
     @Returns
        true if all bytes transfered.
        false if there is still data pending to transfer.
     */
    bool UART1_IsTxDone(void);

    /**
      @Summary
        Allows setting of a the enable bit for the UART1 mode

      @Description
        This routine is used to enable the UART1
  
      @Preconditions
        UART1_Initialize() function should have been 
        called before calling this function.
 
      @Returns
        None

      @Param
        None
  
      @Example
        Refer to UART1_Initialize(); for an example
     */
    void UART1_Enable(void);

    /**
      @Summary
        Allows setting of a the disable bit for the UART1 mode

      @Description
        This routine is used to disable the UART1
  
      @Preconditions
        UART1_Initialize() function should have been 
        called before calling this function.
 
      @Returns
        None

      @Param
        None
  
      @Example
        Refer to UART1_Initialize(); for an example
     */
    void UART1_Disable(void);

    /**
      @Summary
        Returns the transmitter and receiver status

      @Description
        This returns the transmitter and receiver status. The returned status may 
        contain a value with more than one of the bits
        specified in the UART1_STATUS enumeration set.  
        The caller should perform an "AND" with the bit of interest and verify if the
        result is non-zero (as shown in the example) to verify the desired status
        bit.

      @Preconditions
        UART1_Initialize function should have been called 
        before calling this function

      @Param
        None.

      @Returns
        A UART1_STATUS value describing the current status 
        of the transfer.

      @Example
        <code>
            while(!(UART1_StatusGet & UART1_TX_COMPLETE ))
            {
               // Wait for the tranmission to complete
            }
        </code>
     */
    uint16_t UART1_StatusGet(void);

    /**
     * @Summary
     *   Discard incoming serial traffic
     * 
     * @Description
     *   Reads out the RX buffer and clear it for incoming serial traffic at the
     *   startup.
     * 
     * @Returns
     *   None.
     */
    void UART1_ClearBuffer(void);
    
    inline static void UART1_InterruptEnable(void) {
        IEC0bits.U1RXIE = 1;
    }
    
    inline static void UART1_InterruptDisable(void) {
        IEC0bits.U1RXIE = 0;
    }
    
    inline static void UART1_InterruptFlagClear(void) {
        IFS0bits.U1RXIF = 0;
    }

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif  // _UART1_H
