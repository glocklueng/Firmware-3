/****************************************************                                                                                         
* Microcontroller: STM32F103RBT6
* Handware: JRMD1-V1.0
* File: key.c                              
* Revision: 1.0  
* Description: KEY API
* Author: ValarHao
* Date：2015.12.24
****************************************************/
#include "key.h"
#include "gui.h"
#include "lcd.h"
#include "power.h"
#include "led.h"
#include "24cxx.h"
#include "rf.h"

OS_STK KEY_TASK_STK[KEY_STK_SIZE];

static void key_up_function(void);
static void key_down_function(void);
static void key_left_function(void);
static void key_right_function(void);
static void key_ok_function(void);
static void key_return_function(void);
static void key_power_function(void);

static KeyDef key_up     = { KEY_FALSE, 0, 0, key_up_function };
static KeyDef key_down   = { KEY_FALSE, 0, 0, key_down_function };
static KeyDef key_left   = { KEY_FALSE, 0, 0, key_left_function };
static KeyDef key_right  = { KEY_FALSE, 0, 0, key_right_function };
static KeyDef key_ok     = { KEY_FALSE, 0, 0, key_ok_function };
static KeyDef key_return = { KEY_FALSE, 0, 0, key_return_function };
static KeyDef key_power  = { KEY_POWER_FALSE, 0, 0, key_power_function };

void KEY_init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	
	//KEY_UP
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
 	//KEY_DOWN
 	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//KEY_RETURN & KEY_LEFT & KEY_RIGHT & KEY_OK
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	
 	//KEY_POWER
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void KeyIndex(KeyDef *key_temp)
{
	if (key_temp->value==KEY_FALSE)
		{
			key_temp->lock = 0;
			key_temp->cnt = 0;
		}
	else if (key_temp->lock==0)
		{
			if (++key_temp->cnt==KEY_CNT)
				{
					key_temp->lock = 1;
					key_temp->handler();
				}
		}
}

#if KEEP_EN > 0
static void KeyKeepIndex(KeyDef *key_temp)
{
	if (key_temp->value==KEY_TRUE)
		{
			key_temp->cnt++;
			if (key_temp->cnt==KEY_CNT)
				{
					key_temp->handler();
				}
			else if (key_temp->cnt==KEY_KEEP_CNT)
				{
					key_temp->handler();
					key_temp->cnt = KEY_KEEP_INIT;
				}
		}
	else key_temp->cnt = 0;
}
#endif

void KeyService(void)
{
	key_up.value = KEY_UP;
	KeyKeepIndex(&key_up);
	
	key_down.value = KEY_DOWN;
	KeyKeepIndex(&key_down);
	
	key_left.value = KEY_LEFT;
	KeyKeepIndex(&key_left);
	
	key_right.value = KEY_RIGHT;
	KeyKeepIndex(&key_right);
	
	key_ok.value = KEY_OK;
	KeyIndex(&key_ok);
	
	key_return.value = KEY_RETURN;
	KeyIndex(&key_return);
	
	if (KEY_POWER==KEY_POWER_TRUE)
		{
			if (++key_power.cnt==KEY_POWER_KEEP_CNT)
				{
					key_power_function();
				}
		}
	else if (key_power.cnt > KEY_CNT)
		{ 
			LCD_BL_EN ^= 1;		
			key_power.cnt = 0;
		}
	else key_power.cnt = 0;
}

void KeyPowerCheck(void)
{
	u32 cnt;
	
	while (1)
	{
		if (KEY_POWER==KEY_POWER_TRUE)
			{
				if (++cnt>2000000UL)
					{
						POWER_EN = 1;
						break;
					}					
			}
		else cnt = 0;
	}
}

