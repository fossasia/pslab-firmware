#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "../pin_manager.h"
#include "../drivers/spi_master.h"

#include "sd_spi.h"

/******************************************************************************/
/* SD Configuration                                                           */
/******************************************************************************/
// Description: Media Response Delay Timeout Values
#define SD_SPI_STARTUP_DELAY_MS 30u
#define SD_SPI_COMMAND_WAIT_MS 1u
#define SD_NCR_TIMEOUT     (uint16_t)20          //byte times before command response is expected (must be at least 8)
#define SD_NAC_TIMEOUT     (uint32_t)0x40000     //SPI byte times we should wait when performing read operations (should be at least 100ms for SD cards)
#define SD_WRITE_TIMEOUT   (uint32_t)0xA0000     //SPI byte times to wait before timing out when the media is performing a write operation (should be at least 250ms for SD cards).

#define SD_SPI_ChipSelect() SDCard_CS_SetLow()
#define SD_SPI_ChipDeselect() SDCard_CS_SetHigh()
#define SD_SPI_exchangeByte(data) spiMaster[SDFAST].exchangeByte(data)
#define SD_SPI_exchangeBlock(data, length) spiMaster[SDFAST].exchangeBlock(data, length)
#define SD_SPI_master_open(config) spiMaster[config].spiOpen()
#define SD_SPI_close() spiMaster[SDFAST].spiClose()
#define SD_SPI_GetCardDetect() 1
#define SD_SPI_GetWriteProtect() 0

/*****************************************************************************/
/*                        Custom structures and definitions                  */
/*****************************************************************************/

/* in SPI SLOW mode (<400kHz) 400kHz = 400 clocks for 1ms.
 * 8 clocks per byte = 50 bytes of dummy data results in at least 1ms
 * of delay. */
#define SD_SLOW_CLOCK_DELAY_1MS_MIN 50u 

//Definition for a structure used when calling either SD_AsyncReadTasks()
//function, or the SD_AsyncWriteTasks() function.
struct SD_ASYNC_IO
{
    uint16_t wNumBytes;         //Number of bytes to attempt to read or write in the next call to MDD_SDSPI_AsyncReadTasks() or MDD_SDSPI_AsyncWriteTasks.  May be updated between calls to the handler.
    uint32_t dwBytesRemaining; //Should be initialized to the total number of uint8_ts that you wish to read or write.  This value is allowed to be greater than a single block size of the media.
    uint8_t* pBuffer;          //Pointer to where the read/written uint8_ts should be copied to/from.  May be updated between calls to the handler function.
    uint32_t dwAddress;        //Starting block address to read or to write to on the media.  Should only get initialized, do not modify after that.
    uint8_t bStateVariable;    //State machine variable.  Should get initialized to ASYNC_READ_QUEUED or ASYNC_WRITE_QUEUED to start an operation.  After that, do not modify until the read or write is complete.
};

//Response codes for the SD_AsyncReadTasks() function.
#define SD_ASYNC_READ_COMPLETE             0x00
#define SD_ASYNC_READ_BUSY                 0x01
#define SD_ASYNC_READ_NEW_PACKET_READY     0x02
#define SD_ASYNC_READ_ERROR                0xFF

//SD_AsyncReadTasks() state machine variable values. These are used internally to sd_spi.c.
#define SD_ASYNC_READ_COMPLETE             0x00
#define SD_ASYNC_READ_QUEUED               0x01    //Initialize to this to start a read sequence
#define SD_ASYNC_READ_WAIT_START_TOKEN     0x03
#define SD_ASYNC_READ_NEW_PACKET_READY     0x02
#define SD_ASYNC_READ_ABORT                0xFE
#define SD_ASYNC_READ_ERROR                0xFF

//Possible return values when calling SD_AsyncWriteTasks()
#define SD_ASYNC_WRITE_COMPLETE            0x00
#define SD_ASYNC_WRITE_SEND_PACKET         0x02
#define SD_ASYNC_WRITE_BUSY                0x03
#define SD_ASYNC_WRITE_ERROR               0xFF

//SD_AsyncWriteTasks() state machine variable values. These are used internally to sd_spi.c.
#define SD_ASYNC_WRITE_COMPLETE            0x00
#define SD_ASYNC_WRITE_QUEUED              0x01    //Initialize to this to start a write sequence
#define SD_ASYNC_WRITE_TRANSMIT_PACKET     0x02
#define SD_ASYNC_WRITE_MEDIA_BUSY          0x03
#define SD_ASYNC_STOP_TOKEN_SENT_WAIT_BUSY 0x04
#define SD_ASYNC_WRITE_ABORT               0xFE
#define SD_ASYNC_WRITE_ERROR               0xFF

#define SD_MEDIA_BLOCK_SIZE            512u  //Should always be 512 for v1 and v2 devices.

enum SD_STATE
{
    SD_STATE_NOT_INITIALIZED,
    SD_STATE_READY_FOR_COMMAND,
    SD_STATE_BUSY
};

enum MEDIA_ERRORS
{
    MEDIA_NO_ERROR,                     // No errors
    MEDIA_DEVICE_NOT_PRESENT,           // The requested device is not present
    MEDIA_CANNOT_INITIALIZE             // Cannot initialize media
} ;

// Media information flags.  The driver's MediaInitialize function will return a pointer to one of these structures.
struct MEDIA_INFORMATION
{
    enum MEDIA_ERRORS errorCode;              // The status of the initialization MEDIA_ERRORS
    uint16_t    sectorSize;              // The sector size of the target device.
    enum SD_STATE state;
    uint32_t finalLBA;
    uint8_t gSDMode;
};

/******************************************************************************
 * Global Variables
 *****************************************************************************/
enum SD_TOKEN
{ 
    SD_TOKEN_START = 0xFE,
    SD_TOKEN_START_MULTI_BLOCK = 0xFC,
    SD_TOKEN_STOP_TRANSMISSION = 0xFD,
    SD_TOKEN_DATA_ACCEPTED = 0x05,
    SD_TOKEN_FLOATING_BUS = 0xFF
};

enum SD_COMMAND
{
    // Description: This macro defines the command code to reset the SD card
    SD_COMMAND_GO_IDLE_STATE                          = 0,
    // Description: This macro defines the command code to initialize the SD card
    SD_COMMAND_SEND_OP_COND                           = 1,
    // Description: This macro defined the command code to check for sector addressing
    SD_COMMAND_SEND_IF_COND                           = 8,
    // Description: This macro defines the command code to get the Card Specific Data
    SD_COMMAND_SEND_CSD                               = 9,
    // Description: This macro defines the command code to get the Card Information
    SD_COMMAND_SEND_CID                               = 10,
    // Description: This macro defines the command code to stop transmission during a multi-block read
    SD_COMMAND_STOP_TRANSMISSION                      = 12,
    // Description: This macro defines the command code to get the card status information
    SD_COMMAND_SEND_STATUS                            = 13,
    // Description: This macro defines the command code to set the block length of the card
    SD_COMMAND_SET_BLOCK_LENGTH                       = 16,
    // Description: This macro defines the command code to read one block from the card
    SD_COMMAND_READ_SINGLE_BLOCK                      = 17,
    // Description: This macro defines the command code to read multiple blocks from the card
    SD_COMMAND_READ_MULTI_BLOCK                       = 18,
    // Description: This macro defines the command code to tell the media how many blocks to pre-erase (for faster multi-block writes to follow)
    //Note: This is an "application specific" command.  This tells the media how many blocks to pre-erase for the subsequent WRITE_MULTI_BLOCK
    SD_COMMAND_SET_WRITE_BLOCK_ERASE_COUNT            = 23,
    // Description: This macro defines the command code to write one block to the card
    SD_COMMAND_WRITE_SINGLE_BLOCK                     = 24,
    // Description: This macro defines the command code to write multiple blocks to the card
    SD_COMMAND_WRITE_MULTI_BLOCK                      = 25,
    // Description: This macro defines the command code to set the address of the start of an erase operation
    SD_COMMAND_TAG_SECTOR_START                       = 32,
    // Description: This macro defines the command code to set the address of the end of an erase operation
    SD_COMMAND_TAG_SECTOR_END                         = 33,
    // Description: This macro defines the command code to erase all previously selected blocks
    SD_COMMAND_ERASE                                  = 38,
    //Description: This macro defines the command code to initialize an SD card and provide the CSD register value.
    //Note: this is an "application specific" command (specific to SD cards) and must be preceded by cmdAPP_CMD.
    SD_COMMAND_SD_SEND_OP_COND                        = 41,
    // Description: This macro defines the command code to begin application specific command inputs
    SD_COMMAND_APP_CMD                                = 55,
    // Description: This macro defines the command code to get the OCR register information from the card
    SD_COMMAND_READ_OCR                               = 58,
    // Description: This macro defines the command code to disable CRC checking
    SD_COMMAND_CRC_ON_OFF                             = 59
};

