/****************************************************                                                                                         
* Microcontroller: STM32F103RBT6
* Handware: JRMD1-V1.0
* File: rf.c                              
* Revision: 1.0  
* Description: zigbee API
* Author: ValarHao
* Date��2015.12.24
****************************************************/
#include "rf.h"
#include "usart.h"
#include "gui.h"
#include "my9221.h"
#include "tm1829.h"

OS_STK RF_TASK_STK[RF_STK_SIZE];

u8 rf_sendbuf[RF_SIZE];
u8 rf_recvbuf[RF_SIZE];

u8 recv_menu = 0;
u8 recv_modbus = 0;

void RF_SendPacket(u8 *Sendbuffer, u8 length)
{
	u8 status_buff[10];
	
	SI446X_SEND_PACKET(Sendbuffer, length, 0, 0);   //��������    
	
  do
  {
    SI446X_INT_STATUS(status_buff);
  } while (!(status_buff[3] & (1<<5))); //�ȴ��������
}

void rf_task(void *pdata)
{ 	
	u8 status_buff[10];
	u16 addr_temp[3];
	u8 crc_h_temp, crc_l_temp;
	u8 t;
	u16 i;
	
	RF_init();
	
	while (1)
	{
		if (job_mode == JOB_MODE_SLAVE)
			{
				SI446X_INT_STATUS(status_buff);     //����Ƿ��յ�һ�����ݰ�

			  //�յ�һ�����ݰ�
			  if (status_buff[3] & (1<<4))
				  {       
				    SI446X_READ_PACKET(rf_recvbuf);
				    
				    //rf_recvbuf���ĸ�ʽ��cmd packet_num data ������ data crc_h crc_l
				    
				    switch (rf_recvbuf[0]) //cmd
				    {	    	
				    	case RF_CMD_ENABLE_TASK:
				    		{
				    			//��ȡ�յ���CRC
			  					crc_h_temp = rf_recvbuf[4];
			  					crc_l_temp = rf_recvbuf[5];  					
			  					//�����µ�CRC
			  					crc16_1021(rf_recvbuf, 4);	   					   					
			  					//��֤CRC
			  					if ((crc_h_temp==rf_recvbuf[4]) && (crc_l_temp==rf_recvbuf[5]))
			  						{
			  							recv_menu = rf_recvbuf[2];  
			  							recv_modbus = rf_recvbuf[3];			
			  							
			  							switch (recv_menu)
			  							{
												case MENU_INDEX_COLOUR_CHANGE_3:
												case MENU_INDEX_COLOUR_CHANGE_4:
												case MENU_INDEX_COLOUR_ROTATION_3:
												case MENU_INDEX_COLOUR_ROTATION_4:
													{
														pattern_step = 0; //��״̬һ��ʼ			
														pattern_r = 0xFF; //���RGBW����
														pattern_g = 0;
														pattern_b = 0;
														pattern_w = 0;
													}			
													break;
												
												case MENU_INDEX_COLOUR_ALL:
												case MENU_INDEX_COLOUR_CH:
													{
														for (i=0; i<CH_SIZE; i++)
															channel[i] = 0x00;
													}		
													break;
												
												case MENU_INDEX_COLOUR_ROTATION_CH:
													{
														rotation_ch_status = 0;
														rotation_ch_cnt = 0;
													}
													break;
			  							}
			  							
			  							OSTaskResume(GUI_TASK_PRIO); //��������										
			  						}
				    		}
				    		break;
				    	
				    	case RF_CMD_DISABLE_TASK:
				    		{
				    			//��ȡ�յ���CRC
			  					crc_h_temp = rf_recvbuf[4];
			  					crc_l_temp = rf_recvbuf[5];  					
			  					//�����µ�CRC
			  					crc16_1021(rf_recvbuf, 4);	   					   					
			  					//��֤CRC
			  					if ((crc_h_temp==rf_recvbuf[4]) && (crc_l_temp==rf_recvbuf[5]))
			  						{ 							
											OSTaskSuspend(GUI_TASK_PRIO); //��������
			  						}
				    		}
				    		break;
				    	
				    	case MENU_INDEX_ADDRESS_LINE:
				    		{
				    			HIGH(addr_temp[0]) = rf_recvbuf[2];
			  					LOW(addr_temp[0])  = rf_recvbuf[3];
			  					HIGH(addr_temp[1]) = rf_recvbuf[4];
			  					LOW(addr_temp[1])  = rf_recvbuf[5];
			  					HIGH(addr_temp[2]) = rf_recvbuf[6];
			  					LOW(addr_temp[2])  = rf_recvbuf[7];
			  					
			  					//��ȡ�յ���CRC
			  					crc_h_temp = rf_recvbuf[8];
			  					crc_l_temp = rf_recvbuf[9];  					
			  					//�����µ�CRC
			  					crc16_1021(rf_recvbuf, 8);	   					   					
			  					//��֤CRC
			  					if ((crc_h_temp==rf_recvbuf[8]) && (crc_l_temp==rf_recvbuf[9]))
			  						{ 							
											YT_send_address_line(addr_temp[0], addr_temp[1], addr_temp[2]);						
			  						}	    						
				    		}	    		
				    		break;
				    	
				    	case MENU_INDEX_ADDRESS_RELAY:
				    		{
				    			HIGH(addr_temp[0]) = rf_recvbuf[2];
			  					LOW(addr_temp[0])  = rf_recvbuf[3];
			  					HIGH(addr_temp[1]) = rf_recvbuf[4];
			  					LOW(addr_temp[1])  = rf_recvbuf[5];
			  					HIGH(addr_temp[2]) = rf_recvbuf[6];
			  					LOW(addr_temp[2])  = rf_recvbuf[7];
			  					
			  					//��ȡ�յ���CRC
			  					crc_h_temp = rf_recvbuf[8];
			  					crc_l_temp = rf_recvbuf[9];  					
			  					//�����µ�CRC
			  					crc16_1021(rf_recvbuf, 8);	   					   					
			  					//��֤CRC
			  					if ((crc_h_temp==rf_recvbuf[8]) && (crc_l_temp==rf_recvbuf[9]))
			  						{ 							
			  							//ִ�е�ַ����
			  							YT_send_address_start();					
											YT_send_data(addr_temp[0], addr_temp[1], addr_temp[2]);			
											YT_send_stop();  							
			  						}	    						
				    		}	    		
				    		break;
				    	
				    	case MENU_INDEX_PARAMETERS:
				    		{
			  					//��ȡ�յ���CRC
			  					crc_h_temp = rf_recvbuf[22];
			  					crc_l_temp = rf_recvbuf[23];  					
			  					//�����µ�CRC
			  					crc16_1021(rf_recvbuf, 22);	   					   					
			  					//��֤CRC
			  					if ((crc_h_temp==rf_recvbuf[22]) && (crc_l_temp==rf_recvbuf[23]))
			  						{
			  							//ִ�в�������
			  							YT_send_flicker();
											YT_send_parameters_start();
											
											for (t=3; t!=0; t--)
											{
												dmxbuf[0] = DMX_START_SET;
												dmxbuf[1] = 0xFF;
												dmxbuf[2] = 0xFF;
												dmxbuf[3] = 0x59;
												dmxbuf[4] = 0x54;
												dmxbuf[5] = 0x44;
												dmxbuf[6] = 0x4D;
												dmxbuf[7] = 0x57;
												dmxbuf[8] = 0x40;
												dmxbuf[9] = 0x06;
												dmxbuf[10]= 0x0A; //����
												dmxbuf[11]= rf_recvbuf[2]; //PWMˢ���� 01:60Hz 02:120Hz 03:240Hz ����
												dmxbuf[12]= rf_recvbuf[3]; //�ϵ���Զ���ַ 00:��ֹ 01:ʹ��
												dmxbuf[13]= rf_recvbuf[4]; //�Ҷȵȼ���ѡ��PWMʱ�� 10:16bit 0F:15bit 0E:14bit ����
												dmxbuf[14]= rf_recvbuf[5];
												dmxbuf[15]= rf_recvbuf[6]; //DMXͨ���� 0C:12ͨ�� 18:24ͨ�� 24:36ͨ�� ����
												dmxbuf[16]= rf_recvbuf[7]; //��ɫģʽ 01:��ɫ 02:˫ɫ 03:��ɫ 04:��ɫ
												dmxbuf[17]= rf_recvbuf[8]; //оƬ�Լ� 00:���Լ� 01:���������� 02:ͨ��A���� 03:ͨ��B���� 04:ͨ��C���� 05:ͨ��D���� 06:����ͨ������ 07:�߲ʽ��� 08:������˸ 09:������˸ 0A:������˸
												dmxbuf[18]= rf_recvbuf[9]; //оƬ���� 00:PWM_OUTPUT 01:TM18XX_400K 02:MY9221 03:MBI6020 04:MBI6021 05:MBI6023 ����
												dmxbuf[19]= rf_recvbuf[10]; //������� 00:�ߵ�ƽ��Ч 01:�͵�ƽ��Ч
												dmxbuf[20]= rf_recvbuf[11]; //���в�����02:250K;00:����Ӧ
												crc16_1021(dmxbuf, 21);
												SendUart3DMX(dmxbuf, 23);
												OSTimeDly(49/OS_TICK);
												
												dmxbuf[0] = DMX_START_SET;
												dmxbuf[1] = 0xFF;
												dmxbuf[2] = 0xFF;
												dmxbuf[3] = 0x59;
												dmxbuf[4] = 0x54;
												dmxbuf[5] = 0x44;
												dmxbuf[6] = 0x4D;
												dmxbuf[7] = 0x57;
												dmxbuf[8] = 0x40;
												dmxbuf[9] = 0x14;
												dmxbuf[10]= 0x09;
												dmxbuf[11]= rf_recvbuf[12]; //N·��������ͨ������
												dmxbuf[12]= rf_recvbuf[13];
												dmxbuf[13]= rf_recvbuf[14];
												dmxbuf[14]= rf_recvbuf[15];
												dmxbuf[15]= rf_recvbuf[16];
												dmxbuf[16]= rf_recvbuf[17]; //�빦������¶�
												dmxbuf[17]= rf_recvbuf[18]; //�رյƾ��¶�
												dmxbuf[18]= rf_recvbuf[19]; //�ϵ���Զ���ַ ��ֹ:00 00 ʹ��:56 AE
												dmxbuf[19]= rf_recvbuf[20];
												crc16_1021(dmxbuf, 20);
												SendUart3DMX(dmxbuf, 22);
												OSTimeDly(49/OS_TICK);
											}
											
											switch (rf_recvbuf[21]) //gamma
											{
												case GAMMA_1_0: YT_send_gamma_1_0(); break;				
												case GAMMA_2_2: YT_send_gamma_2_2(); break;				
												case GAMMA_3_0: YT_send_gamma_3_0(); break;
											} 
											
											YT_send_stop();							
			  						}	    						
				    		}
				    		break;
				    	
				    	case MENU_INDEX_COLOUR_ALL:
				    		{
				    			//��ȡ�յ���CRC
			  					crc_h_temp = rf_recvbuf[9];
			  					crc_l_temp = rf_recvbuf[10];  					
			  					//�����µ�CRC
			  					crc16_1021(rf_recvbuf, 9);	   					   					
			  					//��֤CRC
			  					if ((crc_h_temp==rf_recvbuf[9]) && (crc_l_temp==rf_recvbuf[10]))
			  						{			  							
			  							for (i=1; i<CH_SIZE; i+=rf_recvbuf[2]) //rf_recvbuf[2]==>recv colour_total
											{
												channel[i] = rf_recvbuf[3];
												if (rf_recvbuf[2]==1) continue;
												channel[i+1] = rf_recvbuf[4];
												if (rf_recvbuf[2]==2) continue;
												channel[i+2] = rf_recvbuf[5];
												if (rf_recvbuf[2]==3) continue;
												channel[i+3] = rf_recvbuf[6];
												if (rf_recvbuf[2]==4) continue;
												channel[i+4] = rf_recvbuf[7];
												if (rf_recvbuf[2]==5) continue;
												channel[i+5] = rf_recvbuf[8];
											}
			  						}
				    		}
				    		break;
				    	
				    	case MENU_INDEX_COLOUR_CH:
				    		{
				    			//��ȡ�յ���CRC
			  					crc_h_temp = rf_recvbuf[4];
			  					crc_l_temp = rf_recvbuf[5];  					
			  					//�����µ�CRC
			  					crc16_1021(rf_recvbuf, 4);	   					   					
			  					//��֤CRC
			  					if ((crc_h_temp==rf_recvbuf[4]) && (crc_l_temp==rf_recvbuf[5]))
			  						{
			  							channel[rf_recvbuf[2]] = rf_recvbuf[3];
			  						}
				    		}
				    		break;
				    	
				    	
				    }	    
				    
				    SI446X_RESET();         // SI446X ģ�鸴λ	
					  SI446X_CONFIG_INIT();   // �Ĵ�����ʼ�����Ĵ�������WDS���õ�ͷ�ļ�  
					  SI446X_SET_POWER(0x7F); // �������������Ϊ���  
					  SI446X_START_RX(0, 0, RF_SIZE, 8, 8, 8);  // �������ģʽ    
				  } 
			}
			
		OSTimeDly(1);
	}	 
}