static void key_up_function(void)
{
	if ((menu->index == MENU_INDEX_SYSTEM)            
	 || (menu->index == MENU_INDEX_COLOUR_CHANGE_3)
	 || (menu->index == MENU_INDEX_COLOUR_CHANGE_4)   
	 || (menu->index == MENU_INDEX_COLOUR_ROTATION_3)
	 || (menu->index == MENU_INDEX_COLOUR_ROTATION_4) 
	 || (menu->index == MENU_INDEX_COLOUR_ROTATION_CH))
	  return;
	
	if (menu->ChildrenMenuTotal > 4) //多于4个子菜单先移动Pointer再移动display
		{
			if (pointer_row > 1)
				{
					LCD_Clear_Pointer(pointer_row, pointer_col);
					pointer_row--;
					LCD_Display_Pointer(pointer_row, pointer_col);
				}
			else
				{
					if (display_row > 1)
						{
							display_row--;
							menu->display();
							LCD_Display_Pointer(pointer_row, pointer_col);
						}	
				}
		}
	else //少于4个子菜单只移动Pointer
		{
			if (pointer_row > 1)
				{
					LCD_Clear_Pointer(pointer_row, pointer_col);
					pointer_row--;
					LCD_Display_Pointer(pointer_row, pointer_col);					
				}
		}
}

static void key_down_function(void)
{
	if ((menu->index == MENU_INDEX_SYSTEM)            
	 || (menu->index == MENU_INDEX_COLOUR_CHANGE_3)
	 || (menu->index == MENU_INDEX_COLOUR_CHANGE_4)   
	 || (menu->index == MENU_INDEX_COLOUR_ROTATION_3)
	 || (menu->index == MENU_INDEX_COLOUR_ROTATION_4) 
	 || (menu->index == MENU_INDEX_COLOUR_ROTATION_CH))
	  return;
	
	if (menu->ChildrenMenuTotal > 4) //多于4个子菜单先移动Pointer再移动display
		{
			if (pointer_row < 4)
				{
					LCD_Clear_Pointer(pointer_row, pointer_col);
					pointer_row++;
					LCD_Display_Pointer(pointer_row, pointer_col);					
				}
			else
				{
					if (display_row < (menu->ChildrenMenuTotal -3))
						{
							display_row++;
							menu->display();
							LCD_Display_Pointer(pointer_row, pointer_col);
						}									
				}
		}
	else //少于4个子菜单只移动Pointer
		{
			if (pointer_row < menu->ChildrenMenuTotal)
				{
					LCD_Clear_Pointer(pointer_row, pointer_col);
					pointer_row++;
					LCD_Display_Pointer(pointer_row, pointer_col);
				}
		}
}

