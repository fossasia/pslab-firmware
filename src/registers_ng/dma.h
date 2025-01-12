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
void DMA_reset(Channel channel);

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
void DMA_setup(
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
void DMA_start(Channel channel);

/**
 * @brief Start transaction on DMA0
 * @details
 * Prefer `DMA_start` over `DMAx_start_fast`. Use the latter only in
 * timing-criticial sections.
 */
static inline void DMA0_start_fast(void)
{
    extern uint16_t volatile DMA0CON;
    DMA0CON |= (1 << 15);
}

/**
 * @brief Start transaction on DMA1
 */
static inline void DMA1_start_fast(void)
{
    extern uint16_t volatile DMA1CON;
    DMA1CON |= (1 << 15);
}

/**
 * @brief Start transaction on DMA2
 */
static inline void DMA2_start_fast(void)
{
    extern uint16_t volatile DMA2CON;
    DMA2CON |= (1 << 15);
}

/**
 * @brief Start transaction on DMA3
 */
static inline void DMA3_start_fast(void)
{
    extern uint16_t volatile DMA3CON;
    DMA3CON |= (1 << 15);
}

/**
 * @brief Enable interrupt on DMA channel
 * @details
 * The interrupt occurs when data transaction is complete. When the interrupt
 * is generated, `callback` is called.
 *
 * @param channel
 * @param callback
 */
void DMA_interrupt_enable(Channel channel, InterruptCallback callback);

/**
 * @brief Get DMA transaction progress
 *
 * @param channel
 * @return uint16_t
 */
uint16_t DMA_get_progress(Channel channel);

#endif // DMA_H
