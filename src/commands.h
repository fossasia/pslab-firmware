/**
 * @file commands.h
 * @brief Command handling.
 *
 */

#ifndef COMMANDS_H
#define	COMMANDS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief Number of commands in each command group.
 *
 * This is used to set the dimensions of the command table, and to sanitize
 * received commands.
 */
#define NUM_PRIMARY_CMDS 14
#define NUM_FLASH_CMDS 4
#define NUM_ADC_CMDS 23
#define NUM_SPI_CMDS 11
#define NUM_I2C_CMDS 16
#define NUM_UART2_CMDS 8
#define NUM_DAC_CMDS 3
#define NUM_WAVEGEN_CMDS 19
#define NUM_DOUT_CMDS 1
#define NUM_DIN_CMDS 2
#define NUM_TIMING_CMDS 17
#define NUM_COMMON_CMDS 27
#define NUM_PASSTHRU_CMDS 1
#define NUM_SENSOR_CMDS 9
#define NUM_SDCARD_CMDS 3
#define NUM_SECONDARY_CMDS_MAX NUM_COMMON_CMDS // Change if necessary.

/**
 * @brief Command functions provide the remote API available to connected hosts.
 *
 * Command functions typically wrap a corresponding function from the C API.
 *
 * @param[in] args
 *   Pointer to a byte-array containing the function arguments.
 *   - The array's size is provided by @c args_size.
 *   - On call, *args is guaranteed to point to valid, malloc'd memory if
 *     @c args_size is non-zero.
 *   - On call, *args is guaranteed to be a null pointer if @c args_size is
 *     zero.
 *   - On return, the caller will call free on *args.
 *   - The function may claim ownership of the input data by setting
 *     *args = NULL, thereby preventing the caller from deallocating it.
 *
 * @param[in] args_size
 *   Number of bytes in @c args.
 *
 * @param[out] rets
 *   Pointer to a byte-array that will hold the function's return values.
 *   - On call, *rets is guaranteed to be a null pointer.
 *   - On return, ownership of output data passes to caller.
 *
 * @param[out] rets_size
 *   Pointer to a uint16_t that will be set to the number of bytes in the
 *   @c rets array.
 *   - On call, the pointed to value is guaranteed to be zero.
 *
 * @return enum Status
 *   Exit code indicating the success or failure of the function.
 */
typedef enum Status (*CmdFunc)(
    uint8_t **args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

CmdFunc COMMAND_get_func(uint16_t cmd);

#ifdef	__cplusplus
}
#endif

#endif	/* COMMANDS_H */
