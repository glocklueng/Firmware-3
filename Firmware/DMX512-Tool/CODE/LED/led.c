/****************************************************                                                                                         
* Microcontroller: STM32F103RBT6
* Handware: JRMD1-V1.0
* File: led.c                              
* Revision: 1.0  
* Description: LED initialize
* Author: ValarHao
* Date£º2015.12.24
****************************************************/
#include "led.h"

void LED_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
	
	//LED_R & LED_G
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_8;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 
	
	//LED_B
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void led_status_switch(LedStatusDef status)
{
	switch (status)
	{
		case LED_STATUS_NORMAL: LED_GREEN(); break;
		case LED_STATUS_LOW_VOLTAGE: LED_RED(); break;
	}
}

 
