/****************************************************                                                                                         
* Microcontroller: STM8S103F3
* Handware: JRW-C-T-V2.0
* File: power.c                              
* Revision: 1.0  
* Description: POWER API
* Author: ValarHao
* Date：2016.6.24
****************************************************/
#include "power.h"
#include "s4011.h"
#include "uart.h"
#include "flash.h"
#include "ds3231.h"

static void PowerProcess();

TaskDef task_power = { 1, 0, 0, PowerProcess };

static u8 percent_val, percent_temp; //设定的最大功率占空比，实际控制时的PWM占空比
#if RTC_EN > 0	
static u16 time_now, time_next;
#endif
static u32 time_next_yearly; //一年模式使用滴答计时
static u16 addr_now; //记录当前指向段的起始地址
static u8 section_step = 0; //当前运行在第几段

static void InitTime()
{
	u8 hour, min; //用于一年模式下将时间转换为系统滴答数

#if RTC_EN > 0	
	DS3231_ReadTime(&time);
	time_now = *(u16 *)(&time.hour);
#endif

  //地址偏移和计算下一个变化的时间点
	switch (EEPROM_DATA_MODE)
	{
		case MODE_YEARLY:
			{
				addr_now = EEPROM_ADDR_YEARLY; //当前地址指向第一段的第一个值
				hour = *((u8 *)(addr_now + 1));
				min = *((u8 *)(addr_now + 2));
				time_next_yearly = sys.tick + ((u32)hour * 60 + min) * 60 * 1000;
			}
			break;
			
#if RTC_EN > 0			
		//由于该模式下记录的是时刻，无需初始化
		case MODE_DAYS:
			break;
		
		case MODE_SEASON:
			{
				if ((time.mon >= EEPROM_DATA_SEASON_MON1) && (time.mon < EEPROM_DATA_SEASON_MON2))
					{
						addr_now = EEPROM_ADDR_SEASON;
					}
				else if ((time.mon >= EEPROM_DATA_SEASON_MON2) && (time.mon < EEPROM_DATA_SEASON_MON3))
					{
						addr_now = EEPROM_ADDR_SEASON + 12;
					}
				else if ((time.mon >= EEPROM_DATA_SEASON_MON3) && (time.mon < EEPROM_DATA_SEASON_MON4))
					{
						addr_now = EEPROM_ADDR_SEASON + 24;
					}
				else if ((time.mon >= EEPROM_DATA_SEASON_MON4) && (time.mon < EEPROM_DATA_SEASON_MON1))
					{
						addr_now = EEPROM_ADDR_SEASON + 36;
					}
				else
					{
						if (EEPROM_DATA_SEASON_MON1 > EEPROM_DATA_SEASON_MON2)
							{
								addr_now = EEPROM_ADDR_SEASON;
							}
						else if (EEPROM_DATA_SEASON_MON2 > EEPROM_DATA_SEASON_MON3)
							{
								addr_now = EEPROM_ADDR_SEASON + 12;
							}
						else if (EEPROM_DATA_SEASON_MON3 > EEPROM_DATA_SEASON_MON4)
							{
								addr_now = EEPROM_ADDR_SEASON + 24;
							}
						else
							{
								addr_now = EEPROM_ADDR_SEASON + 36;
							}
					}
				
				time_next = time_now + *((u16 *)(addr_now + 1));	
			}
			break;
			
		case MODE_MONTHLY:
			{
				addr_now = EEPROM_ADDR_MONTHLY + (time.mon - 1) * 12; //当前地址指向第一段的第一个值
				time_next = time_now + *((u16 *)(addr_now + 1));
			}
			break;
#endif
	}
	
	percent_temp = percent_val * (*((u8 *)addr_now)) / 100;
}

