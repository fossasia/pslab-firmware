#include "i2c.h"
#include "../../helpers/delay.h"
#include "../../registers/system/pin_manager.h"
#include "../../transport/packet_handler.h"
#include "../../transport/host.h"

/**
  I2C Driver Queue Status Type

  @Summary
    Defines the type used for the transaction queue status.

  @Description
    This defines type used to keep track of the queue status.
 */
typedef union {
    struct {
        uint8_t full : 1;
        uint8_t empty : 1;
    } s;
} I2C_TR_QUEUE_STATUS;

/**
  I2C Driver Queue Entry Type

  @Summary
    Defines the object used for an entry in the i2c queue items.

  @Description
    This defines the object in the i2c queue. Each entry is a composed
    of a list of TRBs, the number of the TRBs and the status of the
    currently processed TRB.
 */
typedef struct {
    uint8_t count; // a count of trb's in the trb list
    I2C_TRANSACTION_REQUEST_BLOCK *ptrb_list; // pointer to the trb list
    I2C_MESSAGE_STATUS *pTrFlag; // set with the error of the last trb sent.
} I2C_TR_QUEUE_ENTRY;

/**
  I2C Master Driver Object Type

  @Summary
    Defines the object that manages the i2c master.

  @Description
    This defines the object that manages the sending and receiving of
    i2c master transactions.
 */
typedef struct {
    /* Read/Write Queue */
    I2C_TR_QUEUE_ENTRY *pTrTail; // tail of the queue
    I2C_TR_QUEUE_ENTRY *pTrHead; // head of the queue
    I2C_TR_QUEUE_STATUS trStatus; // status of the last transaction
    uint8_t i2cErrors; // keeps track of errors
} I2C_OBJECT;

/**
  I2C Master Driver State Enumeration

  @Summary
    Defines the different states of the i2c master.

  @Description
    This defines the different states that the i2c master
    used to process transactions on the i2c bus.
 */
typedef enum {
    S_MASTER_IDLE,
    S_MASTER_RESTART,
    S_MASTER_SEND_ADDR,
    S_MASTER_SEND_DATA,
    S_MASTER_SEND_STOP,
    S_MASTER_ACK_ADDR,
    S_MASTER_RCV_DATA,
    S_MASTER_RCV_STOP,
    S_MASTER_ACK_RCV_DATA,
    S_MASTER_NOACK_STOP,
    S_MASTER_SEND_ADDR_10BIT_LSB,
    S_MASTER_10BIT_RESTART,
} I2C_MASTER_STATES;

/**
 Section: Macro Definitions
 */

/* defined for I2C */

#ifndef I2C_CONFIG_TR_QUEUE_LENGTH
#define I2C_CONFIG_TR_QUEUE_LENGTH 1
#endif

// Writing the I2C2TRN register starts a master transmission
#define I2C_TRANSMIT_REG                       I2C2TRN              // Defines the transmit register used to send data.
#define I2C_RECEIVE_REG                        I2C2RCV              // Defines the receive register used to receive data.

// The following control bits are used in the I2C state machine to manage
// the I2C module and determine next states.
#define I2C_WRITE_COLLISION_STATUS_BIT         I2C2STATbits.IWCOL   // Defines the write collision status bit.
#define I2C_ACKNOWLEDGE_STATUS_BIT             I2C2STATbits.ACKSTAT // I2C ACK status bit.
#define I2C_MASTER_TRANSMISSION_STATUS_BIT     I2C2STATbits.TRSTAT  // I2C master transmission in progress.

#define I2C_START_CONDITION_ENABLE_BIT         I2C2CONbits.SEN      // I2C START control bit.
#define I2C_REPEAT_START_CONDITION_ENABLE_BIT  I2C2CONbits.RSEN     // I2C Repeated START control bit.
#define I2C_RECEIVE_ENABLE_BIT                 I2C2CONbits.RCEN     // I2C Receive enable control bit.
#define I2C_STOP_CONDITION_ENABLE_BIT          I2C2CONbits.PEN      // I2C STOP control bit.
#define I2C_ACKNOWLEDGE_ENABLE_BIT             I2C2CONbits.ACKEN    // I2C ACK start control bit.
#define I2C_ACKNOWLEDGE_DATA_BIT               I2C2CONbits.ACKDT    // I2C ACK data control bit.

