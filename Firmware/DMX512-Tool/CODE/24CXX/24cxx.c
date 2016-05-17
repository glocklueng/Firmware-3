/****************************************************                                                                                         
* Microcontroller: STM32F103RBT6
* Handware: JRMD1-V1.0
* File: 24cxx.c                              
* Revision: 1.0  
* Description: at24c02 API
* Author: ValarHao
* Date��2015.12.24
****************************************************/
#include "24cxx.h" 

//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
u8 AT24CXX_ReadByte(u16 ReadAddr)
{				  
	u8 temp = 0;		  	    																 
  I2C_Start();  
  
	if (EE_TYPE>AT24C16)
		{
			I2C_Send_Byte(0xA0);	   //����д����
			I2C_Wait_Ack();
			I2C_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ
			I2C_Wait_Ack();		 
		}
	else 
		I2C_Send_Byte(0xA0+((ReadAddr/256)<<1));   //����������ַ0xA0,д���� 	 

	I2C_Wait_Ack(); 
  I2C_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	I2C_Wait_Ack();	    
	I2C_Start();  	 	   
	I2C_Send_Byte(0xA1);           //�������ģʽ			   
	I2C_Wait_Ack();	 
  temp = I2C_Read_Byte(0);		   
  I2C_Stop(); //����һ��ֹͣ����	    
	return temp;
}

//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteByte(u16 WriteAddr, u8 DataToWrite)
{				   	  	    																 
  I2C_Start();  
  
	if (EE_TYPE>AT24C16)
		{
			I2C_Send_Byte(0xA0);	    //����д����
			I2C_Wait_Ack();
			I2C_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ
	 	}
 	else
		{
			I2C_Send_Byte(0xA0+((WriteAddr/256)<<1));   //����������ַ0xA0,д���� 
		}	 
	
	I2C_Wait_Ack();	   
  I2C_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	I2C_Wait_Ack(); 	 										  		   
	I2C_Send_Byte(DataToWrite);     //�����ֽ�							   
	I2C_Wait_Ack();  		    	   
  I2C_Stop();//����һ��ֹͣ���� 
	OSTimeDly(10/OS_TICK); 
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr,  u16 NumToRead, u8 *pBuffer)
{
	while (NumToRead)
	{
		*pBuffer++ = AT24CXX_ReadByte(ReadAddr++);	
		NumToRead--;
	}
}  

//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u16 WriteAddr, u16 NumToWrite, u8 *pBuffer)
{
	while (NumToWrite--)
	{
		AT24CXX_WriteByte(WriteAddr, *pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
 