#define SD_MODE_NORMAL  0
#define SD_MODE_HC      1

//Constants
#define SD_WRITE_RESPONSE_TOKEN_MASK   0x1F  //Bit mask to AND with the write token response uint8_t from the media, to clear the don't care bits.

// Description: Enumeration of different SD response types
typedef enum
{
    SD_RESPONSE_R1,     // R1 type response
    SD_RESPONSE_R1b,    // R1b type response
    SD_RESPONSE_R2,     // R2 type response
    SD_RESPONSE_R3,     // R3 type response
    SD_RESPONSE_R7      // R7 type response
}SD_RESPONSE_TYPE;

#define SD_COMMAND_CODE_BIT_MASK (0b00111111)
#define SD_COMMAND_TRANSMIT_BIT_MASK (1<<6)
#define SD_COMMAND_START_BIT_MASK (1<<6)


// Summary: The format of an R1 type response
// Description: This union represents different ways to access an SD card R1 type response packet.
typedef union
{
    uint8_t _byte;                         // byte-wise access
    // This structure allows bitwise access of the response
    struct
    {
        unsigned IN_IDLE_STATE:1;       // Card is in idle state
        unsigned ERASE_RESET:1;         // Erase reset flag
        unsigned ILLEGAL_CMD:1;         // Illegal command flag
        unsigned CRC_ERR:1;             // CRC error flag
        unsigned ERASE_SEQ_ERR:1;       // Erase sequence error flag
        unsigned ADDRESS_ERR:1;         // Address error flag
        unsigned PARAM_ERR:1;           // Parameter flag   
        unsigned B7:1;                  // Unused bit 7
    };
} SD_RESPONSE_1;

// Summary: The format of an R2 type response
// Description: This union represents different ways to access an SD card R2 type response packet
typedef union
{
    uint16_t _uint16_t;
    struct
    {
        uint8_t      _byte0;
        uint8_t      _byte1;
    };
    struct
    {
        unsigned IN_IDLE_STATE:1;
        unsigned ERASE_RESET:1;
        unsigned ILLEGAL_CMD:1;
        unsigned CRC_ERR:1;
        unsigned ERASE_SEQ_ERR:1;
        unsigned ADDRESS_ERR:1;
        unsigned PARAM_ERR:1;
        unsigned B7:1;
        unsigned CARD_IS_LOCKED:1;
        unsigned WP_ERASE_SKIP_LK_FAIL:1;
        unsigned ERROR:1;
        unsigned CC_ERROR:1;
        unsigned CARD_ECC_FAIL:1;
        unsigned WP_VIOLATION:1;
        unsigned ERASE_PARAM:1;
        unsigned OUTRANGE_CSD_OVERWRITE:1;
    };
} SD_RESPONSE_2;

// Summary: The format of an R7 or R3 type response
// Description: This union represents different ways to access an SD card R7 type response packet.
typedef union
{
    struct
    {
        uint8_t _byte;                         // byte-wise access
        union
        {
            //Note: The SD card argument response field is 32-bit, big endian format.
            //However, the C compiler stores 32-bit values in little endian in RAM.
            //When writing to the _returnVal/argument bytes, make sure to byte
            //swap the order from which it arrived over the SPI from the SD card.
            uint32_t _returnVal;
            struct
            {
                uint8_t _byte0;
                uint8_t _byte1;
                uint8_t _byte2;
                uint8_t _byte3;
            };    
        }argument;    
    } bytewise;
    // This structure allows bitwise access of the response
    struct
    {
        struct
        {
            unsigned IN_IDLE_STATE:1;       // Card is in idle state
            unsigned ERASE_RESET:1;         // Erase reset flag
            unsigned ILLEGAL_CMD:1;         // Illegal command flag
            unsigned CRC_ERR:1;             // CRC error flag
            unsigned ERASE_SEQ_ERR:1;       // Erase sequence error flag
            unsigned ADDRESS_ERR:1;         // Address error flag
            unsigned PARAM_ERR:1;           // Parameter flag   
            unsigned B7:1;                  // Unused bit 7
        }bits;
        uint32_t _returnVal;
    } bitwise;
} SD_RESPONSE_7;

// Summary: A union of responses from an SD card
// Description: The SD_RESPONSE union represents any of the possible responses that an SD card can return after
//              being issued a command.
typedef union
{
    SD_RESPONSE_1  r1;  
    SD_RESPONSE_2  r2;
    SD_RESPONSE_7  r7;
}SD_RESPONSE;

// Description:  Used for the mass-storage library to determine capacity
static struct MEDIA_INFORMATION mediaInformation = {MEDIA_NO_ERROR, SD_MEDIA_BLOCK_SIZE, SD_STATE_NOT_INITIALIZED, 0ul, SD_MODE_NORMAL};
static struct SD_ASYNC_IO ioInfo; //Declared global context, for fast/code efficient access

// Summary: An enumeration of SD commands
// Description: This enumeration corresponds to the position of each command in the sdmmc_cmdtable array
//              These macros indicate to the SD_SendCmd function which element of the sdmmc_cmdtable array
//              to retrieve command code information from.
enum SD_COMMAND_INDEX
{
    SD_GO_IDLE_STATE = 0,
    SD_SEND_OP_COND,
    SD_SEND_IF_COND,
    SD_SEND_CSD,
    SD_SEND_CID,
    SD_STOP_TRANSMISSION,
    SD_SEND_STATUS,
    SD_SET_BLOCK_LENGTH,
    SD_READ_SINGLE_BLOCK,
    SD_READ_MULTI_BLOCK,
    SD_WRITE_SINGLE_BLOCK,
    SD_WRITE_MULTI_BLOCK,
    SD_TAG_SECTOR_START,
    SD_TAG_SECTOR_END,
    SD_ERASE,
    SD_APP_CMD,
    SD_READ_OCR,
    SD_CRC_ON_OFF,
    SD_SD_SEND_OP_COND,
    SD_SET_WRITE_BLOCK_ERASE_COUNT
} ;

// Summary: SD card command data structure
// Description: The SD_COMMAND structure is used to create a command table of information needed for each relevant SD command
struct SD_COMMAND_TABLE_ENTRY
{
    enum SD_COMMAND     CmdCode;        // The command code
    uint8_t             CRC;            // The CRC value for that command
    SD_RESPONSE_TYPE    responsetype;   // The response type
    bool moreDataExpected;
};


