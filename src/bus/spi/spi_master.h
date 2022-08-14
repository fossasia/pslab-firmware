#ifndef _SPI_MASTER_H
#define _SPI_MASTER_H

#include <stdint.h>
#include <stdbool.h>
#include "spi_driver.h"

/**
 *   \ingroup doc_driver_spi_code
 *   \enum spi_master_configurations_t spi_master.h
 */
typedef enum {
    SDFAST,
    SDSLOW
} spi_master_configurations_t;

/**
 *   \ingroup doc_driver_spi_code
 *   \struct spi_master_functions_t spi_master.h
 */
typedef struct {
    void (*spiClose)(void);
    bool (*spiOpen)(void);
    uint8_t(*exchangeByte)(uint8_t b);
    void (*exchangeBlock)(void * block, size_t blockSize);
    void (*writeBlock)(void * block, size_t blockSize);
    void (*readBlock)(void * block, size_t blockSize);
    void (*writeByte)(uint8_t byte);
    uint8_t(*readByte)(void);
    void (*setSpiISR)(void(*handler)(void));
    void (*spiISR)(void);
} spi_master_functions_t;

extern const spi_master_functions_t spiMaster[];

bool SPI_MASTER_Open(spi_master_configurations_t config); //for backwards compatibility

#endif	// _SPI_MASTER_H
