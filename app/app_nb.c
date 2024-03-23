#include "bsp.h"
#include "app.h"

#include "app_nb.h"
#include "app_fifo_NB_CoapST.h"

 
 
 uint16_t  send_count = 0;

 json_info_t json_info;
 
 

 
 volatile NB_STATE_e  APP_STATE= NB_NONE;
 
 uint8_t* NB_Module_IMEI = NULL;
 uint8_t  seq_record = 0;

 NB_STRCUT    g_nb;


//******************************************************************************
// fn : App_setReport
//
// brief : 设置发送状态
//
// param : none
//
// return : none
void App_setReport(void)
{
  if(isGetTime())
  {
    if(seq_record & NB_SEQ_COAP_SERVER)
    {
      if(bsp_GPS_GetInfo()->longtitude_char != 'X')
      {
        json_info.json_len = JSON_gps(json_info.json_buf,bsp_GPS_GetInfo()->longtitude,
                                  bsp_GPS_GetInfo()->latitude);
			

        APP_STATE = NB_CoAP_ST;   //?CoAP??
      }
      
    }
  }
}

//******************************************************************************
// fn : NB_MsgreportCb
//
// brief : NB模块消息上报回调
//
// param : 
//
// return : none
int  NB_MsgreportCb(msg_types_t types,int len,char* msg)
{
	switch(types)
	{
		case MSG_INIT:
		{
			p_info("\r\nINIT=%s\r\n",msg);
			//LCD_Print("Init=%s",msg);
			if(*msg == 'S')
			{
				//HAL_LED_SET(LED_INDEX_1,0);
				//Display_print0(dispHandle,1,0,"NET=ON");
				APP_STATE = NB_SIGN;
				seq_record |= NB_SEQ_INIT;  //记录初始化成功
				//LCD_Print("S2 to get info",NULL);
				//LCD_Print("S3 to set remote",NULL);
			}
			else
			{
				p_info("Reinit NB with S1\r\n");
				// LCD_Print("Reinit NB...",NULL);
				NBModule_open(&nb_config);
				App_nb_Init_Var();// app 初始化
				bsp_StartTimer(TMR_ID_server_ok,TMR_DELAY_1hour);//1小时重启一次
			}
		}
			break;
			
		case MSG_IMSI:
		{
			p_info("\r\nIMSI=%s\r\n",msg);
			//LCD_Print(msg,NULL);
		}
			break;
			
		case MSG_REG:
		{
			//Display_print1(dispHandle,1,0,"NET=%s",(*msg) == 1 ?"ON":"0FF");
		}
			break;
			
		case MSG_SIGN:
		{
			/*
			0   	-113dBm or less 
			1   	-111dBm 
			2..30 	-109dBm... -53dBm 
			31 		-51dBm or greater 
			99		Not known or not detectable
			*/
			g_wenshi.m_nb_signal = atoi(msg);
			if(g_wenshi.m_nb_signal >SIGNAL_STRENGTH_3 )
			{
				g_wenshi.m_nb_signal_level = 3;
			}
			else if(g_wenshi.m_nb_signal >SIGNAL_STRENGTH_2 )
			{
				g_wenshi.m_nb_signal_level = 2;
			}
			else if(g_wenshi.m_nb_signal >SIGNAL_STRENGTH_1 )
			{
				g_wenshi.m_nb_signal_level = 1;
			}
			else //if(g_wenshi.m_nb_signal >=100 )
			{
				g_wenshi.m_nb_signal_level = 0;
			}			
			p_info("m_nb_signal:%d;m_nb_signal_level:%d;",g_wenshi.m_nb_signal,g_wenshi.m_nb_signal_level);
			//Display_print1(dispHandle,1,8,"%sdbm",msg);
			//APP_STATE = NB_CoAP_SEVER;   //设定远程地址
		}
			break;
	
		case MSG_MODULE_INFO:
		{
			p_info("\r\nMinfo=%s\r\n",msg);
			//LCD_Print("Info=%s",msg);
		}
			break;
			
		case MSG_MID:
		{
			p_info("\r\nMID=%s\r\n",msg);
			//LCD_Print(msg,NULL);
		}
			break;
		
		case MSG_MMODEL:
		{
			p_info("\r\nModel=%s\r\n",msg);
			//LCD_Print(msg,NULL);
		}
			break;
		
		case MSG_MREV:
		{
			p_info("\r\nREV=%s\r\n",msg);
			//LCD_Print(msg,NULL);
		}
			break;
		
		case MSG_BAND:
		{
			p_info("\r\nFreq=%s\r\n",msg);
			//LCD_Print("Freq=%s",msg);
		}
			break;
		
		case MSG_IMEI:
		{
			p_info("\r\nIMEI=%s\r\n",msg);
			//保存IMEI信息
			NB_Module_IMEI = (uint8_t*)msg;
			//LCD_Print(msg,NULL);
		}
			break;
		case MSG_ICCID:
		{
			p_info("\r\nICCID=%s\r\n",msg);
		}
			break;		
		case MSG_NUESTATS:
		{
			p_info("\r\n MSG_NUESTATS=%s\r\n",msg);
		}
			break;				
		case MSG_UDP_CREATE:
		{
			p_info("\r\nUDP_CR=%s\r\n",msg);
			//LCD_Print("UDP=%s",msg);
			if(*msg == 'S')  //"S" 表示创建成功，'F'表示失败
			{
			seq_record |= NB_SEQ_UDP_CR;  //记录初始化成功

			p_info("S4 to register iotxx Cloud\r\n");
			//LCD_Print("S4 reg Packet",NULL);
			}
			else
			{
			p_info("Please, recreate udp\r\n");
			// LCD_Print("Recreate UDP",NULL);
			}
		}
			break;
		
		case MSG_UDP_CLOSE:
		{
			p_info("\r\nUDP_CL=%s\r\n",msg);

			if(*msg == 'S')
			{
			//清除UDP 创建成功标志
			seq_record &= ~NB_SEQ_UDP_CR;
			}
		}
			break;
		
		case MSG_UDP_SEND:
		{
			p_info("\r\nUDP_SEND=%s\r\n",msg);
			//LCD_Print("Send=%s",msg);
		}
			break;
		
		case MSG_UDP_RECE:
		{
			p_info("\r\nUDP_RECE=%s\r\n",msg);
			//LCD_Print(msg,NULL);
		}
			break;
		
		case MSG_COAP:
		{
			p_info("\r\nCOAP=%s\r\n",msg);
			if(*msg == 'S')
			{
				//针对设置
				seq_record |= NB_SEQ_COAP_SERVER;
				// LCD_Print("S4 Send CoAP",NULL);
			}
		}
			break;
		
		case MSG_COAP_SEND:
		{
			p_info("\r\n COAP_SENT =%s\r\n",msg);
			if(*msg == 'F')
			{
				App_fifoNBCoapST_putAgain();
			}
			//LCD_Print("Send=%s",msg);
		}
			break;

		case MSG_COAP_RECE:
		{
			//uint16 lenth = 0;

			//p_info("接收数据:%s;长度%d\r\n",msg,len);
			p_info("MSG_COAP_RECE");
			
			if((msg[0]==PCP_DATA_starting_mark1)&&(msg[1]==PCP_DATA_starting_mark2))
			{
				App_PCP_RXDataFrame_analyse(len,msg,&g_PCP_RXData);
				g_Update.rx_data_in_flag = TRUE;
			}
			else
			{
				App_DataRx_process(len,msg);
			}
			//LCD_Print(msg,NULL);
		}
			break;
		
		default :
		{
			break;
		}
	}
  return 0;
}


