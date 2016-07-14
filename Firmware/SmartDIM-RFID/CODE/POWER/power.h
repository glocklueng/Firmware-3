#ifndef _POWER_H
#define _POWER_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"
#include "stm8s_tim2.h"

/*------------------------------*
*  macros for configurations    *
*-------------------------------*/
#define   PWM_CH    1
#define   PWM_VAL   TIM2->CCR1L

#define   PWM_INITIAL   20

#define   PWM_OUT_FILL()       TIM2->CCMR1 = 0x58 //强制为高电平
#define   PWM_OUT_NOT_FILL()   TIM2->CCMR1 = 0x68 //切换回PWM模式1

/*------------------------------*
*  public functions             *
*-------------------------------*/
extern void InitPower();

/*------------------------------*
*  public variables             *
*-------------------------------*/
extern TaskDef task_power;

#endif