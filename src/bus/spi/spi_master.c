#include "spi_master.h"

bool SDFAST_open(void);
bool SDSLOW_open(void);

const spi_master_functions_t spiMaster[] = {
    {
        SPI_DRIVER_Close,
        SDFAST_open,
        SPI_DRIVER_ExchangeByte,
        SPI_DRIVER_ExchangeBlock,
        SPI_DRIVER_WriteBlock,
        SPI_DRIVER_ReadBlock,
        SPI_DRIVER_WriteByte,
        SPI_DRIVER_ReadByte,
        SPI_DRIVER_SetISR,
        SPI_DRIVER_ISR
    },
    {
        SPI_DRIVER_Close,
        SDSLOW_open,
        SPI_DRIVER_ExchangeByte,
        SPI_DRIVER_ExchangeBlock,
        SPI_DRIVER_WriteBlock,
        SPI_DRIVER_ReadBlock,
        SPI_DRIVER_WriteByte,
        SPI_DRIVER_ReadByte,
        SPI_DRIVER_SetISR,
        SPI_DRIVER_ISR
    }
};

bool SDFAST_open(void) {
    return SPI_DRIVER_Open(SDFAST_CONFIG);
}

bool SDSLOW_open(void) {
    return SPI_DRIVER_Open(SDSLOW_CONFIG);
}

/**
 *  \ingroup doc_driver_spi_code
 *  \brief Open the SPI interface.
 *
 *  This function is to keep the backward compatibility with older API users
 *  \param[in] configuration The configuration to use in the transfer
 *
 *  \return Initialization status.
 *  \retval false The SPI open was unsuccessful
 *  \retval true  The SPI open was successful
 */
bool SPI_MASTER_Open(spi_master_configurations_t config) {
    switch (config) {
        case SDFAST:
            return SDFAST_open();
        case SDSLOW:
            return SDSLOW_open();
        default:
            return 0;
    }
}

/**
 End of File
 */
