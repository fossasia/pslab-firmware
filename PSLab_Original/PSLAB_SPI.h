/* 
 * File:   PSLAB_SPI.h
 * Author: padmal
 *
 * Created on June 3, 2017, 7:42 AM
 */

#ifndef PSLAB_SPI_H
#define	PSLAB_SPI_H

/*-----SPI VARIABLES-------*/
extern unsigned char location, value, ADC_MODE, SPI_MODE, DMA_MODE, ADC_STREAMING;
extern unsigned char SPI_PPRE, SPI_SPRE, SPI_CKE, SPI_CKP, SPI_SMP;

void setSPIMode(unsigned char);
void initSPI();
unsigned char spi_write8(unsigned char);
unsigned int spi_write16(unsigned int value);
void start_spi();
void stop_spi();

#endif	/* PSLAB_SPI_H */

