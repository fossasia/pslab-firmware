#include <stdint.h>
#include "../helpers/interval.h"
#include "../registers/comparators/ic1.h"
#include "../registers/comparators/ic2.h"
#include "../registers/comparators/ic3.h"
#include "../registers/comparators/ic4.h"
#include "../registers/comparators/ic_params.h"
#include "../registers/system/interrupt_manager.h"
#include "../registers/system/pin_manager.h"
#include "../registers/timers/tmr2.h"
#include "../commands.h"

#define FALLING_EDGE        1
#define RISING_EDGE         0

static uint8_t LA_TRIGGER_CHANNEL = 32;
void SetLA_TRIGGER_CHANNEL(uint8_t V) { LA_TRIGGER_CHANNEL = V; }
uint8_t GetLA_TRIGGER_CHANNEL(void) { return LA_TRIGGER_CHANNEL; }

static uint8_t LA_TRIGGER_STATE = 0;
void SetLA_TRIGGER_STATE(uint8_t V) { LA_TRIGGER_STATE = V; }
uint8_t GetLA_TRIGGER_STATE(void) { return LA_TRIGGER_STATE; }

enum Status LOGICANALYZER_one_channel(
    uint8_t const *const args,
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    union Input {
        struct {
            uint16_t points;
            uint8_t trigger;
            uint8_t config;
        };
        uint8_t const *buffer;
    } input = {{0}};

    if (args_size != sizeof(input)) {
        return E_BAD_ARGSIZE;
    }

    input.buffer = args;

    if (input.trigger & 1) {
        INTCON2bits.INT2EP = input.trigger & 2 ? FALLING_EDGE : RISING_EDGE;
        RPINR1bits.INT2R = PIN_MANAGER_DIGITAL_PINS[(input.trigger >> 4) & 0xF];
    }

    INTERVAL_CaptureOne(
        input.points,
        (input.config >> 4) & 0xF,
        input.config & 0xF,
        0
    );

    if (input.trigger & 1) {
        INTERRUPT_ClearExternalInterrupt2Flag();
        INTERRUPT_EnableExternalInterrupt2();
    } else {
        SetDefaultDIGITAL_STATES();
        IC1_ManualTriggerSet();
        IC2_ManualTriggerSet();
    }

    return E_OK;
}

enum Status LOGICANALYZER_one_channel_alt(
    uint8_t const *const args,
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    union Input {
        struct {
            uint16_t points;
            uint8_t config;
            uint8_t trigger;
        };
        uint8_t const *buffer;
    } input = {{0}};

    if (args_size != sizeof(input)) {
        return E_BAD_ARGSIZE;
    }

    input.buffer = args;

    IC4_InterruptHighPriority();
    IC4_InterruptFlagClear();
    IC4_InterruptDisable();

    if (input.trigger & 7) {
        INTERVAL_CaptureOne(
            input.points,
            (input.config >> 4) & 0xF,
            input.config & 0xF,
            input.trigger
        );
        IC4_InterruptEnable();
    } else {
        INTERVAL_CaptureOne(
            input.points,
            (input.config >> 4) & 0xF,
            input.config & 0xF,
            0
        );
        SetDefaultDIGITAL_STATES();
        IC1_ManualTriggerSet();
        IC2_ManualTriggerSet();
    }

    return E_OK;
}

enum Status LOGICANALYZER_two_channel(
    uint8_t const *const args,
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    union Input {
        struct {
            uint16_t points;
            uint8_t trigger;
            uint8_t config;
            uint8_t channel;
        };
        uint8_t const *buffer;
    } input = {{0}};

    if (args_size != sizeof(input)) {
        return E_BAD_ARGSIZE;
    }

    input.buffer = args;

    if (input.trigger & 1) {
        INTCON2bits.INT2EP = input.trigger & 2 ? FALLING_EDGE : RISING_EDGE;
        RPINR1bits.INT2R = PIN_MANAGER_DIGITAL_PINS[(input.trigger >> 4) & 0xF];
    }

    INTERVAL_CaptureTwo(input.points, input.config, input.channel);

    if (input.trigger & 1) {
        INTERRUPT_ClearExternalInterrupt2Flag();
        INTERRUPT_EnableExternalInterrupt2();
    } else {
        SetDefaultDIGITAL_STATES();
        IC_PARAMS_ManualTriggerAll();
        SetDefaultDIGITAL_STATES_ERROR();
    }

    return E_OK;

    // TODO: Implement trigger actions for any of the two channels. Right now
    // only one channel can be set as trigger source. Using CN interrupts, this
    // can be extended to listen to multiple trigger sources.
}

