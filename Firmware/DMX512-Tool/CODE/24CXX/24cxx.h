#ifndef _24CXX_H
#define _24CXX_H

/*------------------------------*
*  include head files           *
*-------------------------------*/
#include "i2c.h"   

/*------------------------------*
*  macros for constants         *
*-------------------------------*/
#define   AT24C01		 127
#define   AT24C02		 255
#define   AT24C04		 511
#define   AT24C08		 1023
#define   AT24C16		 2047
#define   AT24C32		 4095
#define   AT24C64	   8191
#define   AT24C128	 16383
#define   AT24C256   32767  

#define   EEPROM_ADDR_LANGUAGE   0x0010
#define   EEPROM_ADDR_MODE       0x0011
#define   EEPROM_ADDR_MODBUS     0x0012

/*------------------------------*
*  macros for others            *
*-------------------------------*/
//ʹ��24c02�����Զ���EE_TYPEΪAT24C02
#define   EE_TYPE   AT24C02

/*------------------------------*
*  public functions prototypes  *
*-------------------------------*/
#define   AT24CXX_init()   I2C_init()
extern u8 AT24CXX_ReadByte(u16 ReadAddr);							        //ָ����ַ��ȡһ���ֽ�
extern void AT24CXX_WriteByte(u16 WriteAddr, u8 DataToWrite);	//ָ����ַд��һ���ֽ�
extern void AT24CXX_Write(u16 WriteAddr, u16 NumToWrite, u8 *pBuffer);	//��ָ����ַ��ʼд��ָ�����ȵ�����
extern void AT24CXX_Read(u16 ReadAddr, u16 NumToRead, u8 *pBuffer);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

#endif
















