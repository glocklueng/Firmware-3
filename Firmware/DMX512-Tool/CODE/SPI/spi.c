/****************************************************                                                                                         
* Microcontroller: STM32F103RBT6
* Handware: JRMD1-V1.0
* File: spi.c                              
* Revision: 1.0  
* Description: SPI API
* Author: ValarHao
* Date：2015.12.24
****************************************************/
#include "spi.h"

#if _SPI1 > 0
void SPI1_init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //PORTA时钟使能 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE); //SPI1时钟使能 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

 	GPIO_SetBits(GPIOA, GPIO_Pin_7 | GPIO_Pin_6 | GPIO_Pin_5);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//字节模式
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//低电平空闲
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//第一个跳变沿数据有效
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  //软件NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;		//8分频
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//MSB在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
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
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); //检查指定的SPI标志位设置与否:发送缓存空标志位	  	
	SPI_I2S_SendData(SPI1, data); //通过外设SPIx发送一个数据
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位  						   	 
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据
}
#endif


#if _SPI2 > 0
void SPI2_init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //PORTB时钟使能 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,  ENABLE); //SPI2时钟使能 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

 	GPIO_SetBits(GPIOB, GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//字节模式
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//低电平空闲
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//第一个跳变沿数据有效
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  //软件NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//2分频
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//MSB在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
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