// Summary: Table of SD card commands and parameters
// Description: The sdmmc_cmdtable contains an array of SD card commands, the corresponding CRC code, the
//              response type that the card will return, and a parameter indicating whether to expect
//              additional data from the card.
static const struct SD_COMMAND_TABLE_ENTRY sdmmc_cmdtable[] =
{
    // cmd                                      crc     response            more data expected
    {SD_COMMAND_GO_IDLE_STATE,                  0x95,   SD_RESPONSE_R1,     false},
    {SD_COMMAND_SEND_OP_COND,                   0xF9,   SD_RESPONSE_R1,     false},
    {SD_COMMAND_SEND_IF_COND,                   0x87,   SD_RESPONSE_R7,     false},
    {SD_COMMAND_SEND_CSD,                       0xAF,   SD_RESPONSE_R1,     true},
    {SD_COMMAND_SEND_CID,                       0x1B,   SD_RESPONSE_R1,     true},
    {SD_COMMAND_STOP_TRANSMISSION,              0xC3,   SD_RESPONSE_R1b,    false},
    {SD_COMMAND_SEND_STATUS,                    0xAF,   SD_RESPONSE_R2,     false},
    {SD_COMMAND_SET_BLOCK_LENGTH,               0xFF,   SD_RESPONSE_R1,     false},
    {SD_COMMAND_READ_SINGLE_BLOCK,              0xFF,   SD_RESPONSE_R1,     true},
    {SD_COMMAND_READ_MULTI_BLOCK,               0xFF,   SD_RESPONSE_R1,     true},
    {SD_COMMAND_WRITE_SINGLE_BLOCK,             0xFF,   SD_RESPONSE_R1,     true},
    {SD_COMMAND_WRITE_MULTI_BLOCK,              0xFF,   SD_RESPONSE_R1,     true},
    {SD_COMMAND_TAG_SECTOR_START,               0xFF,   SD_RESPONSE_R1,     false},
    {SD_COMMAND_TAG_SECTOR_END,                 0xFF,   SD_RESPONSE_R1,     false},
    {SD_COMMAND_ERASE,                          0xDF,   SD_RESPONSE_R1b,    false},
    {SD_COMMAND_APP_CMD,                        0x73,   SD_RESPONSE_R1,     false},
    {SD_COMMAND_READ_OCR,                       0x25,   SD_RESPONSE_R7,     false},
    {SD_COMMAND_CRC_ON_OFF,                     0x25,   SD_RESPONSE_R1,     false},
    {SD_COMMAND_SD_SEND_OP_COND,                0xFF,   SD_RESPONSE_R7,     false}, //Actual response is R3, but has same number of bytes as R7.
    {SD_COMMAND_SET_WRITE_BLOCK_ERASE_COUNT,    0xFF,   SD_RESPONSE_R1,     false}
};

/******************************************************************************
 * Private Prototypes
 *****************************************************************************/
static SD_RESPONSE SD_SendCmd(uint8_t cmd, uint32_t address);
static uint8_t SD_SPI_AsyncWriteTasks(struct SD_ASYNC_IO* info);
static uint8_t SD_SPI_AsyncReadTasks(struct SD_ASYNC_IO* info);


/******************************************************************************
 * Public Functions
 *****************************************************************************/
bool SD_SPI_IsMediaPresent(void)
{
    return SD_SPI_GetCardDetect() ? true: false;
}//end MediaDetect

uint16_t SD_SPI_GetSectorSize(void)
{
    return mediaInformation.sectorSize;
}

uint32_t SD_SPI_GetSectorCount(void)
{
    return (mediaInformation.finalLBA);
}

bool SD_SPI_SectorRead(uint32_t sector_address, uint8_t* buffer, uint16_t sector_count)
{
    struct SD_ASYNC_IO info;
    uint8_t status;
    bool result = false;
       
    //Initialize info structure for using the SD_SPI_AsyncReadTasks() function.
    info.wNumBytes = sector_count << 9;  //Equivalent to multiply by 512
    info.dwBytesRemaining = sector_count << 9;  //Equivalent to multiply by 512
    info.pBuffer = buffer;
    info.dwAddress = sector_address;
    info.bStateVariable = SD_ASYNC_READ_QUEUED;
        
    if( SD_SPI_master_open(SDFAST) == false )
    {
        return false;
    }
    SD_SPI_ChipSelect();
    
    while(1)
    {
        status = SD_SPI_AsyncReadTasks(&info);
        if(status == SD_ASYNC_READ_COMPLETE)
        {
            result = true;
            break;
        }
        else if(status == SD_ASYNC_READ_ERROR)
        {
            result = false;
            break;
        } 
    }       

    SD_SPI_ChipDeselect();
    SD_SPI_close();
    
    return result;
}    

bool SD_SPI_SectorWrite(uint32_t sector_address, const uint8_t* buffer, uint16_t sector_count)
{
    struct SD_ASYNC_IO info;
    uint8_t status;
    bool result = false;
       
    //Initialize structure so we write a single sector worth of data.
    info.wNumBytes = sector_count << 9;  //Equivalent to multiply by 512;
    info.dwBytesRemaining = sector_count << 9;  //Equivalent to multiply by 512
    info.pBuffer = (uint8_t*)buffer;
    info.dwAddress = sector_address;
    info.bStateVariable = SD_ASYNC_WRITE_QUEUED;
    
    if( SD_SPI_master_open(SDFAST) == false )
    {
        return false;
    }
    SD_SPI_ChipSelect();
    
    while(1)
    {
        status = SD_SPI_AsyncWriteTasks(&info);
        if(status == SD_ASYNC_WRITE_COMPLETE)
        {
            result = true;
            break;
        }    
        else if(status == SD_ASYNC_WRITE_ERROR)
        {
            result = false;
            break;
        }
    }   
    
    SD_SPI_ChipDeselect();
    SD_SPI_close();
    
    return result;
}    

bool  SD_SPI_IsMediaInitialized (void)
{
    return (mediaInformation.state != SD_STATE_NOT_INITIALIZED);
}

bool SD_SPI_IsWriteProtected(void)
{
    return SD_SPI_GetWriteProtect() ? true : false;
}

/* NOTE: can only be used when card SPI bus is not currently active.
 * All items on the SPI bus should be deselected.
 */
static void SD_SPI_DelayMilliseconds(uint8_t milliseconds)
{
    uint16_t timeout = SD_SLOW_CLOCK_DELAY_1MS_MIN * milliseconds;

    SD_SPI_ChipDeselect();
    while(timeout--)
    {
        (void)SD_SPI_exchangeByte(0xFF);
    }
}

