#include "registers/system/system.h"
#include "states.h"

int main(void) {
    
    SYSTEM_Initialize();

    state_t current_state = STATE_STANDBY;

    while (1) {
        current_state = STATES_RunState(current_state);
    }

    return 1;
}
