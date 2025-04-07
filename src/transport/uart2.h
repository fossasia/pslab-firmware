#include <stdint.h>

#include "types.h"
#include "../commands.h"

enum Status UART2_read(
    uint8_t **args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

enum Status UART2_write(
    uint8_t **args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

enum Status UART2_flush_rx(
    uint8_t **args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

enum Status UART2_set_mode(
    uint8_t **args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

enum Status UART2_set_baud(
    uint8_t **args,
    uint16_t  args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

enum Status UART2_rx_ready(
    uint8_t **args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);
