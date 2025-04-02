#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "host.h"
#include "packet_handler.h"
#include "../bus/uart/uart.h"
#include "../commands.h"

enum Status UART2_read(
    uint8_t **args,
    uint16_t const args_size,
    uint8_t **rets,
    uint16_t *const rets_size
) {
    struct Input {
        uint16_t size;
        uint8_t _pad[0];
    } *input = NULL;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) {
        return E_BAD_ARGSIZE;
    }
    input = *(struct Input **)args;

    if (!input->size) { return E_OK; }
    if ( !(*rets = malloc(input->size)) ) { return E_MEMORY_INSUFFICIENT; }

    return UART_read(UART_SECONDARY, *rets, input->size, rets_size);
}

enum Status UART2_write(
    uint8_t **args,
    uint16_t const args_size,
    uint8_t **rets,
    uint16_t *const rets_size
) {
    uint16_t num_bytes_written = 0;

    if ( !(*rets = realloc(*args, sizeof(num_bytes_written))) ) {
        return E_MEMORY_INSUFFICIENT;
    }

    enum Status status = UART_write(
        UART_SECONDARY,
        *args,
        args_size,
        &num_bytes_written
    );

    *args = NULL;
    **(uint16_t **)rets = num_bytes_written;
    *rets_size = sizeof(num_bytes_written);
    return status;
}

enum Status UART2_set_mode(
    uint8_t **args,
    uint16_t const args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *const rets_size
) {

    uint8_t stop = ONE_STOP_BIT;
    uint8_t parity = NO_PARITY;
    uint8_t mode = (parity << 1) | stop;
    if (args_size != sizeof(mode)) { return E_BAD_ARGSIZE; }
    mode = **args;
    stop = mode & 0x01;
    parity = (mode >> 1) & 0x03;
    enum Status status = E_OK;

    if ((status = UART_set_stop(UART_SECONDARY, stop))) { return status; }
    if ((status = UART_set_parity(UART_SECONDARY, parity))) { return status; }

    return E_OK;
}

enum Status UART2_set_baud(
    uint8_t **args,
    uint16_t const args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *const rets_size
) {
    uint16_t baud = BAUD1000000;
    if (args_size != sizeof(baud)) { return E_BAD_ARGSIZE; }
    baud = **(uint16_t **)args;
    return UART_set_baud(UART_SECONDARY, baud);
}

enum Status UART2_flush_rx(
    __attribute__((unused)) uint8_t **args,
    __attribute__((unused)) uint16_t const args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *const rets_size
) {
    return UART_flush_rx(UART_SECONDARY);
}

enum Status UART2_rx_ready(
    __attribute__((unused)) uint8_t **args,
    __attribute__((unused)) uint16_t const args_size,
    uint8_t **rets,
    uint16_t *const rets_size
) {
    bool ready = false;
    if ( !(*rets = malloc(sizeof(ready))) ) { return E_MEMORY_INSUFFICIENT; }
    enum Status status = UART_rx_ready(UART_SECONDARY, &ready);
    **rets = (uint8_t)ready;
    *rets_size = sizeof(ready);
    return status;
}
