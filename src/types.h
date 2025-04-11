/**
 * @file types.h
 * @author Alexander Bessman (alexander.bessman@gmail.com)
 * @brief Common types shared by multiple PSLab firmware modules.
 */

#ifndef PSLAB_TYPES_H
#define PSLAB_TYPES_H

#define TRY(x) do {       \
    status = (x);         \
    if (status != E_OK) { \
        goto error;       \
    }                     \
} while(0)

/** Function return codes. */
enum Status {
    // No error.
    E_OK,
    // General error.
    E_FAILED,
    E_NOT_IMPLEMENTED,
    // Errors related to resource acquisition.
    E_RESOURCE_NOT_INITIALIZED,
    E_RESOURCE_BUSY,
    // Errors related to memory.
    E_MEMORY_INSUFFICIENT,
    // Errors related to user input.
    E_BAD_COMMAND,
    E_BAD_SIZE,
    E_BAD_ARGSIZE,
    E_BAD_ARGUMENT,
    // Communication errors specific to UART bus.
    E_UART_RX_FRAMING,
    E_UART_RX_OVERRUN,
    E_UART_RX_PARITY,
    E_UART_RX_TIMEOUT,
    E_UART_TX_TIMEOUT,
    // Number of status codes.
    STATUS_NUMEL
};

/**
 * @brief Common channel definition for modules with four channels
 *
 * Most of the PSLab's instruments and underlying hardware resources have four
 * channels. It is convenient for these to use a common channel definition.
 *
 * Modules with a different number of channels than four should create their
 * own channel definition.
 */
typedef enum Channel {
    CHANNEL_NONE = 0,
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3,
    CHANNEL_4,
    CHANNEL_NUMEL
} Channel;

/**
 * @brief Check that channel is a value between 1-4.
 *
 *
 * @param channel
 * @return enum Status
 */
static inline enum Status CHANNEL_check(Channel const channel)
{
    return (
        channel == CHANNEL_NONE || channel >= CHANNEL_NUMEL
        ? E_BAD_ARGUMENT
        : E_OK
    );
}

/**
 * @brief Edge type of a logic level change
 */
typedef enum Edge {
    EDGE_ANY,
    EDGE_FALLING,
    EDGE_RISING,
    EDGE_NUMEL
} Edge;

/**
 * @brief Callback function for interrupt service routines
 *
 * The callback must disable its associated interrupt when complete.
 *
 * @param channel
 */
typedef void (*volatile InterruptCallback)(Channel channel);


#endif // PSLAB_TYPES_H
