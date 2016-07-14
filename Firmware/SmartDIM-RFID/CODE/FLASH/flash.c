/****************************************************                                                                                         
* Microcontroller: STM8S103F3
* Handware: JRW-C-T-V2.0
* File: ds3231.c                              
* Revision: 1.0  
* Description: DS3231 API
* Author: ValarHao
* Date��2016.6.3
****************************************************/
#include "flash.h"

//�����һ�����вŻ�ִ��
void init_eeprom_paraments()
{
	if (EEPROM_DATA_FIRST_FLAG != 1)
		{
			EEPROM_DATA_START_TIME = 5;  //5S����ʱ��
			EEPROM_DATA_CHANGE_TIME = 2;
			EEPROM_DATA_MODE = MODE_YEARLY;    
			
			//Ĭ��Ϊ5��Сʱ��75%
			*((u8 *)EEPROM_ADDR_YEARLY) = 100;
			*((u8 *)EEPROM_ADDR_YEARLY+1) = 5;
			*((u8 *)EEPROM_ADDR_YEARLY+2) = 0;
			*((u8 *)EEPROM_ADDR_YEARLY+3) = 75;
			*((u8 *)EEPROM_ADDR_YEARLY+4) = 8;
			*((u8 *)EEPROM_ADDR_YEARLY+5) = 0;
			
			EEPROM_DATA_FIRST_FLAG = 1;
		}
}