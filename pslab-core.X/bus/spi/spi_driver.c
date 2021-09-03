#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../../registers/system/system.h"
#include "spi_driver.h"

void (*spi_interruptHandler)(void);

// con1 == SPIxCON1
// con2 == SPIxCON2
// stat == SPIxSTAT
// operation == Master/Slave

typedef struct {
    uint16_t CON1;
    uint16_t CON2;
    uint16_t STAT;
    uint8_t OP;
} spi_configuration_t;

static const spi_configuration_t spi_configuration[] = {
    { 0x006B, 0x0000, 0x0000, 0},
    { 0x0074, 0x0000, 0x0000, 0},
    { 0x0520, 0x0000, 0x0000, 0},
    { 0x0120, 0x0000, 0x0000, 0},
};

bool SPI_DRIVER_Open(SPI_MODES mode) {
    if (!SPI1STATbits.SPIEN) {
        SPI1CON1 = spi_configuration[mode].CON1;
        SPI1CON2 = spi_configuration[mode].CON2;
        SPI1STAT = spi_configuration[mode].STAT | 0x8000;
        TRISCbits.TRISC3 = spi_configuration[mode].OP;

        return true;
    }
    return false;
}

void SPI_DRIVER_Close(void) {
    SPI1STATbits.SPIEN = 0;
}
// Full Duplex SPI Functions

uint8_t SPI_DRIVER_ExchangeByte(uint8_t b) {
    SPI1BUF = b;
    while (!SPI1STATbits.SPIRBF);
    return SPI1BUF;
}

uint16_t SPI_DRIVER_ExchangeWord(uint16_t w) {
    SPI1BUF = w;
    while (!SPI1STATbits.SPIRBF);
    return SPI1BUF;
}

void SPI_DRIVER_ExchangeBlock(void *block, size_t blockSize) {
    uint8_t *data = block;
    while (blockSize--) {
        *data = SPI_DRIVER_ExchangeByte(*data);
        data++;
    }
}

// Half Duplex SPI Functions

void SPI_DRIVER_WriteBlock(void *block, size_t blockSize) {
    uint8_t *data = block;
    while (blockSize--) {
        SPI_DRIVER_ExchangeByte(*data++);
    }
}

void SPI_DRIVER_ReadBlock(void *block, size_t blockSize) {
    uint8_t *data = block;
    while (blockSize--) {
        *data++ = SPI_DRIVER_ExchangeByte(0);
    }
}

void SPI_DRIVER_WriteByte(uint8_t byte) {
    SPI1BUF = byte;
}

uint8_t SPI_DRIVER_ReadByte(void) {
    return SPI1BUF;
}

/**
 * Interrupt from SPI on bit 8 received and SR moved to buffer
 * If interrupts are not being used, then call this method from the main while(1) loop
 */
void SPI_DRIVER_ISR(void) {
    if (IFS0bits.SPI1IF == 1) {
        if (spi_interruptHandler) {
            spi_interruptHandler();
        }
        IFS0bits.SPI1IF = 0;
    }
}

void SPI_DRIVER_SetISR(void(*handler)(void)) {
    spi_interruptHandler = handler;
}
