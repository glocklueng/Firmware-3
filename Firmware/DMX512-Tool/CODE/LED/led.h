#ifndef _LED_H
#define _LED_H	 

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"

/*------------------------------*
*  macros for constants         *
*-------------------------------*/
#define   LED_ON    0
#define   LED_OFF   1

/*------------------------------*
*  macros for others            *
*-------------------------------*/
#define   LED_R   PAout(11)
#define   LED_G   PAout(8)
#define   LED_B   PCout(9)

#define   LED_RED()     {LED_R=LED_ON;LED_G=LED_OFF;LED_B=LED_OFF;}
#define   LED_GREEN()   {LED_R=LED_OFF;LED_G=LED_ON;LED_B=LED_OFF;}
#define   LED_BLUE()    {LED_R=LED_OFF;LED_G=LED_OFF;LED_B=LED_ON;}

/*------------------------------*
*  type definitions             *
*-------------------------------*/
typedef enum
{
	LED_STATUS_NORMAL = 0,
	LED_STATUS_LOW_VOLTAGE,
} LedStatusDef;

/*------------------------------*
*  public functions prototypes  *
*-------------------------------*/
extern void LED_init(void);
extern void led_status_switch(LedStatusDef status);
		 				    
#endif
