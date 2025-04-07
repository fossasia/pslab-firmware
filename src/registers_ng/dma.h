#ifndef DMA_H
#define DMA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "types.h"

/*********/
/* Types */
/*********/

/**
 * @brief DMA transaction source
 */
typedef enum DMA_Source {
    DMA_SOURCE_ADC,
    DMA_SOURCE_IC,
} DMA_Source;

/*************/
/* Functions */
/*************/

/**
 * @brief Reset DMA channel to default configuration
 *
 * @param channel
 */
enum Status DMA_reset(Channel channel);

/**
 * @brief Configure DMA channel
 * @details
 * Configure a DMA channel to move `count` words from `source` to the memory
 * block starting at `address`.
 *
 * Caller is responsible for managing the memory block starting at `address`.
 * Writes to said memory block after calling `DMA_start` results in undefined
 * behavior.
 *
 * @param channel
 * @param count
 * @param address
 * @param source
 */
enum Status DMA_setup(
    Channel channel,
    uint16_t count,
    size_t address,
    DMA_Source source
);

/**
 * @brief Start DMA transaction
 * @details
 * When the transaction is complete, the DMA channel's interrupt flag is raised
 * if interrupts are enabled for the channel.
 *
 * @param channel
 */
enum Status DMA_start(Channel channel);

/**
 * @brief Enable interrupt on DMA channel
 * @details
 * The interrupt occurs when data transaction is complete. When the interrupt
 * is generated, `callback` is called.
 *
 * @param channel
 * @param callback
 */
enum Status DMA_interrupt_enable(Channel channel, InterruptCallback callback);

#endif // DMA_H
