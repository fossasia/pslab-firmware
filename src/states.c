#include <stdlib.h>

#include "registers/system/system.h"
#include "bus/uart/uart.h"
#include "registers/system/watchdog.h"
#include "commands.h"
#include "states.h"

/**
 * @brief Exit application and drop to bootloader.
 * @return noreturn
 */
static _Noreturn state_t Exit(void) {
    exit(EXIT_SUCCESS);
}

/**
 * @brief Wait for incoming serial traffic.
 * @return STATE_STANDBY or STATE_RUNCOMMAND
 */
static state_t Standby(void) {
    if (UART_IsRxReady(U1SELECT)) {
        return STATE_RUNCOMMAND;
    } else {
        WATCHDOG_TimerClear();
        return STATE_STANDBY;
    }
}

/**
 * @brief Receive commands bytes, run command, and send response.
 * @return STATE_STANDBY
 */
static state_t RunCommand(void) {
    command_t primary_cmd = UART1_Read();
    command_t secondary_cmd = UART1_Read();

    if (((primary_cmd << 8) | secondary_cmd) == 0xDEAD) return STATE_EXIT;

    // Sanitize input.
    if (primary_cmd > NUM_PRIMARY_CMDS) return STATE_STANDBY;
    if (secondary_cmd > num_secondary_cmds[primary_cmd]) return STATE_STANDBY;

    response_t response = cmd_table[primary_cmd][secondary_cmd]();

    if (response) UART1_Write(response);

    return STATE_STANDBY;
}

state_func_t* const state_table[NUM_STATES] = {
    Exit,
    Standby,
    RunCommand,
};

state_t STATES_RunState(state_t current_state) {
    return state_table[current_state]();
};
