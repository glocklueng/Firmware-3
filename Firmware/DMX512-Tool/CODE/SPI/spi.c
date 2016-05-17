/****************************************************                                                                                         
* Microcontroller: STM32F103RBT6
* Handware: JRMD1-V1.0
* File: spi.c                              
* Revision: 1.0  
* Description: SPI API
* Author: ValarHao
* Date��2015.12.24
****************************************************/
#include "spi.h"

#if _SPI1 > 0
void SPI1_init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //PORTAʱ��ʹ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE); //SPI1ʱ��ʹ�� 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

 	GPIO_SetBits(GPIOA, GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//�ֽ�ģʽ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//�͵�ƽ����
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//��һ��������������Ч
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  //���NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//8��Ƶ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//MSB��ǰ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  
 
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
}    

//void SPI1_WriteByte(u8 data)
//{
//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)==RESET);
//	SPI_I2S_SendData(SPI1, data);
//}

//u8 SPI1_ReadByte(void)
//{
//	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)==RESET);
//	return SPI_I2S_ReceiveData(SPI1);
//}

u8 SPI1_ExchangeByte(u8 data)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //���ָ����SPI��־λ�������:���ͻ���ձ�־λ	  	
	SPI_I2S_SendData(SPI1, data); //ͨ������SPIx����һ������
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ  						   	 
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����
}
#endif


#if _SPI2 > 0
void SPI2_init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //PORTBʱ��ʹ�� 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,  ENABLE); //SPI2ʱ��ʹ�� 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

 	GPIO_SetBits(GPIOB, GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//�ֽ�ģʽ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//�͵�ƽ����
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//��һ��������������Ч
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  //���NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//2��Ƶ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//MSB��ǰ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  
 
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
}    

void SPI2_WriteByte(u8 data)
{
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI2, data);
}

//u8 SPI2_ReadByte(void)
//{
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)==RESET);
//	return SPI_I2S_ReceiveData(SPI2);
//}
#endif

