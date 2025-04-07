/**
 * @file types.h
 * @author Alexander Bessman (alexander.bessman@gmail.com)
 * @brief Common types shared by multiple PSLab firmware modules.
 */

#ifndef PSLAB_TYPES_H
#define PSLAB_TYPES_H

/**
 * @brief Sentinel value respresenting nothing
 *
 * @details
 * Zero is unsuitable as a sentinel value because it is almost always a valid
 * value. Therefore, -1 is used to respresent the lack of something.
 */
enum None { NONE = -1 };

/**
 * @brief Common channel definition for modules with four channels
 *
 * @details
 * Most of the PSLab's instruments and underlying hardware resources have four
 * channels. It is convenient for these to use a common channel definition.
 *
 * Modules with a different number of channels than four should create their
 * own channel definition.
 */
typedef enum Channel {
    CHANNEL_NONE = NONE,
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3,
    CHANNEL_4,
    CHANNEL_NUMEL
} Channel;

/**
 * @brief Edge type of a logic level change
 */
typedef enum Edge {
    EDGE_NONE = NONE,
    EDGE_ANY,
    EDGE_FALLING,
    EDGE_RISING,
    EDGE_NUMEL
} Edge;

/**
 * @brief Callback function for interrupt service routines
 * @details
 * The callback must disable its associated interrupt when complete.
 *
 * @param channel
 */
typedef void (*InterruptCallback)(Channel channel);

#endif // PSLAB_TYPES_H
