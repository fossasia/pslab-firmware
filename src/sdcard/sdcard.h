#ifndef _SDCARD_H
#define _SDCARD_H

#include "../commands.h"

/**
 * @brief Write data to a file on the SD-card.
 *
 * @param filename char[256]
 *        Null-terminated filename, at most 256 characters long (including
 *        null-terminator).
 * @param mode uint8_t
 *        Access mode. A combination of:
 *        OPEN_EXISTING = 0x00
 *        CREATE_NEW = 0x04
 *        CREATE_ALWAYS = 0x08
 *        OPEN_ALWAYS = 0x10
 *        OPEN_APPEND = 0x30
 * @param data_size uint32_t
 *        Number of bytes to write to the file.
 * @param data uint8_t[data_size]
 *        Data to write to the SD-card.
 *
 * @return bytes_written uint16_t
 *         Number of bytes which were actually written to the SD-card. This
 *         should be equal to `data_size` if all went well.
 * @return result uint8_t
 *         FatFS result code.
 * @return DO_NOT_BOTHER
 */
response_t SDCARD_write_file(void);

/**
 * @brief Read data from a file on the SD-card.
 *
 * @param filename char[256]
 *        Null-terminated filename, at most 256 characters long (including
 *        null-terminator).
 *
 * @return file_size uint32_t
 *         Size of the file.
 * @return data uint8_t[file_size]
 *         Data read from the SD-card.
 * @return bytes_read uint32_t
 *         Number of bytes which were actually read from the SD-card. This
 *         should be equal to `file_size` if all went well. If it is not
 *         equal to `file_size`, any bytes beyond `bytes_read` are invalid.
 * @return result uint8_t
 *         FatFS result code.
 * @return DO_NOT_BOTHER
 */
response_t SDCARD_read_file(void);

#endif // _SDCARD_H