#define I2C_TRANSMIT_BUFFER_STATUS             I2C2STATbits.TBF     // Flag for Tx buffer status
#define I2C_RECEIVE_BUFFER_STATUS              I2C2STATbits.RBF     // Flag for Rx buffer status

/**
 Section: Local Functions
 */
static void I2C_FunctionComplete(void);
static void I2C_Stop(I2C_MESSAGE_STATUS completion_code);

/**
 Section: Local Variables
 */
static uint16_t I2C_BRG = I2C_BAUD_RATE_100KHZ;
void I2C_SetBaudRate(uint16_t V) { I2C_BRG = V; }
uint16_t I2C_GetBaudRate(void) { return I2C_BRG; }

static I2C_TR_QUEUE_ENTRY i2c_tr_queue[I2C_CONFIG_TR_QUEUE_LENGTH];
static I2C_OBJECT i2c_object;
static I2C_MASTER_STATES i2c_state = S_MASTER_IDLE;
static uint8_t i2c_trb_count;

static I2C_TRANSACTION_REQUEST_BLOCK *p_i2c_trb_current;
static I2C_TR_QUEUE_ENTRY *p_i2c_current = NULL;

static uint16_t counter;

/**
  Section: Driver Interface
 */

void I2C_Initialize(void) {

    i2c_object.pTrHead = i2c_tr_queue;
    i2c_object.pTrTail = i2c_tr_queue;
    i2c_object.trStatus.s.empty = true;
    i2c_object.trStatus.s.full = false;

    i2c_object.i2cErrors = 0;

    I2C_SCL_SetDigitalInput();
    I2C_SCL_SetAsOpenDrain();
    I2C_SCL_PullUp();
    I2C_SDA_SetDigitalInput();
    I2C_SDA_SetAsOpenDrain();
    I2C_SDA_PullUp();

    I2C2BRG = I2C_BRG;
    I2C_InitializeCON();
    I2C_InitializeSTAT();

    I2C_InterruptFlagClear();
    I2C_InterruptEnable();
}

void I2C_InitializeCON(void) {
    // Enables I2C2 module and configure SDA2 and SCL2 as serial port pins
    I2C2CONbits.I2CEN = 1;
    // Module will continue to run in idle mode
    I2C2CONbits.I2CSIDL = 0;
    // Only the software can release clock
    I2C2CONbits.SCLREL = 0;
    // Intelligent Peripheral Management Interface is disabled
    I2C2CONbits.IPMIEN = 0;
    // Slave address is 7-bits
    I2C2CONbits.A10M = 0;
    // Slew rate control is enabled
    I2C2CONbits.DISSLW = 0;
    // Disable SMBus input thresholds
    I2C2CONbits.SMEN = 0;
    // General call address disabled
    I2C2CONbits.GCEN = 0;
    // Disable clock stretching
    I2C2CONbits.STREN = 0;
    // Sends ACK for acknowledgment
    I2C2CONbits.ACKDT = 0;

    // Acknowledgment sequence is not in progress
    I2C2CONbits.ACKEN = 0;
    // Receive sequence is not in progress
    I2C2CONbits.RCEN = 0;
    // Stop condition is not in progress
    I2C2CONbits.PEN = 0;
    // Repeated start condition is not in progress
    I2C2CONbits.RSEN = 0;
    // Start condition is not in progress
    I2C2CONbits.SEN = 0;
}

void I2C_InitializeSTAT(void) {
    // ACK received from slave
    I2C2STATbits.ACKSTAT = 0;
    // Master transmit is not in progress
    I2C2STATbits.TRSTAT = 0;
    // No bus collisions detected
    I2C2STATbits.BCL = 0;
    // General call address was not received
    I2C2STATbits.GCSTAT = 0;
    // 10-bit address was not matched
    I2C2STATbits.ADD10 = 0;
    // No collisions
    I2C2STATbits.IWCOL = 0;
    // No overflow
    I2C2STATbits.I2COV = 0;
    // Last byte received was an address
    I2C2STATbits.D_A = 0;
    // Stop bit was not detected last
    I2C2STATbits.P = 0;
    // Start bit was not detected last
    I2C2STATbits.S = 0;
    // Write
    I2C2STATbits.R_W = 0;
    // Receive is not complete
    I2C2STATbits.RBF = 0;
    // Transmit is complete
    I2C2STATbits.TBF = 0;
}

