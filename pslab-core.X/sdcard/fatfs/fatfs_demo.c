#include "ff.h"
#include "../sd_spi.h"

static FATFS drive;
static FIL file;

void FatFsDemo_Tasks(void) {
    UINT actualLength;
    char data[] = "Hello World!";
    if (SD_SPI_IsMediaPresent() == false) {
        return;
    }

    if (f_mount(&drive, "0:", 1) == FR_OK) {
        if (f_open(&file, "HELLO.TXT", FA_WRITE | FA_CREATE_NEW) == FR_OK) {
            f_write(&file, data, sizeof (data) - 1, &actualLength);
            f_close(&file);
        }

        f_mount(0, "0:", 0);
    }
}
