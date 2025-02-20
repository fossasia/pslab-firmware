#include "../commands.h"
#include "../bus/uart/uart.h"

// Primary control bus.
#define HOST_UART0 U1SELECT
#define HOST_UART1 U2SELECT

#ifndef PSLAB_HOST
#define PSLAB_HOST HOST_UART0
#endif // PSLAB_HOST

enum Status HOST_read(
    uint8_t *const buffer,
    uint16_t const size,
    uint16_t *const num_bytes_read
) {
    enum Status status;
    switch (status = UART_read(U1SELECT, buffer, size, num_bytes_read)) {
    case E_UART_RX_TIMEOUT:
    case E_UART_RX_PARITY:
    case E_UART_RX_FRAMING:
        return E_HOST_READ;
    case E_UART_RX_OVERRUN:
        return E_HOST_RX_OVERRUN;
    default:
        return status;
    }
}

enum Status HOST_read_u8(uint8_t *const u8)
{
    return HOST_read(u8, sizeof(uint8_t));
}

enum Status HOST_read_u16(uint16_t *const u16)
{
    return HOST_read((uint8_t *const)u16, sizeof(uint16_t));
}

enum Status HOST_read_u32(uint32_t *const u32)
{
    return HOST_read((uint8_t *const)u32, sizeof(uint32_t));
}

enum Status HOST_write(
    uint8_t const *const buffer,
    uint16_t const size
) {
    return UART_write(PSLAB_HOST, buffer, size);
}

enum Status HOST_write_u8(uint8_t const u8)
{
    return HOST_write(&u8, sizeof(uint8_t));
}

enum Status HOST_write_u16(uint16_t const u16)
{
    return HOST_write((uint8_t const *const)&u16, sizeof(uint16_t));
}

enum Status HOST_write_u32(uint32_t const u32)
{
    return HOST_write((uint8_t const *const)&u32, sizeof(uint32_t));
}

enum Status HOST_flush_rx(void)
{
    return UART_flush_rx(PSLAB_HOST);
}
