#ifndef _SDCARD_H
#define _SDCARD_H

#include "../commands.h"

/**
 * @brief Write data to a file on the SD-card.
 *
 * @param filename_length uint8_t
 *        Number of chars in the name of the file to write to, including the
 *        null terminator.
 * @param filename char[filename_length]
 *        Null-terminated filename.
 * @param mode uint8_t
 *        Access mode. A combination of:
 *        OPEN_EXISTING = 0x00
 *        CREATE_NEW = 0x04
 *        CREATE_ALWAYS = 0x08
 *        OPEN_ALWAYS = 0x10
 *        OPEN_APPEND = 0x30
 * @param data_size uint16_t
 *        Number of bytes to write to the file.
 *        NOTE: In the current implementation this data is first collected in
 *        a buffer on the stack before being written to the SD-card. If you
 *        try to write too much data at once, the stack will overflow. Keep it
 *        under 1 kB to be safe.
 * @param data uint8_t[data_size]
 *        Data to write to the SD-card.
 *
 * @return result uint8_t
 *         FatFS result code.
 * @return bytes_written uint16_t
 *         Number of bytes which were actually written to the SD-card.
 * @return FAILED if filename is not null-terminated.
 *         FAILED if result is non-zero.
 *         SUCCESS otherwise.
 */
response_t SDCARD_write_file(void);

/**
 * @brief Read data from a file on the SD-card.
 *
 * @param filename_length uint8_t
 *        Number of chars in the name of the file to write to, including the
 *        null terminator.
 * @param filename char[filename_length]
 *        Null-terminated filename.
 * @param data_size uint16_t
 *        Number of bytes to read from the file.
 *        NOTE: In the current implementation this data is first collected in
 *        a buffer on the stack before being sent over serial. If you try to
 *        read too much data at once, the stack will overflow. Keep it under
 *        1 kB to be safe.
 *
 * @return result uint8_t
 *         FatFS result code.
 * @return bytes_read uint16_t
 *         Number of bytes which were actually read from the SD-card.
 * @return data uint8_t[bytes_read]
 *         Data read from the SD-card.
 * @return FAILED if filename is not null-terminated.
 *         FAILED if result is non-zero.
 *         SUCCESS otherwise.
 */
response_t SDCARD_read_file(void);

#endif // _SDCARD_H
