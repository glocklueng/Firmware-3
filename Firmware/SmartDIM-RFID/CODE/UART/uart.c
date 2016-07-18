/****************************************************                                                                                         
* Microcontroller: STM8S103F3
* Handware: JRW-C-T-V2.0
* File: uart.c                              
* Revision: 1.0  
* Description: UART API
* Author: ValarHao
* Date：2016.6.1
****************************************************/
#include "uart.h"
#include "led.h"
#include "ds3231.h"
#include "flash.h"

static void SendUart1(u16 len);
//static void SendUart1DMX(u16 len);
static void Uart1Process();

static u8 uart1_tx_buff[UART1_TX_BUFF_SIZE];
static u8 uart1_rx_buff[UART1_RX_BUFF_SIZE];

UartTxDef Uart1Tx = { uart1_tx_buff, 0, 0, 0, SendUart1 };
UartRxDef Uart1Rx = { uart1_rx_buff, 0, 0, 0, 0, Uart1Process };

static u8 crc16_state;

static void SendUart1(u16 len)
{
	UART1_485_TX;
	Uart1Tx.size = len;
	Uart1Tx.cnt = 1;
	Uart1Tx.busy = 1;
	UART1->DR = Uart1Tx.buff[0];	
}

//static void SendUart1DMX(u16 len)
//{
//	UART1_Cmd(DISABLE);
//	UART1_TX_GPIO = 0;
//	sys.delay_us(150);
//	UART1_TX_GPIO = 1;
//	UART1_Cmd(ENABLE);
//	SendUart1(len);
//}

INTERRUPT_HANDLER(UART1_TX_IRQHandler, ITC_IRQ_UART1_TX)
{
	CLEAR_UART1_IT_TC;
	
	if (Uart1Tx.busy)
		{
			if (--Uart1Tx.size != 0)
				{
					UART1->DR = Uart1Tx.buff[Uart1Tx.cnt++];
				}
			else
				{
					UART1_485_RX;
					Uart1Tx.busy = 0;
				}
		}
}

INTERRUPT_HANDLER(UART1_RX_IRQHandler, ITC_IRQ_UART1_RX)
{
	u8 temp;
	
	temp = UART1->DR;
	
	//LED_R ^= 1;  //红灯闪烁
	
	switch (Uart1Rx.status)
	{
		default:
			
		case 0: //帧首1
			if (temp==UART1_RXHDR1) Uart1Rx.status = 1;
			break;
		
		case 1: //帧首2
			if (temp==UART1_RXHDR2) Uart1Rx.status = 2;
			else Uart1Rx.status = 0;
			break;
		
		case 2:
			Uart1Rx.buff[0] = temp;
			
			if ((temp==CMD_READ_CLK) || (temp==CMD_READ_SET))
				{					
					Uart1Rx.size = 2; //收两个字节的CRC
					Uart1Rx.cnt = 1;
					Uart1Rx.status = 4;
				}
			else
				{
					Uart1Rx.status = 3;
				}			
			break;
		
		case 3:
			Uart1Rx.size = temp + 2;   // n + 2crc
			Uart1Rx.cnt = 1;
			Uart1Rx.status = 4;
			break;
		
		case 4:
			Uart1Rx.buff[Uart1Rx.cnt++] = temp;
			
			if (--Uart1Rx.size==0)
				{
					Uart1Rx.flag = 1;
					crc16_state = 1;
					Uart1Rx.status = 0;
				}
			break;
	}
}

