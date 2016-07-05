/****************************************************                                                                                         
* Microcontroller: STM8S103F3
* Handware: JRW-C-T-V2.0
* File: ds3231.c                              
* Revision: 1.0  
* Description: DS3231 API
* Author: ValarHao
* Date£º2016.6.3
****************************************************/
#include "ds3231.h"

Time time;

static u8 HEX_TO_BCD(u8 data)
{
	return (((data%100)/10)<<4)|(data%10);
}

static u8 BCD_TO_HEX(u8 data)
{
	return (data>>4)*10 + (data&0x0F);
}

static void DS3231_Write(u8 addr, u8 data)
{
	DS_I2C_Start();
	DS_I2C_SendByte(DS3231_WRITE_ADDR);
	DS_I2C_SendByte(addr);
	DS_I2C_SendByte(data);
	DS_I2C_Stop();
}

static u8 DS3231_Read(u8 addr)
{
	u8 temp;
	DS_I2C_Start();
	DS_I2C_SendByte(DS3231_WRITE_ADDR);
	DS_I2C_SendByte(addr);
	DS_I2C_Start();
	DS_I2C_SendByte(DS3231_READ_ADDR);
	temp = DS_I2C_ReceiveByte();
	DS_I2C_Ack(1);
	DS_I2C_Stop();
	return temp;
}

void DS3231_SetTime(Time *t)
{
	u8 temp;
	
	temp = HEX_TO_BCD(t->year);
	DS3231_Write(DS3231_YEAR, temp);
	
	temp = HEX_TO_BCD(t->mon);
	DS3231_Write(DS3231_MON, temp);
	
	temp = HEX_TO_BCD(t->day);
	DS3231_Write(DS3231_DAY, temp);
	
	temp = HEX_TO_BCD(t->week);
	DS3231_Write(DS3231_WEEK, temp);
	
	temp = HEX_TO_BCD(t->hour);
	DS3231_Write(DS3231_HOUR, temp);
	
	temp = HEX_TO_BCD(t->min);
	DS3231_Write(DS3231_MIN, temp);
	
	temp = HEX_TO_BCD(t->sec);
	DS3231_Write(DS3231_SEC, temp);
}

void DS3231_ReadTime(Time *t)
{
	u8 temp;
	
	temp = DS3231_Read(DS3231_YEAR);
	t->year = BCD_TO_HEX(temp);
	
	temp = DS3231_Read(DS3231_MON);
	t->mon = BCD_TO_HEX(temp);
	
	temp = DS3231_Read(DS3231_DAY);
	t->day = BCD_TO_HEX(temp);
	
	temp = DS3231_Read(DS3231_WEEK);
	t->week = BCD_TO_HEX(temp);
	
	temp = DS3231_Read(DS3231_HOUR);
	t->hour = BCD_TO_HEX(temp);
	
	temp = DS3231_Read(DS3231_MIN);
	t->min = BCD_TO_HEX(temp);
	
	temp = DS3231_Read(DS3231_SEC);
	t->sec = BCD_TO_HEX(temp);
}