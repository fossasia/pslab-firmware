#include "registers/system/system.h"
#include "bus/uart1.h"
#include "helpers/light.h"
#include "states.h"

int main(void) {
    SYSTEM_Initialize();
    LIGHT_RGB(0, 20, 0);
    ClearBuffer();

    state_t current_state = STATE_STANDBY;

    while (1) {
        current_state = STATES_RunState(current_state);
    }

    return 1;
}
