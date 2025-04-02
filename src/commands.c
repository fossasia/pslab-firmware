#include "commands.h"
#include "sdcard/sdcard.h"
#include "bus/i2c/i2c.h"
#include "bus/uart/uart.h"
#include "bus/spi/spi.h"
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
#include "transport/uart2.h"

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
    NUM_SDCARD_CMDS,
};

#define Undefined NULL
#define Unimplemented NULL
#define Removed NULL

/* Forward declaration */
CmdFunc const cmd_table[NUM_PRIMARY_CMDS + 1][NUM_SECONDARY_CMDS_MAX + 1];

CmdFunc COMMAND_get_func(uint16_t const cmd)
{
   uint8_t const primary = cmd & 0xFF;
   uint8_t const secondary = (cmd >> 8) & 0xFF;

   if (primary > NUM_PRIMARY_CMDS) {
      return NULL;
   }

   if (secondary > num_secondary_cmds[primary]) {
      return NULL;
   }

   if (primary == 5) {
      LED_SetLow();
   }

   CmdFunc cmdfunc = cmd_table[primary][secondary];

   return cmdfunc;
}

/**
 * Jump table for command function selection. The index matches the
 * communication protocol, i.e. the function corresponding to
 * COMMON (11) -> GET_VERSION (5) is located at cmd_table[11][5].
 */
