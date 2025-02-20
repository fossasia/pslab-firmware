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
void UART_initialize(EUxSelect const select);

/**
 * @brief Read bytes from UARTx bus into buffer.
 *
 * @param select U1SELECT or U2SELECT.
 * @param buffer Pointer to writable uint8_t array.
 * @param size Size of buffer.
 * @return enum Status
 */
enum Status UART_read(EUxSelect select, uint8_t *buffer, uint16_t size);

/**
 * @brief Write bytes to UARTx from buffer.
 *
 * @param select U1SELECT or U2SELECT.
 * @param buffer Pointer to uint8_t array.
 * @param size Size of buffer.
 * @return enum Status
 */
enum Status UART_write(EUxSelect select, uint8_t const *buffer, uint16_t size);

/**
 * @brief Consume any incoming data until bus is idle.
 *
 * @param select U1SELECT or U2SELECT.
 * @return enum Status
 */
enum Status UART_flush_rx(EUxSelect select);

/**
 * @brief Read a one-byte integer value from UART2 and write it to UART1.
 *
 * @return NACK
 */
response_t UART2_read_u8(void);

/**
 * @brief Read a two-byte integer value from UART2 and write it to UART1.
 *
 * @return NACK
 */
response_t UART2_read_u16(void);

/**
 * @brief Read a one-byte integer value from UART1 and write it to UART2.
 *
 * @return NACK
 */
response_t UART2_write_u8(void);

/**
 * @brief Read a two-byte integer value from UART1 and write it to UART2.
 *
 * @return NACK
 */
response_t UART2_write_u16(void);

/**
 * @brief Query whether there is data available to read from UART2.
 *
 * @return rx_ready
 *   True if at least one more byte can be read from UART2.
 * @return NACK
 */
response_t UART2_rx_ready(void);

/**
 * @brief Set baudrate of UART2.
 *
 * @param brgval
 *   BRGVAL to set on UART2.
 *
 * @return SUCCESS
 */
response_t UART2_set_baud(void);

/**
 * @brief Set UART2 stop bit and parity.
 *
 * @param mode
 *   Bit 0 sets UART2 stop bit:
 *   0: One stop bit
 *   1: Two stop bits
 *   Bits 1-2 set UART2 parity:
 *   0: No parity
 *   1: Even parity
 *   2: Odd parity
 *
 * @return response_t
 */
response_t UART2_set_mode(void);

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