void I2C_InitializeIfNot(I2C_BAUD_RATES baud_rate, bool interrupts) {
    if (I2C_GetBaudRate() != baud_rate) {
        I2C_SetBaudRate(baud_rate);
        I2C_Initialize();
    }
    interrupts ? I2C_InterruptEnable() : I2C_InterruptDisable();
}

uint8_t I2C_ErrorCountGet(void) {
    uint8_t ret;
    ret = i2c_object.i2cErrors;
    return ret;
}

/**
 * The MI2CxIF interrupt is generated on completion of following events:
   - Start condition
   - Stop condition
   - Data transfer byte transmitted or received
   - Acknowledge transmit
   - Repeated Start
   - Detection of a bus collision event
 */
void __attribute__((interrupt, no_auto_psv)) _MI2C2Interrupt(void) {

    static uint8_t *pi2c_buf_ptr;
    static uint16_t i2c_address;
    static uint8_t i2c_bytes_left;
    static uint8_t i2c_10bit_address_restart = 0;

    I2C_InterruptFlagClear();

    // Check first if there was a collision.
    // If we have a Write Collision, reset and go to idle state */
    if (I2C_WRITE_COLLISION_STATUS_BIT) {
        // clear the Write collision
        I2C_WRITE_COLLISION_STATUS_BIT = 0;
        i2c_state = S_MASTER_IDLE;
        *(p_i2c_current->pTrFlag) = I2C_MESSAGE_FAIL;
        // reset the buffer pointer
        p_i2c_current = NULL;
        return;
    }

    /* Handle the correct i2c state */
    switch (i2c_state) {
        case S_MASTER_IDLE: /* In reset state, waiting for data to send */
            if (i2c_object.trStatus.s.empty != true) {
                // grab the item pointed by the head
                p_i2c_current = i2c_object.pTrHead;
                i2c_trb_count = i2c_object.pTrHead->count;
                p_i2c_trb_current = i2c_object.pTrHead->ptrb_list;
                i2c_object.pTrHead++;
                // check if the end of the array is reached
                if (i2c_object.pTrHead == (i2c_tr_queue + I2C_CONFIG_TR_QUEUE_LENGTH)) {
                    // adjust to restart at the beginning of the array
                    i2c_object.pTrHead = i2c_tr_queue;
                }
                // since we moved one item to be processed, we know
                // it is not full, so set the full status to false
                i2c_object.trStatus.s.full = false;
                // check if the queue is empty
                if (i2c_object.pTrHead == i2c_object.pTrTail) {
                    // it is empty so set the empty status to true
                    i2c_object.trStatus.s.empty = true;
                }
                // send the start condition
                I2C_START_CONDITION_ENABLE_BIT = 1;
                // start the i2c request
                i2c_state = S_MASTER_SEND_ADDR;
            }
            break;
        case S_MASTER_RESTART:
            /* check for pending i2c Request */
            // ... trigger a REPEATED START
            I2C_REPEAT_START_CONDITION_ENABLE_BIT = 1;
            // start the i2c request
            i2c_state = S_MASTER_SEND_ADDR;
            break;
        case S_MASTER_SEND_ADDR_10BIT_LSB:
            if (I2C_ACKNOWLEDGE_STATUS_BIT) {
                i2c_object.i2cErrors++;
                I2C_Stop(I2C_MESSAGE_ADDRESS_NO_ACK);
            } else {
                // Remove bit 0 as R/W is never sent here
                I2C_TRANSMIT_REG = (i2c_address >> 1) & 0x00FF;
                // determine the next state, check R/W
                if (i2c_address & 0x01) {
                    // if this is a read we must repeat start
                    // the bus to perform a read
                    i2c_state = S_MASTER_10BIT_RESTART;
                } else {
                    // this is a write continue writing data
                    i2c_state = S_MASTER_SEND_DATA;
                }
            }
            break;
        case S_MASTER_10BIT_RESTART:
            if (I2C_ACKNOWLEDGE_STATUS_BIT) {
                i2c_object.i2cErrors++;
                I2C_Stop(I2C_MESSAGE_ADDRESS_NO_ACK);
            } else {
                // ACK Status is good
                // restart the bus
                I2C_REPEAT_START_CONDITION_ENABLE_BIT = 1;
                // fudge the address so S_MASTER_SEND_ADDR works correctly
                // we only do this on a 10-bit address resend
                i2c_address = 0x00F0 | ((i2c_address >> 8) & 0x0006);
                // set the R/W flag
                i2c_address |= 0x0001;
                // set the address restart flag so we do not change the address
                i2c_10bit_address_restart = 1;
                // Resend the address as a read
                i2c_state = S_MASTER_SEND_ADDR;
            }
            break;
        case S_MASTER_SEND_ADDR:
            /* Start has been sent, send the address byte */
            /* Note:
                On a 10-bit address resend (done only during a 10-bit
                device read), the original i2c_address was modified in
                S_MASTER_10BIT_RESTART state. So the check if this is
                a 10-bit address will fail and a normal 7-bit address
                is sent with the R/W bit set to read. The flag
                i2c_10bit_address_restart prevents the  address to
                be re-written.
             */
            if (i2c_10bit_address_restart != 1) {
                // extract the information for this message
                i2c_address = p_i2c_trb_current->address;
                pi2c_buf_ptr = p_i2c_trb_current->pbuffer;
                i2c_bytes_left = p_i2c_trb_current->length;
            } else {
                // reset the flag so the next access is OK
                i2c_10bit_address_restart = 0;
            }
            // check for 10-bit address
            if (i2c_address > 0x00FF) {
                // we have a 10 bit address
                // send bits<9:8>
                // mask bit 0 as this is always a write
                I2C_TRANSMIT_REG = 0xF0 | ((i2c_address >> 8) & 0x0006);
                i2c_state = S_MASTER_SEND_ADDR_10BIT_LSB;
            } else {
                // Transmit the address
                I2C_TRANSMIT_REG = i2c_address;
                if (i2c_address & 0x01) {
                    // Next state is to wait for address to be ACK'ed
                    i2c_state = S_MASTER_ACK_ADDR;
                } else {
                    // Next state is transmit
                    i2c_state = S_MASTER_SEND_DATA;
                }
            }
            break;
        case S_MASTER_SEND_DATA:
            // Make sure the previous byte was acknowledged
            if (I2C_ACKNOWLEDGE_STATUS_BIT) {
                // Transmission was not acknowledged
                i2c_object.i2cErrors++;
                // Reset the ACK flag
                I2C_ACKNOWLEDGE_STATUS_BIT = 0;
                // Send a stop flag and go back to idle
                I2C_Stop(I2C_DATA_NO_ACK);
            } else {
                // Did we send them all ?
                if (i2c_bytes_left-- == 0U) {
                    // yup sent them all!
                    // update the trb pointer
                    p_i2c_trb_current++;
                    // are we done with this string of requests?
                    if (--i2c_trb_count == 0) {
                        I2C_Stop(I2C_MESSAGE_COMPLETE);
                    } else {
                        // no!, there are more TRB to be sent.
                        // I2C_START_CONDITION_ENABLE_BIT = 1;
                        // In some cases, the slave may require
                        // a restart instead of a start. So use this one
                        // instead.
                        I2C_REPEAT_START_CONDITION_ENABLE_BIT = 1;
                        // start the i2c request
                        i2c_state = S_MASTER_SEND_ADDR;
                    }
                } else {
                    // Grab the next data to transmit
                    I2C_TRANSMIT_REG = *pi2c_buf_ptr++;
                }
            }
            break;
        case S_MASTER_ACK_ADDR:
            /* Make sure the previous byte was acknowledged */
            if (I2C_ACKNOWLEDGE_STATUS_BIT) {
                // Transmission was not acknowledged
                i2c_object.i2cErrors++;
                // Send a stop flag and go back to idle
                I2C_Stop(I2C_MESSAGE_ADDRESS_NO_ACK);
                // Reset the ACK flag
                I2C_ACKNOWLEDGE_STATUS_BIT = 0;
            } else {
                I2C_RECEIVE_ENABLE_BIT = 1;
                i2c_state = S_MASTER_ACK_RCV_DATA;
            }
            break;
        case S_MASTER_RCV_DATA:
            /* Acknowledge is completed.  Time for more data */
            // Next thing is to ack the data
            i2c_state = S_MASTER_ACK_RCV_DATA;
            // Set up to receive a byte of data
            I2C_RECEIVE_ENABLE_BIT = 1;
            break;
        case S_MASTER_ACK_RCV_DATA:
            // Grab the byte of data received and acknowledge it
            *pi2c_buf_ptr++ = I2C_RECEIVE_REG;
            // Check if we received them all?
            if (--i2c_bytes_left) {
                /* No, there's more to receive */
                // No, bit 7 is clear.  Data is OK
                // Set the flag to acknowledge the data
                I2C_ACKNOWLEDGE_DATA_BIT = 0;
                // Wait for the acknowledge to complete, then get more
                i2c_state = S_MASTER_RCV_DATA;
            } else {
                // Yes, it's the last byte.  Don't ACK it
                // Flag that we will NACK the data
                I2C_ACKNOWLEDGE_DATA_BIT = 1;
                I2C_FunctionComplete();
            }
            // Initiate the acknowledge
            I2C_ACKNOWLEDGE_ENABLE_BIT = 1;
            break;
        case S_MASTER_RCV_STOP:
        case S_MASTER_SEND_STOP:
            // Send the stop flag
            I2C_Stop(I2C_MESSAGE_COMPLETE);
            break;
        default:
            // This case should not happen, if it does then
            // terminate the transfer
            i2c_object.i2cErrors++;
            I2C_Stop(I2C_LOST_STATE);
            break;

    }
}

