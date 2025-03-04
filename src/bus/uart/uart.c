#include <stdbool.h>
#include <stddef.h>

#include <xc.h>

#include "uart.h"
#include "../../helpers/delay.h"
#include "../../commands.h"
#include "../../registers/system/watchdog.h"

/**********/
/* Macros */
/**********/
#define UART_READ_TIMEOUT (FCY / 10UL)  // 100 ms
#define UART_WRITE_TIMEOUT UART_READ_TIMEOUT

/*********/
/* Types */
/*********/

/**
 * @brief Fields of UxMODE register.
*/
typedef struct
{
    uint16_t STSEL : 1;
    uint16_t PDSEL : 2;
    uint16_t BRGH : 1;
    uint16_t URXINV : 1;
    uint16_t ABAUD : 1;
    uint16_t LPBACK : 1;
    uint16_t WAKE : 1;
    uint16_t UEN : 2;
    uint16_t : 1;
    uint16_t RTSMD : 1;
    uint16_t IREN : 1;
    uint16_t USIDL : 1;
    uint16_t : 1;
    uint16_t UARTEN : 1;
} sUxModeBits;
extern volatile sUxModeBits u1_mode_bits __attribute__((sfr(0x220)));
extern volatile sUxModeBits u2_mode_bits __attribute__((sfr(0x230)));

/**
 * @brief Fields of UxSTA register.
*/
typedef struct {
    uint16_t URXDA : 1;
    uint16_t OERR : 1;
    uint16_t FERR : 1;
    uint16_t PERR : 1;
    uint16_t RIDLE : 1;
    uint16_t ADDEN : 1;
    uint16_t URXISEL : 2;
    uint16_t TRMT : 1;
    uint16_t UTXBF : 1; /** Transmit buffer full. */
    uint16_t UTXEN : 1;
    uint16_t UTXBRK : 1;
    uint16_t : 1;
    uint16_t UTXISEL0 : 1;
    uint16_t UTXINV : 1;
    uint16_t UTXISEL1 : 1;
} sUxStaBits;
extern volatile sUxStaBits u1_sta_bits __attribute__((sfr(0x222)));
extern volatile sUxStaBits u2_sta_bits __attribute__((sfr(0x232)));

/**
 * @brief Collection type for UART register pointers.
*/
typedef struct {
    volatile sUxModeBits* const modebitsptr;
    volatile sUxStaBits* const stabitsptr;
    volatile EBaudrate* const brgptr;
    volatile uint16_t* const txptr;
    volatile uint16_t* const rxptr;
} sUartRegs;

/*************/
/* Constants */
/*************/

/**
 * @brief Collection of UART1 register pointers.
*/
static const sUartRegs UART1_REGS = {
    .modebitsptr = &u1_mode_bits,
    .stabitsptr = &u1_sta_bits,
    .brgptr = &U1BRG,
    .txptr = &U1TXREG,
    .rxptr = &U1RXREG,
};

/**
 * @brief Collection of UART2 register pointers.
*/
static const sUartRegs UART2_REGS = {
    .modebitsptr = &u2_mode_bits,
    .stabitsptr = &u2_sta_bits,
    .brgptr = &U2BRG,
    .txptr = &U2TXREG,
    .rxptr = &U2RXREG,
};

/**
 * @brief Collection of UART configuration values.
*/
static struct UartConf {
    sUxModeBits modebits;
    sUxStaBits stabits;
    EBaudrate brgval;
} const UART_DEFAULT_CONF = {
    .modebits = {
          /* UART is disabled; all UART pins are controlled by PORT latches;
             UART power consumption is minimal. */
          .UARTEN = 0,
          // Continues module operation in Idle mode.
          .USIDL = 0,
          // IrDA encoder and decoder are disabled.
          .IREN = 0,
          // U2RTS pin is in Simplex mode.
          .RTSMD = 1,
          /* UxTX and UxRX pins are enabled and used; UxCTS and UxRTS/BCLKx
             pins are controlled by PORT latches. */
          .UEN = 0b00,
          // No wake-up is enabled.
          .WAKE = 0,
          // Loop-back mode is disabled.
          .LPBACK = 0,
          // Baud rate measurement is disabled or completed.
          .ABAUD = 0,
          // UxRX Idle state is '1'.
          .URXINV = 0,
          /* BRG generates 4 clocks per bit period (4x baud clock, High-Speed
             mode). */
          .BRGH = 1,
          // 8-bit data, no parity.
          .PDSEL = 0,
          // One Stop bit.
          .STSEL = 0,
        },
    .stabits = {
        /* Interrupt when a character is transferred to the Transmit Shift
           Register. This implies that at least one character remains in the
           transmit buffer. */
        .UTXISEL1 = 0,
        .UTXISEL0 = 0,
        // UxTX Idle state is '1'.
        .UTXINV = 0,
        // Sync Break transmission is disabled or completed.
        .UTXBRK = 0,
        /* Transmit is disabled, any pending transmission is aborted and
           buffer is reset; UxTX pin is controlled by the PORT. */
        .UTXEN = 0,
        /* Transmit buffer is not full, at least one more character can be
           written. */
        .UTXBF = 0,
        /* Transmit Shift Register isn't empty, a transmission is in
           progress/queued. */
        .TRMT = 0,
        /* Interrupt is set when any character is received and transferred
           from the UxRSR to the receive buffer; receive buffer has one or
           more characters. */
        .URXISEL = 0b00,
        // Address Detect mode is disabled.
        .ADDEN = 0,
        // Receiver is active.
        .RIDLE = 0,
        // Parity error has not been detected.
        .PERR = 0,
        // Framing error has not been detected.
        .FERR = 0,
        /* Receive buffer has not overflowed; clearing a previously set
           OERRbit (1 -> 0 transition) resets the RX buffer and UxRSR to the
           empty state. */
        .OERR = 0,
        // Receive buffer is empty.
        .URXDA = 0,
    },
    .brgval = BAUD1000000,
};

