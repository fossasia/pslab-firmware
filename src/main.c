#include "registers/system/system.h"
#include "states.h"
#include "sdcard/sdcard.h"

int main(void) {
    
    SYSTEM_Initialize();

    state_t current_state = SDCARD_standalone_check() == SUCCESS ? STATE_STANDALONE : STATE_STANDBY;

    while (1) {
        current_state = STATES_RunState(current_state);
    }

    return 1;
}
