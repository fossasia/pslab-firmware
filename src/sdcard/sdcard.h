#ifndef _SDCARD_H
#define _SDCARD_H

#include "../commands.h"

response_t SDCARD_write_file(void);
response_t SDCARD_read_file(void);

#endif // _SDCARD_H
