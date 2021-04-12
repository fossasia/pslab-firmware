/**
\file
\defgroup doc_driver_spi_code SPI Driver Source Code Reference
\ingroup doc_driver_spi
\brief This file contains the API that implements the SPI master driver functionalities.

\copyright (c) 2020 Microchip Technology Inc. and its subsidiaries.
\page License
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
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

#ifndef _SPI_MASTER_H
#define _SPI_MASTER_H

/**
  Section: Included Files
 */
#include <stdint.h>
#include <stdbool.h>
#include "../spi1_driver.h"

/**
*   \ingroup doc_driver_spi_code
*   \enum spi_master_configurations_t spi_master.h
*/
typedef enum { 
    SDFAST,
    SDSLOW
} spi_master_configurations_t;

/**
*   \ingroup doc_driver_spi_code
*   \struct spi_master_functions_t spi_master.h
*/
typedef struct {    void (*spiClose)(void);
                    bool (*spiOpen)(void);
                    uint8_t (*exchangeByte)(uint8_t b);
                    void (*exchangeBlock)(void * block, size_t blockSize);
                    void (*writeBlock)(void * block, size_t blockSize);
                    void (*readBlock)(void * block, size_t blockSize);
                    void (*writeByte)(uint8_t byte);
                    uint8_t (*readByte)(void);
                    void (*setSpiISR)(void(*handler)(void));
                    void (*spiISR)(void);
} spi_master_functions_t;

extern const spi_master_functions_t spiMaster[];

bool spi_master_open(spi_master_configurations_t config);   //for backwards compatibility


#endif	// _SPI_MASTER_H