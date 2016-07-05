#ifndef _I2C_DS_H
#define _I2C_DS_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"

/*------------------------------*
*  macros for configurations    *
*-------------------------------*/
#define   DS_I2C_SCL_SetOut()   GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_OUT_OD_HIZ_FAST)
#define   DS_I2C_SDA_SetOut()   GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_FAST)
#define   DS_I2C_SDA_SetIn()    GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT)

#define   DS_SCL_OUT   PBout(4)
#define   DS_SDA_OUT   PBout(5)
#define   DS_SDA_IN    PBin(5)

/*------------------------------*
*  public functions             *
*-------------------------------*/
extern void DS_I2C_Start();
extern void DS_I2C_Stop();
extern void DS_I2C_SendByte(u8 data);
extern u8 DS_I2C_ReceiveByte();
extern void DS_I2C_Ack(u8 ack);

#endif