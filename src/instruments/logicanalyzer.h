#ifndef LOGICANALYZER_H
#define	LOGICANALYZER_H

#include "../commands.h"

/**
 * @brief Capture logic level changes on a single channel.
 *
 * @description
 * This routine will log timer values for the changes occurred in a pin defined
 * by the configuration byte.
 *
 * If a trigger is enabled, this will use an external interrupt service routine
 * attached to a pin defined by the trigger byte. Once the logic level is observed
 * to be the same as in EDGE bit, IC1 and IC2 modules will start the combined
 * timer (32-bits) and pass them to BUFFER_sample_buffer using first two DMA channels.
 *
 * If no trigger is set, the timers will be started right away once the function
 * is called and logic level changes will logged referring to IC1 and IC2 capture
 * timers in BUFFER_sample_buffer using the same DMA channels.
 *
 * This command function takes three arguments over serial:
 * 1. (uint16) number of data points to capture
 * 2. (uint8) trigger conditions:
 *            | 7 | 6 | 5 | 4 | 3 | 2 |  1   | 0  |
 *            |  TRIGGER PIN  | X | X | EDGE | EN |
 *            PIN: Digital pin from map:
 *              0: LA1,
 *              1: LA2,
 *              2: LA3,
 *              3: LA4,
 *              4: COMP4,
 *              5: SPI_CS,
 *              6: FREQ
 *            EDGE: Falling (1) or Rising (0) edge trigger
 *            EN: Enable trigger
 * 3. (uint8) channel and mode configurations:
 *            | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 *            |  DIGITAL PIN  |  TRANSITIONS  |
 *            CHANNEL: Digital pin from the same map as above `PIN` maps
 *            MODE: Logic transitions to capture. Refer `IC_PARAMS_CAPTURE_MODE`
 *
 * The data captured from this method should be read using <TODO METHOD NAME>
 *
 * It returns nothing over serial.
 * It sends an acknowledge byte (SUCCESS).
 *
 * @return SUCCESS
 */
