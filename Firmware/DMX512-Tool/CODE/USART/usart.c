/****************************************************                                                                                         
* Microcontroller: STM32F103RBT6
* Handware: JRMD1-V1.0
* File: usart.c                              
* Revision: 1.0  
* Description: USART API
* Author: ValarHao
* Date：2015.12.24
****************************************************/
#include "usart.h"	  
#include "sys.h"

static DMA_InitTypeDef DMA_USART3_InitStructure;

void USART3_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;	
  
  //USART3不完全映射
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 	
  GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);  
  //USART3_TX   GPIOC.10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	//USART3_RX	  GPIOC.11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
   
  //USART3_RD   485引脚   GPIOC.12
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//DMX串口配置初始化 250000bps 2stop bit
	USART_InitStructure.USART_BaudRate = 250000;	
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART3, &USART_InitStructure); //初始化串口3
  
  //DMA初始化
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel2);
	DMA_USART3_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;
  DMA_USART3_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; //外设作为DMA的目的端
  DMA_USART3_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址不增加
  DMA_USART3_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址自增1
  DMA_USART3_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_USART3_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_USART3_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Normal（只传送一次）, DMA_Mode_Circular （不停地传送）
  DMA_USART3_InitStructure.DMA_Priority = DMA_Priority_Medium; //(DMA传送优先级为中等)
  DMA_USART3_InitStructure.DMA_M2M = DMA_M2M_Disable;
  
  //DMA1_Channel2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);  //全传输中断
  
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
  
	//USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接收中断
  USART_Cmd(USART3, ENABLE);                    //使能串口3 
}

static void SendUart3(u8 *buff, u16 len)
{	
	DMA_USART3_InitStructure.DMA_MemoryBaseAddr = (u32)buff;
  DMA_USART3_InitStructure.DMA_BufferSize = len;
  DMA_Init(DMA1_Channel2, &DMA_USART3_InitStructure);  
  DMA_Cmd(DMA1_Channel2, ENABLE);
}

void DMA1_Channel2_IRQHandler(void)
{
  if (DMA_GetITStatus(DMA1_IT_TC2))
	  {
	    DMA_Cmd(DMA1_Channel2, DISABLE);
	    DMA_ClearITPendingBit(DMA1_IT_GL2);	    
	  }
}

static void USART3_DMX_Mode(u8 mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;
   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
	if (mode==DMX_MODE_TX)
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			
		}
	else
		{
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		}
	
	//USART3_TX   GPIOC.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void SendUart3DMX(u8 *buff, u16 len)
{	
	_USART3_TX;
	USART3_DMX_Mode(DMX_MODE_IO);
	DMX_TX3_GPIO = 0;
	delay_us(150);
	DMX_TX3_GPIO = 1;
	delay_us(8); //12us 因IO状态切换需要耗时所以延时8us即可
	USART3_DMX_Mode(DMX_MODE_TX);
	SendUart3(buff, len);
}

void SendUart3Byte(u8 data)
{
	_USART3_TX;
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET);
	SBUF3 = (u16)data;
}


void USART1_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
  //USART1_RX	  GPIOA.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器 

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接收中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 
}

void SendUart1(u8 *buff, u16 len)
{
	u16 i;	

	for (i=0; i<len; i++)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
		SBUF1 = (u16)buff[i];
	}
	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);	
}
