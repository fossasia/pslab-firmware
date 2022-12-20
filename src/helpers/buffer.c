#include <string.h>
#include "../bus/uart/uart.h"
#include "../commands.h"
#include "../registers/system/pin_manager.h"
#include "buffer.h"

// Space in memory to store data.
uint16_t volatile __attribute__((section(".adc_buffer"), far)) BUFFER[BUFFER_SIZE];

response_t BUFFER_Retrieve(void) {
    
    uint16_t volatile* idx = &BUFFER[UART1_ReadInt()];
    uint16_t volatile* end = idx + UART1_ReadInt();

    LED_SetLow();
    while (idx != end) UART1_WriteInt(*(idx++));
    LED_SetHigh();
    
    return SUCCESS;
}

response_t BUFFER_FetchInt(void) {
    
    uint16_t counter = UART1_ReadInt();
    uint8_t channel = UART1_Read();
    
    LED_SetLow();
    uint16_t i;
    for (i = 0; i < counter; i++) {
        UART1_WriteInt(BUFFER[i + channel * (BUFFER_SIZE / 4)]);
    }
    LED_SetHigh();
    
    return SUCCESS;
}

response_t BUFFER_FetchLong(void) {
    
    uint16_t counter = UART1_ReadInt();
    uint8_t channel = UART1_Read();
    
    LED_SetLow();
    uint16_t i;
    for (i = 0; i < counter; i++) {
        UART1_WriteInt(BUFFER[i + 2 * channel * (BUFFER_SIZE / 4)]);
        UART1_WriteInt(BUFFER[i + (2 * channel + 1) * (BUFFER_SIZE / 4)]);
    }
    LED_SetHigh();
    
    return SUCCESS;
}

response_t BUFFER_Fill(void) {
    
    uint16_t start = UART1_ReadInt();
    uint16_t end = UART1_ReadInt();
    
    uint16_t i;
    for (i = start; i < start + end; i++) {
        BUFFER[i] = UART1_ReadInt();
    }
    
    return SUCCESS;
}

response_t BUFFER_Clear(void) {
    
    uint16_t start = UART1_ReadInt();
    uint16_t end = UART1_ReadInt();
    
    memset((void *) &BUFFER[start], 0, end - start);
    
    return SUCCESS;
}
