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

enum Status UART_read(EUxSelect select, uint8_t *buffer, uint16_t size);
enum Status UART_write(EUxSelect select, uint8_t const *buffer, uint16_t size);
enum Status UART_flush_rx(EUxSelect select);

/**
 * @brief Read a byte of data from UARTx.
 * @pre
 *   UART_Initialize() should have been called for UARTx before
 *   calling this function.
 *   UART_IsRxReady() should be used to check if there is incoming data before
 *   calling this function. This function will block for up to 1 ms while
 *   waiting for data to become available on UxRXREG.
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
 *   waiting for data to become available on UxRXREG.
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
 * @brief Read a uint32_t from UARTx.
 *
 * @pre
 *   UART_Initialize() must have been called for UARTx before
 *   calling this function.
 *   UART_IsRxReady() should be used to check if there is incoming data before
 *   calling this function. This function will block for up to 4 ms while
 *   waiting for data to become available on UxRXREG.
 *
 * @param select Either U1SELECT or U2SELECT.
 * @return uint32_t
 */
uint32_t UART_read_u32(EUxSelect const select);

/**
 * @brief Convenience function, equivalent to UART_read_u32(U1SELECT).
 */
uint32_t UART1_read_u32(void);

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
 * @brief Writes a uint32_t to UARTx.
 *
 * @pre
 *   UART2_Initialize() should have been called before calling this function.
 *
 * @param select Either U1SELECT or U2SELECT.
 * @param txData Data word to write to UARTx.
 */
void UART_write_u32(EUxSelect const select, uint32_t const txdata);

/**
 * @brief Convenience function, equivalent to UART_write_u32(U1SELECT, data).
 */
void UART1_write_u32(uint32_t const txdata);

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

/**
 * @brief Read a byte from UART2 and write it to UART1.
 * @return DO_NOT_BOTHER
 */
response_t UART2_Read(void);

/**
 * @brief Read a word from UART2 and write it to UART1.
 * @return DO_NOT_BOTHER
 */
response_t UART2_ReadWord(void);

/**
 * @brief Read a byte from UART1 and write it to UART2.
 * @param TxData One byte of data to write to UART2.
 * @return DO_NOT_BOTHER
 */
response_t UART2_Write(void);

/**
 * @brief Read a word from UART1 and write it to UART2.
 * @param TxData Two bytes of data, lsb first, to write to UART2.
 * @return DO_NOT_BOTHER
 */
response_t UART2_WriteWord(void);

/**
 * @brief Check if data is available on UART2.
 *
 * Writes true (1) to UART1 if data is available, false (0) otherwise.
 *
 * @return DO_NOT_BOTHER
 */
response_t UART2_RxReady(void);

/**
 * @brief Set baudrate of UART2.
 * @param baud A BRGVAL, typically one of EBaudrate.
 * @return SUCCESS
 */
response_t UART2_SetBaud(void);

/**
 * @brief Set stop bits and parity of UART2.
 * @param mode
 *   One byte, where bit 0 is the stop bit configuration value, and bits two
 *   and three are the parity configuration value.
 *
 * @return SUCCESS
 */
response_t UART2_SetMode(void);

/**
 * @brief Send all traffic from UART1 to UART2 and vice versa.
 *
 * After running this function, the normal functionality of the PSLab becomes
 * unavailable. Since all data sent to UART1 is immediately passed to UART2,
 * it is no longer possible to run any commands. To resume normal operation,
 * the PSLab must be hard-reset.
 *
 * @param baud
 *   BRGVAL for both UART1 and UART2. Note that the host device controlling
 *   the PSLab must reconfigure its own UART interface to have the same
 *   baudrate.
 *
 * @return DO_NOT_BOTHER
 */
response_t UART_Passthrough(void);

#endif // _UART_H
