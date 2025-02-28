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

enum Status HOST_read(
    uint8_t *buffer,
    uint16_t size,
    uint16_t *num_bytes_read
);
enum Status HOST_read_u8(uint8_t *u8);
enum Status HOST_read_u16(uint16_t *u16);
enum Status HOST_read_u32(uint32_t *u32);

enum Status HOST_write(
    uint8_t const *buffer,
    uint16_t size,
    uint16_t *num_bytes_written
);
enum Status HOST_write_u8(uint8_t u8);
enum Status HOST_write_u16(uint16_t u16);
enum Status HOST_write_u32(uint32_t u32);
enum Status HOST_flush_rx(void);
enum Status HOST_rx_ready(bool *ready);

#endif // _PSLAB_HOST
