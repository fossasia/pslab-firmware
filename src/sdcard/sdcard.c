#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../bus/uart/uart.h"
#include "../commands.h"
#include "../helpers/debug.h"
#include "../registers/system/watchdog.h"
#include "fatfs/ff.h"
#include "fatfs/ffconf.h"
#include "sd_spi.h"

#define SFN_MAX 8
#define SFN_SUFFIX_LEN 4 // Period + at most three chars.
#define FILENAME_SIZE (SFN_MAX + SFN_SUFFIX_LEN + 1)
#define BUF_MAX FF_MIN_SS
#define SDCARD_DRIVE "0:"

static TCHAR s_sector_buf[BUF_MAX];
_Static_assert(
    sizeof(s_sector_buf) == 512,
    "SD sector buffer must be 512 bytes"
);

static void get_filename(TCHAR *const fn_buf, UINT const size)
{
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
    TCHAR filename[FILENAME_SIZE];
    get_filename(filename, FILENAME_SIZE);
    BYTE const mode = UART1_Read();

    if (!SD_SPI_IsMediaPresent()) {
        return FR_NOT_READY;
    }
    FATFS drive;
    DEBUG_write_u8(f_mount(&drive, SDCARD_DRIVE, 1));

    FIL file;
    // Host must wait for f_open before sending data.
    UART1_Write(f_open(&file, filename, FA_WRITE | mode));

    FSIZE_t data_size = UART1_read_u32();
    FSIZE_t bytes_written = 0;

    for (FSIZE_t block_size, remaining = data_size;
         block_size = remaining > BUF_MAX ? BUF_MAX : remaining, remaining;
         remaining -= block_size) {

        for (UINT i = 0; i < block_size; ++i) {
            s_sector_buf[i] = UART1_Read();
        }

        WATCHDOG_TimerClear();
        UINT written = 0;
        // Host must wait for f_write before sending more data.
        UART1_Write(f_write(&file, s_sector_buf, (UINT)block_size, &written));
        bytes_written += written;
    }

    UART1_write_u32(bytes_written);
    DEBUG_write_u8(f_close(&file));
    DEBUG_write_u8(f_mount(0, SDCARD_DRIVE, 0));

    return DO_NOT_BOTHER;
}

response_t SDCARD_read_file(void)
{
    TCHAR filename[SFN_MAX + SFN_SUFFIX_LEN + 1]; // +1 is null-terminator.
    get_filename(filename, sizeof filename);

    if (!SD_SPI_IsMediaPresent()) {
        UART1_Write(FR_NOT_READY);
        return DO_NOT_BOTHER;
    }
    FATFS drive;
    DEBUG_write_u8(f_mount(&drive, SDCARD_DRIVE, 1));

    FIL file;
    DEBUG_write_u8(f_open(&file, filename, FA_READ));

    FILINFO info = {0, 0, 0, 0, {0}};
    DEBUG_write_u8(f_stat(filename, &info));
    UART1_write_u32(info.fsize);
    FSIZE_t bytes_read = 0;

    for (FSIZE_t block_size, remaining = info.fsize;
         block_size = remaining > BUF_MAX ? BUF_MAX : remaining, remaining;
         remaining -= block_size) {
        WATCHDOG_TimerClear();
        UINT read = 0;
        FRESULT read_result = f_read(&file, s_sector_buf, (UINT)block_size, &read);
        bytes_read += read;

        for (UINT i = 0; i < read; ++i) {
            UART1_Write(s_sector_buf[i]);
        }

        if (read_result != FR_OK || read < block_size) {
            break;
        }
    }

    UART1_write_u32(bytes_read);
    DEBUG_write_u8(f_close(&file));
    DEBUG_write_u8(f_mount(0, SDCARD_DRIVE, 0));

    return DO_NOT_BOTHER;
}

response_t SDCARD_get_file_info(void)
{
    TCHAR filename[SFN_MAX + SFN_SUFFIX_LEN + 1]; // +1 is null-terminator.
    get_filename(filename, sizeof filename);

    if (!SD_SPI_IsMediaPresent()) {
        return FR_NOT_READY;
    }
    FATFS drive;
    DEBUG_write_u8(f_mount(&drive, SDCARD_DRIVE, 1));

    FILINFO info = {0, 0, 0, 0, {0}};
    DEBUG_write_u8(f_stat(filename, &info));

    UART1_write_u32(info.fsize);
    UART1_WriteInt(info.fdate);
    UART1_WriteInt(info.ftime);
    UART1_Write(info.fattrib);

    DEBUG_write_u8(f_mount(0, SDCARD_DRIVE, 0));

    return SUCCESS;
}
