#ifndef _FLASH_H
#define _FLASH_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"

/*------------------------------*
*  macros for address           *
*-------------------------------*/
#define   EEPROM_DATA_FIRST_FLAG    *((u8 *)0x4200)

#define   EEPROM_DATA_START_TIME    *((u8 *)0x4201)    //开机PWM切换至对应功率时间
#define   EEPROM_DATA_CHANGE_TIME   *((u8 *)0x4202)    //渐变时间

#define   EEPROM_DATA_MODE          *((u8 *)0x4203)    //调光方式
#define   MODE_DAYS      0
#define   MODE_YEARLY    1
#define   MODE_SEASON    2
#define   MODE_MONTHLY   3

#define   EEPROM_DATA_SECTION       *((u8 *)0x4204)    //调光段数

// 4000 4001 4002 4003 4004 4005
// PWM1 HOUR MIN  PWM2 HOUR MIN
#define   EEPROM_ADDR_DAYS      0x4000

// 4010 4011 4012 ...共4组... 4019 401A 401B
// PWM  HOUR MIN              PWM  HOUR MIN
#define   EEPROM_ADDR_YEARLY    0x4010

// 4020 4021 4022 ...共16组... 404D 404E 404F 4050 4051 4052 4053
// PWM  HOUR MIN               PWM  HOUR MIN  MON  MON  MON  MON
#define   EEPROM_ADDR_SEASON    0x4020
#define   EEPROM_DATA_SEASON_MON1   *((u8 *)0x4050)
#define   EEPROM_DATA_SEASON_MON2   *((u8 *)0x4051)
#define   EEPROM_DATA_SEASON_MON3   *((u8 *)0x4052)
#define   EEPROM_DATA_SEASON_MON4   *((u8 *)0x4053)

// 4060 4061 4062 ...共48组... 40ED 40EE 40EF
// PWM  HOUR MIN               PWM  HOUR MIN
#define   EEPROM_ADDR_MONTHLY   0x4060

/*------------------------------*
*  public functions             *
*-------------------------------*/
extern void init_eeprom_paraments();

#endif