bool SD_SPI_MediaInitialize (void)
{
    uint16_t timeout;
    SD_RESPONSE response;
    uint8_t CSDResponse[20];
    uint8_t count, index;
    uint32_t c_size;
    uint8_t c_size_mult;
    uint8_t block_len;

    mediaInformation.state = SD_STATE_NOT_INITIALIZED;
    mediaInformation.errorCode = MEDIA_NO_ERROR;
    mediaInformation.finalLBA = 0x00000000;	
    mediaInformation.gSDMode = SD_MODE_NORMAL;

    SD_SPI_ChipDeselect();

    //MMC media powers up in the open-drain mode and cannot handle a clock faster
    //than 400kHz. Initialize SPI port to <= 400kHz
    if( SD_SPI_master_open(SDSLOW) == false )
    {
        return false;
    }

    //Media wants the longer of: Vdd ramp time, 1 ms fixed delay, or 74+ clock pulses.
    //According to spec, CS should be high during the 74+ clock pulses.
    //In practice it is preferable to wait much longer than 1ms, in case of
    //contact bounce, or incomplete mechanical insertion (by the time we start
    //accessing the media). 
    SD_SPI_DelayMilliseconds(SD_SPI_STARTUP_DELAY_MS);

    // Send CMD0 (with CS = 0) to reset the media and put SD cards into SPI mode.
    timeout = 100;
    do
    {
        //Toggle chip select, to make media abandon whatever it may have been doing
        //before.  This ensures the CMD0 is sent freshly after CS is asserted low,
        //minimizing risk of SPI clock pulse master/slave synchronization problems, 
        //due to possible application noise on the SCK line.
        SD_SPI_ChipDeselect();
        (void)SD_SPI_exchangeByte(0xFF);  //Send some "extraneous" clock pulses.  If a previous
                                          //command was terminated before it completed normally,
                                          //the card might not have received the required clocking
                                          //following the transfer.
        SD_SPI_ChipSelect();
        timeout--;

        //Send CMD0 to software reset the device
        response = SD_SendCmd(SD_GO_IDLE_STATE, 0x0);
    }while((response.r1._byte != 0x01) && (timeout != 0));
    
    //Check if all attempts failed and we timed out.  Normally, this won't happen,
    //unless maybe the SD card was busy, because it was previously performing a
    //read or write operation, when it was interrupted by the microcontroller getting
    //reset or power cycled, without also resetting or power cycling the SD card.
    //In this case, the SD card may still be busy (ex: trying to respond with the 
    //read request data), and may not be ready to process CMD0.  In this case,
    //we can try to recover by issuing CMD12 (STOP_TRANSMISSION).
    if(timeout == 0)
    {
        SD_SPI_ChipDeselect();
        (void)SD_SPI_exchangeByte(0xFF);        //Send some "extraneous" clock pulses.  If a previous
                                                //command was terminated before it completed normally,
                                                //the card might not have received the required clocking
                                                //following the transfer.
        SD_SPI_ChipSelect();

        //Send CMD12, to stop any read/write transaction that may have been in progress
        (void)SD_SendCmd(SD_STOP_TRANSMISSION, 0x0);    //Blocks until SD card signals non-busy
        //Now retry to send send CMD0 to perform software reset on the media
        response = SD_SendCmd(SD_GO_IDLE_STATE, 0x0);
        if(response.r1._byte != 0x01) //Check if card in idle state now.
        {
            //Card failed to process CMD0 yet again.  At this point, the proper thing
            //to do would be to power cycle the card and retry, if the host 
            //circuitry supports disconnecting the SD card power.  Since the
            //SD/MMC PICtail+ doesn't support software controlled power removal
            //of the SD card, there is nothing that can be done with this hardware.
            //Therefore, we just give up now.  The user needs to physically 
            //power cycle the media and/or the whole board.
            mediaInformation.errorCode = MEDIA_CANNOT_INITIALIZE;
            
            SD_SPI_ChipDeselect();
            SD_SPI_close();
            return false;
        }            
        else
        {
            //Card successfully processed CMD0 and is now in the idle state.
        }    
    }//if(timeout == 0) [for the CMD0 transmit loop]
    else
    {
    }       
    

    //Send CMD8 (SEND_IF_COND) to specify/request the SD card interface condition (ex: indicate what voltage the host runs at).
    //0x000001AA --> VHS = 0001b = 2.7V to 3.6V.  The 0xAA LSB is the check pattern, and is arbitrary, but 0xAA is recommended (good blend of 0's and '1's).
    //The SD card has to echo back the check pattern correctly however, in the R7 response.
    //If the SD card doesn't support the operating voltage range of the host, then it may not respond.
    //If it does support the range, it will respond with a type R7 response packet (6 bytes/48 bits).	        
    //Additionally, if the SD card is MMC or SD card v1.x spec device, then it may respond with
    //invalid command.  If it is a v2.0 spec SD card, then it is mandatory that the card respond
    //to CMD8.
    response = SD_SendCmd(SD_SEND_IF_COND, 0x1AA);   //Note: If changing "0x1AA", CRC value in table must also change.
    if(((response.r7.bytewise.argument._returnVal & 0xFFF) == 0x1AA) && (!response.r7.bitwise.bits.ILLEGAL_CMD))
    {
        //If we get to here, the device supported the CMD8 command and didn't complain about our host
        //voltage range.
        //Most likely this means it is either a v2.0 spec standard or high capacity SD card (SDHC)
 
        //Send CMD58 (Read OCR [operating conditions register]).  Response type is R3, which has 5 bytes.
        //uint8_t 4 = normal R1 response uint8_t, bytes 3-0 are = OCR register value.

        response = SD_SendCmd(SD_READ_OCR, 0x0);

        //Now that we have the OCR register value in the response packet, we could parse
        //the register contents and learn what voltage the SD card wants to run at.
        //If our host circuitry has variable power supply capability, it could 
        //theoretically adjust the SD card Vdd to the minimum of the OCR to save power.
		
        //Now send CMD55/ACMD41 in a loop, until the card is finished with its internal initialization.
        //Note: SD card specs recommend >= 1 second timeout while waiting for ACMD41 to signal non-busy.
        for(timeout = 0; timeout < 0xFFFF; timeout++)
        {				
            //Send CMD55 (lets SD card know that the next command is application specific (going to be ACMD41)).
            (void)SD_SendCmd(SD_APP_CMD, 0x00000000);

            //Send ACMD41.  This is to check if the SD card is finished booting up/ready for full frequency and all
            //further commands.  Response is R3 type (6 bytes/48 bits, middle four bytes contain potentially useful data).
            //Note: When sending ACMD41, the HCS bit is bit 30, and must be = 1 to tell SD card the host supports SDHC
            response = SD_SendCmd(SD_SD_SEND_OP_COND,0x40000000); //bit 30 set

            //The R1 response should be = 0x00, meaning the card is now in the "standby" state, instead of
            //the "idle" state (which is the default initialization state after CMD0 reset is issued).  Once
            //in the "standby" state, the SD card is finished with basic initialization and is ready 
            //for read/write and other commands.
            if(response.r1._byte == 0)
            {
                break;  //Break out of for() loop.  Card is finished initializing.
            }				
        }
		
        if(timeout >= 0xFFFF)
        {
            mediaInformation.errorCode = MEDIA_CANNOT_INITIALIZE;
        }				
		
        //Now send CMD58 (Read OCR register).  The OCR register contains important
        //info we will want to know about the card (ex: standard capacity vs. SDHC).
        response = SD_SendCmd(SD_READ_OCR, 0x0);

        //Now check the CCS bit (OCR bit 30) in the OCR register, which is in our response packet.
        //This will tell us if it is a SD high capacity (SDHC) or standard capacity device.
        if(response.r7.bytewise.argument._returnVal & 0x40000000)    //Note the HCS bit is only valid when the busy bit is also set (indicating device ready).
        {
            mediaInformation.gSDMode = SD_MODE_HC;
        }				
        else
        {
            mediaInformation.gSDMode = SD_MODE_NORMAL;
        } 
        //SD Card should now be finished with initialization sequence.  Device should be ready
        //for read/write commands.

    }//if(((response.r7.bytewise._returnVal & 0xFFF) == 0x1AA) && (!response.r7.bitwise.bits.ILLEGAL_CMD))
    else
    {
        //The CMD8 wasn't supported.  This means the card is not a v2.0 card.
        //Presumably the card is v1.x device, standard capacity (not SDHC).

        /* Need to delay a bit between last command and this command. */
        SD_SPI_DelayMilliseconds(SD_SPI_COMMAND_WAIT_MS);

        SD_SPI_ChipSelect();                              // select the device

        //The CMD8 wasn't supported.  This means the card is definitely not a v2.0 SDHC card.
        mediaInformation.gSDMode = SD_MODE_NORMAL;
        // According to the spec CMD1 must be repeated until the card is fully initialized
        timeout = 0x1FFF;
        do
        {
            //Send CMD1 to initialize the media.
            response = SD_SendCmd(SD_SEND_OP_COND, 0x00000000);    //When argument is 0x00000000, this queries MMC cards for operating voltage range
            timeout--;
        }while((response.r1._byte != 0x00) && (timeout != 0));

        // see if it failed
        if(timeout == 0)
        {
            mediaInformation.errorCode = MEDIA_CANNOT_INITIALIZE;
            SD_SPI_ChipDeselect();                              // deselect the devices
        }
        else
        {            
            //Set read/write block length to 512 bytes.  Note: commented out since
            //this theoretically isn't necessary, since all cards v1 and v2 are 
            //required to support 512 byte block size, and this is supposed to be
            //the default size selected on cards that support other sizes as well.
            //response = SD_SendCmd(SET_BLOCKLEN, 0x00000200);    //Set read/write block length to 512 bytes
        }   
    }

    //Temporarily deselect device
    SD_SPI_ChipDeselect();
    SD_SPI_close();
    if(SD_SPI_master_open(SDFAST) == false)
    {
        return false;
    }
    
    SD_SPI_ChipSelect();

    /* Send the CMD9 to read the CSD register */
    timeout = SD_NCR_TIMEOUT;
    do
    {
        //Send CMD9: Read CSD data structure.
        response = SD_SendCmd(SD_SEND_CSD, 0x00);
        timeout--;
    }while((response.r1._byte != 0x00) && (timeout != 0));

    if(timeout == 0x00)
    {
        //Media failed to respond to the read CSD register operation.        
        mediaInformation.errorCode = MEDIA_CANNOT_INITIALIZE;
        
        SD_SPI_ChipDeselect();
        SD_SPI_close();
        return false;
    }    

    /* According to the simplified spec, section 7.2.6, the card will respond
    with a standard response token, followed by a data block of 16 bytes
    suffixed with a 16-bit CRC.*/
    index = 0;
    for (count = 0; count < 20u; count ++)
    {
        CSDResponse[index] = SD_SPI_exchangeByte(0xFF);
        index++;
        /* Hopefully the first byte is the datatoken, however, some cards do
        not send the response token before the CSD register.*/
        if((count == 0) && (CSDResponse[0] == SD_TOKEN_START))
        {
            /* As the first byte was the datatoken, we can drop it. */
            index = 0;
        }
    }

    //Extract some fields from the response for computing the card capacity.
    //Note: The structure format depends on if it is a CSD V1 or V2 device.
    //Therefore, need to first determine version of the specs that the card 
    //is designed for, before interpreting the individual fields.

    //-------------------------------------------------------------
    //READ_BL_LEN: CSD Structure v1 cards always support 512 uint8_t
    //read and write block lengths.  Some v1 cards may optionally report
    //READ_BL_LEN = 1024 or 2048 bytes (and therefore WRITE_BL_LEN also
    //1024 or 2048).  However, even on these cards, 512 uint8_t partial reads
    //and 512 uint8_t write are required to be supported.
    //On CSD structure v2 cards, it is always required that READ_BL_LEN 
    //(and therefore WRITE_BL_LEN) be 512 bytes, and partial reads and
    //writes are not allowed.
    //Therefore, all cards support 512 byte reads/writes, but only a subset
    //of cards support other sizes.  For best compatibility with all cards,
    //and the simplest firmware design, it is therefore preferable to 
    //simply ignore the READ_BL_LEN and WRITE_BL_LEN values altogether,
    //and simply hardcode the read/write block size as 512 bytes.
    //-------------------------------------------------------------
    mediaInformation.sectorSize = SD_MEDIA_BLOCK_SIZE;

    //Calculate the finalLBA (see SD card physical layer simplified spec 2.0, section 5.3.2).
    //In USB mass storage applications, we will need this information to 
    //correctly respond to SCSI get capacity requests.  Note: method of computing 
    //finalLBA depends on CSD structure spec version (either v1 or v2).
    if(CSDResponse[0] & 0xC0)	//Check CSD_STRUCTURE field for v2+ struct device
    {
        //Must be a v2 device (or a reserved higher version, that doesn't currently exist)

        //Extract the C_SIZE field from the response.  It is a 22-bit number in bit position 69:48.  This is different from v1.  
        //It spans bytes 7, 8, and 9 of the response.
        c_size = (((uint32_t)CSDResponse[7] & 0x3F) << 16) | ((uint16_t)CSDResponse[8] << 8) | CSDResponse[9];

        mediaInformation.finalLBA = ((uint32_t)(c_size + 1) * (uint16_t)(1024u)) - 1; //-1 on end is correction factor, since LBA = 0 is valid.
    }
    else //if(CSDResponse[0] & 0xC0)	//Check CSD_STRUCTURE field for v1 struct device
    {
        //Must be a v1 device.
        //Extract the C_SIZE field from the response.  It is a 12-bit number in bit position 73:62.  
        //Although it is only a 12-bit number, it spans bytes 6, 7, and 8, since it isn't byte aligned.
        c_size = ((uint32_t)CSDResponse[6] << 16) | ((uint16_t)CSDResponse[7] << 8) | CSDResponse[8];	//Get the bytes in the correct positions
        c_size &= 0x0003FFC0;	//Clear all bits that aren't part of the C_SIZE
        c_size = c_size >> 6;	//Shift value down, so the 12-bit C_SIZE is properly right justified in the uint32_t.

        //Extract the C_SIZE_MULT field from the response.  It is a 3-bit number in bit position 49:47.
        c_size_mult = ((uint16_t)((CSDResponse[9] & 0x03) << 1)) | ((uint16_t)((CSDResponse[10] & 0x80) >> 7));

        //Extract the BLOCK_LEN field from the response. It is a 4-bit number in bit position 83:80.
        block_len = CSDResponse[5] & 0x0F;

        block_len = 1 << (block_len - 9); //-9 because we report the size in sectors of 512 bytes each

        //Calculate the finalLBA (see SD card physical layer simplified spec 2.0, section 5.3.2).
        //In USB mass storage applications, we will need this information to 
        //correctly respond to SCSI get capacity requests (which will cause SD_CapacityRead() to get called).
        mediaInformation.finalLBA = ((uint32_t)(c_size + 1) * (uint16_t)((uint16_t)1 << (c_size_mult + 2)) * block_len) - 1;	//-1 on end is correction factor, since LBA = 0 is valid.		
    }	

    //Turn off CRC7 if we can, might be an invalid cmd on some cards (CMD59)
    //Note: POR default for the media is normally with CRC checking off in SPI 
    //mode anyway, so this is typically redundant.
    (void)SD_SendCmd(SD_CRC_ON_OFF, 0x0);

    //Now set the block length to media sector size. It should be already set to this.
    (void)SD_SendCmd(SD_SET_BLOCK_LENGTH , mediaInformation.sectorSize);

    //Deselect media while not actively accessing the card.
    SD_SPI_ChipDeselect();

    //Finished with the SD card initialization sequence.
    if(mediaInformation.errorCode == MEDIA_NO_ERROR)
    {
        mediaInformation.state = SD_STATE_READY_FOR_COMMAND;
        SD_SPI_close();
        return true;
    }
    
    SD_SPI_close();
    return false;
}//end MediaInitialize