static void key_left_function(void)
{
	u16 i;
	
	switch (menu->index)
	{
		case MENU_INDEX_ADDRESS_LINE:
		case MENU_INDEX_ADDRESS_RELAY:
			{
				if (pointer_row==1)
					{
						if (light_start_num==0)
							{
								light_start_num = 512;
							}										
						else
							{
								light_start_num--;
							}										
					}
				else if (pointer_row==2)
					{
						if (light_total==0)
							{
								light_total = 512;
							}										
						else
							{
								light_total--;
							}	
					}
				
				light_start_address = (light_start_num-1)*light_total +1;
				
				if (light_start_address>512) light_start_address = 512;
				
				menu->display();
				LCD_Display_Pointer(pointer_row, pointer_col);
			}
			break;
		
		case MENU_INDEX_PARAMETERS:
			{
				switch (display_row-1 +pointer_row)
				{
					case 1: 
						if (chip_type==PWM_OUTPUT) chip_type = MBI6033;									
						else chip_type--;
						break;
					
					case 2: 
						if (gray_level==2) gray_level = 16;									
						else gray_level--;
						break;
					
					case 3: 
						if (pwm_frequency==PWM_60HZ) pwm_frequency = PWM_3840HZ;									
						else pwm_frequency--;
						break;
					
					case 4: 
						if (dmx_channel_total==0) dmx_channel_total = 160;									
						else dmx_channel_total--;
						break;
					
					case 5: 
						if (output_type==1) output_type = 160;									
						else output_type--;
						break;
					
					case 6: 
						if (serial_baud==BAUD_SELF) serial_baud = BAUD_250K;									
						else serial_baud = BAUD_SELF;
						break;
					
					case 7: 
						if (output_polarity==VALID_HIGH) output_polarity = VALID_LOW;									
						else output_polarity = VALID_HIGH;
						break;
					
					case 8: 
						if (chip_self_check==SELF_NO) chip_self_check = SELF_SLOW_SPEED;									
						else chip_self_check--;
						break;
					
					case 9: 
						if (half_power_output_temperature==50) half_power_output_temperature = 125;		
						else if (half_power_output_temperature==125) half_power_output_temperature = 100;					 			
						else half_power_output_temperature -= 10;
						break;
					
					case 10: 
						if (close_light_temperature==50) close_light_temperature = 125;		
						else if (close_light_temperature==125) close_light_temperature = 100;					 			
						else close_light_temperature -= 10;
						break;
					
					case 11: 
						if (colour_mode==1) colour_mode = 4;									
						else colour_mode--;
						break;
					
					case 12: 
						if (gamma==GAMMA_1_0) gamma = GAMMA_3_0;									
						else gamma--;
						break;
				}
				
				menu->display();
				LCD_Display_Pointer(pointer_row, pointer_col);
			}
			break;
		
		case MENU_INDEX_SETTING:
			{
				if (pointer_row==1)
					{
						if (language==LANGUAGE_CHINESE)
							{
								language = LANGUAGE_ENGLISH;
							}
						else
							{
								language--;
							}
						
						AT24CXX_WriteByte(EEPROM_ADDR_LANGUAGE, language);
					}
				else if (pointer_row==2)
					{
#if WIRELESS_EN > 0						
						if (job_mode==JOB_MODE_SELF)
							{
								job_mode = JOB_MODE_SLAVE;
							}										
						else
							{
								job_mode--;
							}	
#else
						job_mode = JOB_MODE_SELF;
#endif													
						
						AT24CXX_WriteByte(EEPROM_ADDR_MODE, job_mode);
					}
				else if (pointer_row==3)
					{
						if (modbus==MODBUS_DMX)
							{
								modbus = MODBUS_1804;
							}										
						else
							{
								modbus--;
							}	
						
						AT24CXX_WriteByte(EEPROM_ADDR_MODBUS, modbus);
					}
				
				menu->display();
				LCD_Display_Pointer(pointer_row, pointer_col);
			}
			break;
		
		case MENU_INDEX_COLOUR_ALL:
			{
				if (((display_row-1)+pointer_row) == 1)
					{
						if (colour_total != 1)
							{
								colour_total--;
							}
					}
				else
					{
						if (channel[(((display_row-1)+pointer_row)-1)] != 0)
							{
								channel[(((display_row-1)+pointer_row)-1)]--;
							}
					}
				
				if (job_mode==JOB_MODE_HOST)
					{
						packet_num = 0;			
						rf_sendbuf[0] = MENU_INDEX_COLOUR_ALL;
						rf_sendbuf[1] = packet_num;
						rf_sendbuf[2] = colour_total;
						rf_sendbuf[3] = channel[1];
						rf_sendbuf[4] = channel[2];
						rf_sendbuf[5] = channel[3];
						rf_sendbuf[6] = channel[4];
						rf_sendbuf[7] = channel[5];
						rf_sendbuf[8] = channel[6];
						crc16_1021(rf_sendbuf, 9);
						RF_SendPacket(rf_sendbuf, RF_SIZE);
					}
								
				menu->display();
				LCD_Display_Pointer(pointer_row, pointer_col);
			}
			break;
		
		case MENU_INDEX_COLOUR_CH:
			{
				if (channel[((display_row-1)+pointer_row)] != 0)
					{
						channel[((display_row-1)+pointer_row)]--;
					}
								
				if (job_mode==JOB_MODE_HOST)
					{
						packet_num = 0;			
						rf_sendbuf[0] = MENU_INDEX_COLOUR_CH;
						rf_sendbuf[1] = packet_num;
						rf_sendbuf[2] = (display_row-1)+pointer_row;
						rf_sendbuf[3] = channel[((display_row-1)+pointer_row)];
						crc16_1021(rf_sendbuf, 4);
						RF_SendPacket(rf_sendbuf, RF_SIZE);
					}
								
				menu->display();
				LCD_Display_Pointer(pointer_row, pointer_col);
			}
			break;
			
	#if MENU_EXTRA_EN > 0		
		case MENU_INDEX_EXTRA:
			{
				if (channel[((display_row-1)+pointer_row)] != 0)
					{
						channel[((display_row-1)+pointer_row)]--;
					}
								
				/*if (job_mode==JOB_MODE_HOST)
					{
						packet_num = 0;			
						rf_sendbuf[0] = MENU_INDEX_EXTRA;
						rf_sendbuf[1] = packet_num;
						rf_sendbuf[2] = MENU_EXTRA_HDR1;
						rf_sendbuf[3] = MENU_EXTRA_HDR2;
						rf_sendbuf[4] = MENU_EXTRA_HDR3;
						rf_sendbuf[5] = channel[1];
						rf_sendbuf[6] = channel[2];
						rf_sendbuf[7] = channel[3];
						rf_sendbuf[8] = channel[4];
						//crc16_1021(rf_sendbuf, 4);
						RF_SendPacket(rf_sendbuf, RF_SIZE);
					}*/
								
				menu->display();
				LCD_Display_Pointer(pointer_row, pointer_col);
			}
			break;
	#endif
		
		case MENU_INDEX_COLOUR_ROTATION_CH:
			{
				if (rotation_ch_status==0)
					{
						rotation_ch_status = 1;
					}
				else if (rotation_ch_status==1)
					{
						if (rotation_ch_cnt != 1)
							rotation_ch_cnt--;
						
						for (i=1; i<CH_SIZE; i++)
							channel[i] = 0x00;
						
						channel[rotation_ch_cnt] = 0xFF;
						
						GUI_clear_number(3, 10);
						GUI_display_number(3, 10, rotation_ch_cnt);
					}
			}
			break;
	}		
}

