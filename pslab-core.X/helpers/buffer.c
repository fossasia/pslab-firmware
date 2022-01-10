#include <string.h>
#include "../bus/uart/uart1.h"
#include "../commands.h"
#include "../registers/system/pin_manager.h"

// Space in memory to store data.
int16_t volatile __attribute__((section(".adc_buffer"), far)) BUFFER[10000];

response_t BUFFER_Retrieve(void) {
    int16_t volatile* idx = &BUFFER[UART1_ReadInt()];
    int16_t volatile* end = idx + UART1_ReadInt();

    while (idx != end) UART1_WriteInt(*(idx++));
    
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
