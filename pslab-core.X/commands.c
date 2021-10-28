#include "commands.h"
#include "helpers/buffer.h"
#include "helpers/device.h"
#include "instruments/multimeter.h"
#include "instruments/oscilloscope.h"
#include "instruments/wavegenerator.h"
#include "registers/system/pin_manager.h"

/**
 * This is used to check that a received secondary command does not exceed the
 * bounds of the command table.
 */
unsigned char num_secondary_cmds[NUM_PRIMARY_CMDS + 1] = {
    0,
    NUM_FLASH_CMDS,
    NUM_ADC_CMDS,
    NUM_SPI_CMDS,
    NUM_I2C_CMDS,
    NUM_UART2_CMDS,
    NUM_DAC_CMDS,
    NUM_WAVEGEN_CMDS,
    NUM_DOUT_CMDS,
    NUM_DIN_CMDS,
    NUM_TIMING_CMDS,
    NUM_COMMON_CMDS,
};

/**
 * @brief Undefined command, does nothing.
 * @return DO_NOT_BOTHER
 */
response_t Undefined(void) {
    return DO_NOT_BOTHER;
}

/**
 * @brief Unimplemented command, does nothing.
 * @return DO_NOT_BOTHER
 */
response_t Unimplemented(void) {
    return DO_NOT_BOTHER;
}

/**
 * @brief Removed command (no longer supported), does nothing.
 * @return DO_NOT_BOTHER
 */
response_t Removed(void) {
    return DO_NOT_BOTHER;
}

/**
 * Jump table for command function selection. The index matches the
 * communication protocol, i.e. the function corresponding to
 * COMMON (11) -> GET_VERSION (5) is located at cmd_table[11][5].
 */
