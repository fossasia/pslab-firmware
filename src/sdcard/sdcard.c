#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../bus/uart/uart.h"
#include "../commands.h"
#include "fatfs/ff.h"
#include "fatfs/ffconf.h"
#include "../helpers/debug.h"
#include "../registers/system/watchdog.h"

#define SFN_MAX 8
#define SFN_SUFFIX_LEN 4 // Period + at most three chars.
#define BUF_MAX FF_MIN_SS

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
    // +1 is null-terminator.
    size_t const filename_size = SFN_MAX + SFN_SUFFIX_LEN + 1;
    TCHAR filename[filename_size];
    get_filename(filename, filename_size);
    BYTE const mode = UART1_Read();

    FATFS drive;
    DEBUG_write_u8(f_mount(&drive, "0:", 1));

    FIL file;
    // Host must wait for f_open before sending data.
    UART1_Write(f_open(&file, filename, FA_WRITE | mode));

    FSIZE_t data_size = UART1_read_u32();
    FSIZE_t bytes_written = 0;

    for (
        FSIZE_t block_size, remaining = data_size;
        block_size = remaining > BUF_MAX ? BUF_MAX : remaining,
        remaining;
        remaining -= block_size
    ) {
        TCHAR buffer[block_size];

        for (UINT i = 0; i < block_size; ++i) {
            buffer[i] = UART1_Read();
        }

        WATCHDOG_TimerClear();
        UINT written = 0;
        // Host must wait for f_write before sending more data.
        UART1_Write(f_write(&file, buffer, (UINT)block_size, &written));
        bytes_written += written;
    }

    UART1_write_u32(bytes_written);
    DEBUG_write_u8(f_close(&file));
    DEBUG_write_u8(f_mount(0, "0:", 0));

    return DO_NOT_BOTHER;
}

response_t SDCARD_read_file(void) {
    TCHAR filename[SFN_MAX + SFN_SUFFIX_LEN + 1]; // +1 is null-terminator.
    get_filename(filename, sizeof filename);

    FATFS drive;
    DEBUG_write_u8(f_mount(&drive, "0:", 1));

    FIL file;
    DEBUG_write_u8(f_open(&file, filename, FA_READ));

    FILINFO info = {0, 0, 0, 0, {0}};
    DEBUG_write_u8(f_stat(filename, &info));
    UART1_write_u32(info.fsize);
    FSIZE_t bytes_read = 0;

    for (
        FSIZE_t block_size, remaining = info.fsize;
        block_size = remaining > BUF_MAX ? BUF_MAX : remaining,
        remaining;
        remaining -= block_size
    ) {
        WATCHDOG_TimerClear();
        UINT read = 0;
        TCHAR buffer[block_size];
        f_read(&file, &buffer, (UINT)block_size, &read);
        bytes_read += read;

        for (UINT i = 0; i < block_size; ++i) {
            UART1_Write(buffer[i]);
        }
    }

    UART1_write_u32(bytes_read);
    DEBUG_write_u8(f_close(&file));
    DEBUG_write_u8(f_mount(0, "0:", 0));

    return DO_NOT_BOTHER;
}

response_t SDCARD_get_file_info(void) {
    TCHAR filename[SFN_MAX + SFN_SUFFIX_LEN + 1]; // +1 is null-terminator.
    get_filename(filename, sizeof filename);

    FATFS drive;
    DEBUG_write_u8(f_mount(&drive, "0:", 1));

    FILINFO info = {0, 0, 0, 0, {0}};
    DEBUG_write_u8(f_stat(filename, &info));

    UART1_write_u32(info.fsize);
    UART1_WriteInt(info.fdate);
    UART1_WriteInt(info.ftime);
    UART1_Write(info.fattrib);

    DEBUG_write_u8(f_mount(0, "0:", 0));

    return SUCCESS;
}
