#include <stdint.h>
#include <stddef.h>

#include "../bus/uart/uart.h"
#include "../commands.h"
#include "../registers/system/pin_manager.h"
#include "../registers/system/watchdog.h"

#define SEMVERS 3

static uint8_t const VERSION_HW[] =
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
    .major = 3,
    .minor = 0,
    .patch = 3
}};

response_t DEVICE_GetVersion(void) {
    for (size_t i = 0; i < sizeof(VERSION_HW) - 1; ++i) {
        UART1_Write(VERSION_HW[i]);
    }

    UART1_Write('\n');

    return DO_NOT_BOTHER;
}

response_t DEVICE_get_fw_version(void) {
    for (size_t i = 0; i < SEMVERS; ++i) {
        UART1_Write(VERSION_FW.version[i]);
    }

    return DO_NOT_BOTHER;
}

response_t DEVICE_Reset(void) {
    __asm__ volatile ("reset");

    return DO_NOT_BOTHER;
}

response_t DEVICE_ReadRegisterData(void) {
    uint16_t *address = (uint16_t *) (UART1_ReadInt() & 0xFFFF);
    UART1_WriteInt(*address);

    return SUCCESS;
}

response_t DEVICE_WriteRegisterData(void) {
    uint16_t *address = (uint16_t *) (UART1_ReadInt() & 0xFFFF);
    uint16_t data = UART1_ReadInt();
    *address = data;

    return SUCCESS;
}
