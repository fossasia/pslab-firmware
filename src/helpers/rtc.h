#ifndef RTC_H
#define	RTC_H

#include <time.h>
#include <xc.h>

#include "../commands.h"

#ifdef	__cplusplus
extern "C" {
#endif

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
     * This method decodes the unix_timestamp passed as param, converts it into RTC format and
     * stores the value in RTC hardware.
     *
     * @Param
     *      const* const unix_timestamp: Time to be stored in 32 bit unix timestamp format.
     *
     * @return SUCCESS, FAILED
     */
    enum Status RTC_set_time(time_t const *unix_timestamp);

    /**
     * @brief Stores time in RTC Hardware.
     *
     * @description This method reads 32 bit unix timestamp unsigned integer from the host and
     * stores the value in RTC Hardware by calling RTC_SetTime function.
     *
     * @return SUCCESS, FAILED
     */
    enum Status RTC_cmd_set_time(
        uint8_t args[],
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

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
     * @param
     * *unix_timestamp - Time in RTC hardware is converted to unix_timestamp and stored in the parameter
     *
     * @return SUCCESS, FAILED
     */
    enum Status RTC_get_time(time_t *unix_timestamp);

    /**
     * @Brief Fetches time from RTC Hardware in unix_timestamp format.
     *
     * @Description This method reads time from RTC Hardware and returns the value in unix_timestamp
     * format by calling the RTC_GetTime function.
     *
     * @return SUCCESS, FAILED
     */
    enum Status RTC_cmd_get_time(
        uint8_t args[],
        uint16_t args_size,
        uint8_t **rets,
        uint16_t *rets_size
    );

#ifdef	__cplusplus
}
#endif

#endif	/* RTC_H */
