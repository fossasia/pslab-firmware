#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../bus/uart/uart.h"
#include "../commands.h"
#include "fatfs/ff.h"

#define BUF_SIZE 256

static FATFS drive;
static FIL file;
static char buffer[BUF_SIZE];

static void get_filename(void) {
    for (uint8_t i = 0; i < BUF_SIZE; ++i) {
        if (!(buffer[i] = UART1_Read())) {
            // Received null-terminator.
            return;
        }
    }
    // FatFS handles non-terminated strings gracefully, so no need to do a
    // manual check for null-termination.
}

/// @brief Discard new errors if one has already happened.
/// @param old Pointer to FRESULT.
/// @param new New FRESULT.
static void keep_first_error(FRESULT* old, FRESULT new) {
    *old || (*old = new);
}

response_t SDCARD_write_file(void) {
    struct inbytes {
        char* const filename;
        uint8_t const mode;
        size_t const data_size;
        uint8_t* const data;
    } const inbytes = {
        .filename = buffer,
        .mode = UART1_Read(),
        .data_size = UART1_read_u32(),
        .data = buffer,
    };
    struct outbytes {
        FRESULT result;
        size_t bytes_written;
    } outbytes = {0};


    get_filename();
    keep_first_error(&outbytes.result, f_mount(&drive, "0:", 1));
    keep_first_error(
        &outbytes.result,
        f_open(&file, inbytes.filename, FA_WRITE | inbytes.mode)
    );

    UART1_Write(outbytes.result);

    if (outbytes.result) {
        return DO_NOT_BOTHER;
    }

    for (
        size_t block_size, remaining = inbytes.data_size;
        block_size = remaining > BUF_SIZE ? BUF_SIZE : remaining,
        remaining;
        remaining -= block_size
    ) {
        for (size_t i = 0; i < block_size; ++i) {
            inbytes.data[i] = UART1_Read();
        }

        uint8_t written = 0;
        keep_first_error(
            &outbytes.result,
            f_write(&file, inbytes.data, block_size, &written)
        );
        outbytes.bytes_written += written;
    }

    keep_first_error(&outbytes.result, f_close(&file));
    keep_first_error(&outbytes.result, f_mount(0, "0:", 0));

    UART1_write_u32(outbytes.bytes_written);
    UART1_Write(outbytes.result);

    return DO_NOT_BOTHER;
}

response_t SDCARD_read_file(void) {
    struct inbytes {
        char* const filename;
    } const inbytes = {
        .filename = buffer,
    };
    struct outbytes {
        FRESULT result;
        size_t bytes_read;
        char* data;
    } outbytes = {
        .result = 0,
        .bytes_read = 0,
        .data = buffer,
    };

    get_filename();
    keep_first_error(&outbytes.result, f_mount(&drive, "0:", 1));
    keep_first_error(&outbytes.result, f_open(&file, inbytes.filename, FA_READ));
    FILINFO info = {0};
    keep_first_error(&outbytes.result, f_stat(inbytes.filename, &info));
    UART1_write_u32(info.fsize);

    for (
        size_t block_size, remaining = info.fsize;
        block_size = remaining > BUF_SIZE ? BUF_SIZE : remaining,
        remaining;
        remaining -= block_size
    ) {
        uint8_t read = 0;
        keep_first_error(&outbytes.result, f_read(&file, &buffer, block_size, &read));

        for (size_t i = 0; i < block_size; ++i) {
            UART1_Write(buffer[i]);
        }

        outbytes.bytes_read += read;
    }

    keep_first_error(&outbytes.result, f_close(&file));
    keep_first_error(&outbytes.result, f_mount(0, "0:", 0));

    UART1_write_u32(outbytes.bytes_read);
    UART1_Write(outbytes.result);

    return DO_NOT_BOTHER;
}
