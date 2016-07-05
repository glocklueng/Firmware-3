/****************************************************                                                                                         
* Microcontroller: STM8S103F3
* Handware: JRW-C-T-V2.0
* File: i2c.c                              
* Revision: 1.0  
* Description: I2C API
* Author: ValarHao
* Date£º2016.6.1
****************************************************/
#include "i2c_ds.h"

#define   _1US   2
#define   _2US   6
#define   _3US   10
#define   _5US   18

static void Delay_us(u8 t)
{
	for (u8 i=0; i<t; i++);
}

void DS_I2C_Start()
{
	// SDA 1->0 while SCL high
	DS_SDA_OUT = 1;
	Delay_us(_1US);
	DS_SCL_OUT = 1;
	Delay_us(_5US);
	DS_SDA_OUT = 0;
	Delay_us(_5US);
	DS_SCL_OUT = 0;
	Delay_us(_2US);
}

void DS_I2C_Stop()
{
	// SDA 0->1 while SCL high
	DS_SDA_OUT = 0;
	Delay_us(_1US);
	DS_SCL_OUT = 1;
	Delay_us(_5US);
	DS_SDA_OUT = 1;
	Delay_us(_3US);
}

void DS_I2C_SendByte(u8 data)
{
	for (u8 i=0; i<8; i++)
	{
		if ((data<<i)&0x80)
			DS_SDA_OUT = 1;
		else
			DS_SDA_OUT = 0;
		Delay_us(_1US);
		DS_SCL_OUT = 1;
		Delay_us(_5US);
		DS_SCL_OUT = 0;
	}
	Delay_us(_2US);
	DS_SDA_OUT = 1;
	Delay_us(_2US);
	DS_SCL_OUT = 1;
	Delay_us(_5US);
	DS_SCL_OUT = 0;
	Delay_us(_2US);
}

u8 DS_I2C_ReceiveByte()
{
	u8 data;
	data = 0x00;
	DS_SDA_OUT = 1;
	DS_I2C_SDA_SetIn();
	//I2C_SCL_SetOut();
	for (u8 i=0; i<8; i++)
	{
		Delay_us(_1US);
		DS_SCL_OUT = 0;
		Delay_us(_5US);
		DS_SCL_OUT = 1;
		Delay_us(_1US);
		data <<= 1;
		if (DS_SDA_IN==1)   data |= 0x01;
		//Delay_us(_2US);
	}
	Delay_us(_1US);
	DS_SCL_OUT = 0;
	Delay_us(_2US);
	DS_I2C_SDA_SetOut();
	return data;
}

void DS_I2C_Ack(u8 ack)
{
	if (ack==1)
		DS_SDA_OUT = 1;
	else
		DS_SDA_OUT = 0;
	Delay_us(_3US);
	DS_SCL_OUT = 1;
	Delay_us(_5US);
	DS_SCL_OUT = 0;
	Delay_us(_2US);
}

