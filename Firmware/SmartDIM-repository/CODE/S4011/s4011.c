/****************************************************                                                                                         
* Microcontroller: STM8S103F3
* Handware: JRW-C-T-V2.0
* File: s4011.c                              
* Revision: 1.0  
* Description: SL3S4011 API
* Author: ValarHao
* Date£º2016.6.1
****************************************************/
#include "s4011.h"

u8 S4011_ReadByte(u16 addr)
{
	u8 temp;
	I2C_Start();
	I2C_SendByte(S4011_WRITE);
	I2C_SendByte(HIGH(addr));
	I2C_SendByte(LOW(addr));
	I2C_Start();
	I2C_SendByte(S4011_READ);
	temp = I2C_ReceiveByte();
	I2C_Ack(1);
	I2C_Stop();
	return temp;
}

void S4011_WriteByte(u16 addr, u8 byte)
{
	u16 data;
	data = (u16)byte << 8;
	
	I2C_Start();
	I2C_SendByte(S4011_WRITE);
	I2C_SendByte(HIGH(addr));
	I2C_SendByte(LOW(addr));
	I2C_SendByte(HIGH(data));
	I2C_SendByte(LOW(data));
	I2C_Stop();
}



