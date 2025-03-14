/**
 * @file blink.c
 *
 * LED blinking exercise for FOSSASIA Summit 2025 - PSLab Development 101.
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <xc.h>

#include "../registers/timers/tmr1.h"
#include "../commands.h"
#include "../helpers/delay.h"
#include "../helpers/light.h"


void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    /**
     * This is an interrupt function. If TMR1 interrupt is enabled (), it runs
     * whenever the value of TMR1 is equal to the value of PR1.
     *
     * It is important to clear the TMR1 interrupt flag () inside this function,
     * otherwise it will run again immediately.
     *
     * See registers/timers/tmr1.h for more information.
     *
     * Implement the LED toggling logic here.
     */
}

static void enable_timer(void)
{
    // Configure and enable TMR1 here.
}

static void disable_timer(void)
{
    // Disable and reset TMR1 here.
}

/**
 * @brief Blink the onboard RGB LED.
 *
 * @param period  uint16_t, blink period in milliseconds.
 * @param red     uint8_t, intensity of red channel.
 * @param green   uint8_t, intensity of green channel.
 * @param blue    uint8_t, intensity of blue channel.
 * @return enum Status
 */
enum Status BLINK_start(
    uint8_t args[],
    uint16_t args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    // Write implementation here and change return code to E_OK.
    return E_NOT_IMPLEMENTED;
}

/**
 * @brief Stop blinking.
 *
 * @return enum Status
 */
enum Status BLINK_stop(
    __attribute__((unused)) uint8_t args[],
    __attribute__((unused)) uint16_t args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    // Write implementation here and change return code to E_OK.
    return E_NOT_IMPLEMENTED;
}
