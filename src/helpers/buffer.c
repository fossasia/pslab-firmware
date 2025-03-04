#include <string.h>
#include "../bus/uart/uart.h"
#include "../commands.h"
#include "../registers/system/pin_manager.h"
#include "buffer.h"

// Space in memory to store data.
uint16_t volatile __attribute__((section(".sample_buffer"), far)) BUFFER_sample_buffer[BUFFER_SIZE];

static enum Status read(
    uint16_t const start,
    uint16_t const end,
    uint8_t **const out_samples,
    uint16_t *const out_samples_size
) {
    if (start > end) {
        return E_BAD_ARGUMENT;
    }

    if (end > BUFFER_SIZE) {
        return E_BAD_ARGUMENT;
    }

    *out_samples = (uint8_t *const)(&BUFFER_sample_buffer + start);
    *out_samples_size = end - start;
    return E_OK;
}

enum Status BUFFER_read(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *const rets_size
) {
    uint16_t start = 0;
    uint16_t end = 0;

    if (args_size != (sizeof(start) + sizeof(end))) {
        return E_BAD_ARGSIZE;
    }

    start = ((uint16_t *)args)[0];
    end = ((uint16_t *)args)[1];
    return read(start, end, rets, rets_size);
}

enum Status BUFFER_read_from_channel(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *const rets_size
) {
    uint16_t counter = 0;
    uint8_t channel = 0;

    if (args_size != (sizeof(counter) + sizeof(channel))) {
        return E_BAD_ARGSIZE;
    }

    counter = ((uint16_t *)args)[0];
    channel = args[2];
    uint8_t const num_channels = 4;
    uint16_t const start = channel * (BUFFER_SIZE / num_channels);
    uint16_t const end = start + counter;
    return read(start, end, rets, rets_size);
}

/**
 * @brief Combine lower and upper parts of u32 after DMA capture.
 * @details
 *   DMA works with u16, so when capturing u32 values, the sample buffer is
 *   split into channels * 2 sections, with one DMA targetting the lower
 *   section and one targetting the upper section. After capture is complete,
 *   the u32 values are therefore fragmented between these sections. This
 *   function defragments them.
 *
 * @param channels
 * @param samples
 */
void BUFFER_defragment_dma_u32(uint16_t const channels, uint16_t const samples)
{
    for (uint8_t c = 0; c < channels; ++c) {
        uint8_t const num_channels = 4;
        uint16_t const idx_l = 2 * c * (BUFFER_SIZE / num_channels);
        uint16_t const idx_h = (2 * c + 1) * (BUFFER_SIZE / num_channels);
        uint16_t tmp = BUFFER_sample_buffer[idx_l];

        for (uint16_t s = 0; s < samples; ++s) {
            uint32_t const sample = (
                (uint32_t)tmp +
                ((uint32_t)BUFFER_sample_buffer[idx_h + s] << 16)
            );
            tmp = BUFFER_sample_buffer[idx_l + s + 1];
            ((uint32_t *)BUFFER_sample_buffer)[idx_l + s] = sample;
        }
    }
}

enum Status BUFFER_write(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *const rets_size
) {

    uint16_t start = 0;
    uint16_t count = 0;

    if (args_size != (sizeof(start) + sizeof(count))) {
        return E_BAD_ARGSIZE;
    }

    if (start + count > BUFFER_SIZE) {
        return E_BAD_ARGUMENT;
    }

    start = ((uint16_t *)args)[0];
    count = ((uint16_t *)args)[1];
    *rets = (uint8_t *const)(BUFFER_sample_buffer + start);
    *rets_size = count;
    return E_OK;
}

enum Status BUFFER_clear(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *const rets_size
) {

    uint16_t start = 0;
    uint16_t end = 0;

    if (args_size != (sizeof(start) + sizeof(end))) {
        return E_BAD_ARGSIZE;
    }

    start = ((uint16_t *)args)[0];
    end = ((uint16_t *)args)[1];

    if (start > end) {
        return E_BAD_ARGUMENT;
    }

    if (end > BUFFER_SIZE) {
        return E_BAD_ARGUMENT;
    }

    memset((void *) &BUFFER_sample_buffer[start], 0, (end - start) * sizeof(int));
    return E_OK;
}
