#include "registers/system/system.h"
#include "bus/uart/uart.h"
#include "registers/system/watchdog.h"
#include "commands.h"
#include "states.h"

/**
 * @brief Wait for incoming serial traffic.
 * @return STATE_STANDBY or STATE_RUNCOMMAND
 */
state_t Standby(void) {
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
state_t RunCommand(void) {
    command_t primary_cmd = UART2_Read();
    command_t secondary_cmd = UART2_Read();

    // Sanitize input.
    if (primary_cmd > NUM_PRIMARY_CMDS) return STATE_STANDBY;
    if (secondary_cmd > num_secondary_cmds[primary_cmd]) return STATE_STANDBY;

    response_t response = cmd_table[primary_cmd][secondary_cmd]();

    if (response) UART2_Write(response);

    return STATE_STANDBY;
}

state_func_t* const state_table[NUM_STATES] = {
    Standby,
    RunCommand,
};

state_t STATES_RunState(state_t current_state) {
    return state_table[current_state]();
};
