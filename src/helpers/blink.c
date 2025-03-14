#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <xc.h>

#include "../registers/timers/tmr1.h"
#include "../commands.h"
#include "../helpers/delay.h"
#include "../helpers/light.h"

enum { NUM_COLORS = 3, };

union Color {
    struct {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    };
    uint8_t buffer[NUM_COLORS];
};

bool g_is_led_on = 0;
uint16_t g_period = 0;
uint16_t g_tick = 0;
union Color g_color = {{0}};

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    // This interrupt runs once per millisecond.

    // Clear interrupt flag to prevent interrupt reentry until next tickover.
    IFS0bits.T1IF = 0;

    // LED states flips twice per period, once to on state, once to off state.
    if (g_tick++ < g_period / 2) {
        // It's not yet time to flip the LED state.
        return;
    }

    // Time to flip the LED state. Reset tick counter.
    g_tick = 0;

    // Flip LED state.
    if (g_is_led_on) {
        // Turn off LED.
        LIGHT_RGB(0, 0, 0);
    } else {
        // Turn on LED.
        LIGHT_RGB(g_color.red, g_color.green, g_color.blue);
    }
    g_is_led_on = !g_is_led_on;
}

static void enable_timer(void)
{
    // Reset timer registers.
    TMR1_Initialize();
    // Set the timer interrupt period to the MCU frequency divided by 1000.
    // This will cause the interrupt to trigger once every millisecond.
    uint16_t const millisecond = FCY / 1000;
    TMR1_Period16BitSet(millisecond);
    // Clear interrupt flag.
    IFS0bits.T1IF = 0;
    // Enable interrupt.
    IEC0bits.T1IE = 1;
    // Start timer.
    TMR1_Start();
}

static void disable_timer(void)
{
    // Stop timer.
    TMR1_Stop();
    // Disable interrupt.
    IEC0bits.T1IE = 0;
    // Clear interrupt flag.
    IFS0bits.T1IF = 0;
    // Reset timer registers.
    TMR1_Initialize();
}

enum Status BLINK_start(
    uint8_t args[],
    uint16_t args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    struct Input {
        // Blink period in milliseconds.
        uint16_t period;
        // Intensity of red, green, and blue channels.
        union Color color;
        // Struct aligns to size of largest member, i.e. uint16_t.
        uint8_t _pad[1];
    };
    struct Input *input = NULL;

    if (args_size != sizeof(struct Input) - sizeof(input->_pad)) {
        return E_BAD_ARGSIZE;
    }

    input = (struct Input *)args;
    memcpy(g_color.buffer, input->color.buffer, sizeof(g_color));
    g_period = input->period;
    enable_timer();
    return E_OK;
}

enum Status BLINK_stop(
    __attribute__((unused)) uint8_t args[],
    __attribute__((unused)) uint16_t args_size,
    __attribute__((unused)) uint8_t **rets,
    __attribute__((unused)) uint16_t *rets_size
) {
    disable_timer();
    return E_OK;
}
