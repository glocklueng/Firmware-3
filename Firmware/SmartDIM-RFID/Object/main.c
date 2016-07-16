#include "mcu.h"
#include "uart.h"
#include "led.h"
#include "uart.h"
#include "debug.h"
#include "ds3231.h"
#include "s4011.h"
#include "flash.h"
#include "power.h"

//#define   KEY1   PCin(3)
//#define   KEY2   PCin(4)
//#define   KEY3   PCin(6)
//#define   KEY4   PCin(7)

void main(void)
{	
	InitDevice();	
	
#if DEBUG_EN > 0
	InitDebug();
#endif
	
	init_eeprom_paraments();
	
	LED_R = LED_OFF;
  LED_G = LED_OFF;
  
#ifdef UART1_485_RX
	UART1_485_RX;
#endif

	InitPower();
	
  while (1)
  {
  	task_led.process();
  	task_power.process();
  	Uart1Rx.process();
  }  
}

