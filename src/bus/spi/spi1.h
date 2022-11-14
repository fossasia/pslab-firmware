#ifndef SPI1_H
#define SPI1_H

#include <stdbool.h>
#include <stdint.h>
#include <xc.h>

#include "../../commands.h"

#define SPI_BUFFER_SIZE     1000

#define SPI1_ChipSelect()   CS_SPI_SetLow()
#define SPI1_ChipDeselect() CS_SPI_SetHigh()

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
        SPI1_PRIMARY_PRESCALER_1_TO_1           = 0b11,
        SPI1_PRIMARY_PRESCALER_4_TO_1           = 0b10,
        SPI1_PRIMARY_PRESCALER_16_TO_1          = 0b01,
        SPI1_PRIMARY_PRESCALER_64_TO_1          = 0b00
    } SPI1_PRIMARY_PRESCALER;

    typedef enum {
        SPI1_SECONDARY_PRESCALER_1_TO_1         = 0b111,
        SPI1_SECONDARY_PRESCALER_2_TO_1         = 0b110,
        SPI1_SECONDARY_PRESCALER_3_TO_1         = 0b101,
        SPI1_SECONDARY_PRESCALER_4_TO_1         = 0b100,
        SPI1_SECONDARY_PRESCALER_5_TO_1         = 0b011,
        SPI1_SECONDARY_PRESCALER_6_TO_1         = 0b010,
        SPI1_SECONDARY_PRESCALER_7_TO_1         = 0b001,
        SPI1_SECONDARY_PRESCALER_8_TO_1         = 0b000
    } SPI1_SECONDARY_PRESCALER;

    typedef enum {
        SPI1_MODE_BYTE,
        SPI1_MODE_WORD
    } SPI1_MODE;

    typedef enum {
        SPI1_DATA_SAMPLE_AT_MIDDLE,
        SPI1_DATA_SAMPLE_AT_END
    } SPI1_DATA_SAMPLE;

    typedef enum {
        SPI1_CLOCK_EDGE_IDLE_TO_ACTIVE,
        SPI1_CLOCK_EDGE_ACTIVE_TO_IDLE
    } SPI1_CLOCK_EDGE;

    typedef enum {
        SPI1_CLOCK_POLARITY_IDLE_LOW,
        SPI1_CLOCK_POLARITY_IDLE_HIGH
    } SPI1_CLOCK_POLARITY;

    typedef enum {
        SPI1_OPERATION_WRITE,
        SPI1_OPERATION_READ,
        SPI1_OPERATION_EXCHANGE
    } SPI1_OPERATION;

    typedef struct {
        SPI1_MODE spiMode;
        SPI1_DATA_SAMPLE dataSample;
        SPI1_CLOCK_EDGE clockEdge;
        SPI1_CLOCK_POLARITY clockPolarity;
        SPI1_SECONDARY_PRESCALER secondaryPrescaler;
        SPI1_PRIMARY_PRESCALER primaryPrescaler;
    } SPI1_PARAMETERS;

    inline static void SPI1_EnableModule(void) {
        SPI1STATbits.SPIEN = 1;
    }

    inline static void SPI1_DisableModule(void) {
        SPI1STATbits.SPIEN = 0;
    }

    /**
     * @brief Initialize SPI routine with the set SPI1_PARAMETERS
     * @return None
     */
    void SPI1_Initialize(void);

    /**
     * @brief Set chip select on SPI header extension
     *
     * @description
     * This method takes nothing over serial
     *
     * @return DO_NOT_BOTHER
     */
    response_t SPI1_Start(void);

    /**
     * @brief Unset chip select on SPI header extension
     *
     * @description
     * This method takes nothing over serial
     *
     * @return DO_NOT_BOTHER
     */
    response_t SPI1_Stop(void);

    /**
     * @brief Configure module parameters for SPI
     *
     * @description
     * This method updates the configuration registers for SPI module
     * and enables the module for SPI communication protocol.
     * This command function takes one argument over serial:
     * 1. (uint8)  Configuration byte:
     *             | 7  | 6  | 5  | 4 | 3 | 2 | 1 | 0 |
     *             | DS | CP | CE | P-PRE |   S-PRE   |
     *             DS: Data sampling point
     *             CP: Clock polarity
     *             CE: Clock edge definition
     *             P-PRE: Primary pre-scaler
     *                    0: 64:1
     *                    1: 16:1
     *                    2: 4:1
     *                    3: 1:1
     *             S-PRE: Secondary pre-scaler
     *                    0: 8:1
     *                    1: 7:1
     *                    2: 6:1
     *                    3: 5:1
     *                    4: 4:1
     *                    5: 3:1
     *                    6: 2:1
     *                    7: 1:1
     *
     * It returns nothing over serial.
     * It sends an acknowledge byte (SUCCESS).
     *
     * @return SUCCESS
     */
    response_t SPI1_SetParameters(void);
    
    /**
     * @brief Encapsulates read and write byte sized operations used in SPI 
     * transactions. When reading back results, make sure to read one less byte
     * as the first byte is assigned as register address. 
     * 
     * @param op Type of SPI operation mode (SPI1_OPERATION)
     * @param count Number of UART interactions
     * @param address Starting address of SPI register
     * 
     * @return None
     */
    void SPI1_ByteOperations(SPI1_OPERATION op, uint16_t count, uint8_t address);

    /**
     * @brief Writes a byte to a register address
     *
     * @description
     * This method writes a single byte into a single SPI register address.
     * This command takes two arguments over serial.
     * 1. (uint8) Register address
     * 2. (uint8) Data byte
     *
     * This will not return any result.
     * 
     * It sends an acknowledge byte (SUCCESS).
     *
     * @return SUCCESS
     */
    response_t SPI1_Write8(void);
    
    /**
     * @brief Writes a stream of bytes to consecutive register locations
     *
     * @description
     * This method writes a byte stream to a set of consecutive register 
     * locations starting from a specific SPI address. 
     * This method can also be used to write to multiple registers that are not
     * located next to each other. In this case, user will create a stream of
     * pair of bytes with the first element being the register address and the
     * second element being the data to be written.
     * This command takes three sets of arguments over serial.
     * 1. (uint16) Byte count:
     *             This count should include the total number of data bytes plus
     *             one additional byte for the starting register address
     * 2. (uint8) Starting register address
     * 3. (uint8) Data bytes:
     *            The number of data byte count should be one less than the byte
     *            count in 1.
     *
     * This will not return any result.
     * 
     * It sends an acknowledge byte (SUCCESS).
     *
     * @return SUCCESS
     */
    response_t SPI1_Write8Burst(void);
    
    /**
     * @brief Writes and reads a stream of bytes to and from consecutive 
     * register locations
     *
     * @description
     * This method alternatively write and read from SPI data registers.
     * writes a byte stream to a set of consecutive register 
     * locations starting from a specific SPI address.
     * This command takes three sets of arguments over serial.
     * 1. (uint16) Byte count:
     *             This count should include the total number of control bytes
     *             plus one additional byte for the starting register address
     * 2. (uint8) Starting register address
     * 3. (uint8) Control bytes:
     *            The number of control byte count should be one less than the
     *            byte count in 1.
     *
     * This will return a series of bytes correspond to each control byte sent
     * as input. User must read one byte less than the byte count set as the 
     * first input in 1.
     * 
     * It sends an acknowledge byte (SUCCESS).
     *
     * @return SUCCESS
     */
    response_t SPI1_Send8Burst(void);

    /**
     * @brief Reads bytes from an address
     *
     * @description
     * This method reads byte chunks from a SPI register address.
     * One can use this method to read a single byte or a sequence
     * of bytes by changing the `number of bytes to read` argument.
     * This command takes two arguments over serial.
     * 1. (uint8) Register address
     * 2. (uint16) Number of bytes to read
     *
     * This will return multiple bytes equal to the set number of
     * bytes to read argument over serial.
     * It sends an acknowledge byte (SUCCESS).
     *
     * @return SUCCESS
     */
    response_t SPI1_Read8Burst(void);

    void SPI1_WordOperations(SPI1_OPERATION op, uint16_t count, uint16_t address);

    response_t SPI1_Write16(void);
    response_t SPI1_Write16Burst(void);
    response_t SPI1_Send16Burst(void);
    response_t SPI1_Read16Burst(void);

#ifdef __cplusplus  // Provide C++ Compatibility
}
#endif

#endif // SPI1_H