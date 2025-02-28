#include <stddef.h>
#include <stdint.h>

#include "host.h"
#include "packet_handler.h"
#include "../bus/uart/uart.h"
#include "../commands.h"

enum Status UART2_read(
    uint8_t *const args,
    uint16_t const args_size,
    uint8_t **rets,
    uint16_t *const rets_size
) {
    union Input {
        uint16_t size;
        uint8_t const *buffer;
    } input = {0};

    if (args_size != sizeof(input)) {
        return E_BAD_ARGSIZE;
    }

    input.buffer = args;
    // args will be reused for output, so need to copy to new var.
    uint16_t const size = input.size;

    *rets = args;
    enum Status status = E_OK;

    switch (status = UART_read(UART_SECONDARY, *rets, size, rets_size)) {
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

enum Status UART2_write(
    uint8_t *const args,
    uint16_t const args_size,
    uint8_t **rets,
    uint16_t *const rets_size
) {
    if (args_size > PACKET_SIZE_MAX) {
        return E_BAD_SIZE;
    }

    uint16_t num_bytes_written = 0;
    enum Status status = UART_write(
        UART_SECONDARY,
        args,
        args_size,
        &num_bytes_written
    );
    *rets = args;
    ((uint16_t*)*rets)[0] = num_bytes_written;
    *rets_size = sizeof(num_bytes_written);
    return status;
}

enum Status UART2_flush_rx(
    __attribute__((unused)) uint8_t const *const args,
    __attribute__((unused)) uint16_t const args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *const rets_size
) {
    return UART_flush_rx(UART_SECONDARY);
}
