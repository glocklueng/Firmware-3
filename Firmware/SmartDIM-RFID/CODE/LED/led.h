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
#define   LED_R   PDout(4)
#define   LED_G   PDout(3)

/*------------------------------*
*  public variables             *
*-------------------------------*/
extern TaskDef task_led;
		 				    
#endif
