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
