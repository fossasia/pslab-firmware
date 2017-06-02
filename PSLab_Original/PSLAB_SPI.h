/* 
 * File:   PSLAB_SPI.h
 * Author: padmal
 *
 * Created on June 1, 2017, 12:58 PM
 */

#ifndef PSLAB_SPI_H
#define	PSLAB_SPI_H

/*-----SPI VARIABLES-------*/
static unsigned char location, value, ADC_MODE = 0, SPI_MODE = 0;
static unsigned char MULTIFUNC_PORT = 0, DMA_MODE = 0;
static unsigned char ADC_STREAMING = 0;
static unsigned char SPI_PPRE = 0, SPI_SPRE = 2, SPI_CKE = 1, SPI_CKP = 0, SPI_SMP = 1;

void setSPIMode(unsigned char mode);
void initSPI();
unsigned char spi_write8(unsigned char value);
unsigned int spi_write16(unsigned int value);
void start_spi();
void stop_spi();

#endif	/* PSLAB_SPI_H */

