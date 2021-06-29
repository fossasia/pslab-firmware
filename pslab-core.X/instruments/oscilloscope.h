#ifndef OSCILLOSCOPE_H
#define	OSCILLOSCOPE_H

/**
 * @brief Capture samples on a single channel.
 * 
 * @description
 * This command function takes three arguments over serial:
 * 1. (uint8) The pin to sample
 * 2. (uint16) The number of samples to capture
 * 3. (uint16) The time to wait between samples in instruction cycles
 * It returns nothing over serial.
 * It sends an acknowledge byte (SUCCESS).
 */
response_t OSCILLOSCOPE_CaptureOne(void);

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
