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
 * @brief Acknowledge bytes.
 */
#define DO_NOT_BOTHER 0 // No response sent.
#define SUCCESS 1
#define ARGUMENT_ERROR 2
#define FAILED 3

/** Function return codes. */
enum Status {
    E_OK,
    E_FAILED,
    E_HOST_READ,
    E_HOST_RX_OVERRUN,
    E_BAD_COMMAND,
    E_BAD_SIZE,
    E_BAD_ARGSIZE,
    E_BAD_ARGUMENT,
    E_UART_RX_FRAMING,
    E_UART_RX_OVERRUN,
    E_UART_RX_PARITY,
    E_UART_RX_TIMEOUT,
    STATUS_NUMEL
};

/**
 * @brief Command functions are the main entry points for user actions.
 *
 * @param[in] args
 *      Byte-array containing function arguments. The function may parse its
 *      arguments from the array.
 * @param args_size
 *      Number of bytes in `args`.
 * @param[out] rets
 *      Pointer to an unallocated byte-array containing function return
 *      values.
 * @param[out] rets_size
 *      Pointer to uint16_t holding the number of bytes in `rets`.
 * @return enum Status
 *      Exit code.
 */
typedef enum Status CmdFunc(
    uint8_t const *args,
    uint16_t args_size,
    uint8_t **rets,
    uint16_t *rets_size
);

CmdFunc *COMMAND_get_func(uint16_t code);

typedef unsigned char command_t; /**< Type for command bytes. */
typedef unsigned char response_t; /**< Type for acknowledge bytes. */

/**
 * @brief Array containing the number of commands in each command group.
 */
extern command_t num_secondary_cmds[NUM_PRIMARY_CMDS + 1];

typedef response_t command_func_t(void); /**< Type for command functions. */

/**
 * @brief 2D array containing command functions.
 */
extern command_func_t* const cmd_table[NUM_PRIMARY_CMDS + 1][NUM_SECONDARY_CMDS_MAX + 1];

#ifdef	__cplusplus
}
#endif

#endif	/* COMMANDS_H */
