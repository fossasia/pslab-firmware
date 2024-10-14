#include <string.h>
#include "../bus/uart/uart.h"
#include "../commands.h"
#include "../registers/system/pin_manager.h"
#include "buffer.h"

// Space in memory to store data.
uint16_t volatile __attribute__((section(".adc_buffer"), far)) BUFFER[BUFFER_SIZE];

response_t BUFFER_Retrieve(void) {
    
    uint16_t volatile* idx = &BUFFER[UART2_ReadInt()];
    uint16_t volatile* end = idx + UART2_ReadInt();

    LED_SetLow();
    while (idx != end) UART2_WriteInt(*(idx++));
    LED_SetHigh();
    
    return SUCCESS;
}

response_t BUFFER_FetchInt(void) {
    
    uint16_t counter = UART2_ReadInt();
    uint8_t channel = UART2_Read();
    
    LED_SetLow();
    uint16_t i;
    for (i = 0; i < counter; i++) {
        UART2_WriteInt(BUFFER[i + channel * (BUFFER_SIZE / 4)]);
    }
    LED_SetHigh();
    
    return SUCCESS;
}

response_t BUFFER_FetchLong(void) {
    
    uint16_t counter = UART2_ReadInt();
    uint8_t channel = UART2_Read();
    
    LED_SetLow();
    uint16_t i;
    for (i = 0; i < counter; i++) {
        UART2_WriteInt(BUFFER[i + 2 * channel * (BUFFER_SIZE / 4)]);
        UART2_WriteInt(BUFFER[i + (2 * channel + 1) * (BUFFER_SIZE / 4)]);
    }
    LED_SetHigh();
    
    return SUCCESS;
}

response_t BUFFER_Fill(void) {
    
    uint16_t start = UART2_ReadInt();
    uint16_t end = UART2_ReadInt();
    
    uint16_t i;
    for (i = start; i < start + end; i++) {
        BUFFER[i] = UART2_ReadInt();
    }
    
    return SUCCESS;
}

response_t BUFFER_Clear(void) {
    
    uint16_t start = UART2_ReadInt();
    uint16_t end = UART2_ReadInt();
    
    memset((void *) &BUFFER[start], 0, (end - start) * sizeof(int));
    
    return SUCCESS;
}
