#ifndef OSCILLOSCOPE_H
#define	OSCILLOSCOPE_H

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
response_t OSCILLOSCOPE_CaptureOne(void);

/**
 * @brief Capture samples on two channels simultaneously.
 */
response_t OSCILLOSCOPE_CaptureTwo(void);

/**
 * @brief Capture samples on three channels simultaneously.
 *
 * @description
 * Since the MCU only supports 1, 2, or 4 simultaneous channels, this is
 * accomplished by sampling four channels simultaneously and discarding the
 * samples from the fourth. This function therefore has the same sample rate
 * as OSCILLOSCOPE_CaptureFour, but greater sample depth.
 */
response_t OSCILLOSCOPE_CaptureThree(void);

/**
 * @brief Capture samples on four channels simultaneously.
 */
response_t OSCILLOSCOPE_CaptureFour(void);

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
response_t OSCILLOSCOPE_GetCaptureStatus(void);

/**
 * @brief
 * Select trigger channel and trigger level.
 *
 * @description
 * This command function takes two arguments over serial.
 * 1. (uint8)  Configuration byte:
 *             | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0  |
 *             |   Prescaler   | - | - |  CHSEL |
 *             Prescaler: The trigger times out (i.e. capture starts) after:
 *                        timeout = DELAY * (50000 / (DELAY >> prescaler)) / 8 us
 *                        If DELAY >> prescaler == 0 trigger does not time out.
 *             CHSEL: Trigger channel select.
 *                    0: Trigger on CH0SA,
 *                    1: Trigger on CH123SA[0],
 *                    2: Trigger on CH123SA[1],
 *                    3. Trigger on CH123SA[2],
 *                    Note that it is not necessary to sample the trigger
 *                    channel.
 * 2. (uint16) Trigger voltage.
 * It returns nothing over serial.
 *
 * @return SUCCESS
 */
response_t OSCILLOSCOPE_ConfigureTrigger(void);

/**
 * @brief
 * Set gain.
 *
 * @description
 * This command function takes two arguments over serial:
 * 1. (uint8) Channel:
 *            1: CH1,
 *            2: CH2,
 * 2. (uint8) Gain:
 *            0: 1,
 *            1: 2,
 *            2: 4,
 *            3: 5,
 *            4: 8,
 *            5: 10,
 *            6: 16,
 *            7: 32,
 * It returns nothing over serial.
 *
 * @return SUCCESS
 */
response_t OSCILLOSCOPE_SetPGAGain(void);

#endif	/* OSCILLOSCOPE_H */
