#ifndef _SDCARD_H
#define _SDCARD_H

#include <stdint.h>

#include "../commands.h"

 /**
  * @brief Mount SD-card.
  *
  * @return FRESULT result
  * @return enum Status status
  */
enum Status SDCARD_mount(
   uint8_t args[],
   uint16_t args_size,
   uint8_t *rets[],
   uint16_t *rets_size
);

/**
 * @brief Unmount SD-card.
 *
  * @return FRESULT result
  * @return enum Status status
 */
enum Status SDCARD_unmount(
   uint8_t args[],
   uint16_t args_size,
   uint8_t *rets[],
   uint16_t *rets_size
);

/**
 * @brief Get info about file on SD-card.
 *
 * @param uint8_t fn_size
 *   1 < n <= 13 including termination.
 * @param char *fn
 *   Null-terminated string.
 *
 * @return FILINFO info
 * @return FRESULT result
 * @return enum Status status
 */
enum Status SDCARD_stat(
    uint8_t args[],
    uint16_t args_size,
    uint8_t *rets[],
    uint16_t *rets_size
);

/**
 * @brief Open file for reading or writing.
 *
 * @param uint8_t fn_size
 *   1 < n <= 13 including termination.
 * @param uint8_t mode
 *   READ           0x01
 *   WRITE          0x02
 *   OPEN_EXISTING  0x00
 *   CREATE_NEW     0x04
 *   CREATE_ALWAYS  0x08
 *   OPEN_ALWAYS    0x10
 *   OPEN_APPEND    0x30
 * @param char *fn
 *   Null-terminated string.
 *
 * @return FRESULT result
 * @return enum Status status
 */
enum Status SDCARD_open(
    uint8_t args[],
    uint16_t args_size,
    uint8_t *rets[],
    uint16_t *rets_size
);

/**
 * @brief Close file.
 *
 * @return FRESULT result
 * @return enum Status status
 */
enum Status SDCARD_close(
    uint8_t args[],
    uint16_t args_size,
    uint8_t *rets[],
    uint16_t *rets_size
);

/**
 * @brief Write a block of data to file.
 *
 * @details
 *   This function can be called repeatedly to write sequential data.
 *
 * @param uint16_t data_size
 *   0 < n <= 512
 * @param uint8_t *data
 *
 * @return FRESULT result
 * @return uint16_t bytes_written
 * @return enum Status status
 */
enum Status SDCARD_write(
    uint8_t args[],
    uint16_t args_size,
    uint8_t *rets[],
    uint16_t *rets_size
);

/**
 * @brief Read data from file.
 *
 * @details
 *   This function can be called repeatedly to read sequential data.
 *
 * @param uint16_t data_size
 *   0 < n <= 512
 *
 * @return FRESULT result
 * @return uint16_t bytes_read
 * @return uint8_t *data
 * @return enum Status status
 */
enum Status SDCARD_read(
    uint8_t args[],
    uint16_t args_size,
    uint8_t *rets[],
    uint16_t *rets_size
);

#endif // _SDCARD_H
