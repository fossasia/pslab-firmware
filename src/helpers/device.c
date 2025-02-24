#include <stdint.h>
#include <stddef.h>

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
    __attribute__ ((unused)) uint8_t const *const args,
    __attribute__ ((unused)) uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    *rets = (uint8_t *const)&VERSION_HW;
    *rets_size = sizeof(VERSION_HW);
    return E_OK;
}

enum Status DEVICE_get_fw_version(
    __attribute__ ((unused)) uint8_t const *const args,
    __attribute__ ((unused)) uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    *rets = (uint8_t *const)&VERSION_FW.version;
    *rets_size = sizeof(VERSION_FW);
    return E_OK;
}

__attribute__((noreturn)) enum Status DEVICE_reset(
    __attribute__ ((unused)) uint8_t const *const args,
    __attribute__ ((unused)) uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    __asm__ volatile ("reset");
    __builtin_unreachable();
}

enum Status DEVICE_read_register(
    uint8_t const *const args,
    uint16_t const args_size,
    uint8_t volatile **rets,
    uint16_t *rets_size
) {
    uint16_t volatile *address = NULL;

    if (args_size != sizeof(address)) {
        return E_BAD_ARGSIZE;
    }

    address = *(uint16_t volatile *const *const)args;
   *rets = (uint8_t volatile  *const)address;
   *rets_size = sizeof(*address);
    return E_OK;
}

enum Status DEVICE_write_register(
    uint8_t const *const args,
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    uint16_t volatile *address = NULL;
    uint16_t data = 0;

    if (args_size != (sizeof(address) + sizeof(data))) {
        return E_BAD_ARGSIZE;
    }

    address = *(uint16_t volatile *const *const)args;
    data = *(uint16_t const *const)(args + sizeof(address));
    *address = data;
    return E_OK;
}
