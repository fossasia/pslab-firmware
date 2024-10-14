#ifndef _DEBUG_H
#define _DEBUG_H

#include "../commands.h"

// Enable debug messages over serial with -DENABLE_DEBUG CMake flag.

#ifdef SERIAL_DEBUG
    #define DEBUG_write_u8(byte) UART2_Write(byte)
    #define DEBUG_write_u16(word) UART2_WriteInt(word)
    #define DEBUG_write_u32(dword) UART2_write_u32(dword)
#else
    #define DEBUG_write_u8(byte) ((void)byte)
    #define DEBUG_write_u16(word) ((void)word)
    #define DEBUG_write_u32(dword) ((void)dword)
#endif // SERIAL_DEBUG

/**
 * @brief Tell host whether debug messages are enabled.
 *
 * @details If the firmware was built with -DENABLE_DEBUG, extra messages
 *     containing information that may be useful in debugging are sent over
 *     serial, in addition to the messages which are normally sent. The host
 *     must take this into consideration when communicating with the PSLab, so
 *     that the correct number of bytes are read for each transaction.
 *
 * @return is_debug_enabled bool
 *
 * @return DO_NOT_BOTHER response_t
 */
response_t DEBUG_is_enabled(void);

#endif // _DEBUG_H