/********************/
/* Static functions */
/********************/

/**
 * @brief Getter for register pointer collections.
 *
 * @param EUxSelect select
 *
 * @return sUartRegs registers
*/
static sUartRegs get_registers(const EUxSelect select) {
    return select ? UART2_REGS : UART1_REGS;
}

/**
 * @brief Set enable bit of UARTx.
 *
 * @param EUxSelect select
 */
static void enable_uartx(const EUxSelect select) {
    const sUartRegs regs = get_registers(select);
    regs.modebitsptr->UARTEN = 1;
    regs.stabitsptr->UTXEN = 1;
}

/********************/
/* Public functions */
/********************/

enum Status UART_initialize(const EUxSelect select) {
    const sUartRegs regs = get_registers(select);
    *regs.modebitsptr = UART_DEFAULT_CONF.modebits;
    *regs.stabitsptr = UART_DEFAULT_CONF.stabits;
    *regs.brgptr = UART_DEFAULT_CONF.brgval;
    enable_uartx(select);
    return E_OK;
}

enum Status UART_set_parity(const EUxSelect select, const EParity parity) {
    get_registers(select).modebitsptr->PDSEL = parity;
    return E_OK;
}

enum Status UART_set_stop(const EUxSelect select, const EStopBits stop) {
    get_registers(select).modebitsptr->STSEL = stop;
    return E_OK;
}

enum Status UART_set_baud(const EUxSelect select, const EBaudrate baud) {
    *get_registers(select).brgptr = baud;
    return E_OK;
}

enum Status UART_read(
    EUxSelect const select,
    uint8_t *const buffer,
    uint16_t const buffer_size,
    uint16_t *const num_bytes_read
) {
    // If either buffer is NULL or size is zero, this is a NOP.
    if (!buffer) {return E_OK;}
    if (!buffer_size) {return E_OK;}

    sUartRegs const regs = get_registers(select);
    uint32_t timeout = 0;

    for (uint16_t i = 0; i < buffer_size; ++i) {
        if (regs.stabitsptr->FERR) {return E_UART_RX_FRAMING;}
        if (regs.stabitsptr->OERR) {return E_UART_RX_OVERRUN;}
        if (regs.stabitsptr->PERR) {return E_UART_RX_PARITY;}

        while (!regs.stabitsptr->URXDA) {
            WATCHDOG_TimerClear();
            if (timeout++ >= UART_READ_TIMEOUT) {return E_UART_RX_TIMEOUT;}
        }

        timeout = 0;
        buffer[i] = *regs.rxptr;

        if (num_bytes_read) { (*num_bytes_read)++; }
    }

    return E_OK;
}

enum Status UART_write(
    EUxSelect const select,
    uint8_t const *const buffer,
    uint16_t const buffer_size,
    uint16_t *const num_bytes_written
) {
    // If either buffer is NULL or size is zero, this is a NOP.
    if (!buffer) {return E_OK;}
    if (!buffer_size) {return E_OK;}

    sUartRegs const regs = get_registers(select);
    uint32_t timeout = 0;

    for (uint16_t i = 0; i < buffer_size; ++i) {
        while (regs.stabitsptr->UTXBF) {
            WATCHDOG_TimerClear();
            if (timeout++ >= UART_WRITE_TIMEOUT) {return E_UART_TX_TIMEOUT;}
        }

        timeout = 0;
        *regs.txptr = buffer[i];

        if (num_bytes_written) { (*num_bytes_written)++; }
    }

    return E_OK;
}

enum Status UART_flush_rx(EUxSelect const select)
{
    sUartRegs const regs = get_registers(select);
    while(regs.stabitsptr->URXDA || !regs.stabitsptr->RIDLE) {*regs.rxptr;}
    return E_OK;
}

enum Status UART_rx_ready(
    EUxSelect const select,
    bool *const is_data_available
) {
    sUartRegs const regs = get_registers(select);
    *is_data_available = regs.stabitsptr->URXDA;
    return E_OK;
}
