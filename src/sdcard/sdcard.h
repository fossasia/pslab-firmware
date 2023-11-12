#ifndef _SDCARD_H
#define _SDCARD_H

#include "../commands.h"

/**
 * @brief Write data to a file on the SD-card.
 *
 * @details UART transaction order:
 *             Rx[1-12]    filename
 *             Rx[1]       mode
 *     <DEBUG> Tx[1]       result_mount
 *             Tx[1]       result_open
 *             Rx[4]       data_size
 *             loop until entire file written:
 *                 Rx[512] data
 *                 Tx[1]   result_write
 *     <DEBUG>     Tx[4]   remaining_bytes
 *             Tx[4]       total_bytes
 *     <DEBUG> Tx[1]       result_close
 *     <DEBUG> Tx[1]       result_unmount
 *
 * @param filename TCHAR[12]
 *     Null-terminated 8.3 filename. 8.3 filenames are limited to at most
 *     eight characters (after any directory specifier), followed
 *     optionally by a filename extension consisting of a period and at
 *     most three further characters.
 * @param mode BYTE
 *     Access mode. A combination of:
 *     OPEN_EXISTING = 0x00
 *     CREATE_NEW = 0x04
 *     CREATE_ALWAYS = 0x08
 *     OPEN_ALWAYS = 0x10
 *     OPEN_APPEND = 0x30
 * @param data_size FSIZE_t
 *     Number of bytes to write to the file.
 * @param data TCHAR[512] [repeats until data_size written]
 *     Data to write to the SD-card.
 *
 * @return result_open FResult
 * @return result_write FResult [repeats once per write]
 * @return bytes_written FSIZE_t
 *     Number of bytes which were actually written to the SD-card. This
 *     should be equal to `data_size` if all went well.
 *
 * @return DO_NOT_BOTHER
 */
response_t SDCARD_write_file(void);

/**
 * @brief Read data from a file on the SD-card.
 *
 * @details UART transaction order:
 *             Rx[1-12]    filename
 *     <DEBUG> Tx[1]       result_mount
 *     <DEBUG> Tx[1]       result_open
 *     <DEBUG> Tx[1]       result_stat
 *             Tx[4]       data_size
 *             loop until entire file read:
 *                 Tx[512] data
 *     <DEBUG>     Tx[4]   remaining_bytes
 *             Tx[4]       total_bytes
 *     <DEBUG> Tx[1]       result_close
 *     <DEBUG> Tx[1]       result_unmount
 * 
 * @param filename TCHAR[12]
 *     Null-terminated 8.3 filename. 8.3 filenames are limited to at most
 *     eight characters (after any directory specifier), followed
 *     optionally by a filename extension consisting of a period and at
 *     most three further characters.
 * @return data_size FSIZE_t
 *         Size of the file.
 * @return data TCHAR[512] [repeats until data_size read]
 *     Data read from the SD-card.
 * @return total_bytes FSIZE_t
 *     Number of bytes which were actually read from the SD-card. This
 *     should be equal to `file_size` if all went well. If it is not
 *     equal to `file_size`, any bytes beyond `bytes_read` are invalid.
 *
 * @return DO_NOT_BOTHER
 */
response_t SDCARD_read_file(void);

#endif // _SDCARD_H
