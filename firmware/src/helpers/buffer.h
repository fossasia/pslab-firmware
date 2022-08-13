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
    response_t BUFFER_Retrieve(void);
    
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
    response_t BUFFER_FetchInt(void);
    
    /**
    * @brief Send buffer content as longs.
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
    response_t BUFFER_FetchLong(void);

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
    response_t BUFFER_Fill(void);
    
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
    response_t BUFFER_Clear(void);

#ifdef	__cplusplus
}
#endif

#endif	/* BUFFER_H */
