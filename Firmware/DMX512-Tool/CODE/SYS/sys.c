/****************************************************                                                                                         
* Microcontroller: STM32F103RBT6
* File: sys.c                              
* Revision: 1.0  
* Author: ValarHao
* Date：2015.12.24
****************************************************/
#include "sys.h"

void SysTick_init(void)
{
	SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8; //外部8分频
	SysTick->LOAD  = LOAD_VAL;
	SysTick->VAL   = 0x000000; //清空计数器
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; //开启SYSTICK中断
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  //开启SYSTICK
}

void SysTick_Handler(void)
{	
	if (OSRunning == TRUE)
		{
			OSIntEnter(); //进入中断
			OSTimeTick(); //调用ucos的时钟服务程序               
			OSIntExit();  //触发任务切换软中断
		}	
}

void delay_us(u32 nus)
{
	u32 last_val, nus_val, now_val, us_cnt=0;
	
	nus_val = nus * TICK_1US;
	
	OSSchedLock();
	
	last_val = SysTick->VAL;
	
	while (1)
	{
		now_val = SysTick->VAL;
		
		if (now_val!=last_val)
			{
				if (now_val<last_val)
					us_cnt += last_val - now_val;
				else
					us_cnt += LOAD_VAL - now_val + last_val;
				
				last_val = now_val;
				
				if (us_cnt>=nus_val)
					break;
			}
	}
	
	OSSchedUnlock();
}
