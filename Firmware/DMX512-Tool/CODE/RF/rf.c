/****************************************************                                                                                         
* Microcontroller: STM32F103RBT6
* Handware: JRMD1-V1.0
* File: rf.c                              
* Revision: 1.0  
* Description: zigbee API
* Author: ValarHao
* Date：2015.12.24
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
	
	SI446X_SEND_PACKET(Sendbuffer, length, 0, 0);   //发送数据    
	
  do
  {
    SI446X_INT_STATUS(status_buff);
  } while (!(status_buff[3] & (1<<5))); //等待发射完成
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
				SI446X_INT_STATUS(status_buff);     //检测是否收到一个数据包

			  //收到一个数据包
			  if (status_buff[3] & (1<<4))
				  {       
				    SI446X_READ_PACKET(rf_recvbuf);
				    
				    //rf_recvbuf报文格式：cmd packet_num data ··· data crc_h crc_l
				    
				    switch (rf_recvbuf[0]) //cmd
				    {	    	
				    	case RF_CMD_ENABLE_TASK:
				    		{
				    			//读取收到的CRC
			  					crc_h_temp = rf_recvbuf[4];
			  					crc_l_temp = rf_recvbuf[5];  					
			  					//生成新的CRC
			  					crc16_1021(rf_recvbuf, 4);	   					   					
			  					//验证CRC
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
														pattern_step = 0; //从状态一开始			
														pattern_r = 0xFF; //清空RGBW数据
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
			  							
			  							OSTaskResume(GUI_TASK_PRIO); //唤醒任务										
			  						}
				    		}
				    		break;
				    	
				    	case RF_CMD_DISABLE_TASK:
				    		{
				    			//读取收到的CRC
			  					crc_h_temp = rf_recvbuf[4];
			  					crc_l_temp = rf_recvbuf[5];  					
			  					//生成新的CRC
			  					crc16_1021(rf_recvbuf, 4);	   					   					
			  					//验证CRC
			  					if ((crc_h_temp==rf_recvbuf[4]) && (crc_l_temp==rf_recvbuf[5]))
			  						{ 							
											OSTaskSuspend(GUI_TASK_PRIO); //挂起任务
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
			  					
			  					//读取收到的CRC
			  					crc_h_temp = rf_recvbuf[8];
			  					crc_l_temp = rf_recvbuf[9];  					
			  					//生成新的CRC
			  					crc16_1021(rf_recvbuf, 8);	   					   					
			  					//验证CRC
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
			  					
			  					//读取收到的CRC
			  					crc_h_temp = rf_recvbuf[8];
			  					crc_l_temp = rf_recvbuf[9];  					
			  					//生成新的CRC
			  					crc16_1021(rf_recvbuf, 8);	   					   					
			  					//验证CRC
			  					if ((crc_h_temp==rf_recvbuf[8]) && (crc_l_temp==rf_recvbuf[9]))
			  						{ 							
			  							//执行地址操作
			  							YT_send_address_start();					
											YT_send_data(addr_temp[0], addr_temp[1], addr_temp[2]);			
											YT_send_stop();  							
			  						}	    						
				    		}	    		
				    		break;
				    	
				    	case MENU_INDEX_PARAMETERS:
				    		{
			  					//读取收到的CRC
			  					crc_h_temp = rf_recvbuf[22];
			  					crc_l_temp = rf_recvbuf[23];  					
			  					//生成新的CRC
			  					crc16_1021(rf_recvbuf, 22);	   					   					
			  					//验证CRC
			  					if ((crc_h_temp==rf_recvbuf[22]) && (crc_l_temp==rf_recvbuf[23]))
			  						{
			  							//执行参数操作
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
												dmxbuf[10]= 0x0A; //长度
												dmxbuf[11]= rf_recvbuf[2]; //PWM刷新率 01:60Hz 02:120Hz 03:240Hz ……
												dmxbuf[12]= rf_recvbuf[3]; //上电后自动编址 00:禁止 01:使能
												dmxbuf[13]= rf_recvbuf[4]; //灰度等级（选择PWM时） 10:16bit 0F:15bit 0E:14bit ……
												dmxbuf[14]= rf_recvbuf[5];
												dmxbuf[15]= rf_recvbuf[6]; //DMX通道数 0C:12通道 18:24通道 24:36通道 ……
												dmxbuf[16]= rf_recvbuf[7]; //颜色模式 01:单色 02:双色 03:三色 04:四色
												dmxbuf[17]= rf_recvbuf[8]; //芯片自检 00:无自检 01:红蓝绿跳变 02:通道A常亮 03:通道B常亮 04:通道C常亮 05:通道D常亮 06:所有通道常亮 07:七彩渐变 08:快速闪烁 09:中速闪烁 0A:慢速闪烁
												dmxbuf[18]= rf_recvbuf[9]; //芯片类型 00:PWM_OUTPUT 01:TM18XX_400K 02:MY9221 03:MBI6020 04:MBI6021 05:MBI6023 ……
												dmxbuf[19]= rf_recvbuf[10]; //输出极性 00:高电平有效 01:低电平有效
												dmxbuf[20]= rf_recvbuf[11]; //串行波特率02:250K;00:自适应
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
												dmxbuf[11]= rf_recvbuf[12]; //N路并联后总通道数量
												dmxbuf[12]= rf_recvbuf[13];
												dmxbuf[13]= rf_recvbuf[14];
												dmxbuf[14]= rf_recvbuf[15];
												dmxbuf[15]= rf_recvbuf[16];
												dmxbuf[16]= rf_recvbuf[17]; //半功率输出温度
												dmxbuf[17]= rf_recvbuf[18]; //关闭灯具温度
												dmxbuf[18]= rf_recvbuf[19]; //上电后自动编址 禁止:00 00 使能:56 AE
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
				    			//读取收到的CRC
			  					crc_h_temp = rf_recvbuf[9];
			  					crc_l_temp = rf_recvbuf[10];  					
			  					//生成新的CRC
			  					crc16_1021(rf_recvbuf, 9);	   					   					
			  					//验证CRC
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
				    			//读取收到的CRC
			  					crc_h_temp = rf_recvbuf[4];
			  					crc_l_temp = rf_recvbuf[5];  					
			  					//生成新的CRC
			  					crc16_1021(rf_recvbuf, 4);	   					   					
			  					//验证CRC
			  					if ((crc_h_temp==rf_recvbuf[4]) && (crc_l_temp==rf_recvbuf[5]))
			  						{
			  							channel[rf_recvbuf[2]] = rf_recvbuf[3];
			  						}
				    		}
				    		break;
				    	
				    	
				    }	    
				    
				    SI446X_RESET();         // SI446X 模块复位	
					  SI446X_CONFIG_INIT();   // 寄存器初始化，寄存器来自WDS配置的头文件  
					  SI446X_SET_POWER(0x7F); // 将输出功率配置为最大  
					  SI446X_START_RX(0, 0, RF_SIZE, 8, 8, 8);  // 进入接收模式    
				  } 
			}
			
		OSTimeDly(1);
	}	 
}

