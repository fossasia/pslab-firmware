#include <xc.h>
#include "uart.h"
#include "../../helpers/delay.h"
#include "../../commands.h"
#include "../../registers/system/watchdog.h"

/**********/
/* Macros */
/**********/
#define UART_READ_TIMEOUT (FCY / 10UL)  // 100 ms

/**************/
/* Interrupts */
/**************/

/**
 * @brief Move incoming data on UART1 to TX of UART2.
 */
void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void) {
    U2TXREG = U1RXREG;
    IFS0bits.U1RXIF = 0;
}

/**
 * @brief Move incoming data on UART2 to TX of UART1.
 */
void __attribute__((__interrupt__, no_auto_psv)) _U2RXInterrupt(void) {
    U1TXREG = U2RXREG;
    IFS1bits.U2RXIF = 0;
}

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
 * @brief Suitable values for UxBRG register.
 * @details UxBRG = [FCY / (4 * BAUD)] - 1
*/
typedef enum {
    BAUD1000000 = 15,
    BAUD576000 = 27,
    BAUD500000 = 31,
    BAUD460800 = 34,
    BAUD230400 = 68,
    BAUD115200 = 138,
    BAUD57600 = 277,
    BAUD38400 = 416,
    BAUD19200 = 832,
    BAUD9600 = 1666,
    BAUD4800 = 3332,
    BAUD2400 = 6666,
    BAUD1800 = 8888,
    BAUD1200 = 13332,
    BAUD600 = 26666,
    BAUD300 = 53332,
} EBaudrate;

/**
 * @brief Possible values of UxMODE.STSEL field.
*/
typedef enum {
    ONE_STOP_BIT,
    TWO_STOP_BITS,
} EStopBits;

/**
 * @brief Possible values of UxMODE.PDSEL field.
*/
typedef enum {
    NO_PARITY,
    EVEN_PARITY,
    ODD_PARITY,
    // Unimplemented; Not supported by pyserial.
    // NINE_BIT_DATA,
} EParity;

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
*/
static sUartRegs get_registers(const EUxSelect select) {
    return select ? UART2_REGS : UART1_REGS;
}

/**
 * @brief Set parity.
 *
 * If parity is enabled, one additional bit is sent after the data bits. This
 * bit is used to detect if the data changed during transmission. If the
 * parity mode is even, the total number of 1's in the UART frame (including
 * the parity bit) must be even. If the parity mode is odd, the number of 1's
 * in the UART frame (including the parity bit) must be odd.
 *
 * The default mode is no parity.
 *
 * @param select Either U1SELECT or U2SELECT.
 * @param bits   One of NO_PARITY, ODD_PARITY, or EVEN_PARITY.
 */
static void set_parity(const EUxSelect select, const EParity parity) {
    get_registers(select).modebitsptr->PDSEL = parity;
}

/**
 * @brief Set number of stop bits.
 *
 * A UART frame ends when the transmitter sets its Tx line high for at least
 * one bit period after sending all data bits. This is called a stop bit.
 * Optionally, more than one stop bit can be used, which may make transmission
 * more resilient to e.g. small differences in clock rate between transmitter
 * and reciever.
 *
 * The default is one stop bit.
 *
 * @param select Either U1SELECT or U2SELECT.
 * @param bits   Either ONE_STOP_BIT or TWO_STOP_BITS.
 */
static void set_stop(const EUxSelect select, const EStopBits stop) {
    get_registers(select).modebitsptr->STSEL = stop;
}

/**
 * @brief Set baudrate.
 * @param select Either U1SELECT or U2SELECT.
 * @param baud   A baudrate as defined in tBaudrate.
 */
static void set_baud(const EUxSelect select, const EBaudrate baud) {
    *get_registers(select).brgptr = baud;
}

/**
 * @brief Set enable bit of UARTx.
 * @param select Either U1SELECT or U2SELECT.
 */