enum Status LOGICANALYZER_three_channel(
    uint8_t const *const args,
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    union Input {
        struct {
            uint16_t points;
            uint8_t config;
            uint8_t trigger;
        };
        uint8_t const *buffer;
    } input = {{0}};

    if (args_size != sizeof(input)) {
        return E_BAD_ARGSIZE;
    }

    input.buffer = args;

    IC4_InterruptHighPriority();
    IC4_InterruptFlagClear();
    IC4_InterruptDisable();

    if (input.trigger & 7) {
        INTERVAL_CaptureThree(input.points, input.config & 0x0FFF, input.trigger);
        IC4_InterruptEnable();
    } else {
        INTERVAL_CaptureThree(input.points, input.config & 0x0FFF, 0);
        SetDefaultDIGITAL_STATES();
        IC1_ManualTriggerSet();
        IC2_ManualTriggerSet();
        IC3_ManualTriggerSet();
        SetDefaultDIGITAL_STATES_ERROR();
    }

    return E_OK;

    // TODO: Implement trigger actions for any of the three channels. Right now
    // only one channel can be set as trigger source. Using CN interrupts, this
    // can be extended to listen to multiple trigger sources.
}

enum Status LOGICANALYZER_four_channel(
    uint8_t const *const args,
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    union Input {
        struct {
            uint16_t points;
            uint16_t mode;
            uint8_t prescaler;
            uint8_t trigger;
        };
        uint8_t const *buffer;
    } input = {{0}};

    if (args_size != sizeof(input)) {
        return E_BAD_ARGSIZE;
    }

    input.buffer = args;

    SetDIGITAL_STATES(0);
    INTERVAL_CaptureFour(input.points, input.mode, input.prescaler);

    if (input.trigger & 1) {
        LA_TRIGGER_STATE = input.trigger & 2 ? RISING_EDGE : FALLING_EDGE;
        LA_TRIGGER_CHANNEL = 0;

        if ((input.trigger >> 2) & 1) {
            INTERRUPT_LA1PinChange(true);
            LA_TRIGGER_CHANNEL |= 1;
        }
        if ((input.trigger >> 3) & 1) {
            INTERRUPT_LA2PinChange(true);
            LA_TRIGGER_CHANNEL |= 2;
        }
        if ((input.trigger >> 4) & 1) {
            INTERRUPT_LA3PinChange(true);
            LA_TRIGGER_CHANNEL |= 4;
        }
        if ((input.trigger >> 5) & 1) {
            INTERRUPT_LA4PinChange(true);
            LA_TRIGGER_CHANNEL |= 8;
        }
        INTERRUPT_ClearPinChangeInterruptsFlag();
        INTERRUPT_EnablePinChangeInterrupts();
    } else {
        TMR2_Start();
        SetDefaultDIGITAL_STATES();
        IC_PARAMS_ManualTriggerAll();
        SetDefaultDIGITAL_STATES_ERROR();
    }

    return E_OK;
}

enum Status LOGICANALYZER_stop(
    __attribute__ ((unused)) uint8_t const *args,
    __attribute__ ((unused)) uint16_t const args_size,
    __attribute__ ((unused)) uint8_t **rets,
    __attribute__ ((unused)) uint16_t *rets_size
) {
    IC_PARAMS_DisableAllModules();
    return E_OK;
}
