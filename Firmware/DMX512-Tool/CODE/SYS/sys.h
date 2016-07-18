#ifndef _SYS_H
#define _SYS_H	

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "stm32f10x.h"
#include "includes.h"

/*------------------------------*
*  macros for configurations    *
*-------------------------------*/
#define   WIRELESS_EN     1
#define   MENU_EXTRA_EN   0

/*------------------------------*
*  macros for constants         *
*-------------------------------*/
//#define   BIG_ENDIAN // STM32F==>Ð¡¶Ë==>×¢ÊÍ STM8S==>´ó¶Ë==>²»×¢ÊÍ
#define   SYSCLK   72

/*------------------------------*
*  macros for others            *
*-------------------------------*/
#define   BITBAND(addr, bitnum)    ((addr&0xF0000000)+0x2000000+((addr&0xFFFFF)<<5)+(bitnum<<2)) 
#define   MEM_ADDR(addr)           *((volatile unsigned long *)(addr)) 
#define   BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define   GPIOA_ODR_Addr   (GPIOA_BASE+12) //0x4001080C 
#define   GPIOB_ODR_Addr   (GPIOB_BASE+12) //0x40010C0C 
#define   GPIOC_ODR_Addr   (GPIOC_BASE+12) //0x4001100C 
#define   GPIOD_ODR_Addr   (GPIOD_BASE+12) //0x4001140C 
#define   GPIOE_ODR_Addr   (GPIOE_BASE+12) //0x4001180C 
#define   GPIOF_ODR_Addr   (GPIOF_BASE+12) //0x40011A0C    
#define   GPIOG_ODR_Addr   (GPIOG_BASE+12) //0x40011E0C    
#define   GPIOA_IDR_Addr   (GPIOA_BASE+8)  //0x40010808 
#define   GPIOB_IDR_Addr   (GPIOB_BASE+8)  //0x40010C08 
#define   GPIOC_IDR_Addr   (GPIOC_BASE+8)  //0x40011008 
#define   GPIOD_IDR_Addr   (GPIOD_BASE+8)  //0x40011408 
#define   GPIOE_IDR_Addr   (GPIOE_BASE+8)  //0x40011808 
#define   GPIOF_IDR_Addr   (GPIOF_BASE+8)  //0x40011A08 
#define   GPIOG_IDR_Addr   (GPIOG_BASE+8)  //0x40011E08 

#define   PAout(n)   BIT_ADDR(GPIOA_ODR_Addr, n)
#define   PAin(n)    BIT_ADDR(GPIOA_IDR_Addr, n)
#define   PBout(n)   BIT_ADDR(GPIOB_ODR_Addr, n)
#define   PBin(n)    BIT_ADDR(GPIOB_IDR_Addr, n)
#define   PCout(n)   BIT_ADDR(GPIOC_ODR_Addr, n)
#define   PCin(n)    BIT_ADDR(GPIOC_IDR_Addr, n)
#define   PDout(n)   BIT_ADDR(GPIOD_ODR_Addr, n)
#define   PDin(n)    BIT_ADDR(GPIOD_IDR_Addr, n)
#define   PEout(n)   BIT_ADDR(GPIOE_ODR_Addr, n)
#define   PEin(n)    BIT_ADDR(GPIOE_IDR_Addr, n)
#define   PFout(n)   BIT_ADDR(GPIOF_ODR_Addr, n)
#define   PFin(n)    BIT_ADDR(GPIOF_IDR_Addr, n)
#define   PGout(n)   BIT_ADDR(GPIOG_ODR_Addr, n)
#define   PGin(n)    BIT_ADDR(GPIOG_IDR_Addr, n)

#ifdef BIG_ENDIAN
#define   HIGH(val)        *((char *) &val)
#define   LOW(val)         *(((char *) &val) + 1)
#define   Hxxx_long(val)   *((char *) &val)
#define   xHxx_long(val)   *(((char *) &val) + 1)
#define   xxHx_long(val)   *(((char *) &val) + 2)
#define   xxxH_long(val)   *(((char *) &val) + 3)
#else
#define   HIGH(val)        *(((char *) &val) + 1)
#define   LOW(val)         *((char *) &val)
#define   Hxxx_long(val)   *(((char *) &val) + 3)
#define   xHxx_long(val)   *(((char *) &val) + 2)
#define   xxHx_long(val)   *(((char *) &val) + 1)
#define   xxxH_long(val)   *((char *) &val)
#endif

#ifndef   TRUE
#define   TRUE    1
#endif
#ifndef   FALSE
#define   FALSE   0
#endif
#ifndef   NULL
#define   NULL   (void *)0
#endif

#define   OS_TICK    2
#define   TICK_1US   (SYSCLK/8)
#define   LOAD_VAL   TICK_1US*1000*OS_TICK
#define   BIT(i)     (1<<i)

/*------------------------------*
*  macros for functions         *
*-------------------------------*/
#define   EnableInterrupt()    __ASM volatile("cpsie i")
#define   DisableInterrupt()   __ASM volatile("cpsid i")

/*------------------------------*
*  public functions prototypes  *
*-------------------------------*/
extern void SysTick_init(void);
extern void delay_us(u32 nus);

/*------------------------------*
*  priority for task            *
*-------------------------------*/
#define   START_TASK_PRIO   10
#define   ADC_TASK_PRIO     7
#define   GUI_TASK_PRIO     6
#define   LCD_TASK_PRIO     5
#define   RF_TASK_PRIO      4
#define   KEY_TASK_PRIO     3

/*------------------------------*
*  stack for task               *
*-------------------------------*/
#define   START_STK_SIZE  	64
#define   ADC_STK_SIZE  	  64
#define   GUI_STK_SIZE  	  64
#define   LCD_STK_SIZE  	  64
#define   RF_STK_SIZE  	    64
#define   KEY_STK_SIZE  	  64

#endif