static void key_right_function(void)
{
	u16 i;
	
	switch (menu->index)
	{
		case MENU_INDEX_ADDRESS_LINE:
		case MENU_INDEX_ADDRESS_RELAY:
			{
				if (pointer_row==1)
					{
						if (light_start_num==512)
							{
								light_start_num = 0;
							}										
						else
							{
								light_start_num++;
							}										
					}
				else if (pointer_row==2)
					{
						if (light_total==512)
							{
								light_total = 0;
							}										
						else
							{
								light_total++;
							}	
					}
				
				light_start_address = (light_start_num-1)*light_total +1;
				
				if (light_start_address>512) light_start_address = 512;
				
				menu->display();
				LCD_Display_Pointer(pointer_row, pointer_col);
			}
			break;
		
		case MENU_INDEX_PARAMETERS:
			{
				switch (display_row-1 +pointer_row)
				{
					case 1: 
						if (chip_type==MBI6033) chip_type = PWM_OUTPUT;									
						else chip_type++;
						break;
					
					case 2: 
						if (gray_level==16) gray_level = 2;									
						else gray_level++;
						break;
					
					case 3: 
						if (pwm_frequency==PWM_3840HZ) pwm_frequency = PWM_60HZ;									
						else pwm_frequency++;
						break;
					
					case 4: 
						if (dmx_channel_total==160) dmx_channel_total = 0;									
						else dmx_channel_total++;
						break;
					
					case 5: 
						if (output_type==160) output_type = 1;									
						else output_type++;
						break;
					
					case 6: 
						if (serial_baud==BAUD_250K) serial_baud = BAUD_SELF;									
						else serial_baud = BAUD_250K;
						break;
					
					case 7: 
						if (output_polarity==VALID_LOW) output_polarity = VALID_HIGH;									
						else output_polarity = VALID_LOW;
						break;
					
					case 8: 
						if (chip_self_check==SELF_SLOW_SPEED) chip_self_check = SELF_NO;									
						else chip_self_check++;
						break;
					
					case 9: 
						if (half_power_output_temperature==125) half_power_output_temperature = 50;		
						else if (half_power_output_temperature==100) half_power_output_temperature = 125;					 			
						else half_power_output_temperature += 10;
						break;
					
					case 10: 
						if (close_light_temperature==125) close_light_temperature = 50;		
						else if (close_light_temperature==100) close_light_temperature = 125;					 			
						else close_light_temperature += 10;
						break;
					
					case 11: 
						if (colour_mode==4) colour_mode = 1;									
						else colour_mode++;
						break;
					
					case 12: 
						if (gamma==GAMMA_3_0) gamma = GAMMA_1_0;									
						else gamma++;
						break;
				}
				
				menu->display();
				LCD_Display_Pointer(pointer_row, pointer_col);
			}
			break;
		
		case MENU_INDEX_SETTING:
			{
				if (pointer_row==1)
					{
						if (language==LANGUAGE_ENGLISH)
							{
								language = LANGUAGE_CHINESE;
							}										
						else
							{
								language++;
							}	
						
						AT24CXX_WriteByte(EEPROM_ADDR_LANGUAGE, language);									
					}
				else if (pointer_row==2)
					{
#if WIRELESS_EN > 0							
						if (job_mode==JOB_MODE_SLAVE)
							{
								job_mode = JOB_MODE_SELF;
							}										
						else
							{
								job_mode++;
							}	
#else		
						job_mode = JOB_MODE_SELF;
#endif											
						
						AT24CXX_WriteByte(EEPROM_ADDR_MODE, job_mode);
					}
				else if (pointer_row==3)
					{
						if (modbus==MODBUS_1804)
							{
								modbus = MODBUS_DMX;
							}										
						else
							{
								modbus++;
							}	
						
						AT24CXX_WriteByte(EEPROM_ADDR_MODBUS, modbus);
					}
				
				menu->display();
				LCD_Display_Pointer(pointer_row, pointer_col);
			}
			break;
		
		case MENU_INDEX_COLOUR_ALL:
			{
				if (((display_row-1)+pointer_row) == 1)
					{
						if (colour_total != 6)
							{
								colour_total++;
							}
					}
				else
					{
						if (channel[(((display_row-1)+pointer_row)-1)] != 255)
							{
								channel[(((display_row-1)+pointer_row)-1)]++;
							}
					}
				
				if (job_mode==JOB_MODE_HOST)
					{
						packet_num = 0;			
						rf_sendbuf[0] = MENU_INDEX_COLOUR_ALL;
						rf_sendbuf[1] = packet_num;
						rf_sendbuf[2] = colour_total;
						rf_sendbuf[3] = channel[1];
						rf_sendbuf[4] = channel[2];
						rf_sendbuf[5] = channel[3];
						rf_sendbuf[6] = channel[4];
						rf_sendbuf[7] = channel[5];
						rf_sendbuf[8] = channel[6];
						crc16_1021(rf_sendbuf, 9);
						RF_SendPacket(rf_sendbuf, RF_SIZE);
					}
								
				menu->display();
				LCD_Display_Pointer(pointer_row, pointer_col);
			}
			break;
		
		case MENU_INDEX_COLOUR_CH:
			{
				if (channel[((display_row-1)+pointer_row)] != 255)
					{
						channel[((display_row-1)+pointer_row)]++;
					}
				
				if (job_mode==JOB_MODE_HOST)
					{
						packet_num = 0;			
						rf_sendbuf[0] = MENU_INDEX_COLOUR_CH;
						rf_sendbuf[1] = packet_num;
						rf_sendbuf[2] = (display_row-1)+pointer_row;
						rf_sendbuf[3] = channel[((display_row-1)+pointer_row)];
						crc16_1021(rf_sendbuf, 4);
						RF_SendPacket(rf_sendbuf, RF_SIZE);
					}
						
				menu->display();
				LCD_Display_Pointer(pointer_row, pointer_col);
			}
			break;
	
	#if MENU_EXTRA_EN > 0		
		case MENU_INDEX_EXTRA:
			{
				if (channel[((display_row-1)+pointer_row)] != 255)
					{
						channel[((display_row-1)+pointer_row)]++;
					}
				
				/*if (job_mode==JOB_MODE_HOST)
					{
						packet_num = 0;			
						rf_sendbuf[0] = MENU_INDEX_EXTRA;
						rf_sendbuf[1] = packet_num;
						rf_sendbuf[2] = MENU_EXTRA_HDR1;
						rf_sendbuf[3] = MENU_EXTRA_HDR2;
						rf_sendbuf[4] = MENU_EXTRA_HDR3;
						rf_sendbuf[5] = channel[1];
						rf_sendbuf[6] = channel[2];
						rf_sendbuf[7] = channel[3];
						rf_sendbuf[8] = channel[4];
						//crc16_1021(rf_sendbuf, 4);
						RF_SendPacket(rf_sendbuf, RF_SIZE);
					}*/
						
				menu->display();
				LCD_Display_Pointer(pointer_row, pointer_col);
			}
			break;
	#endif
		
		case MENU_INDEX_COLOUR_ROTATION_CH:
			{
				if (rotation_ch_status==0)
					{
						rotation_ch_status = 1;
					}
				else if (rotation_ch_status==1)
					{
						if (rotation_ch_cnt != CH_SIZE)
							rotation_ch_cnt++;
						
						for (i=1; i<CH_SIZE; i++)
							channel[i] = 0x00;
						
						channel[rotation_ch_cnt] = 0xFF;
						
						GUI_clear_number(3, 10);
						GUI_display_number(3, 10, rotation_ch_cnt);
					}
			}
			break;
	}	
}

