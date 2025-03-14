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
#define NUM_SENSOR_CMDS 11
#define NUM_SDCARD_CMDS 3
#define NUM_SECONDARY_CMDS_MAX NUM_COMMON_CMDS // Change if necessary.

/** Function return codes. */
enum Status {
    // No error.
    E_OK,
    // General error.
    E_FAILED,
    E_NOT_IMPLEMENTED,
    // Errors related to resource acquisition.
    E_RESOURCE_NOT_INITIALIZED,
    E_RESOURCE_BUSY,
    // Errors related to user input->
    E_BAD_COMMAND,
    E_BAD_SIZE,
    E_BAD_ARGSIZE,
    E_BAD_ARGUMENT,
    // Communication errors specific to UART bus.
    E_UART_RX_FRAMING,
    E_UART_RX_OVERRUN,
    E_UART_RX_PARITY,
    E_UART_RX_TIMEOUT,
    E_UART_TX_TIMEOUT,
    // Number of status codes.
    STATUS_NUMEL
};

/**
 * @brief Command functions provide the public-facing API available to hosts.
 *
 * @param[in] uint8_t args[]
 *      Byte-array containing function arguments. The function may parse its
 *      arguments from the array. The function may reuse this array to output
 *      up to `arg_size` bytes.
 * @param uint16_t args_size
 *      Number of bytes in `args`.
 * @param[out] uint8_t **rets
 *      Pointer to an unallocated byte-array containing function return
 *      values. The function does not need to set this if has nothing to
 *      return. The function may set `*rets = args` if
 *      `*rets_size <= args_size`.
 * @param[out] uint16_t *rets_size
 *      Pointer to uint16_t holding the number of bytes in `*rets`. The
 *      function does not need to set this if it has nothing to return.
 * @return enum Status
 *      Exit code.
 */
typedef enum Status (*CmdFunc)(
    uint8_t args[*],
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

CmdFunc COMMAND_get_func(uint16_t cmd);

#ifdef	__cplusplus
}
#endif

#endif	/* COMMANDS_H */