void InitPower()
{
	u32 time_start, t;
	u32 next_pwm_time;
	
	PWM_VAL = PWM_INITIAL;
	sys.delay_ms(300);
	POWER_EN = 1;

	percent_val = S4011_ReadByte(S4011_ADDR_PERCENT);
	
	if ((percent_val < 20) || (percent_val > 100)) //防出错设定默认值
		percent_val = 100;
		
	InitTime(); //初始化时间变量

	time_start = (u32) EEPROM_DATA_START_TIME * 1000;
	t = sys.tick + time_start; //启动时间结束的时间点
	next_pwm_time = sys.tick + time_start / (percent_temp - PWM_INITIAL); //下一个PWM+1的时间点
	
	//启动时间处理
	while (sys.tick < t)
	{
		if (sys.tick >= next_pwm_time)
			{
				next_pwm_time += time_start / (percent_temp - PWM_INITIAL);
				
				if (++PWM_VAL == percent_temp)
					{
						if (percent_temp == 100)
							PWM_OUT_FILL();
						//else
							//PWM_OUT_NOT_FILL();
					}
			}
	}
}

static void PowerProcess()
{
#if RTC_EN > 0	
	u16 time_days_head, time_days_end;
#endif
	u8 hour, min; //用于一年模式下将时间转换为系统滴答数
	
	task_check(task_power);
	
#if RTC_EN > 0	
	DS3231_ReadTime(&time);
	time_now = *(u16 *)(&time.hour);
#endif
	
	switch (EEPROM_DATA_MODE)
	{
		case MODE_YEARLY:
			{
				if (section_step < EEPROM_DATA_SECTION)
					{
						if (sys.tick < time_next_yearly)
							{
								percent_temp = percent_val * (*((u8 *)addr_now)) / 100;
							}
						else
							{
								addr_now += 3; //指向下一段
								
								if ((section_step + 2) != EEPROM_DATA_SECTION)
									{
										hour = *((u8 *)(addr_now + 1));
										min = *((u8 *)(addr_now + 2));
										time_next_yearly = sys.tick + ((u32)hour * 60 + min) * 60 * 1000;
									}
								else
									{
										time_next_yearly = 0xFFFFFFFF; //防止最后一次循环出错
									}
															
								section_step++;
							}
					}
			}
			break;
			
#if RTC_EN > 0
		case MODE_DAYS:
			{
				time_days_head = *((u16 *)(EEPROM_ADDR_DAYS + 1));
				time_days_end = *((u16 *)(EEPROM_ADDR_DAYS + 4));
				
				if (time_days_end > time_days_head)
					{
						if ((time_now >= time_days_head) && (time_now < time_days_end))
							{
								percent_temp = percent_val * (*((u8 *)(EEPROM_ADDR_DAYS + 3))) / 100;
							}
						else
							{
								percent_temp = percent_val * (*((u8 *)EEPROM_ADDR_DAYS)) / 100;
							}
					}
				else
					{
						if ((time_now >= time_days_end) && (time_now < time_days_head))
							{								
								percent_temp = percent_val * (*((u8 *)EEPROM_ADDR_DAYS)) / 100;
							}
						else
							{
								percent_temp = percent_val * (*((u8 *)(EEPROM_ADDR_DAYS + 3))) / 100;
							}
					}
			}
			break;
		
		case MODE_SEASON:
		case MODE_MONTHLY:
			{
				if (section_step < EEPROM_DATA_SECTION)
					{
						if (time_now < time_next)
							{
								percent_temp = percent_val * (*((u8 *)addr_now)) / 100;
							}
						else
							{
								addr_now += 3; //指向下一段
								
								if ((section_step + 2) != EEPROM_DATA_SECTION)
									{
										time_next = time_now + (*((u16 *)(addr_now + 1)));
									}
								else
									{
										time_next = time_now + 0x0001; //防止最后一次循环出错
									}
															
								section_step++;
							}
					}
			}
			break;
#endif
	}
	
	//Uart1Tx.buff[0] = PWM_VAL;
	//Uart1Tx.buff[1] = percent_temp;
	//Uart1Tx.send(2);
	
	while (PWM_VAL > percent_temp)			
	{
		PWM_VAL--;
		sys.delay_ms(50);
	}
	
	while (PWM_VAL < percent_temp)			
	{
		PWM_VAL++;
		sys.delay_ms(50);
	}
	
	if (percent_temp == 100)
		PWM_OUT_FILL();
	else
		PWM_OUT_NOT_FILL();
	
	task_wait(1000);
}
