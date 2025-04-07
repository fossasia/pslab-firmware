#ifndef _UART_H
#define _UART_H

#include <stdbool.h>
#include <stdint.h>

#include "types.h"

#include "../../commands.h"

/**
 * @brief Identifier for UART1 or UART2.
 */
typedef enum {
    U1SELECT,
    U2SELECT,
} EUxSelect;

/**
 * @brief Suitable values for UxBRG register.
 * @details UxBRG = [FCY / (4 * BAUD)] - 1
*/
typedef enum {
  BAUD1000000 = 15,
  BAUD576000 = 27,
  BAUD500000 = 31,
  BAUD460800 = 34,
  BAUD230400 = 68,
  BAUD115200 = 138,
  BAUD57600 = 277,
  BAUD38400 = 416,
  BAUD19200 = 832,
  BAUD9600 = 1666,
  BAUD4800 = 3332,
  BAUD2400 = 6666,
  BAUD1800 = 8888,
  BAUD1200 = 13332,
  BAUD600 = 26666,
  BAUD300 = 53332,
} EBaudrate;

/**
* @brief Possible values of UxMODE.STSEL field.
*/
typedef enum {
  ONE_STOP_BIT,
  TWO_STOP_BITS,
} EStopBits;

/**
* @brief Possible values of UxMODE.PDSEL field.
*/
typedef enum {
  NO_PARITY,
  EVEN_PARITY,
  ODD_PARITY,
  // Unimplemented; Not supported by pyserial.
  // NINE_BIT_DATA,
} EParity;

/**
 * @brief Initialization function that sets up the UARTx driver for use.
 *
 * This function initializes the selected UART driver. This function must be
 * called before any other UART functions are called.
 *
 * @param EUxSelect select
 *
 * @return enum Status status
 */
enum Status UART_initialize(EUxSelect select);

/**
 * @brief Set number of stop bits.
 *
 * A UART frame ends when the transmitter sets its Tx line high for at least
 * one bit period after sending all data bits. This is called a stop bit.
 * Optionally, more than one stop bit can be used, which may make transmission
 * more resilient to e.g. small differences in clock rate between transmitter
 * and reciever.
 *
 * The default is one stop bit.
 *
 * @param EUxSelect select
 * @param EStopBits stop
 *
 * @return enum Status status
 */
enum Status UART_set_stop(EUxSelect select, EStopBits stop);

/**
 * @brief Set parity.
 *
 * If parity is enabled, one additional bit is sent after the data bits. This
 * bit is used to detect if the data changed during transmission. If the
 * parity mode is even, the total number of 1's in the UART frame (including
 * the parity bit) must be even. If the parity mode is odd, the number of 1's
 * in the UART frame (including the parity bit) must be odd.
 *
 * The default mode is no parity.
 *
 * @param EUxSelect select
 * @param EParity parity.
 */
enum Status UART_set_parity(EUxSelect select, EParity parity);

/**
 * @brief Set baudrate.
 *
 * @param EUxSelect select
 * @param EBaudrate baud
 *
 * @return enum Status status
 */
enum Status UART_set_baud(EUxSelect select, EBaudrate baud);

/**
 * @brief Read bytes from UARTx bus into buffer.
 *
 * @param EUxSelect select
 * @param[out] uint8_t *buffer
 * @param uint16_t buffer_size
 * @param[out] uint16_t *num_bytes_read
 *   Caller may pass a null pointer if it is not interested in the number of
 *   bytes read. If return status is zero, *num_bytes_read == size.
 *
 * @return enum Status status
 */
enum Status UART_read(
    EUxSelect select,
    uint8_t *buffer,
    uint16_t buffer_size,
    uint16_t *num_bytes_read
);

/**
 * @brief Write bytes to UARTx from buffer.
 *
 * @param EUxSelect select
 * @param[in] uint8_t *buffer
 * @param uint16_t size
 * @param[out] uint16_t *num_bytes_written
 *   Caller may pass a null pointer if it is not interested in the number of
 *   bytes written. If return status is zero, *num_bytes_written == size.
 *
 * @return enum Status status
 */
enum Status UART_write(
  EUxSelect select,
  uint8_t const *buffer,
  uint16_t size,
  uint16_t *num_bytes_written
);

/**
 * @brief Consume any incoming data until bus is idle.
 *
 * @param EUxSelect select
 *
 * @return enum Status status
 */
enum Status UART_flush_rx(EUxSelect select);

/**
 * @brief Check if at least one byte of data can be read.
 *
 * @param EUxSelect select
 * @param[out] bool *is_data_available
 * @return enum Status status
 */
enum Status UART_rx_ready(EUxSelect select, bool *is_data_available);

#endif // _UART_H
