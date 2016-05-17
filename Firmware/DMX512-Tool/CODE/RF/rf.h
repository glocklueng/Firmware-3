#ifndef _RF_H
#define _RF_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "si446x.h"

/*------------------------------*
*  macros for constants         *
*-------------------------------*/
enum
{
	RF_CMD_ENABLE_TASK = 0xA1,
	RF_CMD_DISABLE_TASK,
};

/*------------------------------*
*  public variables             *
*-------------------------------*/
extern u8 rf_sendbuf[RF_SIZE];
extern u8 rf_recvbuf[RF_SIZE];

extern u8 recv_menu;
extern u8 recv_modbus;

/*------------------------------*
*  public functions prototypes  *
*-------------------------------*/
#define   RF_init()   SI446X_init()
extern void RF_SendPacket(u8 *Sendbuffer, u8 length);

/*------------------------------*
*  others for task *
*-------------------------------*/
extern OS_STK RF_TASK_STK[RF_STK_SIZE];
extern void rf_task(void *pdata);

#endif

