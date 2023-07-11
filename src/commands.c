#include "commands.h"
#include "bus/i2c/i2c.h"
#include "bus/uart/uart.h"
#include "bus/spi/spi.h"
#include "helpers/buffer.h"
#include "helpers/device.h"
#include "helpers/interval.h"
#include "helpers/light.h"
#include "helpers/rtc.h"
#include "instruments/logicanalyzer.h"
#include "instruments/multimeter.h"
#include "instruments/oscilloscope.h"
#include "instruments/powersource.h"
#include "instruments/sensors.h"
#include "instruments/wavegenerator.h"
#include "registers/system/pin_manager.h"
#include "instruments/sensors.h"
#include "registers/converters/ctmu.h"

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
    NUM_PASSTHRU_CMDS,
    NUM_SENSOR_CMDS,
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
     // 0                               1 START_SPI                     2 SEND_SPI8                     3 SEND_SPI16
        Undefined,                      Removed,                        Removed,                        Removed,
     // 4 STOP_SPI                      5 SET_SPI_PARAMETERS            6 SEND_SPI8_BURST               7 SEND_SPI16_BURST
        Removed,                        SPI_conf,                       SPI_exchange_bytes,             SPI_exchange_ints,
     // 8 WRITE_SPI8_BURST              9 WRITE_SPI16_BURST             10 READ_SPI8_BURST              11 READ_SPI16_BURST
        SPI_write_bytes,                SPI_write_ints,                 SPI_read_bytes,                 SPI_read_ints,
     // 12                              13                              14                              15
        Undefined,                      Undefined,                      Undefined,                      Undefined,
     // 16                              17                              18                              19
        Undefined,                      Undefined,                      Undefined,                      Undefined,
     // 20                              21                              22                              23
        Undefined,                      Undefined,                      Undefined,                      Undefined,
     // 24                              25                              26                              27
        Undefined,                      Undefined,                      Undefined,                      Undefined,
    },
    { // 4 I2C
     // 0                               1 I2C_START                     2 I2C_SEND                      3 I2C_STOP
        Undefined,                      I2C_CommandStart,               I2C_CommandSend,                I2C_CommandStop,
     // 4 I2C_RESTART                   5 I2C_READ_END                  6 I2C_READ_MORE                 7 I2C_WAIT
        I2C_CommandRestart,             I2C_CommandReadEnd,             I2C_CommandReadMore,            I2C_CommandWait,
     // 8 I2C_SEND_BURST                9 I2C_CONFIG                    10 I2C_STATUS                   11 I2C_READ_BULK
        I2C_CommandSendBurst,           I2C_CommandConfig,              I2C_CommandStatus,              I2C_CommandReadBulk,
     // 12 I2C_WRITE_BULK               13 I2C_ENABLE_SMBUS             14 I2C_INIT                     15 PULLDOWN_SCL
        I2C_CommandWriteBulk,           I2C_CommandEnableSMBus,         I2C_CommandInit,                I2C_CommandPullDown,
     // 16 I2C_DISABLE_SMBUS            17                              18                              19
        I2C_CommandDisableSMBus,        Undefined,                      Undefined,                      Undefined,
     // 20                              21                              22                              23
        Undefined,                      Undefined,                      Undefined,                      Undefined,
     // 24                              25                              26                              27
        Undefined,                      Undefined,                      Undefined,                      Undefined,
    },
    { // 5 UART2
     // 0                     1 SEND_CHAR      2 SEND_INT       3 SEND_ADDRESS
        Undefined,            UART2_Write,     UART2_WriteWord, Removed,
     // 4 SET_BAUD            5 SET_MODE       6 READ_BYTE      7 READ_INT
        UART2_SetBaud,        UART2_SetMode,   UART2_Read,      UART2_ReadWord,
     // 8 READ_UART2_STATUS   9                10               11
        UART2_RxReady,        Undefined,       Undefined,       Undefined,
     // 12                    13               14               15
        Undefined,            Undefined,       Undefined,       Undefined,
     // 16                    17               18               19
        Undefined,            Undefined,       Undefined,       Undefined,
     // 20                    21               22               23
        Undefined,            Undefined,       Undefined,       Undefined,
     // 24                    25               26               27
        Undefined,            Undefined,       Undefined,       Undefined,
    },
    { // 6 DAC
     // 0                               1 SET_DAC                       2 SET_CALIBRATED_DAC            3 SET_POWER
        Undefined,                      POWER_SOURCE_SetDAC,            Removed,                        POWER_SOURCE_SetPower,
     // 4                               5                               6                               7
        Undefined,                      Undefined,                      Undefined,                      Undefined,
     // 8                               9                               10                              11
        Undefined,                      Undefined,                      Undefined,                      Undefined,
     // 12                              13                              14                              15
        Undefined,                      Undefined,                      Undefined,                      Undefined,
     // 16                              17                              18                              19
        Undefined,                      Undefined,                      Undefined,                      Undefined,
     // 20                              21                              22                              23
        Undefined,                      Undefined,                      Undefined,                      Undefined,
     // 24                              25                              26                              27
        Undefined,                      Undefined,                      Undefined,                      Undefined,
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
     // 0                               1 GET_TIMING                    2                               3
        Undefined,                      Unimplemented,                  Undefined,                      Undefined,
     // 4 START_ONE_CHAN_LA             5 START_TWO_CHAN_LA             6 START_FOUR_CHAN_LA            7 FETCH_DMA_DATA
        LOGICANALYZER_OneChannel,       LOGICANALYZER_TwoChannel,       LOGICANALYZER_FourChannel,      Removed,
     // 8 FETCH_INT_DMA_DATA            9 FETCH_LONG_DMA_DATA           10 COMPARATOR_TO_LA             11 GET_INITIAL_STATES
        BUFFER_FetchInt,                BUFFER_FetchLong,               Unimplemented,                  INTERVAL_GetState,
     // 12 TIMING_MEASUREMENTS          13 INTERVAL_MEASUREMENTS        14 CONFIGURE_COMPARATOR         15 START_ALTERNATE_ONE_CHAN_LA
        Unimplemented,                  Unimplemented,                  Removed,                        LOGICANALYZER_OneChannelAlt,
     // 16 START_THREE_CHAN_LA          17 STOP_LA                      18                              19
        LOGICANALYZER_ThreeChannel,     LOGICANALYZER_Stop,             Undefined,                      Undefined,
     // 20                              21                              22                              23
        Undefined,                      Undefined,                      Undefined,                      Undefined,
     // 24                              25                              26                              27
        Undefined,                      Undefined,                      Undefined,                      Undefined,
    },
    { // 11 COMMON
     // 0                               1 GET_CTMU_VOLTAGE              2 GET_CAPACITANCE               3 GET_FREQUENCY
        Undefined,                      MULTIMETER_GetCTMUVolts,        MULTIMETER_GetCapacitance,      Unimplemented,
     // 4 GET_INDUCTANCE                5 GET_VERSION                   6                               7
        Unimplemented,                  DEVICE_GetVersion,              Undefined,                      Undefined,
     // 8 RETRIEVE_BUFFER               9 GET_HIGH_FREQUENCY            10 CLEAR_BUFFER                 11 SET_RGB1
        BUFFER_Retrieve,                Unimplemented,                  BUFFER_Clear,                   Removed,
     // 12 READ_PROGRAM_ADDRESS         13 WRITE_PROGRAM_ADDRESS        14 READ_DATA_ADDRESS            15 WRITE_DATA_ADDRESS
        Removed,                        Removed,                        DEVICE_ReadRegisterData,        DEVICE_WriteRegisterData,
     // 16 GET_CAP_RANGE                17 SET_RGB2                     18 READ_LOG                     19 RESTORE_STANDALONE
        MULTIMETER_GetCapRange,         Removed,                        Removed,                        DEVICE_Reset,
     // 20 GET_ALTERNATE_HIGH_FREQUENCY 21 SET_RGB_COMMON               22 SET_RGB3                     23 START_CTMU
        Unimplemented,                  LIGHT_RGBPin,                   Removed,                        CTMU_Start,
     // 24 STOP_CTMU                    25 START_COUNTING               26 FETCH_COUNT                  27 FILL_BUFFER
        CTMU_Stop,                      SENSORS_StartCounter,           SENSORS_GetCounter,             BUFFER_Fill,
    },
    { // 12 PASSTHROUGH
     // 0          1                 2          3
        Undefined, UART_Passthrough, Undefined, Undefined,
     // 4          5                 6          7
        Undefined, Undefined,        Undefined, Undefined,
     // 8          9                 10         11
        Undefined, Undefined,        Undefined, Undefined,
     // 12         13                14         15
        Undefined, Undefined,        Undefined, Undefined,
     // 16         17                18         19
        Undefined, Undefined,        Undefined, Undefined,
     // 20         21                22         23
        Undefined, Undefined,        Undefined, Undefined,
     // 24         25                26         27
        Undefined, Undefined,        Undefined, Undefined,
    },
    { // 12 SENSORS
     // 0                               1 RTC_SETTIME                   2 RTC_SETDIGIT                  3 RTC_GETTIME
        Undefined,                      RTC_SetTime,                    RTC_SetDigit,                   RTC_GetTime,
     // 4 RTC_GETDIGIT                  5 HCSR04                        6 AM2302                        7 BMP180
        RTC_GetDigit,                   Unimplemented,                  Unimplemented,                  Unimplemented,
     // 8 TSL2591                       9 TCD1304                       10                              11
        Unimplemented,                  Unimplemented,                  Undefined,                      Undefined,
     // 12                              13                              14                              15
        Undefined,                      Undefined,                      Undefined,                      Undefined,
     // 16                              17                              18                              19
        Undefined,                      Undefined,                      Undefined,                      Undefined,
     // 20                              21                              22                              23
        Undefined,                      Undefined,                      Undefined,                      Undefined,
     // 24                              25                              26                              27
        Undefined,                      Undefined,                      Undefined,                      Undefined,
    },
};