static void key_ok_function(void)
{
	u16 i;
	
	if (menu->ChildrenMenu == NULL) return; 
		
	//当前为第一层菜单，记忆显示指针
	if (menu == &StartMenuVar) 
		{
			last_display_row = display_row;
			last_pointer_row = pointer_row;
		}
	
  //进入下一层菜单
	if ((menu->index == MENU_INDEX_ADDRESS_LINE) || (menu->index == MENU_INDEX_ADDRESS_RELAY))	
		{
			if (((display_row-1)+pointer_row)==4) //只有选中“确认发送”才能发送
				menu = menu->ChildrenMenu;
			else
				return;
		}	
	else if (menu->index == MENU_INDEX_PARAMETERS)		
		{
			if (((display_row-1)+pointer_row)==13) //只有选中“确认发送”才能发送
				menu = menu->ChildrenMenu;
			else
				return;
		}	
#if MENU_EXTRA_EN > 0
	else if (menu->index == MENU_INDEX_EXTRA)
		{
			menu = menu->ChildrenMenu;
		}
#endif		
	else	
		{
			menu = menu->ChildrenMenu + (((display_row-1)+pointer_row)-1);
		}
	
	switch (menu->index)
	{	
		case MENU_INDEX_COLOUR_CHANGE_3:
		case MENU_INDEX_COLOUR_CHANGE_4:
		case MENU_INDEX_COLOUR_ROTATION_3:
		case MENU_INDEX_COLOUR_ROTATION_4:
		case MENU_INDEX_COLOUR_CHANGE_1:
			{
				pattern_step = 0; //从状态一开始			
				pattern_r = 0xFF; //清空RGBW数据
				pattern_g = 0;
				pattern_b = 0;
				pattern_w = 0;
				
				packet_num = 0; //从第一个包开始发
				OSTaskResume(GUI_TASK_PRIO); //唤醒任务
			}			
			break;
		
		case MENU_INDEX_COLOUR_ALL:
		case MENU_INDEX_COLOUR_CH:
	//#if MENU_EXTRA_EN > 0
		//case MENU_INDEX_EXTRA:	
	//#endif
			{
				for (i=0; i<CH_SIZE; i++)
					channel[i] = 0x00;
					
				packet_num = 0; //从第一个包开始发
				OSTaskResume(GUI_TASK_PRIO); //唤醒任务
			}		
			break;
		
		case MENU_INDEX_COLOUR_ROTATION_CH:
			{
				rotation_ch_status = 0;
				rotation_ch_cnt = 0;
				
				packet_num = 0; //从第一个包开始发
				OSTaskResume(GUI_TASK_PRIO); //唤醒任务
			}
			break;
	}

#if WIRELESS_EN > 0
	if ((menu->index == MENU_INDEX_COLOUR_ALL)
	 || (menu->index == MENU_INDEX_COLOUR_CH)
	 || (menu->index == MENU_INDEX_COLOUR_CHANGE_3)
	 || (menu->index == MENU_INDEX_COLOUR_CHANGE_4)
	 || (menu->index == MENU_INDEX_COLOUR_ROTATION_3)
	 || (menu->index == MENU_INDEX_COLOUR_ROTATION_4))
	  {
	  	packet_num = 0;			
			rf_sendbuf[0] = RF_CMD_ENABLE_TASK;
			rf_sendbuf[1] = packet_num;
			rf_sendbuf[2] = menu->index;
			rf_sendbuf[3] = modbus;
			crc16_1021(rf_sendbuf, 4);
			RF_SendPacket(rf_sendbuf, RF_SIZE);
	  }	
#endif	  
	
	//下一层菜单初始化
	display_row = 1;
	pointer_row = 1;
	menu->display();
}

