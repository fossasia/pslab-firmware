// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright 2015 Microchip Technology Inc. (www.microchip.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

To request to license the code under the MLA license (www.microchip.com/mla_license), 
please contact mla_licensing@microchip.com
*******************************************************************************/
//DOM-IGNORE-END

#ifndef SDMMC_H
#define SDMMC_H

#include <stdint.h>
#include <stdbool.h>


/*****************************************************************************
  Function:
    MEDIA_INFORMATION *  SD_MediaInitialize (void)
  Summary:
    Initializes the SD card.
  Conditions:
    The SD_MediaInitialize function pointer must be pointing to this function.
  Input:
    config - An SD Drive configuration structure pointer
  Return Values:
    The function returns a pointer to the MEDIA_INFORMATION structure.  The
    errorCode member may contain the following values:
        * MEDIA_NO_ERROR - The media initialized successfully
        * MEDIA_CANNOT_INITIALIZE - Cannot initialize the media.  
  Side Effects:
    None.
  Description:
    This function will send initialization commands to and SD card.
  Remarks:
    Psuedo code flow for the media initialization process is as follows:

-------------------------------------------------------------------------------------------
SD Card SPI Initialization Sequence (for physical layer v1.x or v2.0 device) is as follows:
-------------------------------------------------------------------------------------------
0.  Power up tasks
    a.  Initialize microcontroller SPI module to no more than 400kbps rate so as to support MMC devices.
    b.  Add delay for SD card power up, prior to sending it any commands.  It wants the 
        longer of: 1ms, the Vdd ramp time (time from 2.7V to Vdd stable), and 74+ clock pulses.
1.  Send CMD0 (GO_IDLE_STATE) with CS = 0.  This puts the media in SPI mode and software resets the SD/MMC card.
2.  Send CMD8 (SEND_IF_COND).  This requests what voltage the card wants to run at. 
    Note: Some cards will not support this command.
    a.  If illegal command response is received, this implies either a v1.x physical spec device, or not an SD card (ex: MMC).
    b.  If normal response is received, then it must be a v2.0 or later SD memory card.

If v1.x device:
-----------------
3.  Send CMD1 repeatedly, until initialization complete (indicated by R1 response uint8_t/idle bit == 0)
4.  Basic initialization is complete.  May now switch to higher SPI frequencies.
5.  Send CMD9 to read the CSD structure.  This will tell us the total flash size and other info which will be useful later.
6.  Parse CSD structure bits (based on v1.x structure format) and extract useful information about the media.
7.  The card is now ready to perform application data transfers.

If v2.0+ device:
-----------------
3.  Verify the voltage range is feasible.  If not, unusable card, should notify user that the card is incompatible with this host.
4.  Send CMD58 (Read OCR).
5.  Send CMD55, then ACMD41 (SD_SEND_OP_COND, with HCS = 1).
    a.  Loop CMD55/ACMD41 until R1 response uint8_t == 0x00 (indicating the card is no longer busy/no longer in idle state).  
6.  Send CMD58 (Get CCS).
    a.  If CCS = 1 --> SDHC card.
    b.  If CCS = 0 --> Standard capacity SD card (which is v2.0+).
7.  Basic initialization is complete.  May now switch to higher SPI frequencies.
8.  Send CMD9 to read the CSD structure.  This will tell us the total flash size and other info which will be useful later.
9.  Parse CSD structure bits (based on v2.0 structure format) and extract useful information about the media.
10. The card is now ready to perform application data transfers.
--------------------------------------------------------------------------------
********************************************************************************/
bool SD_SPI_IsMediaInitialized(void);
bool SD_SPI_MediaInitialize(void);
bool SD_SPI_IsMediaPresent(void);
bool SD_SPI_IsWriteProtected(void);
uint16_t SD_SPI_GetSectorSize(void);
uint32_t SD_SPI_GetSectorCount(void);

/*****************************************************************************
  Function:
    uint8_t SD_SectorRead (uint32_t sector_addr, uint8_t * buffer)
  Summary:
    Reads a sector of data from an SD card.
  Conditions:
    The SD_SectorRead function pointer must be pointing towards this function.
  Input:
    config - An SD Drive configuration structure pointer
    sectorAddress - The address of the sector on the card.
    buffer -      The buffer where the retrieved data will be stored.  If
                  buffer is NULL, do not store the data anywhere.
  Return Values:
    true -  The sector was read successfully
    false - The sector could not be read
  Side Effects:
    None
  Description:
    The SD_SectorRead function reads a sector of data bytes (512 bytes) 
    of data from the SD card starting at the sector address and stores them in 
    the location pointed to by 'buffer.'
  Remarks:
    The card expects the address field in the command packet to be a byte address.
    The sector_addr value is converted to a byte address by shifting it left nine
    times (multiplying by 512).
    
    This function performs a synchronous read operation.  In other words, this
    function is a blocking function, and will not return until either the data
    has fully been read, or, a timeout or other error occurred.
  ***************************************************************************************/
bool SD_SPI_SectorRead(uint32_t sector_address, uint8_t* buffer, uint16_t sector_count);

/*****************************************************************************
  Function:
    bool SD_SectorWrite (SD_DRIVE_CONFIG * config,
        uint32_t sector_addr, uint8_t * buffer, uint8_t allowWriteToZero)
  Summary:
    Writes a sector of data to an SD card.
  Conditions:
    The SD_SectorWrite function pointer must be pointing to this function.
  Input:
    config - An SD Drive configuration structure pointer
    sectorAddress -      The address of the sector on the card.
    buffer -           The buffer with the data to write.
    allowWriteToZero -
                     - true -  Writes to the 0 sector (MBR) are allowed
                     - false - Any write to the 0 sector will fail.
  Return Values:
    true -  The sector was written successfully.
    false - The sector could not be written.
  Side Effects:
    None.
  Description:
    The SD_SectorWrite function writes one sector of data (512 uint8_ts) 
    of data from the location pointed to by 'buffer' to the specified sector of 
    the SD card.
  Remarks:
    The card expects the address field in the command packet to be a uint8_t address.
    The sector_addr value is converted to a uint8_t address by shifting it left nine
    times (multiplying by 512).
  ***************************************************************************************/
bool SD_SPI_SectorWrite(uint32_t sector_address, const uint8_t* buffer, uint16_t sector_count);

#endif
