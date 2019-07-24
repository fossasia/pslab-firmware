/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
 */

#include "spi_master.h"

inline bool SDFAST_open(void);
inline bool SDSLOW_open(void);

const spi_master_functions_t spiMaster[] = {   
    { spi1_close, SDFAST_open, spi1_exchangeByte, spi1_exchangeBlock, spi1_writeBlock, spi1_readBlock, spi1_writeByte, spi1_readByte, spi1_setSpiISR, spi1_isr },
    { spi1_close, SDSLOW_open, spi1_exchangeByte, spi1_exchangeBlock, spi1_writeBlock, spi1_readBlock, spi1_writeByte, spi1_readByte, spi1_setSpiISR, spi1_isr }
};

inline bool SDFAST_open(void){
    return spi1_open(SDFAST_CONFIG);
}

inline bool SDSLOW_open(void){
    return spi1_open(SDSLOW_CONFIG);
}

//This function serves keep backwards compatibility with older api users
inline bool spi_master_open(spi_master_configurations_t config){
    switch(config){
        case SDFAST:
            return SDFAST_open();
        case SDSLOW:
            return SDSLOW_open();
        default:
            return 0;
    }
}
/**
 End of File
 */
