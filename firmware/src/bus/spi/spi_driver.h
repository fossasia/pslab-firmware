#ifndef SPI1_DRIVER_H
#define SPI1_DRIVER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    SDFAST_CONFIG,
    SDSLOW_CONFIG,
    PGA_CONFIG,
    SPI1_DEFAULT,
} SPI_MODES;

bool SPI_DRIVER_Open(SPI_MODES mode);
void SPI_DRIVER_Close(void);

/* SPI native data exchange function */
uint8_t SPI_DRIVER_ExchangeByte(uint8_t b);
uint16_t SPI_DRIVER_ExchangeWord(uint16_t w);
/* SPI Block move functions }(future DMA support will be here) */
void SPI_DRIVER_ExchangeBlock(void *block, size_t blockSize);

void SPI_DRIVER_WriteBlock(void *block, size_t blockSize);
void SPI_DRIVER_WriteByte(uint8_t byte);

void SPI_DRIVER_ReadBlock(void *block, size_t blockSize);
uint8_t SPI_DRIVER_ReadByte(void);

void SPI_DRIVER_ISR(void);
void SPI_DRIVER_SetISR(void(*handler)(void));

#endif // SPI1_DRIVER_H
