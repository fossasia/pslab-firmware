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

/**
 * @brief Number of commands in each command group.
 * 
 * This is used to set the dimensions of the command table, and to sanitize
 * received commands.
 */
#define NUM_PRIMARY_CMDS 13
#define NUM_FLASH_CMDS 4
#define NUM_ADC_CMDS 23
#define NUM_SPI_CMDS 7
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
#define NUM_SECONDARY_CMDS_MAX NUM_COMMON_CMDS // Change if necessary.

/**
 * @brief Acknowledge bytes.
 */
#define DO_NOT_BOTHER 0 // No response sent.
#define SUCCESS 1
#define ARGUMENT_ERROR 2
#define FAILED 3

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
