#ifndef _ADC_H
#define _ADC_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"

/*------------------------------*
*  macros for constants         *
*-------------------------------*/
#define   ADC_CH   1

#define   ADC_BUFF_SIZE_BIT_CNT   3
#define   ADC_BUFF_SIZE_INVALID   2
#define   ADC_BUFF_SIZE   ((1<<ADC_BUFF_SIZE_BIT_CNT)+(ADC_BUFF_SIZE_INVALID*2))

/*------------------------------*
*  public variable              *
*-------------------------------*/
extern u16 voltage;

/*------------------------------*
*  others for task *
*-------------------------------*/
extern OS_STK ADC_TASK_STK[ADC_STK_SIZE];
extern void adc_task(void *pdata);

#endif

