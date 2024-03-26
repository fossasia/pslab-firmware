#include <string.h>
#include "../bus/uart/uart.h"
#include "../commands.h"
#include "../registers/system/pin_manager.h"
#include "buffer.h"

// Space in memory to store data.
uint16_t volatile __attribute__((section(".sample_buffer"), far)) BUFFER[BUFFER_SIZE];

/**
 * @brief Send buffer area over serial
 *
 * @param start
 * @param size
 */
static void BUFFER_send(uint16_t volatile const *const start, size_t const size)
{
    LED_SetLow();
    for (size_t i = 0; i < size; ++i) {
        UART1_WriteInt(*(start + i));
    }
    LED_SetHigh();
}

uint16_t *BUFFER_set(
    uint16_t volatile const *const start,
    uint16_t const val,
    size_t const size
)
{
    return memset((void *)start, val, size);
}

response_t BUFFER_Retrieve(void) {
    
    uint16_t volatile const *const idx = &BUFFER[UART1_ReadInt()];
    uint16_t volatile const *const end = idx + UART1_ReadInt();
    BUFFER_send(idx, (uint16_t)(end - idx));
    return SUCCESS;
}

response_t BUFFER_FetchInt(void) {
    
    uint16_t counter = UART1_ReadInt();
    uint8_t channel = UART1_Read();
    BUFFER_send((uint16_t *)(channel * (BUFFER_SIZE / 4)), counter);
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