static void I2C_FunctionComplete(void) {

    // update the trb pointer
    p_i2c_trb_current++;

    // are we done with this string of requests?
    if (--i2c_trb_count == 0) {
        i2c_state = S_MASTER_SEND_STOP;
    } else {
        i2c_state = S_MASTER_RESTART;
    }
}

static void I2C_Stop(I2C_MESSAGE_STATUS completion_code) {
    // then send a stop
    I2C_STOP_CONDITION_ENABLE_BIT = 1;

    // make sure the flag pointer is not NULL
    if (p_i2c_current->pTrFlag != NULL) {
        // update the flag with the completion code
        *(p_i2c_current->pTrFlag) = completion_code;
    }

    // Done, back to idle
    i2c_state = S_MASTER_IDLE;
}

void I2C_MasterWrite(
        uint8_t *pdata,
        uint8_t length,
        uint16_t address,
        I2C_MESSAGE_STATUS *pstatus) {
    static I2C_TRANSACTION_REQUEST_BLOCK trBlock;

    // check if there is space in the queue
    if (i2c_object.trStatus.s.full != true) {
        I2C_MasterWriteTRBBuild(&trBlock, pdata, length, address);
        I2C_MasterTRBInsert(1, &trBlock, pstatus);
    } else {
        *pstatus = I2C_MESSAGE_FAIL;
    }
}

