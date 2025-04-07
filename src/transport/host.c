#include <stddef.h>

#include "types.h"

#include "../commands.h"
#include "../bus/uart/uart.h"
#include "host.h"

enum Status HOST_read(
    uint8_t *const buffer,
    uint16_t const size,
    uint16_t *const num_bytes_read
) {
    return UART_read(UART_PRIMARY, buffer, size, num_bytes_read);
}

enum Status HOST_read_u8(uint8_t *const u8)
{
    return HOST_read(u8, sizeof(uint8_t), NULL);
}

enum Status HOST_read_u16(uint16_t *const u16)
{
    return HOST_read((uint8_t *const)u16, sizeof(uint16_t), NULL);
}

enum Status HOST_read_u32(uint32_t *const u32)
{
    return HOST_read((uint8_t *const)u32, sizeof(uint32_t), NULL);
}

enum Status HOST_write(
    uint8_t const *const buffer,
    uint16_t const size,
    uint16_t *const num_bytes_written
) {
    return UART_write(UART_PRIMARY, buffer, size, num_bytes_written);
}

enum Status HOST_write_u8(uint8_t const u8)
{
    return HOST_write(&u8, sizeof(uint8_t), NULL);
}

enum Status HOST_write_u16(uint16_t const u16)
{
    return HOST_write((uint8_t const *const)&u16, sizeof(uint16_t), NULL);
}

enum Status HOST_write_u32(uint32_t const u32)
{
    return HOST_write((uint8_t const *const)&u32, sizeof(uint32_t), NULL);
}

enum Status HOST_flush_rx(void)
{
    return UART_flush_rx(UART_PRIMARY);
}

enum Status HOST_rx_ready(bool *const is_data_available)
{
    return UART_rx_ready(UART_PRIMARY, is_data_available);
}
