#include "registers/system/system.h"
#include "states.h"

__attribute__((noreturn)) int main(void) {

    SYSTEM_Initialize();

    EState current_state = STATE_STANDBY;

    while (1) {
        current_state = STATES_run_state(current_state);
    }
}
