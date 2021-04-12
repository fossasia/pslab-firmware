/**
  UART1 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    uart1.h

  @Summary
    This is the generated header file for the UART1 driver using Foundation Services Library

  @Description
    This header file provides APIs for driver for UART1.
    Generation Information :
        Product Revision  :  Foundation Services Library - pic24-dspic-pic32mm : v1.26
        Device            :  PIC24EP256GP204
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.30
        MPLAB             :  MPLAB X 3.45
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

#ifndef _UART1_H
#define _UART1_H

/**
  Section: Included Files
*/

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/**
  Section: UART1 Driver Routines
*/
/**
  @Summary
    Initializes the UART

  @Description
    This routine initializes the UART module.
    This routine must be called before any other UART routine 
    is called.
    
  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Comment
    
*/
void UART1_Initialize(void);

/**
  @Summary
    Read a byte of data from the UART1

  @Description
    This routine reads a byte of data from the UART1.  It will
    block until a byte of data is available.  If you do not wish to block, call 
    the UART1_IsTxReady() function to check to see if there is
    data available to read first.

  @Preconditions
    UART1_Initialize function should have been called 
    before calling this function. 

  @Param
    None.

  @Returns
    A data byte received by the driver.
*/
uint8_t UART1_Read(void);

/**
  @Summary
    Writes a byte of data to the UART1

  @Description
    This routine writes a byte of data to the UART1. This function
    will block if this transmit buffer is currently full until the transmit 
    buffer becomes available.  If you do not wish to block, call the
    UART1_IsTxReady() function to check on the transmit
    availability.

  @Preconditions
    UART1_Initialize() function should have been called
    before calling this function.

  @Param
    byte - data to be written
*/
void UART1_Write(uint8_t byte);

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







/*******************************************************************************

  !!! Deprecated API and types !!!
  !!! These functions will not be supported in future releases !!!

*******************************************************************************/

/**
  @Description
    Indicates of there is data available to read.

  @Returns
    true if byte can be read.
    false if byte can't be read right now.
*/
bool __attribute__((deprecated)) UART1_is_rx_ready(void);

/**
  @Description
    Indicates if a byte can be written.
 
 @Returns
    true if byte can be written.
    false if byte can't be written right now.
*/
bool __attribute__((deprecated)) UART1_is_tx_ready(void);

/**
  @Description
    Indicates if all bytes have been transferred.
 
 @Returns
    true if all bytes transfered.
    false if there is still data pending to transfer.
*/
bool __attribute__((deprecated)) UART1_is_tx_done(void);

/** UART1 Driver Hardware Flags

  @Summary
    Specifies the status of the hardware receive or transmit

  @Description
    This type specifies the status of the hardware receive or transmit.
    More than one of these values may be OR'd together to create a complete
    status value.  To test a value of this type, the bit of interest must be
    AND'ed with value and checked to see if the result is non-zero.
*/
typedef enum
{
    /* Indicates that Receive buffer has data, at least one more character can be read */
    UART1_RX_DATA_AVAILABLE = (1 << 0),
    /* Indicates that Receive buffer has overflowed */
    UART1_RX_OVERRUN_ERROR = (1 << 1),
    /* Indicates that Framing error has been detected for the current character */
    UART1_FRAMING_ERROR = (1 << 2),
    /* Indicates that Parity error has been detected for the current character */
    UART1_PARITY_ERROR = (1 << 3),
    /* Indicates that Receiver is Idle */
    UART1_RECEIVER_IDLE = (1 << 4),
    /* Indicates that the last transmission has completed */
    UART1_TX_COMPLETE = (1 << 8),
    /* Indicates that Transmit buffer is full */
    UART1_TX_FULL = (1 << 9) 
}UART1_STATUS;

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
UART1_STATUS __attribute__((deprecated)) UART1_StatusGet (void );

/**
  @Summary
    Indicates if data is ready for reading

  @Preconditions
    UART1_Initialize() function should have been called 
    before calling this function

  @Returns
    true - data available to read.
    false - no data available
*/
bool __attribute__((deprecated)) UART1_DataReady(void); 

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif  // _UART1_H

