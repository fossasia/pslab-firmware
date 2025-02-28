#ifndef BUFFER_H
#define	BUFFER_H

#include <stdint.h>

#define BUFFER_SIZE     10000

#ifdef	__cplusplus
extern "C" {
#endif

    extern uint16_t volatile __attribute__((section(".adc_buffer"), far)) BUFFER[BUFFER_SIZE];

    /**
    * @brief Send buffer contents.
    *
    * @description
    * This command function takes two arguments over serial:
    * 1. The starting index in the buffer from which to send values.
    * 2. The number of values to be sent.
    * It returns the requested data over serial.
    * It sends an acknowledge byte (SUCCESS)
    *
    * @return SUCCESS
    */
    enum Status BUFFER_read(
        uint8_t const *args,
        uint16_t args_size,
        uint8_t volatile **rets,
        uint16_t *rets_size
    );

    /**
    * @brief Send buffer content as integers.
    *
    * @description
    * This command function takes two arguments over serial:
    * 1. The starting index in the buffer from which to send values.
    * 2. The number of values to be sent.
    *
    * It returns the requested data over serial.
    * It sends an acknowledge byte (SUCCESS)
    *
    * @return SUCCESS
    */
    enum Status BUFFER_read_from_channel(
        uint8_t const *args,
        uint16_t args_size,
        uint8_t volatile **rets,
        uint16_t *rets_size
    );

    /**
    * @brief Populate BUFFER array
    *
    * @description
    * This command function takes two arguments over serial:
    * 1. The starting index in the buffer from which to fill values.
    * 2. The number of values to be sent.
    *
    * It does not return anything over serial.
    * It sends an acknowledge byte (SUCCESS)
    *
    * @return SUCCESS
    */
    enum Status BUFFER_write(
        uint8_t const *args,
        uint16_t args_size,
        uint8_t volatile **rets,
        uint16_t *rets_size
    );


    /**
    * @brief Clear BUFFER array
    *
    * @description
    * This command function takes two arguments over serial:
    * 1. The starting index in the buffer from which it should be emptied
    * 2. The number of indices need cleared.
    *
    * It does not return anything over serial.
    * It sends an acknowledge byte (SUCCESS)
    *
    * @return SUCCESS
    */
    enum Status BUFFER_clear(
        uint8_t const *args,
        uint16_t args_size,
        uint8_t **const rets,
        uint16_t *rets_size
    );

#ifdef	__cplusplus
}
#endif

#endif	/* BUFFER_H */
