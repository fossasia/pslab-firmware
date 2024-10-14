#include <stdbool.h>

#include "../bus/uart/uart.h"
#include "../commands.h"
#include "debug.h"

response_t DEBUG_is_enabled(void) {
    #ifdef PSLAB_DEBUG
        bool const IS_DEBUG_ENABLED = true;
    #else
        bool const IS_DEBUG_ENABLED = false;
    #endif // PSLAB_DEBUG

    UART2_Write(IS_DEBUG_ENABLED);
    return DO_NOT_BOTHER;
}