static void key_return_function(void)
{
	if (menu->ParentMenu == NULL) return;
	
	//父菜单为第一层，显示记忆的指针
	if (menu->ParentMenu == &StartMenuVar)
		{
			display_row = last_display_row;
			pointer_row = last_pointer_row;
		}
	else //其他菜单显示第一行
		{
			display_row = 1;
			pointer_row = 1;
		}
				
	OSTaskSuspend(GUI_TASK_PRIO); //挂起任务
	
	if (job_mode==JOB_MODE_HOST)
		{
			if ((menu->index == MENU_INDEX_COLOUR_ALL)
			 || (menu->index == MENU_INDEX_COLOUR_CH)
			 || (menu->index == MENU_INDEX_COLOUR_CHANGE_3)
			 || (menu->index == MENU_INDEX_COLOUR_CHANGE_4)
			 || (menu->index == MENU_INDEX_COLOUR_ROTATION_3)
			 || (menu->index == MENU_INDEX_COLOUR_ROTATION_4))
			  {
			  	packet_num = 0;			
					rf_sendbuf[0] = RF_CMD_DISABLE_TASK;
					rf_sendbuf[1] = packet_num;
					rf_sendbuf[2] = menu->index;
					rf_sendbuf[3] = modbus;
					crc16_1021(rf_sendbuf, 4);
					RF_SendPacket(rf_sendbuf, RF_SIZE);
			  }		
		}
	
	menu = menu->ParentMenu;		
	menu->display();
}

static void key_power_function(void)
{
	LCD_Clear();
	menu = &StartMenuVar; //防止关于系统界面中ADC向LCD发送电压信息
	OSTimeDly(12/OS_TICK);
	OSRunning = FALSE;
	POWER_EN = 0;
	LED_R = LED_OFF;
	LED_G = LED_OFF;
	LED_B = LED_OFF;
	LCD_BL_EN = 0;	
}

void key_task(void *pdata)
{	
	while (1)
	{
		KeyService();
		OSTimeDly(4/OS_TICK);
	}
}
