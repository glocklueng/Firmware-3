#ifndef _UART_H
#define _UART_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"
#include "stm8s_uart1.h"

/*------------------------------*
*  macros for configurations    *
*-------------------------------*/
#define   UART1_TX_BUFF_SIZE   160
#define   UART1_RX_BUFF_SIZE   160

//UART1_485
#define   UART1_485_PORT   GPIOA
#define   UART1_485_PIN    GPIO_PIN_1
#define   UART1_485        PAout(1)

#define   UART1_485_TX    UART1_485=1
#define   UART1_485_RX    UART1_485=0

//UART1_TX
#define   UART1_TX_PORT   GPIOD
#define   UART1_TX_PIN    GPIO_PIN_5
#define   UART1_TX_GPIO   PDout(5)

//UART1_RX
#define   UART1_RX_PORT   GPIOD
#define   UART1_RX_PIN    GPIO_PIN_6
#define   UART1_RX_GPIO   PDin(6)

/*------------------------------*
*  macros for const             *
*-------------------------------*/
#define   UART1_TXHDR1   0xA7
#define   UART1_TXHDR2   0xB8

#define   UART1_RXHDR1   0xC9
#define   UART1_RXHDR2   0xDA

#define   CMD_START_TIME    0xA0   //上电后10%至对应功率档的启动时间
#define   CMD_CHANGE_TIME   0xA1   //渐变时间
#define   CMD_SET_CLK       0xA2   //设置时钟
#define   CMD_READ_CLK      0xA3   //读取时钟
#define   CMD_READ_SET      0xA4   //读取状态
#define   CMD_DAYS          0xA5   //每天固定时间点调光
#define   CMD_YEARLY        0xA6   //年度调光
#define   CMD_SEASON        0xA7   //季节调光
#define   CMD_MONTHLY       0xA8   //月份调光

/*------------------------------*
*  macros for functions         *
*-------------------------------*/
#define   CLEAR_UART1_IT_TC   UART1->SR &= ~BIT(6)

/*------------------------------*
*  type definitions             *
*-------------------------------*/
typedef struct
{
	u8 *buff;
	u16 cnt;
	u16 size;	
	u8 busy;
	void (*send)(u16);
} UartTxDef;

typedef struct
{
	u8 *buff;
	u16 cnt;
	u16 size;	
	u8 status;
	u8 flag;
	void (*process)(void);
} UartRxDef;

/*------------------------------*
*  public variables             *
*-------------------------------*/
extern UartTxDef Uart1Tx;
extern UartRxDef Uart1Rx;

#endif