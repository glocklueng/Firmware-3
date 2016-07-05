#ifndef _I2C_H
#define _I2C_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"

/*------------------------------*
*  macros for configurations    *
*-------------------------------*/
#define   I2C_SCL_SetOut()   GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_OD_HIZ_FAST)
#define   I2C_SDA_SetOut()   GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_OD_HIZ_FAST)
#define   I2C_SDA_SetIn()    GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT)

#define   SCL_OUT   PCout(3)
#define   SDA_OUT   PCout(4)
#define   SDA_IN    PCin(4)

/*------------------------------*
*  public functions             *
*-------------------------------*/
extern void I2C_Start();
extern void I2C_Stop();
extern void I2C_SendByte(u8 data);
extern u8 I2C_ReceiveByte();
extern void I2C_Ack(u8 ack);

#endif