enum Status LOGICANALYZER_one_channel(
    uint8_t **args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Capture logic level changes in one channel
 *
 * @description
 * This routine is much similar to `LOGICANALYZER_OneChannel` except the trigger
 * is handled differently. If trigger is enabled, IC4 pin will be connected to a
 * pin defined by the trigger byte and an ISR will be activated for IC4 module.
 * This will trigger an interrupt when a logic change defined by the trigger byte
 * is observed at the pin attached to IC4. At the same time, IC1 and IC2 start
 * their timers and log timer values to BUFFER_sample_buffer using DMA0 and DMA1 channels
 *
 * This command function takes three arguments over serial:
 * 1. (uint16) number of data points to capture
 * 2. (uint8) channel and mode configuration:
 *            | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 *            |  DIGITAL PIN  |  TRANSITIONS  |
 * 3. (uint8) trigger conditions:
 *            | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 *            |    TRIG PIN   | X | TRIG MODE |
 *            PIN: Digital pin from map:
 *              0: LA1,
 *              1: LA2,
 *              2: LA3,
 *              3: LA4,
 *              4: COMP4,
 *              5: SPI_CS,
 *              6: FREQ
 *
 * It returns nothing over serial.
 * It sends an acknowledge byte (SUCCESS).
 *
 * @return SUCCESS
 */
enum Status LOGICANALYZER_one_channel_alt(
    uint8_t **args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Capture logic level changes in two channels
 *
 * @description
 * This routine will log timer values on logic level changes in two different
 * pins defined by the channel byte.
 *
 * If a trigger is enabled, this will use an external interrupt service routine
 * attached to a pin defined by trigger byte. Once the logic level is observed
 * to be the same as in EDGE bit, All IC modules will start the combined
 * timers (32-bits) and pass them to BUFFER_sample_buffer using all four DMA channels.
 *
 * If no trigger is enabled, the timers will be started right away once the
 * function is called and logic level changes will logged referring to IC1 and
 * IC3 combined capture timers in BUFFER_sample_buffer using the same DMA channels.
 *
 * This command function takes four arguments over serial:
 * 1. (uint16) number of data points to capture
 * 2. (uint8) trigger conditions:
 *            | 7 | 6 | 5 | 4 | 3 | 2 |   1  | 0  |
 *            |    TRIG PIN   | X | X | EDGE | EN |
 *            PIN: Digital pin from map:
 *              0: LA1,
 *              1: LA2,
 *              2: LA3,
 *              3: LA4,
 *              4: COMP4,
 *              5: SPI_CS,
 *              6: FREQ
 * 3. (uint8) mode configurations:
 *            | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 *            |  PIN 2 MODES  |  PIN 1 MODES  |
 * 4. (uint8) channel configurations:
 *            | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 *            | PIN 2 CHANNEL | PIN 1 CHANNEL |
 *
 * It returns nothing over serial.
 * It sends an acknowledge byte (SUCCESS).
 *
 * @return SUCCESS
 */
enum Status LOGICANALYZER_two_channel(
    uint8_t **args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Capture logic level changes in three channels
 *
 * @description
 * This routine will log timer values on logic level changes in LA1, LA2 and LA3
 * pins. These pins are fixed for this mode of operation.
 *
 * If a trigger is enabled, this will use IC4 module as a trigger source to start
 * IC1, 2 and 3 timers. Once the logic level change as defined in trigger byte is
 * observed at IC4, it will generate an interrupt that will turn on all IC1,2,3
 * timers which will capture logic level changes defined in configuration integer.
 * IC modules will have 16-bit timers and the value is logged to BUFFER_sample_buffer using
 * three DMA channels.
 *
 * If no trigger is enabled, the timers will be started right away once the
 * function is called and logic level changes will logged referring to IC1, 2
 * and IC3 combined capture timers in BUFFER_sample_buffer using the same DMA channels.
 *
 * This command function takes three arguments over serial:
 * 1. (uint16) number of data points to capture
 * 2. (uint16) configurations:
 *            | F | E | D | C | B | A | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 *            | X | X | X | X |   LA3 EDGES   |   LA2 EDGES   |   LA1 EDGES   |
 * 3. (uint8) trigger conditions:
 *            | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 *            |    TRIG PIN   | X | TRIG MODE |
 *            PIN: Trig pin from map:
 *              0: LA1,
 *              1: LA2,
 *              2: LA3,
 *              3: LA4,
 *              4: COMP4,
 *              5: SPI_CS,
 *              6: FREQ
 *
 * It returns nothing over serial.
 * It sends an acknowledge byte (SUCCESS).
 *
 * @return SUCCESS
 */
enum Status LOGICANALYZER_three_channel(
    uint8_t **args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Capture logic level changes in four channels
 *
 * @description
 * This routine will log timer values on logic level changes in LA1, LA2, LA3
 * and LA4 pins. These pins are fixed for this mode of operation.
 *
 * If a trigger is enabled, this will use CN (Change Notifier) Interrupt Service
 * Routine to trigger the IC timers to log logic level changes. If any of the 4
 * pins observe a logic change as defined in trigger byte, the timers will get
 * started logging logic level changes in all 4 pins defined in the configuration
 * integer. IC modules will have 16-bit timers and the value is logged to BUFFER_sample_buffer
 * using all four DMA channels.
 *
 * If no trigger is enabled, the timers will be started right away once the
 * function is called and logic level changes will logged referring to IC1, IC2,
 * IC3 and IC4 capture timers in BUFFER_sample_buffer using the same DMA channels.
 *
 * This command function takes four arguments over serial:
 * 1. (uint16) number of data points to capture
 * 2. (uint16) mode configurations:
 *            | F | E | D | C | B | A | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 *            |   LA4 EDGES   |   LA3 EDGES   |   LA2 EDGES   |   LA1 EDGES   |
 * 3. (uint8) pre-scaler settings for timer 2
 * 4. (uint8) trigger settings:
 *            | 7 | 6 |  5  |  4  |  3  |  2  |  1   | 0  |
 *            | X | X | LA4 | LA3 | LA2 | LA1 | EDGE | EN |
 *
 * It returns nothing over serial.
 * It sends an acknowledge byte (SUCCESS).
 *
 * @return SUCCESS
 */
enum Status LOGICANALYZER_four_channel(
    uint8_t **args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Stop capture modules
 *
 * @description
 * This command function does not take any arguments over serial. It will stop
 * all the input capture modules.
 *
 * It returns nothing over serial.
 * It sends an acknowledge byte (SUCCESS).
 *
 * @return SUCCESS
 */
enum Status LOGICANALYZER_stop(
    uint8_t **args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

// Getters and setters

void SetLA_TRIGGER_CHANNEL(uint8_t);
uint8_t GetLA_TRIGGER_CHANNEL(void);

void SetLA_TRIGGER_STATE(uint8_t);
uint8_t GetLA_TRIGGER_STATE(void);

#endif	/* LOGICANALYZER_H */
