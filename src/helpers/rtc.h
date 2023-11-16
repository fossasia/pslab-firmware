#ifndef RTC_H
#define	RTC_H

#include <xc.h>

#define DS1307_I2C_DEVICE_ADDRESS        0x68

#ifdef	__cplusplus
extern "C" {
#endif
    
    typedef enum {
        DS1307_DATA_REG_SECONDS, // Clock Halt (1) | 10 Seconds (3) | Seconds (4)
        DS1307_DATA_REG_MINUTES, // XX (1) | 10 Minutes (3) | Minutes (4)
        DS1307_DATA_REG_HOURS,   // XX (1) | 12/24 | AM/PM | 10h | Hours (4)
        DS1307_DATA_REG_DAYS,    // XX (5) | Day (3)
        DS1307_DATA_REG_DATES,   // XX (2) | 10 Dates (2) | Dates (4)
        DS1307_DATA_REG_MONTHS,  // XX (3) | 10 Months (1) | Month (4)
        DS1307_DATA_REG_YEARS,   // 10 Years (4) | Year (4) [00 - 99]
        DS1307_DATA_REG_CONTROL  // OUT (1) | XX (2) | SQWE (1) | XX (2) | RS1:0 (2)
    } DS1307_DATA_REG;

    /**
     * @brief Updates the time in DS1307 real-time clock
     * 
     * @description
     * This command takes eight arguments over serial
     * 1. (uint8) seconds
     *    This 8-bit variable defines the second in time and it should have the 
     *    following format and Clock Halt bit needs to be 0 for the RTC to work.
     *    | Clock Halt (1) | 10 Seconds (3) | Seconds (4) |
     * 2. (uint8) minutes
     *    This 8-bit variable defines the minute in time and it should have the
     *    following format. (Note: XX is a don't care bit)
     *    | XX (1) | 10 Minutes (3) | Minutes (4) |
     * 3. (uint8) hours
     *    This 8-bit variable defines the hour in time and it should have the
     *    following format. (Note: XX is a don't care bit)
     *    | XX (1) | 12/24 | AM/PM | 10h | Hours (4) |
     * 4. (uint8) day
     *    This 8-bit variable defines the week of the day (0 - Monday, ...) and 
     *    it should have the following format. (Note: XX is a don't care bit)
     *    | XX (5) | Day (3) |
     * 5. (uint8) date
     *    This 8-bit variable defines the date in month and it should have the 
     *    following format. (Note: XX is a don't care bit)
     *    | // XX (2) | 10 Dates (2) | Dates (4) |
     * 6. (uint8) month
     *    This 8-bit variable defines the month in year (0 - January, ...) and
     *    it should have the following format. (Note: XX is a don't care bit)
     *    | XX (3) | 10 Months (1) | Month (4) |
     * 7. (uint8) year
     *    This 8-bit variable defines the year (00 - 99) and it should have the
     *    following format.
     *    | 10 Years (4) | Year (4) |
     * 8. (uint8) control
     *    This is a control sequence to enable the SQR pin in DS1307 module and 
     *    to power up the timing clock. We are not using the SQR functions. The
     *    MSB needs to be set to high to enable to clock.
     *    | OUT (1) | XX (2) | SQWE (1) | XX (2) | RS1:0 (2) |
     * 
     * This method does not pass any messages over the serial back to host.
     * 
     * @return SUCCESS, FAILED
     */
    response_t RTC_SetTime(uint32_t* unix_timestamp);

    response_t RTC_CmdSetTime(void);

    /**
     * @brief Updates a single time parameter in DS1307 real-time clock
     * 
     * @description
     * This command takes two arguments over serial.
     * 1. (uint8) address
     *    This 8-bit variable defines one of the 8 registers in DS1307 as defined
     *    in DS1307_DATA_REG enum.
     * 2. (uint8) data
     *    This variable has the data to be written to the register defined using
     *    the address variable.
     * 
     * This method does not pass any messages over the serial back to host.
     * 
     * @return SUCCESS, FAILED
     */
    response_t RTC_SetDigit(void);
    
    /**
     * @brief Fetch time from DS1307 real-time clock
     * 
     * @description
     * This command reads the real time value from clock module and send it over
     * the serial back to host. There will be seven byte transfers from PSLab to
     * host with the order following the register pattern defined in the 
     * DS1307_DATA_REG enum.
     * 
     * This method will pass 8 unsigned bytes to the host over serial and host
     * will need to read all 7 bytes to complete the transaction.
     * 
     * @return SUCCESS, FAILED
     */
    response_t RTC_GetTime(uint32_t* unix_timestamp);

    response_t RTC_CmdGetTime(void);
    /**
     * @brief Fetch a single time parameter from DS1307 real-time clock
     * 
     * @description
     * This command takes one argument over serial allowing the host to read a 
     * single register value from DS1307 without having to read data from the 
     * whole register set.
     * 1. (uint8) register
     *    This variable defines the register value to read the data from. It has
     *    to be one of the registers in DS1307_DATA_REG enum.
     * 
     * This method will pass one unsigned byte with the data content in the called
     * register to the host over serial
     * 
     * @return SUCCESS, FAILED
     */
    response_t RTC_GetDigit(void);

#ifdef	__cplusplus
}
#endif

#endif	/* RTC_H */

