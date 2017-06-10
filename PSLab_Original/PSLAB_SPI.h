/* 
 * File:   PSLAB_SPI.h
 * Author: padmal
 *
 * Created on June 3, 2017, 7:42 AM
 */

#ifndef PSLAB_SPI_H
#define	PSLAB_SPI_H

/*-----SPI VARIABLES-------*/
extern BYTE location, value, ADC_MODE, SPI_MODE, DMA_MODE, ADC_STREAMING;
extern BYTE SPI_PPRE, SPI_SPRE, SPI_CKE, SPI_CKP, SPI_SMP;

void setSPIMode(BYTE);
void initSPI();
BYTE spi_write8(BYTE);
uint16 spi_write16(uint16 value);
void start_spi();
void stop_spi();

#endif	/* PSLAB_SPI_H */

