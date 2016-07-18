/****************************************************                                                                                         
* Microcontroller: STM8S103F3
* File: sys.c                              
* Revision: 1.0  
* Author: ValarHao
* Date：2016.2.1
****************************************************/
#include "sys.h"
#include "stm8s_tim4.h"

static void SysTick_init();
static void delay_us(u16 nus);
static void delay_ms(u32 nms);

SysDef sys = { 0, SysTick_init, delay_us, delay_ms };
TaskDef *p_task;
volatile u8 fac_us;

static void SysTick_init()
{
	SetSoftwarePriority(ITC_IRQ_TIM4_OVF, ITC_PRIORITYLEVEL_3); //Software priority level 3
	
	TIM4_TimeBaseInit(TIM4_PRESCALER_128, 124);  //128Prescaler,124==>1ms tick
	TIM4_ClearFlag(TIM4_FLAG_UPDATE);  //Clear TIM4 update flag
	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);  //Enable update interrupt  
	TIM4_Cmd(ENABLE);  //Enable TIM4
	
	//initialize delay variable 
	if (SYSCLK > 16)
		fac_us = (16-4)/4;
	else if (SYSCLK > 4)
		fac_us = (SYSCLK-4)/4;
	else
		fac_us = 1;
}

INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, ITC_IRQ_TIM4_OVF)
{
	CLEAR_TIM4_IT_OVF;
	
	sys.tick++;
}

static void delay_us(u16 nus)
{
	DisableInterrupt();
	
	__asm(
	"PUSH A          \n"  //1T,压栈
	"DELAY_XUS:      \n"   
	"LD A,fac_us     \n"  //1T,fac_us加载到累加器A
	"DELAY_US_1:     \n"  
	"NOP             \n"  //1T,nop延时
	"DEC A           \n"  //1T,A--
	"JRNE DELAY_US_1 \n"  //不等于0,则跳转(2T)到DELAY_US_1继续执行,若等于0,则不跳转(1T).
	"NOP             \n"  //1T,nop延时
	"DECW X          \n"  //1T,x--
	"JRNE DELAY_XUS  \n"  //不等于0,则跳转(2T)到DELAY_XUS继续执行,若等于0,则不跳转(1T).
	"POP A           \n"  //1T,出栈
	);
	
	EnableInterrupt();
}

static void delay_ms(u32 nms)
{
	u8 t;
	
	if (nms > 65)
		{
			t = nms / 65;
			while (t--) delay_us(65000);
			nms %= 65;
		}
	
	delay_us(nms*1000);
}

void SetSoftwarePriority(u8 vector, u8 level)
{
	u8 Mask = 0;
	u8 NewPriority = 0;
	
	Mask = (u8)(~(u8)(0x03U << ((vector % 4U) * 2U)));
	
	NewPriority = (u8)((level) << ((vector % 4U) * 2U));
	
	switch (vector)
	{
		case ITC_IRQ_TLI:
		case ITC_IRQ_AWU:
		case ITC_IRQ_CLK:
		case ITC_IRQ_PORTA:	
			ITC->ISPR1 &= Mask;
			ITC->ISPR1 |= NewPriority;
			break;
			
		case ITC_IRQ_PORTB:
		case ITC_IRQ_PORTC:
		case ITC_IRQ_PORTD:
		case ITC_IRQ_PORTE:
			ITC->ISPR2 &= Mask;
			ITC->ISPR2 |= NewPriority;
			break;
			
		case ITC_IRQ_SPI:
		case ITC_IRQ_TIM1_OVF:
			ITC->ISPR3 &= Mask;
			ITC->ISPR3 |= NewPriority;
			break;
			
		case ITC_IRQ_TIM1_CAPCOM:
		case ITC_IRQ_TIM2_OVF:
		case ITC_IRQ_TIM2_CAPCOM:
		case ITC_IRQ_TIM3_OVF:
			ITC->ISPR4 &= Mask;
			ITC->ISPR4 |= NewPriority;
			break;
			
		case ITC_IRQ_TIM3_CAPCOM:
		case ITC_IRQ_UART1_TX:
		case ITC_IRQ_UART1_RX:
		case ITC_IRQ_I2C:
			ITC->ISPR5 &= Mask;
			ITC->ISPR5 |= NewPriority;
			break;
			
		case ITC_IRQ_ADC1:
		case ITC_IRQ_TIM4_OVF:
			ITC->ISPR6 &= Mask;
			ITC->ISPR6 |= NewPriority;
			break;
			
		case ITC_IRQ_EEPROM_EEC:
			ITC->ISPR7 &= Mask;
			ITC->ISPR7 |= NewPriority;
			break;
			
		default:
			break;
	}
}

