#include "bus/uart1.h"
#include "commands.h"

const unsigned char VERSION_HW[] = "PSLab V6"; /**< Hardware version. */

response_t VERSION_SendHw(void) {
    unsigned char i;
    for (i = 0; i < sizeof (VERSION_HW) - 1; i++) UART1_Write(VERSION_HW[i]);
    UART1_Write('\n');
    return DO_NOT_BOTHER;
}
