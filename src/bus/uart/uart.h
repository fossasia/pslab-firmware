#ifndef _UART_H
#define _UART_H

#include <stdbool.h>
#include <stdint.h>
#include "../../commands.h"

/**
 * @brief Identifier for UART1 or UART2.
 */
typedef enum
{
  U1SELECT,
  U2SELECT,
} EUxSelect;

/**
 * @brief Initialization function that sets up the UARTx driver for use.
 *
 * This function initializes the selected UART driver. This function must be
 * called before any other UART functions are called.
 *
 * @param select Either U1SELECT or U2SELECT.
 */
void UART_Initialize(const EUxSelect select);

/**
 * @brief Read a byte of data from UARTx.
 * @pre
 *   UART_Initialize() should have been called for UARTx before
 *   calling this function.
 *   UART_IsRxReady() should be used to check if there is incoming data before
 *   calling this function. This function will block for up to 1 ms while
 *   waiting data to become available on UxRXREG.
 *
 * @param select Either U1SELECT or U2SELECT.
 * @return
 *   Eight bits of data. The return value is zero case of bus error or timeout.
 *   There is no indication of whether a return value of zero is valid or not.
 */
uint8_t UART_Read(const EUxSelect select);

/**
 * @brief Convenience function, equivalent to UART_Read(U1SELECT).
 */
uint8_t UART1_Read(void);

/**
 * @brief Read a word of data from UARTx.
 *
 * The word consists of two bytes, lsb first.
 *
 * @pre
 *   UART_Initialize() should have been called for UARTx before
 *   calling this function.
 *   UART_IsRxReady() should be used to check if there is incoming data before
 *   calling this function. This function will block for up to 2 ms while
 *   waiting data to become available on UxRXREG.
 *
 * @param select Either U1SELECT or U2SELECT.
 * @return
 *   16 bits of data. The return value is zero case of bus error or timeout.
 *   Note that it is possible for one of the bytes to be valid, while the
 *   other may be invalid due to an error.
*/
uint16_t UART_ReadInt(const EUxSelect select);

/**
 * @brief Convenience function, equivalent to UART_ReadInt(U1SELECT).
 */
uint16_t UART1_ReadInt(void);

/**
 * @brief Writes a byte of data to the UARTx.
 * @pre
 *   UART2_Initialize() should have been called before calling this function.
 *
 * @param select Either U1SELECT or U2SELECT.
 * @param txData Data byte to write to UARTx.
 */
void UART_Write(const EUxSelect select, uint8_t txData);

/**
 * @brief Convenience function, equivalent to UART_Write(U1SELECT, data).
 */
uint16_t UART1_ReadInt(void);
void UART1_Write(uint8_t txData);

/**
 * @brief Writes a word of data to the UARTx.
 *
 * The word consists of two bytes, lsb first.
 *
 * @pre
 *   UART2_Initialize() should have been called before calling this function.
 *
 * @param select Either U1SELECT or U2SELECT.
 * @param txData Data word to write to UARTx.
 */
void UART_WriteInt(const EUxSelect select, uint16_t txData);

/**
 * @brief Convenience function, equivalent to UART_WriteInt(U1SELECT, data).
 */
void UART1_WriteInt(uint16_t txData);

/**
 * @brief Indicates whether there is data available to read.
 * @return True if at least one byte can be read.
 */
bool UART_IsRxReady(const EUxSelect select);

/**
 * @brief Discard incoming serial traffic.
 *
 * Reads the RX buffer until no more data is available.
 */
void UART_ClearBuffer(const EUxSelect select);

#endif // _UART_H
