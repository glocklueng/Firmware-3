/****************************************************                                                                                         
* Microcontroller: STM8S103F3
* Handware: JRD6-24-CD-4-V0
* File: mcu.c                              
* Revision: 1.0  
* Description: call InitDevice() initialize MCU
* Author: ValarHao
* Date£º2015.12.28
****************************************************/
#include "led.h"

static void led_task();

TaskDef task_led = { 1, 0, 0, led_task };

static void led_task()
{
	static u8 led_status = 0;
	
	task_check(task_led);
	
	switch (led_status)
	{
		case 0:
			LED_G = LED_ON;
			led_status = 1;
			task_wait(500);
			break;
		
		case 1:
			LED_G = LED_OFF;
			led_status = 0;
			task_wait(500);
			break;
	}
}
 