static void enable_uartx(const EUxSelect select) {
    const sUartRegs regs = get_registers(select);
    regs.modebitsptr->UARTEN = 1;
    regs.stabitsptr->UTXEN = 1;
}

/**
 * @brief Enable UART1 and UART2 RX interrupts.
 *
 * The RX interrupts are used in passthrough mode. Interrupt flags are cleared
 * before enabling the interrupts.
 */
static void enable_interrupts(void)
{
    IFS0bits.U1RXIF = 0; // Clear U1RX interrupt flag.
    IEC0bits.U1RXIE = 1; // Enable U1RX interrupt.
    IFS1bits.U2RXIF = 0; // Clear U2RX interrupt flag.
    IEC1bits.U2RXIE = 1; // Enable U2RX interrupt.
}

/********************/
/* Public functions */
/********************/

void UART_initialize(const EUxSelect select) {
    const sUartRegs regs = get_registers(select);
    *regs.modebitsptr = UART_DEFAULT_CONF.modebits;
    *regs.stabitsptr = UART_DEFAULT_CONF.stabits;
    *regs.brgptr = UART_DEFAULT_CONF.brgval;
    enable_uartx(select);
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
        (*num_bytes_read)++;
    }

    return E_OK;
}

enum Status UART_write(
    EUxSelect const select,
    uint8_t const *const buffer,
    uint16_t const size
) {
    // If either buffer is NULL or size is zero, this is a NOP.
    if (!buffer) {return E_OK;}
    if (!size) {return E_OK;}

    sUartRegs const regs = get_registers(select);

    for (uint16_t i = 0; i < size; ++i) {
        while (regs.stabitsptr->UTXBF) {;}
        *regs.txptr = buffer[i];
    }

    return E_OK;
}

enum Status UART_flush_rx(EUxSelect const select)
{
    sUartRegs const regs = get_registers(select);
    while(regs.stabitsptr->URXDA || !regs.stabitsptr->RIDLE) {*regs.rxptr;}
    return E_OK;
}

/*********************/
/* Command functions */
/*********************/

response_t UART2_read_u8(void) {
    UART_write(U1SELECT, (uint8_t const *const)&U2RXREG, sizeof(U2RXREG));
    return DO_NOT_BOTHER;
}


response_t UART2_read_u16(void) {
    UART_write(U1SELECT, (uint8_t const *const)&U2RXREG, sizeof(U2RXREG));
    return DO_NOT_BOTHER;
}

response_t UART2_write_u8(void) {
    UART_write(U2SELECT, (uint8_t const *const)&U1RXREG, sizeof(U1RXREG));
    return DO_NOT_BOTHER;
}

response_t UART2_write_u16(void) {
    UART_write(U2SELECT, (uint8_t const *const)&U1RXREG, sizeof(U1RXREG));
    return DO_NOT_BOTHER;
}

response_t UART2_rx_ready(void) {
    uint8_t const rx_ready = U2STAbits.URXDA;
    UART_write(U1SELECT, &rx_ready, sizeof(rx_ready));
    return DO_NOT_BOTHER;
}

response_t UART2_set_baud(void) {
    UART_read(U1SELECT, (uint8_t *const)&U2BRG, sizeof(U2BRG));
    return SUCCESS;
}

response_t UART2_set_mode(void) {
    uint16_t mode = 0;
    UART_read(U1SELECT, (uint8_t *const)&mode, sizeof(mode));
    EStopBits const stop_bits = mode & 1;
    EParity const parity_data_bits = (mode & 6) >> 1;
    set_stop(U2SELECT, stop_bits);
    set_parity(U2SELECT, parity_data_bits);
    return SUCCESS;
}

response_t UART_Passthrough(void) {
    uint16_t baud = 0;
    UART_read(U1SELECT, (uint8_t *const)&baud, sizeof(baud));
    set_baud(U1SELECT, baud);
    set_baud(U2SELECT, baud);
    enable_interrupts();
    return DO_NOT_BOTHER;
}
