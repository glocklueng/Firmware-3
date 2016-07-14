#ifndef _SYS_H
#define _SYS_H	

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "stm8s.h"

/*------------------------------*
*  macros for configurations    *
*-------------------------------*/
#define   RTC_EN   1

/*------------------------------*
*  macros for constants         *
*-------------------------------*/
#define   BIG_ENDIAN // STM32F==>Ð¡¶Ë==>×¢ÊÍ STM8S==>´ó¶Ë==>²»×¢ÊÍ
#define   SYSCLK   16

/*------------------------------*
*  macros for others            *
*-------------------------------*/
#define   BIT_ADDR(addr, bitnum)   (*((volatile BYTE_BITS *)addr)).bit##bitnum

#define   GPIOA_ODR_ADDR   GPIOA_BaseAddress
#define   GPIOB_ODR_ADDR   GPIOB_BaseAddress
#define   GPIOC_ODR_ADDR   GPIOC_BaseAddress
#define   GPIOD_ODR_ADDR   GPIOD_BaseAddress

#define   GPIOA_IDR_ADDR   GPIOA_BaseAddress+1
#define   GPIOB_IDR_ADDR   GPIOB_BaseAddress+1
#define   GPIOC_IDR_ADDR   GPIOC_BaseAddress+1
#define   GPIOD_IDR_ADDR   GPIOD_BaseAddress+1

#define   PAout(n)   BIT_ADDR(GPIOA_ODR_ADDR, n)
#define   PAin(n)    BIT_ADDR(GPIOA_IDR_ADDR, n)
#define   PBout(n)   BIT_ADDR(GPIOB_ODR_ADDR, n)
#define   PBin(n)    BIT_ADDR(GPIOB_IDR_ADDR, n)
#define   PCout(n)   BIT_ADDR(GPIOC_ODR_ADDR, n)
#define   PCin(n)    BIT_ADDR(GPIOC_IDR_ADDR, n)
#define   PDout(n)   BIT_ADDR(GPIOD_ODR_ADDR, n)
#define   PDin(n)    BIT_ADDR(GPIOD_IDR_ADDR, n)

#ifdef BIG_ENDIAN
#define   HIGH(val)         *((char *) &val)
#define   LOW(val)         *(((char *) &val) + 1)
#define   Hxxx_long(val)    *((char *) &val)
#define   xHxx_long(val)   *(((char *) &val) + 1)
#define   xxHx_long(val)   *(((char *) &val) + 2)
#define   xxxH_long(val)   *(((char *) &val) + 3)
#else
#define   HIGH(val)        *(((char *) &val) + 1)
#define   LOW(val)          *((char *) &val)
#define   Hxxx_long(val)   *(((char *) &val) + 3)
#define   xHxx_long(val)   *(((char *) &val) + 2)
#define   xxHx_long(val)   *(((char *) &val) + 1)
#define   xxxH_long(val)    *((char *) &val)
#endif

#define   NULL     ((void *)0)
#define   BIT(i)   (1<<i)

#define   ADDR_DATA(addr)   *((u8 *)(addr))
#define   TYPE_CONVERSION(type, addr)   *((type *)(addr))

/*------------------------------*
*  macros for functions         *
*-------------------------------*/
#define   CLEAR_TIM4_IT_OVF   TIM4->SR1 &= ~BIT(0)

#define   EnableInterrupt()    asm("rim")
#define   DisableInterrupt()   asm("sim")

#define   task_check(task_name)   { \
	p_task = &task_name;   \
	 \
	if (p_task->enable==0) \
		return; \
	 \
	if (p_task->suspend==1) \
		{ \
			if (sys.tick < p_task->tick) return; \
			p_task->suspend = 0; \
		} \
}

#define   task_wait(t_tick)   { \
	p_task->tick = sys.tick + t_tick; \
	p_task->suspend = 1; \
}

#define   task_enable(task_name)    { \
	task_name.enable = 1; \
}

#define   task_disable(task_name)   { \
	task_name.enable = 0; \
}

/*------------------------------*
*  type definitions             *
*-------------------------------*/
typedef struct
{
	u32 tick;
	void (*init)();
	void (*delay_us)(u16);
	void (*delay_ms)(u32);
} SysDef;

typedef struct
{
	u8 enable;
	u8 suspend;
	u32 tick;	
	void (*process)();
} TaskDef;

typedef struct
{
  u8 bit0: 1;
  u8 bit1: 1;
  u8 bit2: 1;
  u8 bit3: 1;
  u8 bit4: 1;
  u8 bit5: 1;
  u8 bit6: 1;
  u8 bit7: 1;
} BYTE_BITS;

/*------------------------------*
*  public variables             *
*-------------------------------*/
extern SysDef sys;
extern TaskDef *p_task;

/*------------------------------*
*  public functions prototypes  *
*-------------------------------*/
extern void SetSoftwarePriority(u8 vector, u8 level);

#endif
