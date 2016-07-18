/****************************************************                                                                                         
* Microcontroller: STM8S103F3
* Handware: JRD6-24-CD-4-V0
* File: mcu.c                              
* Revision: 1.0  
* Description: call InitDevice() initialize MCU
* Author: ValarHao
* Date：2015.12.28
****************************************************/
#include "mcu.h"
#include "uart.h"
#include "i2c_ds.h"
#include "i2c.h"
#include "power.h"

static void FLASH_init()
{
	FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);//Define flash programming Time  
	FLASH_Unlock(FLASH_MEMTYPE_DATA); //Unlock EEPROM
}

static void UART1_init()
{
	GPIO_Init(UART1_485_PORT, UART1_485_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	UART1_485_RX;
	
	//GPIO_Init(UART1_TX_PORT, UART1_TX_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	//GPIO_Init(UART1_RX_PORT, UART1_RX_PIN, GPIO_MODE_IN_PU_NO_IT);
	
	UART1_DeInit();	
	SetSoftwarePriority(ITC_IRQ_UART1_TX, ITC_PRIORITYLEVEL_1);
	SetSoftwarePriority(ITC_IRQ_UART1_RX, ITC_PRIORITYLEVEL_3);	
	UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, \
	UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
	CLEAR_UART1_IT_TC;
	UART1_ITConfig(UART1_IT_TC, ENABLE); 
	UART1_ITConfig(UART1_IT_RXNE, ENABLE);  
	UART1_Cmd(ENABLE);
}

/*static void KEY_init()
{
	GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT); //KEY4
	GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT); //KEY3
	GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT); //KEY2
	GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT); //KEY1
}*/

static void LED_init()
{
	GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST); //LED_R
	GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST); //LED_G
}

static void PWM_init()
{
#if PWM_CH==1
	TIM2_TimeBaseInit(TIM2_PRESCALER_512, 100);
	TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 0x40, TIM2_OCPOLARITY_HIGH);
	TIM2_OC1PreloadConfig(ENABLE);  
      
#elif PWM_CH==2
	TIM2_TimeBaseInit(TIM2_PRESCALER_512, 0xFF);
	TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 0x40, TIM2_OCPOLARITY_HIGH);
	TIM2_OC2PreloadConfig(ENABLE);
  
#elif PWM_CH==3
	TIM2_TimeBaseInit(TIM2_PRESCALER_512, 0xFF);
	TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 250, TIM2_OCPOLARITY_HIGH);
	TIM2_OC3PreloadConfig(ENABLE);
#endif
    
  TIM2_Cmd(ENABLE);
}

/*static void SPI_init()
{
	GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);
	PCout(4) = 1; //SPI Mode
	GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);
	PCout(3) = 1; //XCS
	GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST); //PWM
	
	SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_8, SPI_MODE_MASTER, \
					 SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, \
					 SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
	SPI_Cmd(ENABLE);
}*/

#if RTC_EN > 0
static void DS_I2C_init()
{
	DS_I2C_SDA_SetOut();
	DS_I2C_SCL_SetOut();
	DS_SDA_OUT = 0;
	DS_SCL_OUT = 0;
}
#endif

static void I2C_init()
{
	I2C_SDA_SetOut();
	I2C_SCL_SetOut();
	SDA_OUT = 0;
	SCL_OUT = 0;
}

void InitDevice()
{
	DisableInterrupt();
	
	CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); //时钟设置为内部16M
	FLASH_init();
	sys.init();
	UART1_init();
	//KEY_init();
	LED_init();
	PWM_init();
	//SPI_init();
	I2C_init();
	
#if RTC_EN > 0
	DS_I2C_init();
#endif

	GPIO_Init(GPIOA, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);
	
	EnableInterrupt();
}