#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../bus/uart/uart.h"
#include "../commands.h"
#include "../registers/system/pin_manager.h"
#include "../registers/system/watchdog.h"

#define SEMVERS 3

static char const VERSION_HW[] =
#ifndef V5_HW
    "PSLab V6";
#else
    "PSLab V5";
#endif // V5_HW
static union {
    struct {
        uint8_t const major;
        uint8_t const minor;
        uint8_t const patch;
    };
    uint8_t const version[SEMVERS];
} const VERSION_FW = {{
    .major = 4,
    .minor = 0,
    .patch = 0
}};

enum Status DEVICE_get_hw_version(
    __attribute__ ((unused)) uint8_t **args,
    __attribute__ ((unused)) uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    // Caller will free *rets so we can't just hand over &VERSION_HW.
    if ( !(*rets = malloc(sizeof(VERSION_HW))) ) {
        return E_MEMORY_INSUFFICIENT;
    }
    memcpy(*rets, VERSION_HW, sizeof(VERSION_HW));
    *rets_size = sizeof(VERSION_HW);
    return E_OK;
}

enum Status DEVICE_get_fw_version(
    __attribute__ ((unused)) uint8_t **args,
    __attribute__ ((unused)) uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    // Caller will free *rets so we can't just hand over &VERSION_FW.
    if ( !(*rets = malloc(sizeof(VERSION_FW))) ) {
        return E_MEMORY_INSUFFICIENT;
    }
    memcpy(*rets, VERSION_FW.version, sizeof(VERSION_FW));
    *rets_size = sizeof(VERSION_FW);
    return E_OK;
}

__attribute__((noreturn)) enum Status DEVICE_reset(
    __attribute__ ((unused)) uint8_t **args,
    __attribute__ ((unused)) uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    __asm__ volatile ("reset");
    __builtin_unreachable();
}

enum Status DEVICE_read_register(
    uint8_t **args,
    uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    uint16_t volatile *address = NULL;

    if (args_size != sizeof(address)) {
        return E_BAD_ARGSIZE;
    }

    address = *(uint16_t volatile **)args;
   *rets = (uint8_t *)address;
   *rets_size = sizeof(*address);
    return E_OK;
}

enum Status DEVICE_write_register(
    uint8_t **args,
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    struct Input {
        uint16_t volatile *address;
        uint16_t data;
        uint8_t _pad[0];
    } *input = NULL;

    if (args_size != (sizeof(struct Input) + sizeof(input->_pad))) {
        return E_BAD_ARGSIZE;
    }

    input = *(struct Input **)args;
    *input->address = input->data;
    return E_OK;
}
