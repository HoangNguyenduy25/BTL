#ifndef DS3231_H
#define DS3231_H    

#include "types_def.h"

/*DS3231 I2C address*/
#define DS3231_ADDRESS 0x68
#define DS3231_ADDRESS_WRITE 0xD0
#define DS3231_ADDRESS_READ 0xD1

/*Register definitions*/
#define DS3231_REG_SECONDS 0x00
#define DS3231_REG_MINUTES 0x01
#define DS3231_REG_HOURS 0x02
#define DS3231_REG_DAY 0x03
#define DS3231_REG_DATE 0x04
#define DS3231_REG_MONTH 0x05
#define DS3231_REG_YEAR 0x06
#define DS3231_REG_ALARM1_SECONDS 0x07
#define DS3231_REG_ALARM1_MINUTES 0x08
#define DS3231_REG_ALARM1_HOURS 0x09
#define DS3231_REG_ALARM1_DAY_DATE 0x0A
#define DS3231_REG_CONTROL 0x0E
#define DS3231_REG_STATUS 0x0F

/*Types declarations*/
typedef struct {
    uint8 seconds;   /* 0 - 59 */
    uint8 minutes;   /* 0 - 59 */
    uint8 hours;     /* 0 - 23 or 1-12 for am/pm */
    uint8 day;       /* 1 - 7 */
    uint8 date;      /* 1 - 31 */
    uint8 month;     /* 1 - 12 */
    uint8 year;      /* 0 - 99 */
    uint8 am_pm;     /* 0 - AM, 1 - PM */
} DS3231_Time;  

/**
 * @brief ds3231 alarm1 enumeration definition
 */
typedef enum
{
    DS3231_ALARM1_MODE_ONCE_A_SECOND                 = 0x0F,        /**< interrupt once a second */
    DS3231_ALARM1_MODE_SECOND_MATCH                  = 0x0E,        /**< interrupt second match */
    DS3231_ALARM1_MODE_MINUTE_SECOND_MATCH           = 0x0C,        /**< interrupt minute second match */
    DS3231_ALARM1_MODE_HOUR_MINUTE_SECOND_MATCH      = 0x08,        /**< interrupt hour minute second match */
    DS3231_ALARM1_MODE_DATE_HOUR_MINUTE_SECOND_MATCH = 0x00,        /**< interrupt date hour minute second match */
    DS3231_ALARM1_MODE_WEEK_HOUR_MINUTE_SECOND_MATCH = 0x10,        /**< interrupt week hour minute second match */
} ds3231_alarm1_mode_t;

/*
 * Description:
 *  - Initialize the DS3231:
 *    + Clear the CH (Clock Halt) bit in the seconds register to allow the
 *      oscillator to run.
 *    + Write Control register = 0x00 (disable SQW/OUT if not used).
 *  - Requirement: TWI_init() must be called beforehand.
 */
void DS3231_init(void);

/*
 * Description:
 *  - Write the full date/time into the DS1307, using 24-hour format.
 *  - Field values in the struct are in decimal (0–59, 0–23, etc.).
 */
void DS3231_setDateTime(const DS3231_Time *datetimePtr);

/*
 * Description:
 *  - Read the full date/time from the DS3231.
 *  - Field values returned in the struct are in decimal (0–59, 0–23, etc.).
 */
void DS3231_getDateTime(DS3231_Time *datetimePtr);



#endif /* DS3231_H */