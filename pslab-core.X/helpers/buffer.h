#ifndef BUFFER_H
#define	BUFFER_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

    extern int16_t volatile __attribute__((section(".adc_buffer"), far)) BUFFER[10000];
    
    /**
    * @brief
    * Send buffer contents.
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

#ifdef	__cplusplus
}
#endif

#endif	/* BUFFER_H */
