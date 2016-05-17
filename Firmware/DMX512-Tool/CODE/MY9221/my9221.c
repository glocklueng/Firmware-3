/****************************************************                                                                                         
* Microcontroller: STM32F103RBT6
* Handware: JRMD1-V1.0
* File: my9221.c                              
* Revision: 1.0  
* Description: MY9221 API
* Author: ValarHao
* Date£º2015.12.24
****************************************************/
#include "my9221.h"

void MY9221_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 
	
	//MY9221_DATA & MY9221_CLK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void MY9221_data(u16 data)
{
	u8 i;
	
	for (i=0; i<8; i++)
	{
		if (data & 0x8000)
			MY9221_DATA = 1;
		else 
			MY9221_DATA = 0;		
		__nop();		
		MY9221_CLK = 0;		
		data <<= 1;
		
		if (data & 0x8000)
			MY9221_DATA = 1;
		else 
			MY9221_DATA = 0;		
		__nop();		
		MY9221_CLK = 1;		
		data <<= 1;
	}
}

#define   MY9221_cmd(cmd)   MY9221_data(cmd)

void MY9221_stop()
{
	u8 i;
	
	MY9221_CLK = 1;
	MY9221_DATA = 0;
	delay_us(225);
	
	for (i=0; i<4; i++)
	{
		MY9221_DATA = 1;
		__nop();		
		MY9221_DATA = 0;
		__nop();
		__nop();
	}
	delay_us(2);
}

void SendMY9221(u8 *buff, u16 len)
{
	u16 i;
	
	OSSchedLock();
	
	for (i=len; i!=0; i--)
	{
		if (i%12==0)
			MY9221_cmd(MY9221_CMD);	
		MY9221_data(buff[i-1]);	
	}
	MY9221_stop();
	
	OSSchedUnlock();
}

