#ifndef _I2C_H
#define _I2C_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "sys.h"

/*------------------------------*
*  macros for others            *
*-------------------------------*/
#define   SDA_IN()    {GPIOB->CRH&=0xFFFF0FFF;GPIOB->CRH|=(u32)8<<12;}
#define   SDA_OUT()   {GPIOB->CRH&=0xFFFF0FFF;GPIOB->CRH|=(u32)3<<12;}

#define   I2C_SCL    PBout(10)
#define   I2C_SDA    PBout(11)
#define   READ_SDA   PBin(11)

/*------------------------------*
*  public functions prototypes  *
*-------------------------------*/
extern void I2C_init(void);		 
extern void I2C_Start(void);
extern void I2C_Stop(void);
extern void I2C_Send_Byte(u8 txd);
extern u8 I2C_Read_Byte(u8 ack);
extern u8 I2C_Wait_Ack(void);
extern void I2C_Ack(void);
extern void I2C_NAck(void);

extern void I2C_Write_One_Byte(u8 daddr, u8 addr, u8 data);
extern u8 I2C_Read_One_Byte(u8 daddr, u8 addr);	  

#endif
















