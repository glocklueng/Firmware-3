/****************************************************                                                                                         
* Microcontroller: STM32F103RBT6
* Handware: JRMD1-V1.0
* File: power.c                              
* Revision: 1.0  
* Description: POWER API
* Author: ValarHao
* Date��2015.12.24
****************************************************/
#include "adc.h"
#include "gui.h"
#include "lcd.h"
#include "led.h"
#include "power.h"

OS_STK ADC_TASK_STK[ADC_STK_SIZE];

static u16 adc_buff[ADC_BUFF_SIZE];
static u8 adc_cnt = 0;
static u16 adc_sum;

u16 voltage;

static void ADC_init(void)
{ 		
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	//PWR_AD                
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	ADC_DeInit(ADC1);	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);  		
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);	//����ָ����ADC1��У׼�Ĵ���
	while(ADC_GetResetCalibrationStatus(ADC1));	//��ȡADC1����У׼�Ĵ�����״̬,����״̬��ȴ�
	ADC_StartCalibration(ADC1);		//��ʼָ��ADC1��У׼״̬	
	while(ADC_GetCalibrationStatus(ADC1));		//��ȡָ��ADC1��У׼����,����״̬��ȴ�
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������
}				
  
static u16 GetAdc(u8 ch)   
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,�������˳��ֵΪ1,����ʱ��Ϊ239.5����	  			     
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ת��	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	return ADC_GetConversionValue(ADC1);
}

static void SortAdcBuff(u16 *buff, u8 n)
{
	u16 i, j, temp;
	
	for (i=0; i<n-1; i++)
	{
		for (j=0; j<n-1-i; j++)
		{
			if (buff[j]>buff[j+1])
				{
					temp = buff[j];
					buff[j] = buff[j+1];
					buff[j+1] = temp;
				}
		}
	}
}

void adc_task(void *pdata)
{
	u8 i;
	u32 t;
	
	//USART1_init();
	ADC_init();
	
	while (1)
	{
		adc_buff[adc_cnt] = GetAdc(ADC_CH);
		
		if (++adc_cnt==ADC_BUFF_SIZE)
			{
				adc_cnt = 0;
				
				SortAdcBuff(adc_buff, ADC_BUFF_SIZE);
				
				adc_sum = 0;
				
				for (i=ADC_BUFF_SIZE_INVALID; i<(ADC_BUFF_SIZE-ADC_BUFF_SIZE_INVALID); i++)
				{
					adc_sum += adc_buff[i];
				}
				
				adc_sum >>= ADC_BUFF_SIZE_BIT_CNT;
				
				voltage = (((u32)(adc_sum * 330) >> 12) << 1); 
				
				if (menu->index == MENU_INDEX_SYSTEM)
					{
						LCD_Display_Num(3, 11, voltage%1000/100);
						LCD_Display_Num(3, 13, voltage%100/10);
						LCD_Display_Num(3, 14, voltage%10);
					}				
				
				if (voltage < 335)
					{
						GUI_display_shutdown();
						OSTimeDly(12/OS_TICK);
						OSRunning = FALSE;
						for (t=0; t<16000000UL; t++);
						POWER_EN = 0;
					}
				
//				SendUart1((u8 *)&voltage, 1);
//				OSTimeDly(1);
//				SendUart1((u8 *)&adc_sum+1, 1);
//				OSTimeDly(1);
//				SendUart1((u8 *)&adc_sum, 1);		
//				OSTimeDly(1000/OS_TICK);
			}
		
		OSTimeDly(100/OS_TICK);
	}
}
