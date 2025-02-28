#include <stdbool.h>

#include "registers/system/system.h"
#include "helpers/light.h"
#include "transport/host.h"
#include "registers/system/watchdog.h"
#include "transport/packet_handler.h"
#include "commands.h"
#include "states.h"

/**
 * @brief Wait for incoming serial traffic.
 * @return STATE_STANDBY or STATE_RUNCOMMAND
 */
EState standby(void) {
    bool ready = false;
    HOST_rx_ready(&ready);

    if (ready) {
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
EState run_command(void)
{
    enum Status status;

    if ( (status = PACKET_exchange()) ) {
        // TODO: Add error handling.
        LIGHT_RGB(20, 0, 0);
    }

    return STATE_STANDBY;
}

StateFunc const state_table[NUM_STATES] = {
    standby,
    run_command,
};

EState STATES_run_state(EState current_state) {
    return state_table[current_state]();
};
