/****************************************************                                                                                         
* Microcontroller: STM8S103F3
* Handware: JRW-C-T-V2.0
* File: i2c.c                              
* Revision: 1.0  
* Description: I2C API
* Author: ValarHao
* Date£º2016.6.1
****************************************************/
#include "i2c.h"

#define   _1US   2
#define   _2US   6
#define   _3US   10
#define   _5US   18

static void Delay_us(u8 t)
{
	for (u8 i=0; i<t; i++);
}

void I2C_Start()
{
	// SDA 1->0 while SCL high
	SDA_OUT = 1;
	Delay_us(_1US);
	SCL_OUT = 1;
	Delay_us(_5US);
	SDA_OUT = 0;
	Delay_us(_5US);
	SCL_OUT = 0;
	Delay_us(_2US);
}

void I2C_Stop()
{
	// SDA 0->1 while SCL high
	SDA_OUT = 0;
	Delay_us(_1US);
	SCL_OUT = 1;
	Delay_us(_5US);
	SDA_OUT = 1;
	Delay_us(_3US);
}

void I2C_SendByte(u8 data)
{
	for (u8 i=0; i<8; i++)
	{
		if ((data<<i)&0x80)
			SDA_OUT = 1;
		else
			SDA_OUT = 0;
		Delay_us(_1US);
		SCL_OUT = 1;
		Delay_us(_5US);
		SCL_OUT = 0;
	}
	Delay_us(_2US);
	SDA_OUT = 1;
	Delay_us(_2US);
	SCL_OUT = 1;
	Delay_us(_5US);
	SCL_OUT = 0;
	Delay_us(_2US);
}

u8 I2C_ReceiveByte()
{
	u8 data;
	data = 0x00;
	SDA_OUT = 1;
	I2C_SDA_SetIn();
	//I2C_SCL_SetOut();
	for (u8 i=0; i<8; i++)
	{
		Delay_us(_1US);
		SCL_OUT = 0;
		Delay_us(_5US);
		SCL_OUT = 1;
		Delay_us(_1US);
		data <<= 1;
		if (SDA_IN==1)   data |= 0x01;
		//Delay_us(_2US);
	}
	Delay_us(_1US);
	SCL_OUT = 0;
	Delay_us(_2US);
	I2C_SDA_SetOut();
	return data;
}

void I2C_Ack(u8 ack)
{
	if (ack==1)
		SDA_OUT = 1;
	else
		SDA_OUT = 0;
	Delay_us(_3US);
	SCL_OUT = 1;
	Delay_us(_5US);
	SCL_OUT = 0;
	Delay_us(_2US);
}

