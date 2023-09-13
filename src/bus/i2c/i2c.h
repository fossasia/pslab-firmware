#ifndef _I2C_H
#define _I2C_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <xc.h>

#include "../../commands.h"

#define SLAVE_I2C_GENERIC_RETRY_MAX           100
#define SLAVE_I2C_GENERIC_DEVICE_TIMEOUT      50

#define I2C_ENABLE_INTERRUPTS                     true
#define I2C_DISABLE_INTERRUPTS                    false

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
        I2C_RESPONSE_NEGATIVE_ACKNOWLEDGE = 0,
        I2C_RESPONSE_ACKNOWLEDGE          = 1
    } I2C_RESPONSE;

    typedef enum {
        I2C_BAUD_RATE_100KHZ = 0x272,
        I2C_BAUD_RATE_400KHZ = 0x90,
        I2C_BAUD_RATE_1MHZ   = 0x32
    } I2C_BAUD_RATES;
    /**
      I2C Driver Message Status Type Enumeration

      @Summary
        Defines the different message status when processing TRBs.

      @Description
        This type enumeration specifies the different types of status
        that an i2c request will have. For every submission of an i2c
        transaction, the status of that transaction is available.
        Based on the status, new transactions can be requested to the
        driver or a recovery can be performed to resend the transaction.
     */
    typedef enum {
        I2C_MESSAGE_FAIL,
        I2C_MESSAGE_PENDING,
        I2C_MESSAGE_COMPLETE,
        I2C_STUCK_START,
        I2C_MESSAGE_ADDRESS_NO_ACK,
        I2C_DATA_NO_ACK,
        I2C_LOST_STATE
    } I2C_MESSAGE_STATUS;

    /**
      I2C Driver Transaction Request Block (TRB) type definition.

      @Summary
        This defines the Transaction Request Block (TRB) used by the
        i2c master in sending/receiving data to the i2c bus.

      @Description
        This data type is the i2c Transaction Request Block (TRB) that
        needs to be built and sent to the driver to handle each i2c requests.
        Using the TRB, simple to complex i2c transactions can be constructed
        and sent to the i2c bus. This data type is only used by the master mode.
     */
    typedef struct {
        // Bits <10:1> are the 10 bit address.
        // Bits <7:1> are the 7 bit address
        // Bit 0 is R/W (1 for read)
        uint16_t address;
        // the # of bytes in the buffer
        uint8_t length;
        // a pointer to a buffer of length bytes
        uint8_t *pbuffer;
    } I2C_TRANSACTION_REQUEST_BLOCK;

    /**
      Section: Interface Routines
     */
    /**
      @Summary
        Initializes the I2C instance

      @Description
        This routine initializes the i2c driver instance for
        index, making it ready for clients to open and use it.
        This routine must be called before any other I2C routine is called.
        This routine should only be called once during system initialization.

      @Preconditions
        None.

      @Param
        None.

      @Returns
        None.

      @Example
        <code>
            #define SLAVE_I2C_GENERIC_RETRY_MAX           100
            #define SLAVE_I2C_GENERIC_DEVICE_TIMEOUT      50   // define slave timeout 
 
            // initialize the module
            I2C_Initialize();

            // write to an EEPROM Device
        
            uint16_t        dataAddress;
            uint8_t         sourceData[16] = {  0xA0, 0xA1, 0xA2, 0xA3, 
                                                0xA4, 0xA5, 0xA6, 0xA7, 
                                                0xA8, 0xA9, 0xAA, 0xAB, 
                                                0xAC, 0xAD, 0xAE, 0xAF }; 
            uint8_t         *pData;
            uint16_t        nCount;

            uint8_t         writeBuffer[3];
            uint8_t         *pD;
            uint16_t        counter, timeOut, slaveTimeOut;

            I2C_MESSAGE_STATUS status = I2C_MESSAGE_PENDING;

            dataAddress = 0x10;             // starting EEPROM address 
            pD = sourceData;                // initialize the source of the data
            nCount = 16;                    // number of bytes to write

            for (counter = 0; counter < nCount; counter++)
            {
                // build the write buffer first
                // starting address of the EEPROM memory
                writeBuffer[0] = (dataAddress >> 8);                // high address
                writeBuffer[1] = (uint8_t)(dataAddress);            // low low address

                // data to be written
                writeBuffer[2] = *pD++;

                // Now it is possible that the slave device will be slow.
                // As a work around on these slaves, the application can
                // retry sending the transaction
                timeOut = 0;
                slaveTimeOut = 0;
 
                while(status != I2C_MESSAGE_FAIL) {
                    // write one byte to EEPROM (3 is the number of bytes to write)
                    I2C_MasterWrite(writeBuffer,
                                    3,
                                    slaveDeviceAddress,
                                    &status);

                    // wait for the message to be sent or status has changed.
                    while(status == I2C_MESSAGE_PENDING) {
                        // add some delay here

                        // timeout checking
                        // check for max retry and skip this byte
                        if (slaveTimeOut == SLAVE_I2C_GENERIC_DEVICE_TIMEOUT)
                            break;
                        else
                            slaveTimeOut++;
                    } 
                    if ((slaveTimeOut == SLAVE_I2C_GENERIC_DEVICE_TIMEOUT) || 
                        (status == I2C_MESSAGE_COMPLETE))
                        break;

                    // if status is  I2C_MESSAGE_ADDRESS_NO_ACK, or
                    //               I2C_DATA_NO_ACK,
                    // The device may be busy and needs more time for the last
                    // write so we can retry writing the data, this is why we
                    // use a while loop here

                    // check for max retry and skip this byte
                    if (timeOut == SLAVE_I2C_GENERIC_RETRY_MAX)
                        break;
                    else
                        timeOut++;
                }

                if (status == I2C_MESSAGE_FAIL) {
                    break;
                }
                dataAddress++;
            }
        </code>
     */
    void I2C_Initialize(void);

    void I2C_InitializeCON(void);
    void I2C_InitializeSTAT(void);

    /**
     *  @Summary 
     *      Initialize I2C interface if it is not initialized before
     * 
     *  @Description
     *      This method will help making I2C transactions execute faster by
     *      reinitializing registers only if the settings are new. The only
     *      setting that can be changed is the baud rate and if it is the 
     *      same for a different transactions, this will skip the initialization
     *      process to move right away to transaction.
     * 
     *  @Param
     *      baud_rate - Speed at which the communication takes place. Could be
     *      I2C_BAUD_RATE_100KHZ or I2C_BAUD_RATE_400KHZ or I2C_BAUD_RATE_1MHZ
     * 
     *  @Param
     *      interrupts - Boolean condition to whether enable or disable
     *      interrupts. Note that it is necessary to enable interrupts with
     *      newly implemented I2C_BulkRead and I2C_BulkWrite functions. However
     *      interrupts should be disabled for generic I2C access functions.
     *      Support for generic I2C functions will be depreciated in a future
     *      release.
     * 
     *  @Returns
     *      None
     */
    void I2C_InitializeIfNot(I2C_BAUD_RATES baud_rate, bool interrupts);

    /**
        @Summary
            Handles one i2c master write transaction with the
            supplied parameters.

        @Description
            This function prepares a TRB, then inserts it on the i2c queue.
            Finally, it waits for the transaction to complete and returns
            the result.

        @Preconditions
            None

        @Param
            address - The address of the i2c peripheral to be accessed

        @Param
            length - The length of the data block to be sent
    
        @Param
            *pdata - A pointer to the block of data to be sent
    
        @Param
            *pstatus - A pointer to the status variable that the i2c driver
                updates during the execution of the message.

        @Returns
            I2C_MESSAGE_STATUS

         @Example
            <code>
                Refer to I2C_Initialize() and 
                I2C_MasterRead() for an examples	
            </code>
     */
    void I2C_MasterWrite(
            uint8_t *pdata,
            uint8_t length,
            uint16_t address,
            I2C_MESSAGE_STATUS *pstatus);

    /**
        @Summary
            Handles one i2c master read transaction with the
            supplied parameters.

        @Description
            This function prepares a TRB, then inserts it on the i2c queue.
            Finally, it waits for the transaction to complete and returns
            the result.

        @Preconditions
            None

        @Param
            address - The address of the i2c peripheral to be accessed
    
        @Param
            length - The length of the data block to be sent
    
        @Param
            *pdata - A pointer to the memory location where received data will
                     be stored

        @Param
            *pstatus - A pointer to the status variable that the i2c driver
                updates during the execution of the message.

        @Returns
            I2C_MESSAGE_STATUS

        @Example
            <code>
                #define MCHP24AA512_RETRY_MAX       100  // define the retry count
                #define MCHP24AA512_ADDRESS         0x50 // slave device address
                #define MCHP24AA512_DEVICE_TIMEOUT  50   // define slave timeout 


                uint8_t MCHP24AA512_Read(
                                         uint16_t address,
                                         uint8_t *pData,
                                         uint16_t nCount)
                {
                    I2C_MESSAGE_STATUS status;
                    uint8_t     writeBuffer[3];
                    uint16_t    retryTimeOut, slaveTimeOut;
                    uint16_t    counter;
                    uint8_t     *pD;

                    pD = pData;

                    for (counter = 0; counter < nCount; counter++) {
                        // build the write buffer first
                        // starting address of the EEPROM memory
                        writeBuffer[0] = (address >> 8);                // high address
                        writeBuffer[1] = (uint8_t)(address);            // low low address

                        // Now it is possible that the slave device will be slow.
                        // As a work around on these slaves, the application can
                        // retry sending the transaction
                        retryTimeOut = 0;
                        slaveTimeOut = 0;

                        while(status != I2C_MESSAGE_FAIL) {
                            // write one byte to EEPROM (2 is the count of bytes to write)
                            I2C_MasterWrite(writeBuffer,
                                            2,
                                            MCHP24AA512_ADDRESS,
                                            &status);

                            // wait for the message to be sent or status has changed.
                            while(status == I2C_MESSAGE_PENDING) {
                                // add some delay here

                                // timeout checking
                                // check for max retry and skip this byte
                                if (slaveTimeOut == MCHP24AA512_DEVICE_TIMEOUT)
                                    return (0);
                                else
                                    slaveTimeOut++;
                            }

                            if (status == I2C_MESSAGE_COMPLETE)
                                break;

                            // if status is  I2C_MESSAGE_ADDRESS_NO_ACK, or
                            //               I2C_DATA_NO_ACK,
                            // The device may be busy and needs more time for the last
                            // write so we can retry writing the data, this is why we
                            // use a while loop here

                            // check for max retry and skip this byte
                            if (retryTimeOut == MCHP24AA512_RETRY_MAX)
                                break;
                            else
                                retryTimeOut++;
                        }

                        if (status == I2C_MESSAGE_COMPLETE) {

                            // this portion will read the byte from the memory location.
                            retryTimeOut = 0;
                            slaveTimeOut = 0;

                            while(status != I2C_MESSAGE_FAIL) {
                                // write one byte to EEPROM (2 is the count of bytes to write)
                                I2C_MasterRead(pD,
                                               1,
                                               MCHP24AA512_ADDRESS,
                                               &status);

                                // wait for the message to be sent or status has changed.
                                while(status == I2C_MESSAGE_PENDING) {
                                    // add some delay here

                                    // timeout checking
                                    // check for max retry and skip this byte
                                    if (slaveTimeOut == MCHP24AA512_DEVICE_TIMEOUT)
                                        return (0);
                                    else
                                        slaveTimeOut++;
                                }

                                if (status == I2C_MESSAGE_COMPLETE)
                                    break;

                                // if status is  I2C_MESSAGE_ADDRESS_NO_ACK, or
                                //               I2C_DATA_NO_ACK,
                                // The device may be busy and needs more time for the last
                                // write so we can retry writing the data, this is why we
                                // use a while loop here

                                // check for max retry and skip this byte
                                if (retryTimeOut == MCHP24AA512_RETRY_MAX)
                                    break;
                                else
                                    retryTimeOut++;
                            }
                        }

                        // exit if the last transaction failed
                        if (status == I2C_MESSAGE_FAIL) {
                            return(0);
                            break;
                        }
                        pD++;
                        address++;
                    }
                    return(1);
                }
            </code>
     */
    void I2C_MasterRead(
            uint8_t *pdata,
            uint8_t length,
            uint16_t address,
            I2C_MESSAGE_STATUS *pstatus);

    /**
        @Summary
            Inserts a list of i2c transaction requests into the i2c
            transaction queue.

        @Description
            The i2c processes lists of transaction requests.  Each transaction
            list is handled as a string of i2c restarts.  When the list of
            transactions is complete, an i2c stop is produced, the flag is set
            with the correct condition code and the next list is processed
            from the queue.

            This function inserts lists of requests prepared by the user
            application into the queue along with a pointer to the completion
            flag.

            The transaction is inserted into the list only if there is space
            in the list. If there is no space, the function exits with the
            flag set to I2C_MESSAGE_FAIL.

        @Preconditions
            None

        @Param
            count - The number of transaction requests in the trb_list.

        @Param
            *ptrb_list - A pointer to an array of transaction requests (TRB).
                See I2C_TRANSACTION_REQUEST_BLOCK definition for details.
    
        @Param
            *pflag - A pointer to a completion flag.

        @Returns
            None

        @Example
            <code>  
                uint8_t EMULATED_EEPROM_Read(uint16_t slaveDeviceAddress,
                                             uint16_t dataAddress,
                                             uint8_t *pData,
                                             uint16_t nCount)
                {
                    I2C_MESSAGE_STATUS status;
                    I2C_TRANSACTION_REQUEST_BLOCK readTRB[2];
                    uint8_t     writeBuffer[3];
                    uint16_t    timeOut, slaveTimeOut;

                    // this initial value is important
                    status = I2C_MESSAGE_PENDING;

                    // build the write buffer first
                    // starting address of the EEPROM memory
                    writeBuffer[0] = (dataAddress >> 8);                        // high address
                    writeBuffer[1] = (uint8_t)(dataAddress);                    // low low address

                    // we need to create the TRBs for a random read sequence to the EEPROM
                    // Build TRB for sending address
                    I2C_MasterWriteTRBBuild(&readTRB[0],
                                            writeBuffer,
                                            2,
                                            slaveDeviceAddress);
                    // Build TRB for receiving data
                    I2C_MasterReadTRBBuild(&readTRB[1],
                                           pData,
                                           nCount,
                                           slaveDeviceAddress);

                    timeOut = 0;
                    slaveTimeOut = 0;

                    while(status != I2C_MESSAGE_FAIL) {
                        // now send the transactions
                        I2C_MasterTRBInsert(2, readTRB, &status);

                        // wait for the message to be sent or status has changed.
                        while(status == I2C_MESSAGE_PENDING) {
                            // add some delay here

                            // timeout checking
                            // check for max retry and skip this byte
                            if (slaveTimeOut == SLAVE_I2C_GENERIC_SLAVE_TIMEOUT)
                                return (0);
                            else
                                slaveTimeOut++;
                        }

                        if (status == I2C_MESSAGE_COMPLETE)
                            break;

                        // if status is  I2C_MESSAGE_ADDRESS_NO_ACK, or
                        //               I2C_DATA_NO_ACK,
                        // The device may be busy and needs more time for the last
                        // write so we can retry writing the data, this is why we
                        // use a while loop here

                        // check for max retry and skip this byte
                        if (timeOut == SLAVE_I2C_GENERIC_RETRY_MAX)
                            return (0);
                        else
                            timeOut++;
                    }
                    return (1);
                }   
            </code>
     */
    void I2C_MasterTRBInsert(
            uint8_t count,
            I2C_TRANSACTION_REQUEST_BLOCK *ptrb_list,
            I2C_MESSAGE_STATUS *pflag);

    /**
        @Summary
            This function populates a trb supplied by the calling function
            with the parameters supplied by the calling function.

        @Description
            All i2c requests are in the form of TRB's. This helper function
            takes standard parameters and correctly formats the TRB. The R/W
            bit is set to ensure that the resulting TRB describes an i2c read
            operation.

            This function does not send the transaction. To send the transaction,
            the TRB insert function (I2C_MasterTRBInsert()) must be called.

        @Preconditions
            None

        @Param
            *ptrb - A pointer to a caller supplied TRB.

        @Param
            *pdata - A pointer to the block of data to be received

        @Param
            length - The length of the data block to be received

        @Param
            address - The address of the i2c peripheral to be accessed

        @Returns
            None

        @Example
            <code>
                Refer to I2C_MasterTRBInsert() for an example	
            </code>
     */
    void I2C_MasterReadTRBBuild(
            I2C_TRANSACTION_REQUEST_BLOCK *ptrb,
            uint8_t *pdata,
            uint8_t length,
            uint16_t address);

    /**
        @Summary
            This function populates a trb supplied by the calling function
            with the parameters supplied by the calling function.

        @Description
            All i2c requests are in the form of TRB's.  This helper function
            takes standard parameters and correctly formats the TRB. The R/W
            bit is cleared to ensure that the resulting TRB describes an i2c
            write operation.

            This function does not send the transaction. To send the transaction,
            the TRB insert function (I2C_MasterTRBInsert()) must be called.

        @Preconditions
            None

        @Param
            *ptrb - A pointer to a caller supplied TRB.

        @Param
            *pdata - A pointer to the block of data to be sent

        @Param
            length - The length of the data block to be sent

        @Param
            address - The address of the i2c peripheral to be accessed

        @Returns
            None

        @Example
            <code>
                Refer to I2C_MasterTRBInsert() for an example	
            </code>

     */
    void I2C_MasterWriteTRBBuild(
            I2C_TRANSACTION_REQUEST_BLOCK *ptrb,
            uint8_t *pdata,
            uint8_t length,
            uint16_t address);

    /**
        @Summary
            This function returns the empty status of the Master
            queue.

        @Description
            This function returns the empty status of the Master
            queue. Use this function to check if the queue is empty.
            This can verify if the Master is currently idle.

        @Preconditions
            None

        @Param
            None

        @Returns
            True if the queue is empty and false if the queue is not empty.

        @Example
            <code>
                #define MCHP24AA512_ADDRESS    0x50 // slave device address

                // check until queue is empty
                while(I2C_MasterQueueIsEmpty() == false);
            
                // now send more data (assume readBuffer is initialized)
                I2C_MasterRead(readBuffer,
                               3,
                               MCHP24AA512_ADDRESS,
                               &status);   
            </code>
     */
    bool I2C_MasterQueueIsEmpty(void);

    /**
        @Summary
            This function returns the full status of the Master
            queue.

        @Description
            This function returns the full status of the Master
            queue. Use this function to check if the queue is full.
            This can verify if the Master will not be able to accept
            addition transactions.

        @Preconditions
            None

        @Param
            None

        @Returns
            True if the queue is full and false if the queue is not full.

        @Example
            <code>
                #define MCHP24AA512_ADDRESS    0x50 // slave device address
 
                // check until queue has space
                while(I2C_MasterQueueIsFull() == true);
            
                // now send more data (assume readBuffer is initialized)
                I2C_MasterRead(readBuffer,
                               3,
                               MCHP24AA512_ADDRESS,
                               &status); 
            </code>
     */
    bool I2C_MasterQueueIsFull(void);
    
    /**
        @Summary
            This function writes and control data transfer from master(PSLab) to
            a slave device.

        @Description
            Using primitive I2C functions, this method initiates a multiple write 
            transaction and retry and wait until it is either failed or completed. 
            The return value will show if it's failed or not.

        @Preconditions
            I2C interface needs to be initiated

        @Param
            *pdata - A pointer to the block of data to be received

        @Param
            length - The length of the data block to be transmitted

        @Param
            address - The address of the i2c peripheral to be accessed
     */
    response_t I2C_BulkWrite(uint8_t *pdata, uint8_t length, uint16_t address);

    /**
        @Summary
            This function reads consecutive registers from a slave device

        @Description
            Using primitive I2C functions, this method initiates a multiple read 
            transaction and retry and wait until it is either failed or completed.
            The return value will show if it's failed or not.

        @Preconditions
            I2C interface needs to be initiated

        @Param
            start - Start address of the register block

        @Param
            address - The address of the i2c peripheral to be accessed

        @Param
            *pdata - A pointer to the block of data to be received

        @Param
            length - The length of the data block to be received
     */
    response_t I2C_BulkRead(uint8_t *start, uint16_t address, uint8_t *pdata, uint8_t length);

    /***************************************************************************
     * Commands for state machine
     **************************************************************************/
    /**
        @Summary
            This function starts the I2C bus with interrupts disabled

        @Description
            This function uses pre-stored baudrate for I2C bus to initiate the
            I2C module and disables interrupts to use raw I2C commands

        @Preconditions
            None

        @Return
            If no write collisions or ack from slave, SUCCESS. Otherwise FAILED
     */
    response_t I2C_CommandStart(void);

    /**
        @Summary
            This function stops the I2C bus

        @Description
            This function uses raw I2C commands to stop the I2C bus transactions

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled

        @Return
            SUCCESS
     */
    response_t I2C_CommandStop(void);

    /**
        @Summary
            This function wait for a transaction to complete

        @Description
            This function uses raw I2C commands to hold other operations until
            the current transaction is complete before a timer elapsed

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled

        @Return
            SUCCESS
     */
    response_t I2C_CommandWait(void);

    /**
        @Summary
            This function sends out a byte over I2C bus

        @Description
            This function takes one argument over serial to transmit the data 
            over the I2C bus using raw I2C commands.
            1. (uint8) data
               Data byte the needs to be transferred to slave device

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled

        @Return
            If no write collisions or ack from slave, SUCCESS. Otherwise FAILED
     */
    response_t I2C_CommandSend(void);

    /**
        @Summary
            This function sends out a byte over I2C bus without waiting for ACK

        @Description
            This function takes one argument over serial to transmit the data 
            over the I2C bus using raw I2C commands without waiting for an 
            acknowledgement from the slave device.
            1. (uint8) data
               Data byte the needs to be transferred to slave device

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled

        @Return
            SUCCESS
     */
    response_t I2C_CommandSendBurst(void);

    /**
        @Summary
            This function sends out a restart signal

        @Description
            This function takes one argument over serial to initiate a restart
            on the I2C bus using raw I2C commands.
            1. (uint8) address
               Slave device address

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled

        @Return
            If no write collisions or ack from slave, SUCCESS. Otherwise FAILED
     */
    response_t I2C_CommandRestart(void);

    /**
        @Summary
            This function reads a byte in continous mode

        @Description
            This function does not take any argument over serial. It will read
            from I2C slave device and sends out an ACK to signal the slave that
            the transaction is not finished yet.
            This method should be followed by I2C_CommandReadEnd() to signal the
            end of transaction.

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled

        @Return
            SUCCESS
     */
    response_t I2C_CommandReadMore(void);

    /**
        @Summary
            This function reads the final byte from slave device

        @Description
            This function does not take any argument over serial. It will read
            the last byte in a transaction from a slave device and sends out an
            NACK signal to the slave indicating that the current transaction is
            over.

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled

        @Return
            SUCCESS
     */
    response_t I2C_CommandReadEnd(void);

    /**
        @Summary
            This function will re-initiate the I2C bus with a new baud rate

        @Description
            This function takes one argument over serial to re-initialize the
            I2C bus with a new baud rate value.
            1. (uint16) baud_rate
               Baud rate value to be set

        @Preconditions
            None

        @Return
            SUCCESS
     */
    response_t I2C_CommandConfig(void);

    /**
        @Summary
            This function reads the STATUS register

        @Description
            This function reads and transmit over serial the Status register bits
            on I2C module

        @Preconditions
            None

        @Return
            SUCCESS
     */
    response_t I2C_CommandStatus(void);

    /**
        @Summary
            This function reads a continous byte stream from slave device

        @Description
            This function takes three argument over serial to read the data 
            over the I2C bus using raw I2C commands and the data is transmitted
            back to host over serial byte by byte.
            1. (uint8) device
               Slave device address without tamperting the R/W bit (LSB)
            2. (uint8) address
               Slave device register address
            3. (uint8) count
               Number of bytes to read

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled

        @Return
            SUCCESS
     */
    response_t I2C_CommandReadBulk(void);

    /**
        @Summary
            This function writes a continous byte stream to slave device

        @Description
            This function initially takes two argument over serial to write the 
            data over the I2C bus using raw I2C commands.
            1. (uint8) device
               Slave device address without tamperting the R/W bit (LSB)
            2. (uint8) count
               Number of bytes to write
            Then it will keep on transmitting the receiving bytes from serial for
            count number of times.

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled

        @Return
            SUCCESS
     */
    response_t I2C_CommandWriteBulk(void);

    /**
        @Summary
            This function enables I2C SMBus

        @Description
            This function takes no argument over serial. It will enable the 
            system management bus subprotocol

        @Preconditions
            None

        @Return
            SUCCESS
     */
    response_t I2C_CommandEnableSMBus(void);

    /**
        @Summary
            This function disables I2C SMBus

        @Description
            This function takes no argument over serial. It will disable the 
            system management bus subprotocol

        @Preconditions
            None

        @Return
            SUCCESS
     */
    response_t I2C_CommandDisableSMBus(void);

    /**
        @Summary
            This function initialize the I2C bus

        @Description
            This function takes no argument over serial. It will initialize the
            I2C bus.

        @Preconditions
            None

        @Return
            SUCCESS
     */
    response_t I2C_CommandInit(void);

    /**
        @Summary
            This function will convert the clock line to an input

        @Description
            This function takes one argument over serial. It will set the SCL
            pin to an input after setting it low for a period of time defined by
            user.
            1. (uint16) delay
               Time to hold the SCL line low

        @Preconditions
            None

        @Return
            SUCCESS
     */
    response_t I2C_CommandPullDown(void);

    /***************************************************************************
     * Support functions for state machine commands
     **************************************************************************/
    /**
        @Summary
            This function starts the I2C bus

        @Description
            This function uses timers to enable and wait until the I2C bus is
            ready for transmission

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled
     */
    void I2C_StartSignal(void);

    /**
        @Summary
            This function stops the I2C bus

        @Description
            This function uses timers to disable and wait until the I2C bus is
            stopped after a transaction

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled
     */
    void I2C_StopSignal(void);

    /**
        @Summary
            This function sends out a repeated start signal

        @Description
            This function uses timers to send out a repeated start signal needed
            when reading from I2C slaves

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled
     */
    void I2C_RestartSignal(void);

    /**
        @Summary
            This function sends out an acknowldegement signal

        @Description
            This function uses timers to transmit an acknowledgement signal to 
            slave device

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled
     */
    void I2C_AcknowledgeSignal(void);

    /**
        @Summary
            This function sends out a negative acknowldegement signal

        @Description
            This function uses timers to transmit a negative acknowledgement 
            signal to slave devices indicating it's a transaction complete from
            master device

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled
     */
    void I2C_NAcknowledgeSignal(void);

    /**
        @Summary
            This function wait until transmission is in progress

        @Description
            This function uses timers to stay idle until the transmit buffer is
            empty

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled
     */
    void I2C_WaitSignal(void);

    /**
        @Summary
            This function sends out a data byte over I2C bus

        @Description
            This function takes a byte as input argument and pass it onto I2C bus
            and wait until it is out
        
        @Params
            data: uint8_t data byte

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled
     */
    void I2C_Transmit(uint8_t data);

    /**
        @Summary
            This function reads from the receive buffer

        @Description
            This function reads from I2C RX buffer and return the data captured.
            It will take an I2C_RESPONSE as argument to determine if it's the
            last byte to read or continous read

        @Params
            I2C_RESPONSE: Response type from master (ACK or NACK)

        @Preconditions
            I2C interface needs to be initiated with interrupts disabled
     */
    uint8_t I2C_Receive(I2C_RESPONSE);

    /***************************************************************************
     * Interrupt flags and settings
     **************************************************************************/
    inline static void I2C_InterruptDisable(void) {
        IEC3bits.MI2C2IE = 0;
    }
    
    inline static void I2C_InterruptEnable(void) {
        IEC3bits.MI2C2IE = 1;
    }
    
    inline static void I2C_InterruptFlagClear(void) {
        IFS3bits.MI2C2IF = 0;
    }
    
    inline static void I2C_InterruptFlagSet(void) {
        IFS3bits.MI2C2IF = 1;
    }
    
    // Getter and setter for variables
    void I2C_SetBaudRate(I2C_BAUD_RATES V);
    I2C_BAUD_RATES I2C_GetBaudRate(void);

#ifdef __cplusplus
}
#endif

#endif //_I2C_H
