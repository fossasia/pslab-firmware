#ifndef LOGICANALYZER_H
#define LOGICANALYZER_H

#include "../commands.h"
#include "types.h"

/**
 * @brief Capture logic level changes on pins LA1-4
 *
 * Every time the logic level changes on an active pin, a timestamp is logged.
 * Three types of logic level change can be captured: any, falling, or rising.
 * One pin may be chosen as trigger. If so, capture only starts once an edge is
 * detected on that pin. Otherwise, capture begins immediately.
 *
 * @param n_channels
 * Value from 1 to 4, determining how many LA pins to use.
 * @param events
 * Number of edges to capture per channel.
 * @param edge
 * @param trigger
 *
 * @return status
 */
enum Status LA_capture(
    uint8_t n_channels,
    uint16_t events,
    Edge edge,
    Channel trigger
);

/**
 * @brief Fetch logged timestamps.
 *
 * @param[out] buffer
 * @param[out] n_items
 * @return enum Status
 */
enum Status LA_fetch(uint16_t **buffer, uint16_t *n_items);

/**
 * @brief Stop capture
 *
 * @details
 * Stop edge capture immediately, and release associated resourses.
 *
 * @return status
 */
enum Status LA_stop(void);

/**
 * @brief Get states of LA1-4 immediately before capture started
 *
 * Low nibble of returned byte correspond to pins LA1-4, one pin per bit.
 *
 * @param[out] initial_states
 * Valid pointer.
 *
 * @return status
 */
enum Status LA_get_initial_states(uint8_t *initial_states);

enum Status LA_cmd_capture(
    uint8_t **args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);
enum Status LA_cmd_fetch(
    uint8_t **args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);
enum Status LA_cmd_stop(
    uint8_t **args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);
enum Status LA_cmd_get_initial_states(
    uint8_t **args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

#endif /* LOGICANALYZER_H */
