#ifndef LOGICANALYZER_H
#define LOGICANALYZER_H

#include "../commands.h"

/**
 * @brief Capture logic level changes on pins LA1-4
 *
 * @details
 * Every time the logic level changes on an active pin, a timestamp is copied
 * to the sample buffer.
 *
 * Three types of logic level change can be captured: any, falling, or rising.
 *
 * One pin may be chosen as trigger. If so, capture only starts once an edge is
 * detected on that pin. Otherwise, capture begins immediately.
 *
 * @param uint8_t num_channels
 * Value from 1 to 4, determining how many of the LA pins to use.
 * @param uint16_t events
 * Number of edges to capture per channel.
 * @param Edge edge
 * @param Channel trigger
 *
 * @return SUCCESS
 */
response_t LA_capture(void);

/**
 * @brief Stop capture
 *
 * @details
 * Stop edge capture ahead of time, and release associated resourses.
 *
 * @return SUCCESS
 */
response_t LA_stop(void);

/**
 * @brief Get states of LA1-4
 *
 * @details
 * Low nibble of returned byte correspond to pins LA1-4, one pin per bit.
 *
 * @return SUCCESS
 */
response_t LA_get_states(void);

/**
 * @brief Get capture progress on configured channels
 *
 * @return uint8_t    num_channels
 * @return bool       capture_complete
 * @return uint16_t   progress[num_channels]
 * @return response_t SUCCESS
 */
response_t LA_get_progress(void);

/**
 * @brief Get initial states and captured timestamps
 * @details
 * Captured timestamps are sent as timedeltas between events.
 *
 * @return uint8_t    initial_states
 * @return uint32_t   scaling
 * @return uint8_t    num_channels
 * @return uint16_t   num_events[num_channels]
 * @return uint16_t   timedeltas[num_channels][num_events]
 * @return response_t SUCCESS
 */
response_t LA_get_timestamps(void);

#endif /* LOGICANALYZER_H */
