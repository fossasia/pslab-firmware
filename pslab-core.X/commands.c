#include "commands.h"
#include "version.h"

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
 * @brief Default command function which does nothing.
 * @return DO_NOT_BOTHER
 */
response_t DoNothing(void) {
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
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 4          5          6          7
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 8          9          10         11
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 12         13         14         15
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 16         17         18         19
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 20         21         22         23
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 24         25         26         27
        DoNothing, DoNothing, DoNothing, DoNothing,
    },
    { // 1 FLASH
     // 0          1          2          3
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 4          5          6          7
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 8          9          10         11
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 12         13         14         15
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 16         17         18         19
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 20         21         22         23
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 24         25         26         27
        DoNothing, DoNothing, DoNothing, DoNothing,
    },
    { // 2 ADC
     // 0          1          2          3
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 4          5          6          7
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 8          9          10         11
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 12         13         14         15
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 16         17         18         19
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 20         21         22         23
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 24         25         26         27
        DoNothing, DoNothing, DoNothing, DoNothing,
    },
    { // 3 SPI
     // 0          1          2          3
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 4          5          6          7
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 8          9          10         11
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 12         13         14         15
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 16         17         18         19
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 20         21         22         23
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 24         25         26         27
        DoNothing, DoNothing, DoNothing, DoNothing,
    },
    { // 4 I2C
     // 0          1          2          3
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 4          5          6          7
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 8          9          10         11
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 12         13         14         15
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 16         17         18         19
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 20         21         22         23
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 24         25         26         27
        DoNothing, DoNothing, DoNothing, DoNothing,
    },
    { // 5 UART2
     // 0          1          2          3
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 4          5          6          7
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 8          9          10         11
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 12         13         14         15
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 16         17         18         19
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 20         21         22         23
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 24         25         26         27
        DoNothing, DoNothing, DoNothing, DoNothing,
    },
    { // 6 DAC
     // 0          1          2          3
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 4          5          6          7
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 8          9          10         11
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 12         13         14         15
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 16         17         18         19
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 20         21         22         23
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 24         25         26         27
        DoNothing, DoNothing, DoNothing, DoNothing,
    },
    { // 7 WAVEGEN
     // 0          1          2          3
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 4          5          6          7
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 8          9          10         11
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 12         13         14         15
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 16         17         18         19
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 20         21         22         23
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 24         25         26         27
        DoNothing, DoNothing, DoNothing, DoNothing,
    },
    { // 8 DOUT
     // 0          1          2          3
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 4          5          6          7
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 8          9          10         11
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 12         13         14         15
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 16         17         18         19
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 20         21         22         23
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 24         25         26         27
        DoNothing, DoNothing, DoNothing, DoNothing,
    },
    { // 9 DIN
     // 0          1          2          3
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 4          5          6          7
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 8          9          10         11
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 12         13         14         15
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 16         17         18         19
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 20         21         22         23
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 24         25         26         27
        DoNothing, DoNothing, DoNothing, DoNothing,
    },
    { // 10 TIMING
     // 0          1          2          3
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 4          5          6          7
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 8          9          10         11
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 12         13         14         15
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 16         17         18         19
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 20         21         22         23
        DoNothing, DoNothing, DoNothing, DoNothing,
     // 24         25         26         27
        DoNothing, DoNothing, DoNothing, DoNothing,
    },
    { // 11 COMMON
     // 0          1               2          3
        DoNothing, DoNothing,      DoNothing, DoNothing,
     // 4          5               6          7
        DoNothing, VERSION_SendHw, DoNothing, DoNothing,
     // 8          9               10         11
        DoNothing, DoNothing,      DoNothing, DoNothing,
     // 12         13              14         15
        DoNothing, DoNothing,      DoNothing, DoNothing,
     // 16         17              18         19
        DoNothing, DoNothing,      DoNothing, DoNothing,
     // 20         21              22         23
        DoNothing, DoNothing,      DoNothing, DoNothing,
     // 24         25              26         27
        DoNothing, DoNothing,      DoNothing, DoNothing,
    },
};
