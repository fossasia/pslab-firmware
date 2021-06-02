#include "mcc_generated_files/system.h"
#include "mcc_generated_files/fatfs/fatfs_demo.h"
#include "mcc_generated_files/uart1.h"

#include "states.h"

/**
 * @brief: Discard incoming serial traffic.
 */
void ClearBuffer(void) {
    while (UART1_IsRxReady()) UART1_Read();
}

int main(void) {
    SYSTEM_Initialize();
    ClearBuffer();

    state_t current_state = STATE_STANDBY;

    while (1) {
        current_state = STATES_RunState(current_state);
    }

    return 1;
}