static uint8_t SD_SPI_AsyncReadTasks(struct SD_ASYNC_IO* info)
{
    uint8_t bData;
    SD_RESPONSE response;
    static uint16_t blockCounter;
    static uint32_t longTimeoutCounter;
    static bool SingleBlockRead;

    //Check what state we are in, to decide what to do.
    switch(info->bStateVariable)
    {
        case SD_ASYNC_READ_COMPLETE:
            return SD_ASYNC_READ_COMPLETE;
        case SD_ASYNC_READ_QUEUED:
            //Start the read request.

            //Initialize some variables we will use later.
            mediaInformation.state = SD_STATE_BUSY;       //Set global media status to busy, so no other code tries to send commands to the SD card
            blockCounter =  SD_MEDIA_BLOCK_SIZE; //Counter will be used later for block boundary tracking
            ioInfo = *info; //Get local copy of structure, for quicker access with less code size

            //SDHC cards are addressed on a 512 byte block basis.  This is 1:1 equivalent
            //to LBA addressing.  For standard capacity media, the media is expecting
            //a complete byte address.  Therefore, to convert from the LBA address to the
            //byte address, we need to multiply by 512.
            if (mediaInformation.gSDMode == SD_MODE_NORMAL)
            {
                ioInfo.dwAddress <<= 9; //Equivalent to multiply by 512
            }  
            if(ioInfo.dwBytesRemaining <=  SD_MEDIA_BLOCK_SIZE)
            {
                SingleBlockRead = true;
                response = SD_SendCmd(SD_READ_SINGLE_BLOCK, ioInfo.dwAddress);
            }    
            else
            {
                SingleBlockRead = false;
                response = SD_SendCmd(SD_READ_MULTI_BLOCK, ioInfo.dwAddress);
            }    
            //Note: SendMMCmd() sends 8 SPI clock cycles after getting the
            //response.  This meets the NAC min timing parameter, so we don't
            //need additional clocking here.
            
            // Make sure the command was accepted successfully
            if(response.r1._byte != 0x00)
            {
                //Perhaps the card isn't initialized or present.
                info->bStateVariable = SD_ASYNC_READ_ABORT;
                return SD_ASYNC_READ_BUSY; 
            }
            
            //We successfully sent the READ_MULTI_BLOCK command to the media.
            //We now need to keep polling the media until it sends us the data
            //start token byte.
            longTimeoutCounter = SD_NAC_TIMEOUT; //prepare timeout counter for next state
            info->bStateVariable = SD_ASYNC_READ_WAIT_START_TOKEN;
            return SD_ASYNC_READ_BUSY;
            
        case SD_ASYNC_READ_WAIT_START_TOKEN:
            //In this case, we have already issued the READ_MULTI_BLOCK command
            //to the media, and we need to keep polling the media until it sends
            //us the data start token byte.  This could typically take a
            //couple/few milliseconds, up to a maximum of 100ms.
            if(longTimeoutCounter != 0x00000000)
            {
                longTimeoutCounter--;
                bData = SD_SPI_exchangeByte(0xFF);
                
                if(bData != SD_TOKEN_FLOATING_BUS)
                {
                    if(bData == SD_TOKEN_START)
                    {   
                        //We got the start token.  Ready to receive the data
                        //block now.
                        info->bStateVariable = SD_ASYNC_READ_NEW_PACKET_READY;
                        return SD_ASYNC_READ_NEW_PACKET_READY;
                    }
                    else
                    {
                        //We got an unexpected non-0xFF, non-start token byte back?
                        //Some kind of error must have occurred. 
                        info->bStateVariable = SD_ASYNC_READ_ABORT; 
                        return SD_ASYNC_READ_BUSY;
                    }        
                }
                else
                {
                    //Media is still busy.  Start token not received yet.
                    return SD_ASYNC_READ_BUSY;
                }                    
            } 
            else
            {
                //The media didn't respond with the start data token in the timeout
                //interval allowed.  Operation failed.  Abort the operation.
                info->bStateVariable = SD_ASYNC_READ_ABORT; 
                return SD_ASYNC_READ_BUSY;                
            }       
            break;
            
        case SD_ASYNC_READ_NEW_PACKET_READY:
            //We have sent the READ_MULTI_BLOCK command and have successfully
            //received the data start token byte.  Therefore, we are ready
            //to receive raw data bytes from the media.
            if(ioInfo.dwBytesRemaining != 0x00000000)
            {
                //Re-update local copy of pointer and number of bytes to read in this
                //call.  These parameters are allowed to change between packets.
                ioInfo.wNumBytes = info->wNumBytes;
                ioInfo.pBuffer = info->pBuffer;

                //Update counters for state tracking and loop exit condition tracking.
                ioInfo.dwBytesRemaining -= ioInfo.wNumBytes;
                blockCounter -= ioInfo.wNumBytes;

                //Now read a ioInfo.wNumbytes packet worth of SPI bytes, 
                //and place the received bytes in the user specified pBuffer.
                (void)memset(ioInfo.pBuffer, 0xFF, ioInfo.wNumBytes);
                SD_SPI_exchangeBlock(ioInfo.pBuffer, ioInfo.wNumBytes);

                //Check if we have received a multiple of the media block 
                //size (ex: 512 bytes).  If so, the next two bytes are going to 
                //be CRC values, rather than data bytes.  
                if(blockCounter == 0)
                {
                    //Read two bytes to receive the CRC-16 value on the data block.
                    (void)SD_SPI_exchangeByte(0xFF);
                    (void)SD_SPI_exchangeByte(0xFF);
                    //Following sending of the CRC-16 value, the media may still
                    //need more access time to internally fetch the next block.
                    //Therefore, it will send back 0xFF idle value, until it is
                    //ready.  Then it will send a new data start token, followed
                    //by the next block of useful data.
                    if(ioInfo.dwBytesRemaining != 0x00000000)
                    {
                        info->bStateVariable = SD_ASYNC_READ_WAIT_START_TOKEN;
                    }
                    blockCounter =  SD_MEDIA_BLOCK_SIZE;
                    return SD_ASYNC_READ_BUSY;
                }
                    
                return SD_ASYNC_READ_NEW_PACKET_READY;
            }//if(ioInfo.dwbytesRemaining != 0x00000000)
            else
            {
                //We completed the read operation successfully and have returned
                //all data bytes requested.
                //Send CMD12 to let the media know we are finished reading
                //blocks from it, if we sent a multi-block read request earlier.
                if(SingleBlockRead == false)
                {
                    (void)SD_SendCmd(SD_STOP_TRANSMISSION, 0x00000000);
                }
                SD_SPI_ChipDeselect();      // De-select media
                (void)SD_SPI_exchangeByte(0xFF);
                info->bStateVariable = SD_ASYNC_READ_COMPLETE;
                mediaInformation.state = SD_STATE_READY_FOR_COMMAND;       //Free the media for new commands, since we are now done with it
                return SD_ASYNC_READ_COMPLETE;
            }
        case SD_ASYNC_READ_ABORT:
            //If the application firmware wants to cancel a read request.
            info->bStateVariable = SD_ASYNC_READ_ERROR;
            //Send CMD12 to terminate the multi-block read request.
            response = SD_SendCmd(SD_STOP_TRANSMISSION, 0x00000000);
            //Fall through to SD_ASYNC_READ_ERROR/default case.
        case SD_ASYNC_READ_ERROR:
        default:
            //Some error must have happened.
            SD_SPI_ChipDeselect();       // De-select media
            (void)SD_SPI_exchangeByte(0xFF);
            mediaInformation.state = SD_STATE_READY_FOR_COMMAND;
            return SD_ASYNC_READ_ERROR; 
    }//switch(info->stateVariable)    
}    


