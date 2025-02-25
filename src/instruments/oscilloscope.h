#ifndef OSCILLOSCOPE_H
#define	OSCILLOSCOPE_H

#include "../commands.h"

enum Status OSCILLOSCOPE_fetch_samples(
    uint8_t const *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Capture samples on a single channel.
 *
 * @description
 * This command function takes three arguments over serial:
 * 1. (uint8)  Configuration byte:
 *             | 7  | 6 | 5 | 4       | 3 | 2 | 1 | 0 |
 *             | TE | - | - | CH123SA |     CH0SA     |
 *             TE: Trigger enable
 *             CH123SA: Second, third, and fourth channels input map:
 *                      0: CH2, CH3, MIC
 *                      1: CH1, CH2, CAP
 *             CH0SA: First channel input map:
 *                    3: CH1,
 *                    0: CH2,
 *                    1: CH3,
 *                    2: MIC,
 *                    7: RES,
 *                    5: CAP,
 *                    8: VOL,
 * 2. (uint16) The number of samples to capture.
 * 3. (uint16) The time to wait between samples in instruction cycles.
 * It returns nothing over serial.
 * It sends an acknowledge byte (SUCCESS).
 *
 * @return SUCCESS
 */
enum Status OSCILLOSCOPE_capture_one(
    uint8_t const *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Capture samples on two channels simultaneously.
 */
enum Status OSCILLOSCOPE_capture_two(
    uint8_t const *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Capture samples on three channels simultaneously.
 *
 * @description
 * Since the MCU only supports 1, 2, or 4 simultaneous channels, this is
 * accomplished by sampling four channels simultaneously and discarding the
 * samples from the fourth. This function therefore has the same sample rate
 * as OSCILLOSCOPE_CaptureFour, but greater sample depth.
 */
enum Status OSCILLOSCOPE_capture_three(
    uint8_t const *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Capture samples on four channels simultaneously.
 */
enum Status OSCILLOSCOPE_capture_four(
    uint8_t const *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Capture samples on one channel as fast as possible.
 *
 * @description
 * Direct memory access moves samples from ADC to RAM buffer as soon as
 * conversion is complete. Allows for faster sample rate than using ADC
 * interrupt, at the cost of only supporting a single channel and no trigger.
 * This command function takes three arguments over serial:
 * 1. (uint8)  Configuration byte:
 *             | 7   | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 *             | RES | - | - | - |     CH0SA     |
 *             RES: Sample resolution:
 *                  0: 10-bit,
 *                  1: 12-bit,
 *             CH0SA: First channel input map:
 *                    3: CH1,
 *                    0: CH2,
 *                    1: CH3,
 *                    2: MIC,
 *                    7: RES,
 *                    5: CAP,
 *                    8: VOL,
 * 2. (uint16) The number of samples to capture.
 * 3. (uint16) The time to wait between samples in instruction cycles.
 * It returns nothing over serial.
 * It sends an acknowledge byte (SUCCESS).
 */
enum Status OSCILLOSCOPE_capture_dma(
    uint8_t const *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief
 * Send capture progress.
 *
 * @description
 * This command function takes no arguments over serial.
 * It returns two values over serial:
 * 1. (bool) True if all requested samples have been captured.
 * 2. (uint16) Number of captured samples.
 * It sends an acknowledge byte (SUCCESS).
 *
 * @return SUCCESS
 */
enum Status OSCILLOSCOPE_get_capture_status(
    uint8_t *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief
 * Select trigger channel and trigger level.
 *
 * @description
 * This command function takes two arguments over serial.
 * 1. (uint8)  Configuration byte:
 *             | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0  |
 *             |   Prescaler   |      CHSEL     |
 *             Prescaler: The trigger times out (i.e. capture starts) after:
 *                        timeout = DELAY * (50000 / (DELAY >> prescaler)) / 8 us
 *                        If DELAY >> prescaler == 0 trigger does not time out.
 *             CHSEL: Trigger channel select.
 *                    b0000: Disable trigger,
 *                    b0001: Trigger on CH0SA,
 *                    b0010: Trigger on CH123SA[0],
 *                    b0100: Trigger on CH123SA[1],
 *                    b1000. Trigger on CH123SA[2],
 *                    It is not necessary to sample the trigger channel, but
 *                    the trigger channel must still be converted which affects
 *                    the sample rate.
 * 2. (uint16) Trigger voltage.
 * It returns nothing over serial.
 *
 * @return SUCCESS
 */
enum Status OSCILLOSCOPE_configure_trigger(
    uint8_t const *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

/**
 * @brief Set gain on CH1 or CH2.
 * @param channel uint8_t
 *                1: CH1
 *                2: CH2
 * @param gain uint8_t
 *             0: 1
 *             1: 2
 *             2: 4
 *             3: 5
 *             4: 8
 *             5: 10
 *             6: 16
 *             7: 32
 *
 * @return FAILED if channel is not 1 or 2.
 *         FAILED if gain is greater than 7.
 *         FAILED if SPI transaction fails.
 *         SUCCESS otherwise.
 */
enum Status OSCILLOSCOPE_set_pga_gain(
    uint8_t const *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

#endif	/* OSCILLOSCOPE_H */