void I2C_MasterRead(
        uint8_t *pdata,
        uint8_t length,
        uint16_t address,
        I2C_MESSAGE_STATUS *pstatus) {
    static I2C_TRANSACTION_REQUEST_BLOCK trBlock;


    // check if there is space in the queue
    if (i2c_object.trStatus.s.full != true) {
        I2C_MasterReadTRBBuild(&trBlock, pdata, length, address);
        I2C_MasterTRBInsert(1, &trBlock, pstatus);
    } else {
        *pstatus = I2C_MESSAGE_FAIL;
    }
}

void I2C_MasterTRBInsert(
        uint8_t count,
        I2C_TRANSACTION_REQUEST_BLOCK *ptrb_list,
        I2C_MESSAGE_STATUS *pflag) {

    // check if there is space in the queue
    if (i2c_object.trStatus.s.full != true) {
        *pflag = I2C_MESSAGE_PENDING;

        i2c_object.pTrTail->ptrb_list = ptrb_list;
        i2c_object.pTrTail->count = count;
        i2c_object.pTrTail->pTrFlag = pflag;
        i2c_object.pTrTail++;

        // check if the end of the array is reached
        if (i2c_object.pTrTail == (i2c_tr_queue + I2C_CONFIG_TR_QUEUE_LENGTH)) {
            // adjust to restart at the beginning of the array
            i2c_object.pTrTail = i2c_tr_queue;
        }

        // since we added one item to be processed, we know
        // it is not empty, so set the empty status to false
        i2c_object.trStatus.s.empty = false;

        // check if full
        if (i2c_object.pTrHead == i2c_object.pTrTail) {
            // it is full, set the full status to true
            i2c_object.trStatus.s.full = true;
        }

        // for interrupt based
        if (i2c_state == S_MASTER_IDLE) {
            // force the task to run since we know that the queue has
            // something that needs to be sent. This will trigger I2C2 ISR
            I2C_InterruptFlagSet();
        }
    } else {
        *pflag = I2C_MESSAGE_FAIL;
    }
}

