/****************************************************                                                                                         
* Microcontroller: STM32F103RBT6
* Handware: JRMD1-V1.0
* File: tm1829.c                              
* Revision: 1.0  
* Description: TM1829 API
* Author: ValarHao
* Date：2015.12.24
****************************************************/
#include "tm1829.h"

static void TM1829_WriteByte(u8 data)
{
	u8 i, t;
	
	for (i=0; i<8; i++)
	{
		if (data&0x80) // 0->833ns 1->500ns
			{
				TM1829_DATA = 0;
				t = 7;
				while (--t);		
				TM1829_DATA = 1;
				__nop();
				__nop();
				__nop();
				__nop();
				__nop();
				__nop();
				__nop();
				__nop();
			}
		else // 0->291ns 1->1041ns
			{
				TM1829_DATA = 0;
				t = 2;
				while (--t);
				TM1829_DATA = 1;
				t = 6;
				while (--t);
			}
		
		data <<= 1;
	}
}

static void TM1829_Reset(void)
{
	TM1829_DATA = 1;
	delay_us(200);
}

void TM1829_WriteCurrent(u8 Ir, u8 Ig, u8 Ib)
{
	u8 i, temp_h, temp_l;
	
	Ir -= 10;
	Ig -= 10;
	Ib -= 10;
	
	temp_h = (Ir<<3) + (Ig>>2);
	temp_l = (Ig<<6) + (Ib<<1);
	
	for (i=0; i<170; i++)
	{
		TM1829_WriteByte(0xFF);
		TM1829_WriteByte(temp_h);
		TM1829_WriteByte(temp_l);
	}	
	
	TM1829_Reset();
}

void SendTM1829(u8 *buff, u16 len)
{
	u16 i;
	
	OSSchedLock();
	
	for (i=0; i<len; i++)
	{
		if (buff[i]==255) buff[i]--; //TM1829高字节不能全为1，特殊处理
		TM1829_WriteByte(buff[i]);	
	}
	TM1829_Reset();
	
	OSSchedUnlock();
}

