/****************************************************                                                                                         
* Microcontroller: STM32F103RBT6
* Handware: JRMD1-V1.0
* File: main.c                              
* Revision: 1.0  
* Description: 
* Author: ValarHao
* Date£∫2015.12.24
****************************************************/
#include "sys.h"
#include "key.h"
#include "power.h"
#include "led.h"
#include "lcd.h"
#include "gui.h"
#include "rf.h"
#include "adc.h"

OS_STK START_TASK_STK[START_STK_SIZE];
static void start_task(void *pdata);	

int main(void)
{	
	KEY_init();
	POWER_init();
	KeyPowerCheck();	
	
	LED_init();
	led_status_switch(LED_STATUS_NORMAL);
		
	OSInit();  
	 
 	OSTaskCreate( start_task, 
 							  (void *)0, 
 							  (OS_STK *)&START_TASK_STK[START_STK_SIZE-1],
 							  START_TASK_PRIO );
 							 
	OSStart();
}

static void start_task(void *pdata)
{	
	SysTick_init();
		
#if OS_TASK_STAT_EN > 0
  OSStatInit();                                                                                                            
#endif						   
	
	OSTaskCreate( lcd_task,
 							  (void *)0,
 							  (OS_STK*)&LCD_TASK_STK[LCD_STK_SIZE-1],
 							  LCD_TASK_PRIO );
 	
 	OSTaskCreate( gui_task,
 							  (void *)0,
 							  (OS_STK*)&GUI_TASK_STK[GUI_STK_SIZE-1],
 							  GUI_TASK_PRIO );
 	
 	OSTaskCreate( adc_task,
 							  (void *)0,
 							  (OS_STK*)&ADC_TASK_STK[ADC_STK_SIZE-1],
 							  ADC_TASK_PRIO );
 	
#if WIRELESS_EN > 0
 	OSTaskCreate( rf_task,
 							  (void *)0,
 							  (OS_STK*)&RF_TASK_STK[RF_STK_SIZE-1],
 							  RF_TASK_PRIO );
#endif
 	
 	while (KEY_POWER==KEY_POWER_TRUE); //À… ÷ºÏ≤‚
 							  
 	OSTaskCreate( key_task,
 							  (void *)0,
 							  (OS_STK*)&KEY_TASK_STK[KEY_STK_SIZE-1],
 							  KEY_TASK_PRIO );						  
	
	OSTaskDel(OS_PRIO_SELF);
}