void I2C_MasterReadTRBBuild(
        I2C_TRANSACTION_REQUEST_BLOCK *ptrb,
        uint8_t *pdata,
        uint8_t length,
        uint16_t address) {
    ptrb->address = address << 1;
    // make this a read
    ptrb->address |= 0x01;
    ptrb->length = length;
    ptrb->pbuffer = pdata;
}

void I2C_MasterWriteTRBBuild(
        I2C_TRANSACTION_REQUEST_BLOCK *ptrb,
        uint8_t *pdata,
        uint8_t length,
        uint16_t address) {
    ptrb->address = address << 1;
    ptrb->length = length;
    ptrb->pbuffer = pdata;
}

bool I2C_MasterQueueIsEmpty(void) {
    return ((bool) i2c_object.trStatus.s.empty);
}

bool I2C_MasterQueueIsFull(void) {
    return ((bool) i2c_object.trStatus.s.full);
}

enum Status I2C_BulkWrite(uint8_t *pdata, uint8_t length, uint16_t address) {

    I2C_MESSAGE_STATUS status = I2C_MESSAGE_PENDING;

    LED_SetLow();
    // Retry transmission
    uint16_t timeOut = 0;
    uint16_t slaveTimeOut = 0;

    while (status != I2C_MESSAGE_FAIL) {
        I2C_MasterWrite(pdata, length, address, &status);
        while (status == I2C_MESSAGE_PENDING) {
            DELAY_us(100);
            if (slaveTimeOut == SLAVE_I2C_GENERIC_DEVICE_TIMEOUT) break;
            else slaveTimeOut++;
        }
        if ((slaveTimeOut == SLAVE_I2C_GENERIC_DEVICE_TIMEOUT) ||
                (status == I2C_MESSAGE_COMPLETE)) break;
        if (timeOut == SLAVE_I2C_GENERIC_RETRY_MAX) break;
        else timeOut++;
    }
    LED_SetHigh();

    if (status == I2C_MESSAGE_FAIL ||
            status == I2C_STUCK_START ||
            status == I2C_MESSAGE_ADDRESS_NO_ACK ||
            status == I2C_DATA_NO_ACK ||
            status == I2C_LOST_STATE) {
        return E_FAILED;
    }
    return E_OK;
}

enum Status I2C_BulkRead(uint8_t *start, uint16_t address, uint8_t *pdata, uint8_t length) {

    I2C_TRANSACTION_REQUEST_BLOCK readTRB[2];
    I2C_MESSAGE_STATUS status = I2C_MESSAGE_PENDING;

    uint16_t timeOut = 0;
    uint16_t slaveTimeOut = 0;

    I2C_MasterWriteTRBBuild(&readTRB[0], start, 1 , address);
    I2C_MasterReadTRBBuild(&readTRB[1], pdata, length, address);

    LED_SetLow();
    while (status != I2C_MESSAGE_FAIL) {
        I2C_MasterTRBInsert(2, readTRB, &status);
        while (status == I2C_MESSAGE_PENDING) {
            DELAY_us(100);
            if (slaveTimeOut == SLAVE_I2C_GENERIC_DEVICE_TIMEOUT) break;
            else slaveTimeOut++;
        }

        if (status == I2C_MESSAGE_COMPLETE) break;

        if (timeOut == SLAVE_I2C_GENERIC_RETRY_MAX) break;
        else timeOut++;
    }
    LED_SetHigh();

    if (status == I2C_MESSAGE_FAIL ||
            status == I2C_STUCK_START ||
            status == I2C_MESSAGE_ADDRESS_NO_ACK ||
            status == I2C_DATA_NO_ACK ||
            status == I2C_LOST_STATE) {
        return E_FAILED;
    }
    return E_OK;
}

void I2C_StartSignal(void) {

    I2C_START_CONDITION_ENABLE_BIT = 1;

    counter = 1000;
    while (I2C_START_CONDITION_ENABLE_BIT && counter--) DELAY_us(1);
}

void I2C_StopSignal(void) {

    I2C_STOP_CONDITION_ENABLE_BIT = 1;

    counter = 1000;
    while (I2C_STOP_CONDITION_ENABLE_BIT && counter--) DELAY_us(1);
}

