#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "types.h"

#include "../commands.h"
#include "fatfs/ff.h"
#include "fatfs/ffconf.h"

#define SFN_MIN 2 // At least one character + null terminator.
#define SFN_NAME 8
#define SFN_SUFFIX_LEN 4 // Period + at most three chars.
#define SFN_MAX (SFN_NAME + SFN_SUFFIX_LEN + 1) // + null terminator.
#define BUF_MAX FF_MIN_SS

FATFS *g_drive_p = NULL;
FIL *g_file_p = NULL;

enum Status SDCARD_mount(
    uint8_t *const args,
    __attribute__((unused)) uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {

    FRESULT const res = f_mount(g_drive_p, "0:", 1);
    *rets = args;
    *rets_size = sizeof(res);
    memcpy(*rets, &res, *rets_size);
    return res ? E_FAILED : E_OK;
}

enum Status SDCARD_unmount(
    uint8_t *const args,
    __attribute__((unused)) uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    g_drive_p = NULL;
    FRESULT const res = f_mount(g_drive_p, "0:", 0);
    *rets = args;
    *rets_size = sizeof(res);
    memcpy(*rets, &res, *rets_size);
    return res ? E_FAILED : E_OK;
}

enum Status SDCARD_stat(
    uint8_t *const args,
    uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    struct Input {
        uint8_t fn_size;
        char fn[];
    } *input = (struct Input *)args;

    if (args_size < sizeof(input->fn_size)) {
        return E_BAD_ARGSIZE;
    }

    if (input->fn_size < SFN_MIN) {
        return E_BAD_SIZE;
    }

    if (input->fn_size > SFN_MAX) {
        return E_BAD_SIZE;
    }

    struct Output {
        FILINFO info;
        FRESULT res;
    } output = {.info = {0}, .res = 0};
    output.res = f_stat(input->fn, &output.info);
    *rets = args;
    *rets_size = sizeof(output);
    memcpy(*rets, &output, *rets_size);
    return output.res ? E_FAILED : E_OK;
}

enum Status SDCARD_open(
    uint8_t *const args,
    uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    struct Input {
        uint8_t fn_size;
        uint8_t mode;
        char fn[];
    } *input = (struct Input *)args;

    if (args_size < sizeof(input->fn_size)) {
        return E_BAD_ARGSIZE;
    }

    if (input->fn_size < SFN_MIN) {
        return E_BAD_SIZE;
    }

    if (input->fn_size > SFN_MAX) {
        return E_BAD_SIZE;
    }

    FRESULT const res = f_open(g_file_p, input->fn, input->mode);
    *rets = args;
    *rets_size = sizeof(res);
    memcpy(*rets, &res, *rets_size);
    return res ? E_FAILED : E_OK;
}

enum Status SDCARD_close(
    uint8_t *const args,
    __attribute__((unused)) uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    FRESULT const res = f_close(g_file_p);
    g_file_p = NULL;
    *rets = args;
    *rets_size = sizeof(res);
    memcpy(*rets, &res, *rets_size);
    return res ? E_FAILED : E_OK;
}

enum Status SDCARD_write(
    uint8_t *const args,
    uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    struct Input {
            uint16_t data_size;
            uint8_t data[];
    } *input = (struct Input *)args;;

    if (args_size < sizeof(input->data_size)) {
        return E_BAD_ARGSIZE;
    }

    if (args_size != (sizeof(input->data_size) + input->data_size)) {
        return E_BAD_ARGSIZE;
    }

    if (input->data_size > BUF_MAX) {
        return E_BAD_SIZE;
    }

    struct Output {
        FRESULT res;
        uint16_t written;
    } output = {0};
    output.res = f_write(g_file_p, input->data, input->data_size, &output.written);
    *rets = args;
    *rets_size = sizeof(output);
    memcpy(*rets, &output, *rets_size);
    return output.res ? E_FAILED : E_OK;
}

enum Status SDCARD_read(
    uint8_t *const args,
    uint16_t const args_size,
    uint8_t **rets,
    uint16_t *rets_size
) {
    uint16_t data_size = 0;

    if (args_size != sizeof(data_size)) {
        return E_BAD_ARGSIZE;
    }

    data_size = *(uint16_t*)args;

    union Output {
        struct {
            FRESULT res;
            uint16_t num_bytes_read;
            uint8_t *data[];
        };
        uint8_t *buffer;
    } output = {{0}};
    *rets = args;
    output.buffer = *rets;
    output.res = f_read(
        g_file_p,
        output.data,
        data_size,
        &output.num_bytes_read
    );
    *rets_size = sizeof(output) + output.num_bytes_read;
    memcpy(*rets, &output, *rets_size);
    return output.res ? E_FAILED : E_OK;
}
