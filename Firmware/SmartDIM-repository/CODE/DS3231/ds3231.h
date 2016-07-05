#ifndef _DS3231_H
#define _DS3231_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"
#include "i2c_ds.h"

/*------------------------------*
*  macros for const             *
*-------------------------------*/
#define   DS3231_WRITE_ADDR   0xD0
#define   DS3231_READ_ADDR    0xD1

#define   DS3231_SEC    0x00
#define   DS3231_MIN    0x01
#define   DS3231_HOUR   0x02
#define   DS3231_WEEK   0x03
#define   DS3231_DAY    0x04
#define   DS3231_MON    0x05
#define   DS3231_YEAR   0x06

/*------------------------------*
*  type definitions             *
*-------------------------------*/
typedef union TIME
{
	struct
	{
		u8 year;
		u8 mon;
		u8 day;
		u8 week;
		u8 hour;
		u8 min;
		u8 sec;
	};
	u8 buff[7];
} Time;

/*------------------------------*
*  public variables             *
*-------------------------------*/
extern Time time;

/*------------------------------*
*  public functions             *
*-------------------------------*/
extern void DS3231_SetTime(Time *t);
extern void DS3231_ReadTime(Time *t);

#endif