void I2C_RestartSignal(void) {

    I2C_REPEAT_START_CONDITION_ENABLE_BIT = 1;

    counter = 1000;
    while (I2C_REPEAT_START_CONDITION_ENABLE_BIT && counter--) DELAY_us(1);
}

void I2C_AcknowledgeSignal(void) {

    I2C_ACKNOWLEDGE_DATA_BIT = 0;
    I2C_ACKNOWLEDGE_ENABLE_BIT = 1;

    counter = 1000;
    while (I2C_ACKNOWLEDGE_ENABLE_BIT && counter--) DELAY_us(1);
}

void I2C_NAcknowledgeSignal(void) {

    I2C_ACKNOWLEDGE_DATA_BIT = 1;
    I2C_ACKNOWLEDGE_ENABLE_BIT = 1;

    counter = 1000;
    while (I2C_ACKNOWLEDGE_ENABLE_BIT && counter--) DELAY_us(1);
}

void I2C_WaitSignal() {

    counter = 1000;
    while (I2C_TRANSMIT_BUFFER_STATUS && counter--) DELAY_us(1);
}

void I2C_Transmit(uint8_t data) {

    I2C_TRANSMIT_REG = data;
    counter = 1000;
    while (I2C_MASTER_TRANSMISSION_STATUS_BIT && counter--) DELAY_us(1);
}

uint8_t I2C_Receive(I2C_RESPONSE r) {

    I2C_WaitSignal();
    I2C_RECEIVE_ENABLE_BIT = 1;

    counter = 1000;
    while (I2C_RECEIVE_ENABLE_BIT && counter--) DELAY_us(1);
    while (!I2C_RECEIVE_BUFFER_STATUS && counter--) DELAY_us(1);

    uint8_t data = I2C_RECEIVE_REG;

    r == I2C_RESPONSE_ACKNOWLEDGE
        ? I2C_AcknowledgeSignal()
        : I2C_NAcknowledgeSignal();

    return data;
}