static uint8_t SD_SPI_AsyncWriteTasks(struct SD_ASYNC_IO* info)
{
    static uint8_t data_byte;
    static uint16_t blockCounter;
    static uint32_t WriteTimeout;
    static uint8_t command;
    uint32_t preEraseBlockCount;
    SD_RESPONSE response;
    
    //Check what state we are in, to decide what to do.
    switch(info->bStateVariable)
    {
        case SD_ASYNC_WRITE_COMPLETE:
            return SD_ASYNC_WRITE_COMPLETE;
            
        case SD_ASYNC_WRITE_QUEUED:
            //Initiate the write sequence.
            mediaInformation.state = SD_STATE_BUSY;         //Let other code in the app know that the media is busy (so it doesn't also try to send the SD card commands of it's own)
            blockCounter = SD_MEDIA_BLOCK_SIZE;    //Initialize counter.  Will be used later for block boundary tracking.

            //Copy input structure into a statically allocated global instance 
            //of the structure, for faster local access of the parameters with 
            //smaller code size.
            ioInfo = *info;

            //Check if we are writing only a single block worth of data, or 
            //multiple blocks worth of data.
            if(ioInfo.dwBytesRemaining <=  SD_MEDIA_BLOCK_SIZE)
            {
                command = SD_WRITE_SINGLE_BLOCK;
            }    
            else
            {
                command = SD_WRITE_MULTI_BLOCK;
                
                //Compute the number of blocks that we are going to be writing in this multi-block write operation.
                preEraseBlockCount = (ioInfo.dwBytesRemaining >> 9); //Divide by 512 to get the number of blocks to write
                //Always need to erase at least one block.
                if(preEraseBlockCount == 0)
                {
                    preEraseBlockCount++;   
                } 
    
                //Should send CMD55/ACMD23 to let the media know how many blocks it should 
                //pre-erase.  This isn't essential, but it allows for faster multi-block 
                //writes, and probably also reduces flash wear on the media.
                response = SD_SendCmd(SD_APP_CMD, 0x00000000);    //Send CMD55
                if(response.r1._byte == 0x00)   //Check if successful.
                {
                    SD_SendCmd(SD_SET_WRITE_BLOCK_ERASE_COUNT , preEraseBlockCount);    //Send ACMD23
                }
            }    

            //The info->dwAddress parameter is the block address.
            //For standard capacity SD cards, the card expects a complete byte address.
            //To convert the block address into a byte address, we multiply by the block size (512).
            //For SDHC (high capacity) cards, the card expects a block address already, so no
            //address conversion is needed
            if (mediaInformation.gSDMode == SD_MODE_NORMAL)  
            {
                ioInfo.dwAddress <<= 9;   //<< 9 multiplies by 512
            }    

            //Send the write single or write multi command, with the LBA or byte 
            //address (depending upon SDHC or standard capacity card)
            response = SD_SendCmd(command, ioInfo.dwAddress);

            //See if it was accepted
            if(response.r1._byte != 0x00)
            {
                //Perhaps the card isn't initialized or present.
                info->bStateVariable = SD_ASYNC_WRITE_ERROR;
                return SD_ASYNC_WRITE_ERROR; 
            }    
            else
            {
                //Card is ready to receive start token and data bytes.
                info->bStateVariable = SD_ASYNC_WRITE_TRANSMIT_PACKET;
            } 
            return SD_ASYNC_WRITE_SEND_PACKET;   

        case SD_ASYNC_WRITE_TRANSMIT_PACKET:
            //Check if we just finished programming a block, or we are starting
            //for the first time.  In this case, we need to send the data start token.
            if(blockCounter ==  SD_MEDIA_BLOCK_SIZE)
            {
                //Send the correct data start token, based on the type of write we are doing
                if(command == SD_WRITE_MULTI_BLOCK)
                {
                    (void)SD_SPI_exchangeByte (SD_TOKEN_START_MULTI_BLOCK);
                }
                else
                {
                    //Else must be a single block write
                    (void)SD_SPI_exchangeByte (SD_TOKEN_START);
                }        
            } 
               
            //Update local copy of pointer and uint8_t count.  Application firmware
            //is allowed to change these between calls to this handler function.
            ioInfo.wNumBytes = info->wNumBytes;
            ioInfo.pBuffer = info->pBuffer;
            
            //Keep track of variables for loop/state exit conditions.
            ioInfo.dwBytesRemaining -= ioInfo.wNumBytes;
            blockCounter -= ioInfo.wNumBytes;
            
            SD_SPI_exchangeBlock(ioInfo.pBuffer, ioInfo.wNumBytes);
 
            //Check if we have finished sending a 512 byte block.  If so,
            //need to receive 16-bit CRC, and retrieve the data_response token
            if(blockCounter == 0)
            {
                blockCounter =  SD_MEDIA_BLOCK_SIZE;    //Re-initialize counter
                
                //This version does not calculate CRC, send dummy data.
                (void)SD_SPI_exchangeByte(0xFF);
                (void)SD_SPI_exchangeByte(0xFF);
                
                //Read response token byte from media, mask out top three don't 
                //care bits, and check if there was an error
                if((SD_SPI_exchangeByte(0xFF) &  SD_WRITE_RESPONSE_TOKEN_MASK) != SD_TOKEN_DATA_ACCEPTED)
                {
                    //Something went wrong.  Try and terminate as gracefully as 
                    //possible, so as allow possible recovery.
                    info->bStateVariable = SD_ASYNC_WRITE_ABORT; 
                    return SD_ASYNC_WRITE_BUSY;
                }
                
                //The media will now send busy token (0x00) bytes until
                //it is internally ready again (after the block is successfully
                //written and the card is ready to accept a new block.
                info->bStateVariable = SD_ASYNC_WRITE_MEDIA_BUSY;
                WriteTimeout = SD_WRITE_TIMEOUT;       //Initialize timeout counter
                return SD_ASYNC_WRITE_BUSY;
            }//if(blockCounter == 0)
            
            //If we get to here, we haven't reached a block boundary yet.  Keep 
            //on requesting packets of data from the application.
            return SD_ASYNC_WRITE_SEND_PACKET;   

        case SD_ASYNC_WRITE_MEDIA_BUSY:
            if(WriteTimeout != 0)
            {
                WriteTimeout--;
                (void)SD_SPI_exchangeByte(0xFF);  //Dummy read to gobble up a uint8_t (ex: to ensure we meet NBR timing parameter)
                data_byte = SD_SPI_exchangeByte(0xFF);  //Poll the media.  Will return 0x00 if still busy.  Will return non-0x00 is ready for next data block.
                if(data_byte != 0x00)
                {
                    //The media is done and is no longer busy.  Go ahead and
                    //either send the next packet of data to the media, or the stop
                    //token if we are finished.
                    if(ioInfo.dwBytesRemaining == 0)
                    {
                        WriteTimeout = SD_WRITE_TIMEOUT;
                        if(command == SD_WRITE_MULTI_BLOCK)
                        {
                            //We finished sending all bytes of data.  Send the stop token uint8_t.
                            (void)SD_SPI_exchangeByte (SD_TOKEN_STOP_TRANSMISSION);
                            
                            //After sending the stop transmission token, we need to
                            //gobble up one uint8_t before checking for media busy (0x00).
                            //This is to meet the NBR timing parameter.  During the NBR
                            //interval the SD card may not respond with the busy signal, even
                            //though it is internally busy.
                            (void)SD_SPI_exchangeByte(0xFF);
                                                
                            //The media still needs to finish internally writing.
                            info->bStateVariable = SD_ASYNC_STOP_TOKEN_SENT_WAIT_BUSY;
                            return SD_ASYNC_WRITE_BUSY;
                        }
                        else
                        {
                            //In this case we were doing a single block write,
                            //so no stop token is necessary.  In this case we are
                            //now fully complete with the write operation.
                            SD_SPI_ChipDeselect();       // De-select media
                            (void)SD_SPI_exchangeByte(0xFF);
                            info->bStateVariable = SD_ASYNC_WRITE_COMPLETE;
                            mediaInformation.state = SD_STATE_READY_FOR_COMMAND;       //Free the media for new commands, since we are now done with it
                            return SD_ASYNC_WRITE_COMPLETE;                            
                        }                            
                        
                    }
                    //Else we have more data to write in the multi-block write.    
                    info->bStateVariable = SD_ASYNC_WRITE_TRANSMIT_PACKET;  
                    return SD_ASYNC_WRITE_SEND_PACKET;                    
                }    
                else
                {
                    //The media is still busy.
                    return SD_ASYNC_WRITE_BUSY;
                }    
            }
            else
            {
                //Timeout occurred.  Something went wrong.  The media should not 
                //have taken this long to finish the write.
                info->bStateVariable = SD_ASYNC_WRITE_ABORT;
                return SD_ASYNC_WRITE_BUSY;
            }        
        
        case SD_ASYNC_STOP_TOKEN_SENT_WAIT_BUSY:
            //We already sent the stop transmit token for the multi-block write 
            //operation.  Now all we need to do, is keep waiting until the card
            //signals it is no longer busy.  Card will keep sending 0x00 bytes
            //until it is no longer busy.
            if(WriteTimeout != 0)
            {
                WriteTimeout--;
                data_byte = SD_SPI_exchangeByte (0xFF);
                //Check if card is no longer busy.  
                if(data_byte != 0x00)
                {
                    //If we get to here, multi-block write operation is fully
                    //complete now.  

                    //Should send CMD13 (SEND_STATUS) after a programming sequence, 
                    //to confirm if it was successful or not inside the media.
                                
                    //Prepare to receive the next command.
                    SD_SPI_ChipDeselect();       // De-select media
                    (void)SD_SPI_exchangeByte(0xFF);  //NEC timing parameter clocking
                    info->bStateVariable = SD_ASYNC_WRITE_COMPLETE;
                    mediaInformation.state = SD_STATE_READY_FOR_COMMAND;       //Free the media for new commands, since we are now done with it
                    return SD_ASYNC_WRITE_COMPLETE;
                }
                //If we get to here, the media is still busy with the write.
                return SD_ASYNC_WRITE_BUSY;    
            }    
            //Timeout occurred.  Something went wrong.  Fall through to SD_ASYNC_WRITE_ABORT.
        case SD_ASYNC_WRITE_ABORT:
            //An error occurred, and we need to stop the write sequence so as to try and allow
            //for recovery/re-attempt later.
            (void)SD_SendCmd(SD_STOP_TRANSMISSION, 0x00000000);
            SD_SPI_ChipDeselect();  // De-select media
            (void)SD_SPI_exchangeByte(0xFF);  //After raising CS pin, media may not tri-state data out for 1 bit time.
            info->bStateVariable = SD_ASYNC_WRITE_ERROR; 
            //Fall through to default case.
        default:
            //Used for SD_ASYNC_WRITE_ERROR case.
            mediaInformation.state = SD_STATE_READY_FOR_COMMAND;       //Free the media for new commands, since we are now done with it
            return SD_ASYNC_WRITE_ERROR; 
    }//switch(info->stateVariable)    
} 

