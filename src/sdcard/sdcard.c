#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../bus/uart/uart.h"
#include "../commands.h"
#include "fatfs/ff.h"
#include "fatfs/ffconf.h"
#include "../helpers/debug.h"
#include "../registers/system/watchdog.h"

#define BUF_SIZE FF_MIN_SS

static FATFS drive;
static FIL file;
static TCHAR buffer[BUF_SIZE];

static void get_filename(TCHAR* const fn_buf, UINT const size) {
    for (UINT i = 0; i < size; ++i) {
        if (!(fn_buf[i] = UART1_Read())) {
            // Received null-terminator.
            return;
        }
    }
    // FatFS handles non-terminated strings gracefully, so no need to do a
    // manual check for null-termination.
}

response_t SDCARD_write_file(void) {
    TCHAR* const filename = buffer;
    get_filename(filename, BUF_SIZE);
    BYTE const mode = UART1_Read();
    DEBUG_write_u8(f_mount(&drive, "0:", 1));
    // Host must wait for f_open before sending data.
    UART1_Write(f_open(&file, filename, FA_WRITE | mode));

    FSIZE_t data_size = UART1_read_u32();
    FSIZE_t bytes_written = 0;

    for (
        FSIZE_t block_size, remaining = data_size;
        block_size = remaining > BUF_SIZE ? BUF_SIZE : remaining,
        remaining;
        remaining -= block_size
    ) {
        for (UINT i = 0; i < block_size; ++i) {
            buffer[i] = UART1_Read();
        }

        WATCHDOG_TimerClear();
        UINT written = 0;
        // Host must wait for f_write before sending more data.
        UART1_Write(f_write(&file, buffer, (UINT)block_size, &written));
        bytes_written += written;
        DEBUG_write_u32(remaining);
    }

    UART1_write_u32(bytes_written);
    DEBUG_write_u8(f_close(&file));
    DEBUG_write_u8(f_mount(0, "0:", 0));

    return DO_NOT_BOTHER;
}

response_t SDCARD_read_file(void) {
    TCHAR* const filename = buffer;
    get_filename(filename, BUF_SIZE);
    DEBUG_write_u8(f_mount(&drive, "0:", 1));
    DEBUG_write_u8(f_open(&file, filename, FA_READ));

    FILINFO info = {0, 0, 0, 0, {0}};
    DEBUG_write_u8(f_stat(filename, &info));
    UART1_write_u32(info.fsize);
    FSIZE_t bytes_read = 0;

    for (
        FSIZE_t block_size, remaining = info.fsize;
        block_size = remaining > BUF_SIZE ? BUF_SIZE : remaining,
        remaining;
        remaining -= block_size
    ) {
        WATCHDOG_TimerClear();
        UINT read = 0;
        f_read(&file, &buffer, (UINT)block_size, &read);
        bytes_read += read;

        for (UINT i = 0; i < block_size; ++i) {
            UART1_Write(buffer[i]);
        }

        DEBUG_write_u32(remaining);
    }

    UART1_write_u32(bytes_read);
    DEBUG_write_u8(f_close(&file));
    DEBUG_write_u8(f_mount(0, "0:", 0));

    return DO_NOT_BOTHER;
}
