#ifndef OSCILLOSCOPE_H
#define	OSCILLOSCOPE_H

/**
 * @brief Capture samples on a single channel.
 * 
 * @description
 * This command function takes three arguments over serial:
 * 1. (uint8)  Configuration byte; The first four LSB encode channel one mapping,
 *             the fifth LSB encodes channels two, three, and four mappings, and
 *             the MSB enables the trigger.
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

#endif	/* OSCILLOSCOPE_H */
