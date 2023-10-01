#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../bus/uart/uart.h"
#include "../commands.h"
#include "fatfs/ff.h"

static FATFS drive;
static FIL file;

static bool get_filename(char filename[], uint8_t const filename_length) {
    for (uint8_t i = 0; i < filename_length; ++i) {
        filename[i] = UART1_Read();
    }

    if (filename[filename_length - 1]) {
        return false; // String must be null terminated.
    }

    return true;
}

response_t SDCARD_write_file(void) {
    uint8_t filename_length = UART1_Read();
    char filename[filename_length];

    if (!get_filename(filename, filename_length)) {
        return FAILED;
    }

    uint8_t mode = UART1_Read();
    size_t data_length = UART1_ReadInt();
    uint8_t data[data_length];

    for (size_t i = 0; i < data_length; ++i) {
        data[i] = UART1_Read();
    }

    size_t bytes_written = 0;
    FRESULT ret = f_mount(&drive, "0:", 1);

    if (ret == FR_OK) {
        ret = f_open(&file, filename, FA_WRITE | mode);

        if (ret == FR_OK) {
            f_write(&file, data, data_length, &bytes_written);
            f_close(&file);
        }

        f_mount(0, "0:", 0);
    }

    UART1_Write(ret);
    UART1_WriteInt(bytes_written);

    return SUCCESS;
}

response_t SDCARD_read_file(void) {
    uint8_t filename_length = UART1_Read();
    char filename[filename_length];

    if (!get_filename(filename, filename_length)) {
        return FAILED;
    }

    size_t bytes_to_read = UART1_ReadInt();
    char data[bytes_to_read];
    size_t bytes_read = 0;
    FRESULT ret = f_mount(&drive, "0:", 1);

    if (ret == FR_OK) {
        ret = f_open(&file, filename, FA_READ);

        if (ret == FR_OK) {
            f_read(&file, &data, bytes_to_read, &bytes_read);
            f_close(&file);
        }

        f_mount(0, "0:", 0);
    }

    UART1_Write(ret);
    UART1_WriteInt(bytes_read);

    for (size_t i = 0; i < bytes_read; ++i) {
        UART1_Write(data[i]);
    }

    return SUCCESS;
}