command_func_t* const cmd_table[NUM_PRIMARY_CMDS + 1][NUM_SECONDARY_CMDS_MAX + 1] = {
    { // 0 UNDEFINED
     // 0          1          2          3
        Undefined, Undefined, Undefined, Undefined,
     // 4          5          6          7
        Undefined, Undefined, Undefined, Undefined,
     // 8          9          10         11
        Undefined, Undefined, Undefined, Undefined,
     // 12         13         14         15
        Undefined, Undefined, Undefined, Undefined,
     // 16         17         18         19
        Undefined, Undefined, Undefined, Undefined,
     // 20         21         22         23
        Undefined, Undefined, Undefined, Undefined,
     // 24         25         26         27
        Undefined, Undefined, Undefined, Undefined,
    },
    { // 1 FLASH
     // 0                  1 READ_FLASH   2 WRITE_FLASH  3 WRITE_BULK_FLASH
        Undefined,         Unimplemented, Unimplemented, Unimplemented,
     // 4 READ_BULD_FLASH  5              6              7
        Unimplemented,     Undefined,     Undefined,     Undefined,
     // 8                  9              10             11
        Undefined,         Undefined,     Undefined,     Undefined,
     // 12                 13             14             15
        Undefined,         Undefined,     Undefined,     Undefined,
     // 16                 17             18             19
        Undefined,         Undefined,     Undefined,     Undefined,
     // 20                 21             22             23
        Undefined,         Undefined,     Undefined,     Undefined,
     // 24                 25             26             27
        Undefined,         Undefined,     Undefined,     Undefined,
    },
    { // 2 ADC
     // 0                          1 CAPTURE_ONE                  2 CAPTURE_TWO                   3 CAPTURE_DMASPEED
        Undefined,                 OSCILLOSCOPE_CaptureOne,       OSCILLOSCOPE_CaptureTwo,        OSCILLOSCOPE_CaptureDMA,
     // 4 CAPTURE_FOUR             5 CONFIGURE_TRIGGER            6 GET_CAPTURE_STATUS            7 GET_CAPTURE_CHANNEL
        OSCILLOSCOPE_CaptureFour,  OSCILLOSCOPE_ConfigureTrigger, OSCILLOSCOPE_GetCaptureStatus,  Unimplemented,
     // 8 SET_PGA_GAIN             9 GET_VOLTAGE                  10 GET_VOLTAGE_SUMMED           11 START_ADC_STREAMING
        OSCILLOSCOPE_SetPGAGain,   MULTIMETER_GetVoltage,         MULTIMETER_GetVoltageSummed,    Removed,
     // 12 SELECT_PGA_CHANNEL      13 CAPTURE_12BIT               14 CAPTURE_MULTIPLE             15 SET_HI_CAPTURE
        Unimplemented,             Unimplemented,                 Removed,                        Removed,
     // 16 SET_LO_CAPTURE          17 SET_HI_CAPTURE12            18 SET_LO_CAPTURE12             19 CAPTURE_DMASPEED12
        Removed,                   Removed,                       Removed,                        Removed,
     // 20 MULTIPOINT_CAPACITANCE  21 SET_CAP                     22 PULSE_TRAIN                  23 CAPTURE_THREE
        Removed,                   MULTIMETER_ChargeCapacitor,    Removed,                        OSCILLOSCOPE_CaptureThree,
     // 24                         25                             26                              27
        Undefined,                 Undefined,                     Undefined,                      Undefined,
    },
    { // 3 SPI
     // 0               1 START_SPI           2 SEND_SPI8        3 SEND_SPI16
        Undefined,      Unimplemented,        Unimplemented,     Unimplemented,
     // 4 STOP_SPI      5 SET_SPI_PARAMETERS  6 SEND_SPI8_BURST  7 SEND_SPI16_BURST
        Unimplemented,  Unimplemented,        Removed,           Removed,
     // 8               9                     10                 11
        Undefined,      Undefined,            Undefined,         Undefined,
     // 12              13                    14                 15
        Undefined,      Undefined,            Undefined,         Undefined,
     // 16              17                    18                 19
        Undefined,      Undefined,            Undefined,         Undefined,
     // 20              21                    22                 23
        Undefined,      Undefined,            Undefined,         Undefined,
     // 24              25                    26                 27
        Undefined,      Undefined,            Undefined,         Undefined,
    },
    { // 4 I2C
     // 0                     1 I2C_START          2 I2C_SEND       3 I2C_STOP
        Undefined,            Unimplemented,       Unimplemented,   Unimplemented,
     // 4 I2C_RESTART         5 I2C_READ_END       6 I2C_READ_MORE  7 I2C_WAIT
        Unimplemented,        Unimplemented,       Unimplemented,   Unimplemented,
     // 8 I2C_SEND_BURST      9 I2C_CONFIG         10 I2C_STATUS    11 I2C_READ_BULK
        Unimplemented,        Unimplemented,       Unimplemented,   Unimplemented,
     // 12 I2C_WRITE_BULK     13 I2C_ENABLE_SMBUS  14 I2C_INIT      15 PULLDOWN_SCL
        Unimplemented,        Unimplemented,       Unimplemented,   Unimplemented,
     // 16 I2C_DISABLE_SMBUS  17                   18               19
        Unimplemented,        Undefined,           Undefined,       Undefined,
     // 20                    21                   22               23
        Undefined,            Undefined,           Undefined,       Undefined,
     // 24                    25                   26               27
        Undefined,            Undefined,           Undefined,       Undefined,
    },
    { // 5 UART2
     // 0                    1 SEND_CHAR      2 SEND_INT       3 SEND_ADDRESS
        Undefined,           Unimplemented,   Unimplemented,   Unimplemented,
     // 4 SET_BAUD           5 SET_MODE       6 READ_BYTE      7 READ_INT
        Unimplemented,       Unimplemented,   Unimplemented,   Unimplemented,
     // 8 READ_UART2_STATUS  9                10               11
        Unimplemented,       Undefined,       Undefined,       Undefined,
     // 12                   13               14               15
        Undefined,           Undefined,       Undefined,       Undefined,
     // 16                   17               18               19
        Undefined,           Undefined,       Undefined,       Undefined,
     // 20                   21               22               23
        Undefined,           Undefined,       Undefined,       Undefined,
     // 24                   25               26               27
        Undefined,           Undefined,       Undefined,       Undefined,
    },
    { // 6 DAC
     // 0          1 SET_DAC      2 SET_CALIBRATED_DAC      3
        Undefined, Unimplemented, Unimplemented,            Undefined,
     // 4          5              6                         7
        Undefined, Undefined,     Undefined,                Undefined,
     // 8          9              10                        11
        Undefined, Undefined,     Undefined,                Undefined,
     // 12         13             14                        15
        Undefined, Undefined,     Undefined,                Undefined,
     // 16         17             18                        19
        Undefined, Undefined,     Undefined,                Undefined,
     // 20         21             22                        23
        Undefined, Undefined,     Undefined,                Undefined,
     // 24         25             26                        27
        Undefined, Undefined,     Undefined,                Undefined,
    },
    { // 7 WAVEGEN
     // 0                            1 SET_WG                   2                             3 SET_SQR1
        Undefined,                   Unimplemented,             Undefined,                    WAVEGENERATOR_SetSquare1,
     // 4 SET_SQR2                   5 SET_SQRS                 6                             7 SQR4
        WAVEGENERATOR_SetSquare2,    Removed,                   Undefined,                    WAVEGENERATOR_SetSquareAll,
     // 8 MAP_REFERENCE              9 SET_WG_PHASE             10 SET_WAVEFORM_TYPE          11 SELECT_FREQ_REGISTER
        WAVEGENERATOR_MapReference,  WAVEGENERATOR_SetSineDual, Unimplemented,                Unimplemented,
     // 12 DELAY_GENERATOR           13 SET_SINE1               14 SET_SINE2                  15 LOAD_WAVEFORM1
        Unimplemented,               WAVEGENERATOR_SetSine1,    WAVEGENERATOR_SetSine2,       WAVEGENERATOR_LoadWaveForm1,
     // 16 LOAD_WAVEFORM2            17 SQR1_PATTERN            18 READ_WAVEFORM1             19 READ_WAVEFORM2
        WAVEGENERATOR_LoadWaveForm2, Removed,                   WAVEGENERATOR_ReadWaveForm1,  WAVEGENERATOR_ReadWaveForm2,
     // 20                           21                         22                            23
        Undefined,                   Undefined,                 Undefined,                    Undefined,
     // 24                           25                         26                            27
        Undefined,                   Undefined,                 Undefined,                    Undefined,
    },
    { // 8 DOUT
     // 0          1 SET_STATE                  2          3
        Undefined, PIN_MANAGER_SetWavePinState, Undefined, Undefined,
     // 4          5                            6          7
        Undefined, Undefined,                   Undefined, Undefined,
     // 8          9                            10         11
        Undefined, Undefined,                   Undefined, Undefined,
     // 12         13                           14         15
        Undefined, Undefined,                   Undefined, Undefined,
     // 16         17                           18         19
        Undefined, Undefined,                   Undefined, Undefined,
     // 20         21                           22         23
        Undefined, Undefined,                   Undefined, Undefined,
     // 24         25                           26         27
        Undefined, Undefined,                   Undefined, Undefined,
    },
    { // 9 DIN
     // 0          1 GET_STATE    2 GET_STATES               3
        Undefined, Unimplemented, PIN_MANAGER_GetLAPinState, Undefined,
     // 4          5              6                          7
        Undefined, Undefined,     Undefined,                 Undefined,
     // 8          9              10                         11
        Undefined, Undefined,     Undefined,                 Undefined,
     // 12         13             14                         15
        Undefined, Undefined,     Undefined,                 Undefined,
     // 16         17             18                         19
        Undefined, Undefined,     Undefined,                 Undefined,
     // 20         21             22                         23
        Undefined, Undefined,     Undefined,                 Undefined,
     // 24         25             26                         27
        Undefined, Undefined,     Undefined,                 Undefined,
    },
    { // 10 TIMING
     // 0                       1 GET_TIMING              2                        3
        Undefined,              Unimplemented,            Undefined,               Undefined,
     // 4 START_ONE_CHAN_LA     5 START_TWO_CHAN_LA       6 START_FOUR_CHAN_LA     7 FETCH_DMA_DATA
        Unimplemented,          Unimplemented,            Unimplemented,           Unimplemented,
     // 8 FETCH_INT_DMA_DATA    9 FETCH_LONG_DMA_DATA     10 COMPARATOR_TO_LA      11 GET_INITIAL_STATES
        Unimplemented,          Unimplemented,            Unimplemented,           Unimplemented,
     // 12 TIMING_MEASUREMENTS  13 INTERVAL_MEASUREMENTS  14 CONFIGURE_COMPARATOR  15 START_ALTERNATE_ONE_CHAN_LA
        Unimplemented,          Unimplemented,            Unimplemented,           Unimplemented,
     // 16 START_THREE_CHAN_LA  17 STOP_LA                18                       19
        Unimplemented,          Unimplemented,            Undefined,               Undefined,
     // 20                      21                        22                       23
        Undefined,              Undefined,                Undefined,               Undefined,
     // 24                      25                        26                       27
        Undefined,              Undefined,                Undefined,               Undefined,
    },
    { // 11 COMMON
     // 0                                1 GET_CTMU_VOLTAGE        2 GET_CAPACITANCE           3 GET_FREQUENCY
        Undefined,                       Unimplemented,            MULTIMETER_GetCapacitance,  Unimplemented,
     // 4 GET_INDUCTANCE                 5 GET_VERSION             6                           7
        Unimplemented,                   DEVICE_GetVersion,        Undefined,                  Undefined,
     // 8 RETRIEVE_BUFFER                9 GET_HIGH_FREQUENCY      10 CLEAR_BUFFER             11 SETRGB
        BUFFER_Retrieve,                 Unimplemented,            Unimplemented,              Unimplemented,
     // 12 READ_PROGRAM_ADDRESS          13 WRITE_PROGRAM_ADDRESS  14 READ_DATA_ADDRESS        15 WRITE_DATA_ADDRESS
        Removed,                         Removed,                  DEVICE_ReadRegisterData,    DEVICE_WriteRegisterData,
     // 16 GET_CAP_RANGE                 17 SETRGB2                18 READ_LOG                 19 RESTORE_STANDALONE
        Unimplemented,                   Unimplemented,            Unimplemented,              DEVICE_Reset,
     // 20 GET_ALTERNATE_HIGH_FREQUENCY  21                        22 SETRGB3                  23 START_CTMU
        Unimplemented,                   Undefined,                Unimplemented,              Unimplemented,
     // 24 STOP_CTMU                     25 START_COUNTING         26 FETCH_COUNT              27 FILL_BUFFER
        Unimplemented,                   Unimplemented,            Unimplemented,              Unimplemented,
    },
};