enum Status I2C_command_start(
    uint8_t *const args,
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {
    uint8_t address = 0;

    if (args_size != sizeof(address)) {
        return E_BAD_ARGSIZE;
    }

    address = *args;

    I2C_InitializeIfNot(I2C_GetBaudRate(), I2C_DISABLE_INTERRUPTS);
    I2C_StartSignal();
    I2C_Transmit(address);

    *rets = args;
    (*rets)[0] = (uint8_t)I2C2STATbits.ACKSTAT;
    *rets_size = 1;

    return I2C_ACKNOWLEDGE_STATUS_BIT && I2C2STATbits.BCL ? E_FAILED : E_OK;
}

enum Status I2C_command_stop(
    __attribute__ ((unused)) uint8_t args[],
    __attribute__ ((unused)) uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {
    I2C_StopSignal();
    return E_OK;
}

enum Status I2C_command_wait(
    __attribute__ ((unused)) uint8_t args[],
    __attribute__ ((unused)) uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {
    I2C_WaitSignal();
    return E_OK;
}

enum Status I2C_command_send(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *rets_size
) {
    if (args_size != 1) {
        return E_BAD_ARGSIZE;
    }

    uint8_t data = *args;
    I2C_Transmit(data);
    **rets = (uint8_t)I2C2STATbits.ACKSTAT;
    *rets_size = 1;

    if (I2C_ACKNOWLEDGE_STATUS_BIT && I2C2STATbits.BCL) {
        return E_FAILED;
    }
    return E_OK;
}

enum Status I2C_command_send_burst(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {
    if (args_size != 1) {
        return E_BAD_ARGSIZE;
    }

    uint8_t const data = *args;
    I2C_Transmit(data);

    return E_OK;
}

enum Status I2C_command_restart(
    uint8_t args[],
    uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *rets_size
) {
    if (args_size != 1) {
        return E_BAD_ARGSIZE;
    }

    uint8_t const address = *args;
    I2C_RestartSignal();
    I2C_Transmit(address);
    **rets = (uint8_t)I2C2STATbits.ACKSTAT;
    *rets_size = 1;

    if (I2C_ACKNOWLEDGE_STATUS_BIT && I2C2STATbits.BCL) {
        return E_FAILED;
    }

    return E_OK;
}

enum Status I2C_command_read_more(
    __attribute__ ((unused)) uint8_t args[],
    __attribute__ ((unused)) uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *rets_size
) {
    uint8_t const data = I2C_Receive(I2C_RESPONSE_ACKNOWLEDGE);
    **rets = data;
    *rets_size = 1;
    return E_OK;
}

enum Status I2C_command_read_end(
    __attribute__ ((unused)) uint8_t args[],
    __attribute__ ((unused)) uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *rets_size
) {
    uint8_t const data = I2C_Receive(I2C_RESPONSE_NEGATIVE_ACKNOWLEDGE);
    **rets = data;
    *rets_size = 1;
    return E_OK;
}

enum Status I2C_command_config(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {
    if (args_size != 2) {
        return E_BAD_ARGSIZE;
    }

    uint16_t const baud_rate = *(uint16_t const *const)args;
    I2C_InitializeIfNot(baud_rate, I2C_DISABLE_INTERRUPTS);
    return E_OK;
}

enum Status I2C_command_status(
    __attribute__ ((unused)) uint8_t args[],
    __attribute__ ((unused)) uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *rets_size
) {
    *rets = (uint8_t *)&I2C2STAT;
    *rets_size = 2;
    return E_OK;
}

enum Status I2C_command_read_bulk(
    uint8_t *const args,
    uint16_t const args_size,
    uint8_t *rets[],
    uint16_t *rets_size
) {
    if (args_size != 3) {
        return E_BAD_ARGSIZE;
    }

    uint8_t const device = args[0];
    uint8_t const address = args[1];
    uint8_t count = args[2];

    if (!count) {
        return E_OK;
    }

    // count is uint8_t and will always fit in payload buffer.

    I2C_StartSignal();
    I2C_Transmit(device << 1);
    I2C_Transmit(address);
    I2C_RestartSignal();
    I2C_Transmit((device << 1) | 1);

    *rets = args;  // Write output to payload buffer.
    *rets_size = count;

    while (--count) {
        **rets++ = (I2C_Receive(I2C_RESPONSE_ACKNOWLEDGE));
    }
    **rets = I2C_Receive(I2C_RESPONSE_NEGATIVE_ACKNOWLEDGE);

    I2C_StopSignal();
    return E_OK;
}

enum Status I2C_command_write_bulk(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {
    if (args_size < 2) {
        return E_BAD_ARGSIZE;
    }

    uint8_t const device = args[0];
    uint8_t const count = args[1];

    if (args_size != 2U + count) {
        return E_BAD_ARGSIZE;
    }

    uint8_t const *data = args + 2;

    I2C_StartSignal();
    I2C_Transmit(device << 1);

    for (uint8_t i = 0; i < count; i++) {
        I2C_Transmit(*data++);
    }

    I2C_StopSignal();
    return E_OK;
}

enum Status I2C_command_enable_smbus(
    __attribute__ ((unused)) uint8_t args[],
    __attribute__ ((unused)) uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {

    I2C_InitializeSTAT();
    // Enable SMBus input thresholds
    I2C2CONbits.SMEN = 1;
    // Enables I2C2 module and configure SDA2 and SCL2 as serial port pins
    I2C2CONbits.I2CEN = 1;
    return E_OK;
}

enum Status I2C_command_disable_smbus(
    __attribute__ ((unused)) uint8_t args[],
    __attribute__ ((unused)) uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {

    I2C_InitializeSTAT();
    // Disable SMBus input thresholds
    I2C2CONbits.SMEN = 0;
    // Enables I2C2 module and configure SDA2 and SCL2 as serial port pins
    I2C2CONbits.I2CEN = 1;
    return E_OK;
}

enum Status I2C_command_init(
    __attribute__ ((unused)) uint8_t args[],
    __attribute__ ((unused)) uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {

    I2C_InitializeIfNot(I2C_GetBaudRate(), I2C_DISABLE_INTERRUPTS);
    return E_OK;
}

enum Status I2C_command_pull_down(
    uint8_t args[],
    uint16_t const args_size,
    __attribute__ ((unused)) uint8_t *rets[],
    __attribute__ ((unused)) uint16_t *rets_size
) {
    if (args_size != 2) {
        return E_BAD_ARGSIZE;
    }

    uint16_t const delay = *(uint16_t const *const)args;
    I2C_SCL_SetDigitalOutput();
    I2C_SCL_SetLow();
    DELAY_us(delay);
    I2C_SCL_SetHigh();
    I2C_SCL_SetDigitalInput();
    return E_OK;
}
