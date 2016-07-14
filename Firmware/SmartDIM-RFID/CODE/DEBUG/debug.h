#ifndef _DEBUG_H
#define _DEBUG_H

#include "sys.h"

#define   DEBUG_EN   0

#if DEBUG_EN > 0

#define   DEBUG_TX_PORT   GPIOD
#define   DEBUG_TX_PIN    GPIO_PIN_1
#define   DEBUG_TX        PDout(1)

#define   DEBUG_BAUD   9600
#define   BIT_DELAY_TIME    1000000UL/DEBUG_BAUD

extern void InitDebug();
extern void SendDebugByte(u8 data);
extern void SendDebugBuff(u8 *buff, u16 len);

#endif

#endif