#ifndef _PSLAB_HOST
#define _PSLAB_HOST

#include <stdint.h>

#include "../bus/uart/uart.h"

#ifndef UART_PRIMARY
#define UART_PRIMARY U1SELECT
#define UART_SECONDARY U2SELECT
#elif UART_PRIMARY == U2SELECT
#define UART_SECONDARY U1SELECT
#endif // UART_PRIMARY

/**
 * @brief Read bytes from primary UART.
 *
 * @param[out] uint8_t *buffer
 * @param uint16_t size
 * @param[out] uint16_t *num_bytes_read
 *   Caller may pass a null pointer if it is not interested in the number of
 *   bytes read. If return status is zero, *num_bytes_read == size.
 *
 * @return enum Status status
 */
enum Status HOST_read(
    uint8_t *buffer,
    uint16_t size,
    uint16_t *num_bytes_read
);
enum Status HOST_read_u8(uint8_t *u8);
enum Status HOST_read_u16(uint16_t *u16);
enum Status HOST_read_u32(uint32_t *u32);

/**
 * @brief Write bytes to primary UART.
 *
 * @param[in] uint8_t *buffer
 * @param uint16_t size
 * @param[out] uint16_t *num_bytes_written
 *   Caller may pass a null pointer if it is not interested in the number of
 *   bytes written. If return status is zero, *num_bytes_written == size.
 *
 * @return enum Status status
 */
enum Status HOST_write(
    uint8_t const *buffer,
    uint16_t size,
    uint16_t *num_bytes_written
);
enum Status HOST_write_u8(uint8_t u8);
enum Status HOST_write_u16(uint16_t u16);
enum Status HOST_write_u32(uint32_t u32);

/**
 * @brief Discard any data present in primary UART RX buffer.
 *
 * @return enum Status status
 */
enum Status HOST_flush_rx(void);

/**
 * @brief Check if at least one byte of data can be read from primary UART.
 *
 * @param[out] bool *is_data_available
 *
 * @return enum Status status
 */
enum Status HOST_rx_ready(bool *is_data_available);

#endif // _PSLAB_HOST
