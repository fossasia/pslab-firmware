#include <xc.h>
#include <string.h>
#include "spi1.h"
#include "../spi/spi_driver.h"
#include "../uart/uart1.h"
#include "../../registers/system/pin_manager.h"

uint8_t volatile __attribute__((section(".spi_buffer"), far)) SPI_BUFFER[SPI_BUFFER_SIZE];

static SPI1_PARAMETERS parameters = {
    SPI1_MODE_BYTE,
    SPI1_DATA_SAMPLE_AT_END,
    SPI1_CLOCK_EDGE_ACTIVE_TO_IDLE,
    SPI1_CLOCK_POLARITY_IDLE_LOW,
    SPI1_SECONDARY_PRESCALER_6_TO_1,
    SPI1_PRIMARY_PRESCALER_64_TO_1
};

void SPI1_Initialize(void) {

    // Disable SPI module and clear flags
    SPI1STAT = 0x0000;
    // Set SPI configurations
    SPI1CON1bits.PPRE = parameters.primaryPrescaler;
    SPI1CON1bits.SPRE = parameters.secondaryPrescaler;
    // Enable internal clock
    SPI1CON1bits.DISSCK = 0;
    // Module controls SDO
    SPI1CON1bits.DISSDO = 0;
    // CS pin not used
    SPI1CON1bits.SSEN = 0;
    SPI1CON1bits.MODE16 = parameters.spiMode;
    SPI1CON1bits.CKE = parameters.clockEdge;
    SPI1CON1bits.CKP = parameters.clockPolarity;
    // Module is the master
    SPI1CON1bits.MSTEN = 1;
    SPI1CON1bits.SMP = parameters.dataSample;
    // Clear frame settings
    SPI1CON2 = 0x0000;
}

response_t SPI1_SetParameters(void) {

    uint8_t configuration = UART1_Read();

    parameters.clockEdge = (configuration >> 5) & 1;
    parameters.clockPolarity = (configuration >> 6) & 1;
    parameters.dataSample = (configuration >> 7) & 1;
    parameters.secondaryPrescaler = configuration & 0b111;
    parameters.primaryPrescaler = (configuration >> 3) & 0b11;

    SPI1_Initialize();

    SPI1_EnableModule();

    return SUCCESS;
}

response_t SPI1_Start(void) {

    SPI1_ChipSelect();

    return DO_NOT_BOTHER;
}

response_t SPI1_Stop(void) {

    SPI1_ChipDeselect();

    return DO_NOT_BOTHER;
}

void SPI1_ByteOperations(SPI1_OPERATION op, uint16_t count, uint8_t address) {

    uint16_t i;
    uint16_t turns = count;
    if (op == SPI1_OPERATION_READ) {
        turns = turns + 1;
        memset((void *) &SPI_BUFFER[1], 0x00, turns);
        SPI_BUFFER[0] = address;
    } else {
        for (i = 0; i < turns; i++) {
            SPI_BUFFER[i] = UART1_Read();
        }
    }

    if (parameters.spiMode != SPI1_MODE_BYTE) {
        parameters.spiMode = SPI1_MODE_BYTE;
        SPI1_Initialize();
    }
    SPI1_EnableModule();

    SPI1_ChipSelect();
    SPI_DRIVER_ExchangeBlock((void *) SPI_BUFFER, turns);
    SPI1_ChipDeselect();

    if (op == SPI1_OPERATION_EXCHANGE) {
        for (i = 1; i < turns; i++) {
            UART1_Write(SPI_BUFFER[i]);
        }
    } else if (op == SPI1_OPERATION_READ) {
        for (i = 1; i <= count; i++) {
            UART1_Write(SPI_BUFFER[i]);
        }
    }
}

response_t SPI1_Write8(void) {

    SPI1_ByteOperations(SPI1_OPERATION_WRITE, 2, 0);

    return SUCCESS;
}

response_t SPI1_Write8Burst(void) {

    uint16_t count = UART1_ReadInt();

    SPI1_ByteOperations(SPI1_OPERATION_WRITE, count, 0);

    return SUCCESS;
}

response_t SPI1_Send8Burst(void) {

    uint16_t count = UART1_ReadInt();

    SPI1_ByteOperations(SPI1_OPERATION_EXCHANGE, count, 0);

    return SUCCESS;
}

response_t SPI1_Read8Burst(void) {

    uint8_t address = UART1_Read();
    uint16_t count = UART1_ReadInt();

    SPI1_ByteOperations(SPI1_OPERATION_READ, count, address);

    return SUCCESS;
}