static SD_RESPONSE SD_SendCmd (uint8_t cmd, uint32_t address)
{   
    SD_RESPONSE    response;
    uint16_t timeout;
    uint32_t longTimeout;
    uint8_t address_bytes[4];
    
    SD_SPI_ChipSelect();
    
    (void)memcpy(address_bytes, &address, sizeof(address));
    
    (void)SD_SPI_exchangeByte(sdmmc_cmdtable[cmd].CmdCode | SD_COMMAND_TRANSMIT_BIT_MASK);
    
    (void)SD_SPI_exchangeByte(address_bytes[3]);
    (void)SD_SPI_exchangeByte(address_bytes[2]);
    (void)SD_SPI_exchangeByte(address_bytes[1]);
    (void)SD_SPI_exchangeByte(address_bytes[0]);  
    
    (void)SD_SPI_exchangeByte(sdmmc_cmdtable[cmd].CRC);      

    //Special handling for CMD12 (STOP_TRANSMISSION).  The very first byte after
    //sending the command packet may contain bogus non-0xFF data.  This 
    //"residual data" byte should not be interpreted as the R1 response byte.
    if(cmd == SD_STOP_TRANSMISSION)
    {
        (void)SD_SPI_exchangeByte(0xFF); //Perform dummy read to fetch the residual non R1 byte
    } 

    //Loop until we get a response from the media.  Delay (NCR) could be up 
    //to 8 SPI byte times.  First byte of response is always the equivalent of 
    //the R1 byte, even for R1b, R2, R3, R7 responses.
    timeout = SD_NCR_TIMEOUT;
    do
    {
        response.r1._byte = SD_SPI_exchangeByte(0xFF);
        timeout--;
    }while((response.r1._byte == SD_TOKEN_FLOATING_BUS) && (timeout != 0));
    
    //Check if we should read more bytes, depending upon the response type expected.  
    if(sdmmc_cmdtable[cmd].responsetype == SD_RESPONSE_R2)
    {
        response.r2._byte1 = response.r1._byte; //We already received the first byte, just make sure it is in the correct location in the struct.
        response.r2._byte0 = SD_SPI_exchangeByte(0xFF); //Fetch the second byte of the response.
    }
    else if(sdmmc_cmdtable[cmd].responsetype == SD_RESPONSE_R1b)
    {
        //Keep trying to read from the media, until it signals it is no longer
        //busy.  It will continuously send 0x00 bytes until it is not busy.
        //A non-zero value means it is ready for the next command.
        //The R1b response is received after a CMD12 STOP_TRANSMISSION
        //command, where the media card may be busy writing its internal buffer
        //to the flash memory.  This can typically take a few milliseconds, 
        //with a recommended maximum timeout of 250ms or longer for SD cards.
        longTimeout = SD_WRITE_TIMEOUT;
        do
        {
            response.r1._byte = SD_SPI_exchangeByte (0xFF);
            longTimeout--;
        }while((response.r1._byte == 0x00) && (longTimeout != 0));

        response.r1._byte = 0x00;
    }
    else if (sdmmc_cmdtable[cmd].responsetype == SD_RESPONSE_R7) //also used for response R3 type
    {
        //Fetch the other four bytes of the R3 or R7 response.
        //Note: The SD card argument response field is 32-bit, big endian format.
        //However, the C compiler stores 32-bit values in little endian in RAM.
        //When writing to the _returnVal/argument bytes, make sure the order it 
        //gets stored in is correct.      
        response.r7.bytewise.argument._byte3 = SD_SPI_exchangeByte(0xFF);
        response.r7.bytewise.argument._byte2 = SD_SPI_exchangeByte(0xFF);
        response.r7.bytewise.argument._byte1 = SD_SPI_exchangeByte(0xFF);
        response.r7.bytewise.argument._byte0 = SD_SPI_exchangeByte(0xFF);
    }

    //Device requires at least 8 clock pulses after
    //the response has been sent, before if can process
    //the next command.  CS may be high or low.
    (void)SD_SPI_exchangeByte(0xFF);    

    // see if we are expecting more data or not
    if( sdmmc_cmdtable[cmd].moreDataExpected == false )
    {
        SD_SPI_ChipDeselect();
    }

    return(response);
}

