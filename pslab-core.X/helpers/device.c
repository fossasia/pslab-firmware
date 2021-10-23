#include <stdint.h>

#include "../bus/uart/uart1.h"
#include "../commands.h"
#include "device.h"

const uint8_t VERSION_HW[] = "PSLab V6"; /** Hardware version. **/

response_t DEVICE_GetVersion(void) {
    uint8_t i;
    for (i = 0; i < sizeof(VERSION_HW) - 1; i++) UART1_Write(VERSION_HW[i]);
    UART1_Write('\n');
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