CmdFunc const cmd_table[NUM_PRIMARY_CMDS + 1][NUM_SECONDARY_CMDS_MAX + 1] = {
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
     // 0                          1 CAPTURE_ONE                   2 CAPTURE_TWO                    3 CAPTURE_DMASPEED
        Undefined,                 OSCILLOSCOPE_capture_one,       OSCILLOSCOPE_capture_two,        OSCILLOSCOPE_capture_dma,
     // 4 CAPTURE_FOUR             5 CONFIGURE_TRIGGER             6 GET_CAPTURE_STATUS             7 GET_CAPTURE_CHANNEL
        OSCILLOSCOPE_capture_four, OSCILLOSCOPE_configure_trigger, OSCILLOSCOPE_get_capture_status, Unimplemented,
     // 8 SET_PGA_GAIN             9 GET_VOLTAGE                   10 GET_VOLTAGE_SUMMED            11 START_ADC_STREAMING
        OSCILLOSCOPE_set_pga_gain, MULTIMETER_get_voltage,         MULTIMETER_get_voltage_summed,   Removed,
     // 12 SELECT_PGA_CHANNEL      13 CAPTURE_12BIT                14 CAPTURE_MULTIPLE              15 SET_HI_CAPTURE
        Unimplemented,             Unimplemented,                  Removed,                         Removed,
     // 16 SET_LO_CAPTURE          17 SET_HI_CAPTURE12             18 SET_LO_CAPTURE12              19 CAPTURE_DMASPEED12
        Removed,                   Removed,                        Removed,                         Removed,
     // 20 MULTIPOINT_CAPACITANCE  21 SET_CAP                      22 PULSE_TRAIN                   23 CAPTURE_THREE
        Removed,                   MULTIMETER_charge_capacitor,    Removed,                         OSCILLOSCOPE_capture_three,
     // 24                         25                              26                               27
        Undefined,                 Undefined,                      Undefined,                       Undefined,
    },
    { // 3 SPI
     // 0                               1 START_SPI                     2 SEND_SPI8                     3 SEND_SPI16
        Undefined,                      Removed,                        Removed,                        Removed,
     // 4 STOP_SPI                      5 SET_SPI_PARAMETERS            6 SEND_SPI8_BURST               7 SEND_SPI16_BURST
        Removed,                        Removed,                        Removed,                        Removed,
     // 8 WRITE_SPI8_BURST              9 WRITE_SPI16_BURST             10 READ_SPI8_BURST              11 READ_SPI16_BURST
        Removed,                        Removed,                        Removed,                        Removed,
     // 12                              13                              14                              15
        SPI_cmd_open,                   SPI_cmd_close,                  SPI_cmd_set_mode,               SPI_cmd_set_clock,
     // 16                              17                              18                              19
        SPI_cmd_set_word_size,          SPI_cmd_read,                   SPI_cmd_write,                  SPI_cmd_exchange,
     // 20                              21                              22                              23
        Undefined,                      Undefined,                      Undefined,                      Undefined,
     // 24                              25                              26                              27
        Undefined,                      Undefined,                      Undefined,                      Undefined,
    },
    { // 4 I2C
     // 0                               1 I2C_START                     2 I2C_SEND                      3 I2C_STOP
        Undefined,                      I2C_command_start,              I2C_command_send,               I2C_command_stop,
     // 4 I2C_RESTART                   5 I2C_READ_END                  6 I2C_READ_MORE                 7 I2C_WAIT
        I2C_command_restart,            I2C_command_read_end,           I2C_command_read_more,          I2C_command_wait,
     // 8 I2C_SEND_BURST                9 I2C_CONFIG                    10 I2C_STATUS                   11 I2C_READ_BULK
        I2C_command_send_burst,         I2C_command_config,             I2C_command_status,             I2C_command_read_bulk,
     // 12 I2C_WRITE_BULK               13 I2C_ENABLE_SMBUS             14 I2C_INIT                     15 PULLDOWN_SCL
        I2C_command_write_bulk,         I2C_command_enable_smbus,       I2C_command_init,               I2C_command_pull_down,
     // 16 I2C_DISABLE_SMBUS            17                              18                              19
        I2C_command_disable_smbus,      Undefined,                      Undefined,                      Undefined,
     // 20                              21                              22                              23
        Undefined,                      Undefined,                      Undefined,                      Undefined,
     // 24                              25                              26                              27
        Undefined,                      Undefined,                      Undefined,                      Undefined,
    },
    { // 5 UART2
     // 0                     1 SEND_CHAR      2 SEND_INT       3 SEND_ADDRESS
        Undefined,            UART2_write,     Removed,         Removed,
     // 4 SET_BAUD            5 SET_MODE       6 READ_BYTE      7 READ_INT
        UART2_set_baud,       UART2_set_mode,  UART2_read,      Removed,
     // 8 READ_UART2_STATUS   9                10               11
        UART2_rx_ready,       Undefined,       Undefined,       Undefined,
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
        Undefined,                      Removed,                        Removed,                        POWER_SOURCE_SetPower,
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
     // 0                            1 SET_WG                     2                             3 SET_SQR1
        Undefined,                   Unimplemented,               Undefined,                    WAVEGENERATOR_set_square_1,
     // 4 SET_SQR2                   5 SET_SQRS                   6                             7 SQR4
        WAVEGENERATOR_set_square_2,  Removed,                     Undefined,                    WAVEGENERATOR_set_square_all,
     // 8 MAP_REFERENCE              9 SET_WG_PHASE               10 SET_WAVEFORM_TYPE          11 SELECT_FREQ_REGISTER
        WAVEGENERATOR_map_reference, WAVEGENERATOR_set_sine_dual, Unimplemented,                Unimplemented,
     // 12 DELAY_GENERATOR           13 SET_SINE1                 14 SET_SINE2                  15 LOAD_WAVEFORM1
        Unimplemented,               WAVEGENERATOR_set_sine_1,    WAVEGENERATOR_set_sine_2,     WAVEGENERATOR_load_wave_1,
     // 16 LOAD_WAVEFORM2            17 SQR1_PATTERN              18 READ_WAVEFORM1             19 READ_WAVEFORM2
        WAVEGENERATOR_load_wave_2,   Removed,                     WAVEGENERATOR_read_wave_1,    WAVEGENERATOR_read_wave_2,
     // 20                           21                           22                            23
        Undefined,                   Undefined,                   Undefined,                    Undefined,
     // 24                           25                           26                            27
        Undefined,                   Undefined,                   Undefined,                    Undefined,
    },
    { // 8 DOUT
     // 0          1 SET_STATE                   2          3
        Undefined, PIN_MANAGER_set_sq_pin_state, Undefined, Undefined,
     // 4          5                             6          7
        Undefined, Undefined,                    Undefined, Undefined,
     // 8          9                             10         11
        Undefined, Undefined,                    Undefined, Undefined,
     // 12         13                            14         15
        Undefined, Undefined,                    Undefined, Undefined,
     // 16         17                            18         19
        Undefined, Undefined,                    Undefined, Undefined,
     // 20         21                            22         23
        Undefined, Undefined,                    Undefined, Undefined,
     // 24         25                            26         27
        Undefined, Undefined,                    Undefined, Undefined,
    },
    { // 9 DIN
     // 0          1 GET_STATE    2 GET_STATES                  3
        Undefined, Unimplemented, PIN_MANAGER_get_la_pin_state, Undefined,
     // 4          5              6                             7
        Undefined, Undefined,     Undefined,                    Undefined,
     // 8          9              10                            11
        Undefined, Undefined,     Undefined,                    Undefined,
     // 12         13             14                            15
        Undefined, Undefined,     Undefined,                    Undefined,
     // 16         17             18                            19
        Undefined, Undefined,     Undefined,                    Undefined,
     // 20         21             22                            23
        Undefined, Undefined,     Undefined,                    Undefined,
     // 24         25             26                            27
        Undefined, Undefined,     Undefined,                    Undefined,
    },
    { // 10 TIMING
     // 0                               1 GET_TIMING                    2                               3
        Undefined,                      Unimplemented,                  Undefined,                      Undefined,
     // 4 START_ONE_CHAN_LA             5 START_TWO_CHAN_LA             6 START_FOUR_CHAN_LA            7 FETCH_DMA_DATA
        LOGICANALYZER_one_channel,      LOGICANALYZER_two_channel,      LOGICANALYZER_four_channel,     Removed,
     // 8 FETCH_INT_DMA_DATA            9 FETCH_LONG_DMA_DATA           10 COMPARATOR_TO_LA             11 GET_INITIAL_STATES
        Removed,                        Removed,                        Unimplemented,                  INTERVAL_get_state,
     // 12 TIMING_MEASUREMENTS          13 INTERVAL_MEASUREMENTS        14 CONFIGURE_COMPARATOR         15 START_ALTERNATE_ONE_CHAN_LA
        Unimplemented,                  Unimplemented,                  Removed,                        LOGICANALYZER_one_channel_alt,
     // 16 START_THREE_CHAN_LA          17 STOP_LA                      18                              19
        LOGICANALYZER_three_channel,    LOGICANALYZER_stop,             Undefined,                      Undefined,
     // 20                              21                              22                              23
        Undefined,                      Undefined,                      Undefined,                      Undefined,
     // 24                              25                              26                              27
        Undefined,                      Undefined,                      Undefined,                      Undefined,
    },
    { // 11 COMMON
     // 0                               1 GET_CTMU_VOLTAGE              2 GET_CAPACITANCE               3 GET_FREQUENCY
        Undefined,                      MULTIMETER_get_ctmu_volts,      MULTIMETER_get_capacitance,     Unimplemented,
     // 4 GET_INDUCTANCE                5 GET_VERSION                   6 GET_FW_VERSION                7 DEBUG_IS_ENABLED
        Unimplemented,                  DEVICE_get_hw_version,          DEVICE_get_fw_version,          Removed,
     // 8 RETRIEVE_BUFFER               9 GET_HIGH_FREQUENCY            10 CLEAR_BUFFER                 11 SET_RGB1
        Removed,                        Unimplemented,                  Removed,                        Removed,
     // 12 READ_PROGRAM_ADDRESS         13 WRITE_PROGRAM_ADDRESS        14 READ_DATA_ADDRESS            15 WRITE_DATA_ADDRESS
        Removed,                        Removed,                        DEVICE_read_register,           DEVICE_write_register,
     // 16 GET_CAP_RANGE                17 SET_RGB2                     18 READ_LOG                     19 RESTORE_STANDALONE
        MULTIMETER_get_cap_range,       Removed,                        Removed,                        DEVICE_reset,
     // 20 GET_ALTERNATE_HIGH_FREQUENCY 21 SET_RGB_COMMON               22 SET_RGB3                     23 START_CTMU
        Unimplemented,                  LIGHT_rgb_pin,                  Removed,                        CTMU_start,
     // 24 STOP_CTMU                    25 START_COUNTING               26 FETCH_COUNT                  27 FILL_BUFFER
        CTMU_stop,                      SENSORS_start_counter,          SENSORS_get_counter,            Removed,
    },
    { // 12 PASSTHROUGH
     // 0          1                 2          3
        Undefined, Removed,          Undefined, Undefined,
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
    { // 13 SENSORS
     // 0                               1 RTC_SETTIME                   2 RTC_SETDIGIT                  3 RTC_GETTIME
        Undefined,                      RTC_cmd_set_time,               Removed,                        RTC_cmd_get_time,
     // 4 RTC_GETDIGIT                  5 HCSR04                        6 AM2302                        7 BMP180
        Removed,                        Unimplemented,                  Unimplemented,                  Unimplemented,
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
    { // 14 SDCARD
     // 0                               1 SDCARD_READ                   2 SDCARD_WRITE                  3 SDCARD_INFO
        Undefined,                      SDCARD_read,                    SDCARD_write,                   SDCARD_stat,
     // 4 SDCARD_MOUNT                  5 SDCARD_UNMOUNT                6 SDCARD_OPEN                   7 SDCARD_CLOSE
        SDCARD_mount,                   SDCARD_unmount,                 SDCARD_open,                    SDCARD_close,
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
};