/*
*********************************************************************************************************
*	函 数 名: App_nb_Init_Var
*	功能说明:   
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_nb_Init_Var(void)
{
	g_nb.start_flag = FALSE;
	g_nb.work_state = NB_STATE_MAX;
	
	APP_STATE = NB_NONE;
	seq_record = 0;

}


/*
*********************************************************************************************************
*	函 数 名: App_nb_get_APPSTATE
*	功能说明:   
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t App_nb_get_APPSTATE(void)
{
	if(APP_STATE == NB_END)
		return 1;
	else
		return 0;
}


/*
*********************************************************************************************************
*	函 数 名: App_nb_Init_Var
*	功能说明:   
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_nb_Init_start(void)
{
	g_nb.start_flag = TRUE;
	g_nb.work_state = NB_STATE_INIT;
	
}

/*
*********************************************************************************************************
*	函 数 名: App_Gps_Loop
*	功能说明: 读取GPS经纬度值
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_nb_Loop(void)
{
	if(g_nb.start_flag == TRUE)
	{

	}
	else
	{


		return ;
	}

	switch(g_nb.work_state)
	{
		case NB_STATE_INIT :
		{				
			g_nb.work_state  = NB_STATE_LOOP;
			bsp_StartTimer(TMR_ID_read_NB_buffer, TMR_DELAY_5S);// 主动读取NB模块缓存	
			bsp_StartTimer(TMR_ID_read_NB_signal, TMR_DELAY_2min);// 主动读取NB模块信号 
			p_info(" NB_STATE_INIT");	

		}
			break;	

		case NB_STATE_LOOP :// 没有经历过5min，直接人工退出
		{
#if 1//暂时不要主动读取缓存
			/*只有在空闲的时候，才主动读取缓存命令；*/
			if((App_nb_get_APPSTATE()&&state_get()) == 1)
			{
				/* 主动读取NB模块缓存	 */
				if(bsp_CheckTimer(TMR_ID_read_NB_buffer))
				{
					bsp_StartTimer(TMR_ID_read_NB_buffer, TMR_DELAY_5S);// 主动读取NB模块缓存	
					p_info("主动读取NB模块缓存 timer"); 			
					
					/*开始 gps 采集初始化*/ 
					nb_read_message();	
					//bc95_coapReadMsg(&nb_config);
				
				}
				/*主动读取NB模块signal 信号强度*/
				if(bsp_CheckTimer(TMR_ID_read_NB_signal))
				{
					bsp_StartTimer(TMR_ID_read_NB_signal, TMR_DELAY_1hour);// 主动读取NB模块信号	
					p_info("主动读取NB模块信号 timer");				
					/*开始 gps 采集初始化*/ 
				     NBModule_Sign(&nb_config);  			
				}			
			}				
