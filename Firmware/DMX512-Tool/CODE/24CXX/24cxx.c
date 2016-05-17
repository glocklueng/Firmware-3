/****************************************************                                                                                         
* Microcontroller: STM32F103RBT6
* Handware: JRMD1-V1.0
* File: 24cxx.c                              
* Revision: 1.0  
* Description: at24c02 API
* Author: ValarHao
* Date：2015.12.24
****************************************************/
#include "24cxx.h" 

//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 AT24CXX_ReadByte(u16 ReadAddr)
{				  
	u8 temp = 0;		  	    																 
  I2C_Start();  
  
	if (EE_TYPE>AT24C16)
		{
			I2C_Send_Byte(0xA0);	   //发送写命令
			I2C_Wait_Ack();
			I2C_Send_Byte(ReadAddr>>8);//发送高地址
			I2C_Wait_Ack();		 
		}
	else 
		I2C_Send_Byte(0xA0+((ReadAddr/256)<<1));   //发送器件地址0xA0,写数据 	 

	I2C_Wait_Ack(); 
  I2C_Send_Byte(ReadAddr%256);   //发送低地址
	I2C_Wait_Ack();	    
	I2C_Start();  	 	   
	I2C_Send_Byte(0xA1);           //进入接收模式			   
	I2C_Wait_Ack();	 
  temp = I2C_Read_Byte(0);		   
  I2C_Stop(); //产生一个停止条件	    
	return temp;
}

//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteByte(u16 WriteAddr, u8 DataToWrite)
{				   	  	    																 
  I2C_Start();  
  
	if (EE_TYPE>AT24C16)
		{
			I2C_Send_Byte(0xA0);	    //发送写命令
			I2C_Wait_Ack();
			I2C_Send_Byte(WriteAddr>>8);//发送高地址
	 	}
 	else
		{
			I2C_Send_Byte(0xA0+((WriteAddr/256)<<1));   //发送器件地址0xA0,写数据 
		}	 
	
	I2C_Wait_Ack();	   
  I2C_Send_Byte(WriteAddr%256);   //发送低地址
	I2C_Wait_Ack(); 	 										  		   
	I2C_Send_Byte(DataToWrite);     //发送字节							   
	I2C_Wait_Ack();  		    	   
  I2C_Stop();//产生一个停止条件 
	OSTimeDly(10/OS_TICK); 
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr,  u16 NumToRead, u8 *pBuffer)
{
	while (NumToRead)
	{
		*pBuffer++ = AT24CXX_ReadByte(ReadAddr++);	
		NumToRead--;
	}
}  

//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void AT24CXX_Write(u16 WriteAddr, u16 NumToWrite, u8 *pBuffer)
{
	while (NumToWrite--)
	{
		AT24CXX_WriteByte(WriteAddr, *pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
 