static void Uart1Process()
{
	if (Uart1Rx.flag)
		{
			Uart1Rx.flag = 0;
			
			//LED_R = LED_OFF;   //收完数据灭红灯
			
			//rxbuf[]格式：CMD DATA ... DATA
			
			switch (Uart1Rx.buff[0])
			{	
				// PC: C9 DA A0 01 05 00 00
				//MCU: A7 B8 A0 01 00 00
				case CMD_START_TIME:
					{
						EEPROM_DATA_START_TIME = Uart1Rx.buff[1];
					
						Uart1Tx.buff[0] = UART1_TXHDR1;
						Uart1Tx.buff[1] = UART1_TXHDR2;
						Uart1Tx.buff[2] = CMD_START_TIME;
						Uart1Tx.buff[3] = crc16_state;
						Uart1Tx.buff[4] = 0x00;
						Uart1Tx.buff[5] = 0x00;						
						Uart1Tx.send(6);
					}										
					break;
				
				// PC: C9 DA A1 01 02 00 00
				//MCU: A7 B8 A1 01 00 00
				case CMD_CHANGE_TIME:
					{
						EEPROM_DATA_CHANGE_TIME = Uart1Rx.buff[1];
					
						Uart1Tx.buff[0] = UART1_TXHDR1;
						Uart1Tx.buff[1] = UART1_TXHDR2;
						Uart1Tx.buff[2] = CMD_CHANGE_TIME;
						Uart1Tx.buff[3] = crc16_state;
						Uart1Tx.buff[4] = 0x00;
						Uart1Tx.buff[5] = 0x00;
						Uart1Tx.send(6);
					}										
					break;
					
#if RTC_EN > 0
				// PC: C9 DA A2 07 0E 09 11 04 10 14 00 00 00
				//MCU: A7 B8 A2 01 00 00
				case CMD_SET_CLK:
					{
						for (u8 i=0; i<7; i++)
						{
							time.buff[i] = Uart1Rx.buff[i+1];
						}
						
						DS3231_SetTime(&time);
						
						Uart1Tx.buff[0] = UART1_TXHDR1;
						Uart1Tx.buff[1] = UART1_TXHDR2;
						Uart1Tx.buff[2] = CMD_SET_CLK;
						Uart1Tx.buff[3] = crc16_state;
						Uart1Tx.buff[4] = 0x00;
						Uart1Tx.buff[5] = 0x00;						
						Uart1Tx.send(6);
					}							
					break;
				
				// PC: C9 DA A3 00 00
				//MCU: A7 B8 A3 07 0F 09 16 02 0A 00 00 00 00
				case CMD_READ_CLK:
					{
						DS3231_ReadTime(&time);
					
						Uart1Tx.buff[0] = UART1_TXHDR1;
						Uart1Tx.buff[1] = UART1_TXHDR2;
						Uart1Tx.buff[2] = CMD_READ_CLK;
						Uart1Tx.buff[3] = 0x07;
						
						for (u8 i=0; i<7; i++)
						{
							Uart1Tx.buff[i+4] = time.buff[i];
						}
						
						Uart1Tx.buff[11] = 0x00;
						Uart1Tx.buff[12] = 0x00;						
						Uart1Tx.send(13);
					}				
					break;
#endif				

				// PC: C9 DA A4 00 00
				//MCU:不定长
				case CMD_READ_SET:
					{
						Uart1Tx.buff[0] = UART1_TXHDR1;
						Uart1Tx.buff[1] = UART1_TXHDR2;
						Uart1Tx.buff[2] = CMD_READ_SET;
						//Uart1Tx.buff[3]数据长度在switch里设置
						Uart1Tx.buff[4] = EEPROM_DATA_START_TIME;
						Uart1Tx.buff[5] = EEPROM_DATA_CHANGE_TIME;
						
						DS3231_ReadTime(&time);
						for (u8 i=0; i<7; i++)
						{
							Uart1Tx.buff[i+6] = time.buff[i];
						}
						
						Uart1Tx.buff[13] = EEPROM_DATA_MODE;
						switch (EEPROM_DATA_MODE)
						{
							case MODE_YEARLY:
								{
									Uart1Tx.buff[3] = 22; 
									for (u8 i=0; i<12; i++)
									{
										Uart1Tx.buff[i+14] = *((u8 *)(EEPROM_ADDR_YEARLY + i));
									}
									Uart1Tx.buff[26] = 0x00;
									Uart1Tx.buff[27] = 0x00;
									Uart1Tx.send(28);
								}
								break;
							
#if RTC_EN > 0
							case MODE_DAYS:
								{
									Uart1Tx.buff[3] = 16;   //LEN-4-2
									for (u8 i=0; i<6; i++)
									{
										Uart1Tx.buff[i+14] = *((u8 *)(EEPROM_ADDR_DAYS + i));
									}
									Uart1Tx.buff[20] = 0x00;
									Uart1Tx.buff[21] = 0x00;
									Uart1Tx.send(22);
								}
								break;
							
							case MODE_SEASON:
								{
									Uart1Tx.buff[3] = 34; 
									for (u8 i=0; i<52; i++)
									{
										Uart1Tx.buff[i+14] = *((u8 *)(EEPROM_ADDR_SEASON + i));
									}
									Uart1Tx.buff[66] = 0x00;
									Uart1Tx.buff[67] = 0x00;
									Uart1Tx.send(68);
								}
								break;
							
							case MODE_MONTHLY:
								{
									Uart1Tx.buff[3] = 58; 
									for (u8 i=0; i<144; i++)
									{
										Uart1Tx.buff[i+14] = *((u8 *)(EEPROM_ADDR_MONTHLY + i));
									}
									Uart1Tx.buff[158] = 0x00;
									Uart1Tx.buff[159] = 0x00;
									Uart1Tx.send(160);
								}
								break;
#endif
						}
					}		
					break;
				
				// PC: C9 DA A6 0C 64 00 0E 00 00
				//                |---4---|
				//MCU: A7 B8 A6 01 00 00
				case CMD_YEARLY:
					{
						EEPROM_DATA_MODE = MODE_YEARLY;
						
						for (u8 i=0; i<12; i++)
						{
							*((u8 *)(EEPROM_ADDR_YEARLY + i)) = Uart1Rx.buff[i+1];
						}
						
						//判断段数
						if ((*((u8 *)(EEPROM_ADDR_YEARLY + 6))) == 0x00) //PWM3
							{
								EEPROM_DATA_SECTION = 2;
							}
						else
							{
								if ((*((u8 *)(EEPROM_ADDR_YEARLY + 9))) == 0x00) //PWM4
									{
										EEPROM_DATA_SECTION = 3;
									}
								else
									{
										EEPROM_DATA_SECTION = 4;
									}
							}
						
						Uart1Tx.buff[0] = UART1_TXHDR1;
						Uart1Tx.buff[1] = UART1_TXHDR2;
						Uart1Tx.buff[2] = CMD_YEARLY;
						Uart1Tx.buff[3] = crc16_state;
						Uart1Tx.buff[4] = 0x00;
						Uart1Tx.buff[5] = 0x00;
						Uart1Tx.send(6);
					}
					break;

#if RTC_EN > 0				
				// PC: C9 DA A5 06 64 12 00 32 14 00 00 00
				//MCU: A7 B8 A5 01 00 00
				case CMD_DAYS:
					{
						EEPROM_DATA_MODE = MODE_DAYS;
						
						for (u8 i=0; i<6; i++)
						{
							*((u8 *)(EEPROM_ADDR_DAYS + i)) = Uart1Rx.buff[i+1];
						}
						
						Uart1Tx.buff[0] = UART1_TXHDR1;
						Uart1Tx.buff[1] = UART1_TXHDR2;
						Uart1Tx.buff[2] = CMD_DAYS;
						Uart1Tx.buff[3] = crc16_state;
						Uart1Tx.buff[4] = 0x00;
						Uart1Tx.buff[5] = 0x00;
						Uart1Tx.send(6);
					}
					break;
				
				// PC: C9 DA A7 34 64 00 0E 01 04 07 0A 00 00
				//                |---16---|
				//MCU: A7 B8 A7 01 00 00
				case CMD_SEASON:
					{
						EEPROM_DATA_MODE = MODE_SEASON;						
						
						for (u8 i=0; i<52; i++)
						{
							*((u8 *)(EEPROM_ADDR_SEASON + i)) = Uart1Rx.buff[i+1];
						}
						
						//判断段数
						if ((*((u8 *)(EEPROM_ADDR_SEASON+6))) == 0x00) //PWM3
							{
								EEPROM_DATA_SECTION = 2;
							}
						else
							{
								if ((*((u8 *)(EEPROM_ADDR_SEASON+9))) == 0x00) //PWM4
									{
										EEPROM_DATA_SECTION = 3;
									}
								else
									{
										EEPROM_DATA_SECTION = 4;
									}
							}
						
						Uart1Tx.buff[0] = UART1_TXHDR1;
						Uart1Tx.buff[1] = UART1_TXHDR2;
						Uart1Tx.buff[2] = CMD_SEASON;
						Uart1Tx.buff[3] = crc16_state;
						Uart1Tx.buff[4] = 0x00;
						Uart1Tx.buff[5] = 0x00;
						Uart1Tx.send(6);
					}
					break;
				
				// PC: C9 DA A8 90 64 00 0E 00 00
				//                |---48---|
				//MCU: A7 B8 A8 01 00 00
				case CMD_MONTHLY:
					{
						EEPROM_DATA_MODE = MODE_MONTHLY;
						
						for (u8 i=0; i<144; i++)
						{
							*((u8 *)(EEPROM_ADDR_MONTHLY + i)) = Uart1Rx.buff[i+1];
						}
						
						//判断段数
						if ((*((u8 *)(EEPROM_ADDR_MONTHLY+6))) == 0x00) //PWM3
							{
								EEPROM_DATA_SECTION = 2;
							}
						else
							{
								if ((*((u8 *)(EEPROM_ADDR_MONTHLY+9))) == 0x00) //PWM4
									{
										EEPROM_DATA_SECTION = 3;
									}
								else
									{
										EEPROM_DATA_SECTION = 4;
									}
							}
						
						Uart1Tx.buff[0] = UART1_TXHDR1;
						Uart1Tx.buff[1] = UART1_TXHDR2;
						Uart1Tx.buff[2] = CMD_MONTHLY;
						Uart1Tx.buff[3] = crc16_state;
						Uart1Tx.buff[4] = 0x00;
						Uart1Tx.buff[5] = 0x00;
						Uart1Tx.send(6);
					}
					break;
#endif
			}
		}
}