#endif			
			switch(APP_STATE)
			{
				case NB_NONE:
			  {
				//wait for key
				  APP_STATE = NB_INIT;
			  }
				  break;
				case NB_INIT:
			  {
				printf("\r\n<----BC95 Init---->\r\n");
				//LCD_Print("Init start...",NULL);
				seq_record = 0;
				NBModule_Init(&nb_config);// 初始化
				APP_STATE = NB_END;
			  }
				  break;
				case NB_SIGN:
			  {
				 printf("\r\n<----BC95 get signal---->\r\n");
				 NBModule_Sign(&nb_config);
				 APP_STATE = NB_END;
			  }
				  break;
				case NB_MODULE:
			  {
				printf("\r\n<----Module info ---->\r\n");
				//LCD_Print("Module info...",NULL);
				NBModule_Info(&nb_config);
				APP_STATE = NB_END;
			  }
				  break;
				case NB_UDP_CR:
			  {
				printf("\r\n<----Create udp ---->\r\n");
				//LCD_Print("UDP Create...",NULL);
				NBModule_CreateUDP(&nb_config);
				APP_STATE = NB_END;
			  }
				  break;
				case NB_UDP_CL:
			  {
				printf("\r\n<----Close udp ---->\r\n");
				NBModule_CloseUDP(&nb_config);
				APP_STATE = NB_END;
			  }
				  break;
				case NB_UDP_REG:
			  {
				//发送注册包
				//如果不与谷雨云平台通信，则不需要发送注册包
				printf("\r\n<----NB registes to Cloud ---->\r\n");
				//LCD_Print("Udp send...",NULL);
				
				char regPacket[30];
				uint8_t msgLen = 0;
				
				//利用库函数进行合格化数据
				msgLen = sprintf(regPacket,"ep=%s&pw=123456",NB_Module_IMEI);
				regPacket[msgLen] = 0;
				
				//char* regPacket = "ep=863703036005069&pw=123456";
				NBModule_SendData(&nb_config,msgLen,regPacket);
				seq_record |= NB_SEQ_UDP_REG;
				//LCD_Print("S3 send UDP",NULL);
				APP_STATE = NB_END;
			  }
				  break;
				case NB_UDP_ST:
			  {
				printf("\r\n<---- Send udp ---->\r\n");
				//LCD_Print("Udp send...",NULL);
				char* userPacket = "NB_EK_L476";
				NBModule_SendData(&nb_config,sizeof("NB_EK_L476"),userPacket);
			
				APP_STATE = NB_END;
			  }
				  break;
				  
				case NB_UDP_RE:
			  {
				
			  }
				  break;
				case NB_CoAP_SEVER:
			  {
				printf("\r\n<---- CoAP Server set ---->\r\n");
				//LCD_Print("Coap remote...",NULL);
				
				NBModule_CoAPServer(&nb_config,1,NULL);
				APP_STATE = NB_END;
			  }
				  break;
				case NB_CoAP_ST:
			  {
				if(json_info.json_len)
				{
				  	send_count++;

				  /*  发送数据成功*/
					if(bc95_coapSendMsg(&nb_config,json_info.json_len,json_info.json_buf))
					{
						p_info("nb发送数据fail");
					}
					else
					{
						p_info("nb发送数据 success");
						memset(&json_info,'0',sizeof(json_info));//发送完成，清除数据
						json_info.json_len = 0; 			  						
					}

				  //打印发送次数
				  //Display_print1(dispHandle,4,0,"Count = %d	",send_count);
				}
				APP_STATE = NB_END;
			  }
				  break;
				case NB_CoAP_RE:
			  {
				
			  }
				  break;
				default:
			  {
				
			  }
				  break;
			}	


		
		}		
			break;	

			
		default:
			break;
	}



}

/*
*********************************************************************************************************
*	函 数 名: App_Battery_test
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_nb_test(void)
{

#ifdef TEST_App_nb_test

	while(1)
	{

	}
#endif
	
}

