/* Packet handler
 *
 * Description of communication between PSLab and host:
 *
 * 1. The host sends a command to the PSLab, consisting of a command code, an
 *    input payload size, and a payload containing command function input.
 * 2. The PSLab runs the command function associated with the command code
 *    with the payload as input.
 * 3. The command function parses the input payload and does its thing,
 *    returning a status code, and optionally an output payload.
 * 3. The PSLab sends a response to the host, consisting of the status code
 *    from the command function, the command function output payload size, and
 *    the command function output payload.
 *
 * In case of error on either end, both the PSLab and the host must try to
 * consume any bytes remaining on the bus, except in the event of an
 * irrecoverable error in which case the exchange must be cancelled.
 *
 * The following errors are considered irrecoverable:
 *
 * 1. Read timeout, aka. fewer bytes available to read than expected. One
 *    party sent fewer bytes than the other expected. This is a protocol
 *    violation on at least one side of the exchange. Caused by
 *    firmware / driver version incompatibility or an incorrect protocol
 *    implementation.
 * 2. Bus error, e.g. UART framing or parity error. Indicates either a
 *    hardware problem, e.g. electromagnetic interference or glitchy wire, or
 *    a mismatch between host and device UART configuration.
 * 3. Data overrun, aka. one party wrote bytes faster than the other could
 *    read them. This is either a firmware or driver bug.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../commands.h"
#include "../instruments/wavegenerator.h"
#include "host.h"

#include "../helpers/light.h"
#include "../registers/system/pin_manager.h"

#include "packet_handler.h"

#define HEADER_SIZE 8
/** Packet header. */
typedef union Header {
    /** Header representation as byte-array. */
    uint8_t as_bytes[HEADER_SIZE];
    struct {
        union {
            /** Command function index. */
            uint16_t command;
            /** Exit code from command function. */
            uint16_t status;
        };
        /** Number of bytes in payload. */
        uint16_t payload_size;
        uint8_t _reserved[4];
    };
} Header;

static enum Status receive(
    CmdFunc *cmdfunc,
    uint8_t **payload,
    uint16_t *payload_size
);

static enum Status send(Header header, uint8_t const *payload);

/**
 * @brief Receive a command from host, run it, and send back the result.
 *
 * @return enum Status
 *      Exit code.
 */
enum Status PACKET_exchange(void)
{
    CmdFunc command = NULL;
    // receive allocates RX payload.
    uint8_t *payload_rx = NULL;
    uint16_t payload_rx_size = 0;
    uint16_t status = receive(
        &command,
        &payload_rx,
        &payload_rx_size
    );

    // Command function allocates TX payload.
    uint8_t *payload_tx = NULL;
    uint16_t payload_tx_size = 0;

    if (status == E_OK) {
        status = command(
            &payload_rx,
            payload_rx_size,
            &payload_tx,
            &payload_tx_size
        );
    }
    free(payload_rx);
    payload_rx = NULL;

    Header header = {{0}};
    header.status = status;
    header.payload_size = payload_tx_size;
    enum Status send_status = send(header, payload_tx);
    free(payload_tx);
    payload_tx = NULL;

    return status ? status : send_status;
}

/**
 * @brief Get a command function and its arguments from host.
 *
 * @param[out] cmdfunc
 *      Pointer to pointer to function of type `CmdFunc`. Will be set to the
 *      address of the command function requested by the user, if it exists.
 *      If the user requested an invalid command function, this will be set to
 *      NULL.
 * @param[out] payload
 *      Pointer to a byte-array containing input arguments to `cmdfunc`. If
 *      @c payload_size is zero, this is NULL.
 * @param[out] payload_size
 *      Pointer to a uint16_t holding the number of bytes in `payload`.
 * @return enum Status
 *      Exit code.
 */
static enum Status receive(
    CmdFunc *const cmdfunc,
    uint8_t **const payload,
    uint16_t *const payload_size
) {
    Header header = {{0}};
    enum Status status = E_OK;

    if ( (status = HOST_read(header.as_bytes, HEADER_SIZE, NULL)) ) {
        goto cleanup;
    }

    *payload_size = header.payload_size;
    // malloc(0) is implementation defined. Explicitly make a null pointer
    // instead.
    *payload = header.payload_size ? malloc(*payload_size) : NULL;

    if (*payload_size && !*payload) {
        status = E_MEMORY_INSUFFICIENT;
        goto cleanup;
    }

    if ( !(*cmdfunc = COMMAND_get_func(header.command)) ) {
        status = E_BAD_COMMAND;
        goto cleanup;
    }

    status = HOST_read(*payload, header.payload_size, NULL);
    return status;

    cleanup:
    // Consume any remaining bytes.
    HOST_flush_rx();
    return status;
}

/**
 * @brief Return command function result to host.
 *
 * @param header
 *      Response header.
 * @param[in] payload
 *      Pointer to byte-array holding return data from command function. Array
 *      size is found in header.
 * @return enum Status
 *      Exit code.
 */
static enum Status send(Header const header, uint8_t const *const payload)
{
    enum Status status = E_OK;

    if ( (status = HOST_write(header.as_bytes, HEADER_SIZE, NULL)) ) {
        goto cleanup;
    }

    status = HOST_write(payload, header.payload_size, NULL);

    cleanup:
    return status;
}
