#ifndef SPI1_DRIVER_H
#define SPI1_DRIVER_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "spi1_types.h"

#define INLINE  inline 

/* arbitration interface */
INLINE void spi1_close(void);

bool spi1_open(spi1_modes spiUniqueConfiguration);
/* SPI native data exchange function */
uint8_t spi1_exchangeByte(uint8_t b);
/* SPI Block move functions }(future DMA support will be here) */
void spi1_exchangeBlock(void *block, size_t blockSize);
void spi1_writeBlock(void *block, size_t blockSize);
void spi1_readBlock(void *block, size_t blockSize);

void spi1_writeByte(uint8_t byte);
uint8_t spi1_readByte(void);

void spi1_isr(void);
void spi1_setSpiISR(void(*handler)(void));

#endif // SPI1_DRIVER_H
