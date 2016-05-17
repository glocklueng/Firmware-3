#ifndef _USART_H
#define _USART_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"

/*------------------------------*
*  macros for constants         *
*-------------------------------*/
#define   DMX_START_DAT   0x00
#define   DMX_START_SET   0xAA

#define   _USART3_TX   PCout(12)=1
//#define   _USART3_RX   PCout(12)=0

#define   DMX_MODE_TX   0
#define   DMX_MODE_IO   1

#define   DMX_TX3_GPIO   PCout(10)

/*------------------------------*
*  public functions prototypes  *
*-------------------------------*/
#define   SBUF3   USART3->DR
extern void USART3_init(void);	
extern void SendUart3DMX(u8 *buff, u16 len);
extern void SendUart3Byte(u8 data);

#define   SBUF1   USART1->DR
extern void USART1_init(void);
extern void SendUart1(u8 *buff, u16 len);

#endif


