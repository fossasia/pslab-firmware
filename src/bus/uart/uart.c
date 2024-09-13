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
static sUartRegs GetRegisters(const EUxSelect select) {
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
static void SetParity(const EUxSelect select, const EParity parity) {
    GetRegisters(select).modebitsptr->PDSEL = parity;
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
static void SetStop(const EUxSelect select, const EStopBits stop) {
    GetRegisters(select).modebitsptr->STSEL = stop;
}

/**
 * @brief Set baudrate.
 * @param select Either U1SELECT or U2SELECT.
 * @param baud   A baudrate as defined in tBaudrate.
 */
static void SetBaud(const EUxSelect select, const EBaudrate baud) {
    *GetRegisters(select).brgptr = baud;
}

/**
 * @brief Set enable bit of UARTx.
 * @param select Either U1SELECT or U2SELECT.
 */
static void EnableUx(const EUxSelect select) {
    const sUartRegs regs = GetRegisters(select);
    regs.modebitsptr->UARTEN = 1;
    regs.stabitsptr->UTXEN = 1;
}

/**
 * @brief Enable UART1 and UART2 RX interrupts.
 *
 * The RX interrupts are used in passthrough mode. Interrupt flags are cleared
 * before enabling the interrupts.
 */
static void EnableInterrupts(void)
{
    IFS0bits.U1RXIF = 0; // Clear U1RX interrupt flag.
    IEC0bits.U1RXIE = 1; // Enable U1RX interrupt.
    IFS1bits.U2RXIF = 0; // Clear U2RX interrupt flag.
    IEC1bits.U2RXIE = 1; // Enable U2RX interrupt.
}

/********************/
/* Public functions */
/********************/

void UART_Initialize(const EUxSelect select) {
    const sUartRegs regs = GetRegisters(select);
    *regs.modebitsptr = UART_DEFAULT_CONF.modebits;
    *regs.stabitsptr = UART_DEFAULT_CONF.stabits;
    *regs.brgptr = UART_DEFAULT_CONF.brgval;
    EnableUx(select);
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

    sUartRegs const regs = GetRegisters(select);
    uint16_t timeout = 0;

    for (uint16_t i = 0; i < buffer_size; ++i) {
        if (regs.stabitsptr->FERR) {return E_UART_RX_FRAMING;}
        if (regs.stabitsptr->OERR) {return E_UART_RX_OVERRUN;}
        if (regs.stabitsptr->PERR) {return E_UART_RX_PARITY;}

        while (!regs.stabitsptr->URXDA) {
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

    sUartRegs const regs = GetRegisters(select);

    for (uint16_t i = 0; i < size; ++i) {
        while (regs.stabitsptr->UTXBF) {;}
        *regs.txptr = buffer[i];
    }

    return E_OK;
}

enum Status UART_flush_rx(EUxSelect const select)
{
    sUartRegs const regs = GetRegisters(select);
    while(regs.stabitsptr->URXDA || !regs.stabitsptr->RIDLE) {*regs.rxptr;}
    return E_OK;
}

uint8_t UART_Read(const EUxSelect select) {
    sUartRegs const regs = GetRegisters(select);

    // Wait for data to become available.
    uint32_t timeout = 0;

    while (timeout++ < UART_READ_TIMEOUT) {
        WATCHDOG_TimerClear();
        if (UART_IsRxReady(select)) {
            break;
        }
    }

    if (timeout >= UART_READ_TIMEOUT) {
        // Timed out while waiting for data.
        return 0;
    }

    if (regs.stabitsptr->FERR) {
        // Try to recover from framing error.
        UART_ClearBuffer(select);
        return 0;
    }

    // Clear buffer overrun.
    regs.stabitsptr->OERR = 0;

    return *regs.rxptr;
}

uint8_t UART1_Read(void) {
    return UART_Read(U1SELECT);
}

uint16_t UART_ReadInt(const EUxSelect select) {
    const uint8_t lsb = UART_Read(select);
    const uint16_t msb = UART_Read(select);
    const uint16_t retval = (msb << 8) | lsb;
    return retval;
}

uint16_t UART1_ReadInt(void) {
    return UART_ReadInt(U1SELECT);
}

uint32_t UART_read_u32(EUxSelect const select) {
    uint16_t const lsw = UART_ReadInt(select);
    uint32_t const msw = UART_ReadInt(select);
    uint32_t const retval = (msw << 16) | lsw;
    return retval;
}

uint32_t UART1_read_u32(void) {
    return UART_read_u32(U1SELECT);
}

void UART_Write(const EUxSelect select, const uint8_t txData) {
    const sUartRegs regs = GetRegisters(select);
    while (regs.stabitsptr->UTXBF == 1) {;}
    *regs.txptr = txData;
}

void UART1_Write(const uint8_t txData) {
    UART_Write(U1SELECT, txData);
}

void UART_WriteInt(const EUxSelect select, const uint16_t txData) {
    UART_Write(select, txData & 0xFF);
    UART_Write(select, (txData >> 8) & 0xFF);
}

void UART1_WriteInt(uint16_t txData) {
    UART_WriteInt(U1SELECT, txData);
}

void UART_write_u32(EUxSelect const select, uint32_t const txdata) {
    UART_WriteInt(select, (uint16_t)(txdata & 0xFFFF));
    UART_WriteInt(select, (uint16_t)(txdata >> 16));
}

void UART1_write_u32(uint32_t const txdata) {
    UART_write_u32(U1SELECT, txdata);
}

bool UART_IsRxReady(const EUxSelect select) {
    return GetRegisters(select).stabitsptr->URXDA;
}

void UART_ClearBuffer(const EUxSelect select) {
    const sUartRegs regs = GetRegisters(select);
    while (regs.stabitsptr->URXDA) {*regs.rxptr;}
}

/*********************/
/* Command functions */
/*********************/

response_t UART2_Read(void) {
    UART_Write(U1SELECT, UART_Read(U2SELECT));
    return DO_NOT_BOTHER;
}


response_t UART2_ReadWord(void) {
    UART_WriteInt(U1SELECT, UART_ReadInt(U2SELECT));
    return DO_NOT_BOTHER;
}

response_t UART2_Write(void) {
    UART_Write(U2SELECT, UART_Read(U1SELECT));
    return DO_NOT_BOTHER;
}

response_t UART2_WriteWord(void) {
    UART_WriteInt(U2SELECT, UART_ReadInt(U1SELECT));
    return DO_NOT_BOTHER;
}

response_t UART2_RxReady(void) {
    UART_Write(U1SELECT, UART_IsRxReady(U2SELECT));
    return DO_NOT_BOTHER;
}

response_t UART2_SetBaud(void) {
    SetBaud(U2SELECT, UART_ReadInt(U1SELECT));
    return SUCCESS;
}

response_t UART2_SetMode(void) {
    const uint16_t mode = UART_Read(U1SELECT);
    const EStopBits stop_bits = mode & 1;
    const EParity parity_data_bits = (mode & 6) >> 1;
    SetStop(U2SELECT, stop_bits);
    SetParity(U2SELECT, parity_data_bits);
    return SUCCESS;
}

response_t UART_Passthrough(void) {
    const uint16_t baud = UART1_ReadInt();
    SetBaud(U1SELECT, baud);
    SetBaud(U2SELECT, baud);
    EnableInterrupts();
    return DO_NOT_BOTHER;
}
