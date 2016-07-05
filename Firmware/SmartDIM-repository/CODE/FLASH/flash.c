/****************************************************                                                                                         
* Microcontroller: STM8S103F3
* Handware: JRW-C-T-V2.0
* File: ds3231.c                              
* Revision: 1.0  
* Description: DS3231 API
* Author: ValarHao
* Date：2016.6.3
****************************************************/
#include "flash.h"

//程序第一次运行才会执行
void init_eeprom_paraments()
{
	if (EEPROM_DATA_FIRST_FLAG != 1)
		{
			EEPROM_DATA_START_TIME = 5;  //5S启动时间
			EEPROM_DATA_CHANGE_TIME = 2;
			EEPROM_DATA_MODE = MODE_YEARLY;    
			
			//默认为5个小时后75%
			*((u8 *)EEPROM_ADDR_YEARLY) = 100;
			*((u8 *)EEPROM_ADDR_YEARLY+1) = 5;
			*((u8 *)EEPROM_ADDR_YEARLY+2) = 0;
			*((u8 *)EEPROM_ADDR_YEARLY+3) = 75;
			*((u8 *)EEPROM_ADDR_YEARLY+4) = 8;
			*((u8 *)EEPROM_ADDR_YEARLY+5) = 0;
			
			EEPROM_DATA_FIRST_FLAG = 1;
		}
}