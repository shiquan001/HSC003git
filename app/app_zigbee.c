#include "app_zigbee.h"

#include "app.h"
#include "bsp.h"
#if 0
_end_device_data_type g_tZigbeeEndDevice1;// 终端1
_end_device_data_type g_tZigbeeEndDevice2;// 终端2
_end_device_data_type g_tZigbeeEndDevice3;// 终端3
_end_device_data_type g_tZigbeeEndDevice4;// 终端4

 _server_data_type g_tZigbeeCoordinator;// 协调器 Coordinator

u8 rx_data_diwen[UART1_RX_BUF_SIZE];


/*
*********************************************************************************************************
*	函 数 名: App_zigbee_Init_Var
*	功能说明:  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_zigbee_Init_Var(void)
{

	g_tZigbeeCoordinator.work_state = ZIGBEE_INIT_GetVersion;
	g_tZigbeeCoordinator.flag_tx = FALSE;
	g_tZigbeeCoordinator.count_tx = 0;// ++
	
	g_tZigbeeCoordinator.version[0]= 0;
	g_tZigbeeCoordinator.version[1]= 0;
	g_tZigbeeCoordinator.version[2]= 0;

	g_tZigbeeCoordinator.nwkAddr0 = 0x00;
	g_tZigbeeCoordinator.nwkAddr1 = 0x00;
	
}



/**************************************************************************************                      
* 名    称：App_zigbee_config_read_cmd
* 功    能:
* 入口参数：
* 出口参数：无
* 说    明：
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void App_zigbee_config_read_cmd(u8  cmd )
{
	u8 buffer[16];
	u8 buffer_lenth=0;
	
	
	buffer[buffer_lenth] = ZIGBEE_DATA_SOF_CONFIG_Coordinator;//SOF
	buffer_lenth++;	
	
	buffer[ZIGBEE_DATA_LENTH_POS] = 0;//	LEN
	buffer_lenth++;	
	
	buffer[buffer_lenth] = cmd;//CMD	
	buffer_lenth++;		
	
	buffer[buffer_lenth] = 0x02;//	AddrMode0x1F   //0x02 - 单播方式
	buffer_lenth++;	

	buffer[buffer_lenth] = 0;//nwkAddr0
	buffer_lenth++;	
	buffer[buffer_lenth] = 0;//	nwkAddr1
	buffer_lenth++;	

	buffer[buffer_lenth] = 0;//	seqID
	buffer_lenth++;	

	buffer[buffer_lenth] = ZIGBEE_CMD_READ;//	R|W
	buffer_lenth++;		

	buffer[ZIGBEE_DATA_LENTH_POS] = buffer_lenth;//	LEN
	
	HAL_UART_Transmit(&hlpuart1, buffer, buffer_lenth, 0xFFFF);

}



/**************************************************************************************                      
* 名    称：App_zigbee_app_write_cmd
* 功    能:
* 入口参数：
	Mode：	数据发送方式：			
	0x01 - 组播方式，网络地址为目标组Id			
	0x02 - 单播方式			
	0x0F - 广播方式，广播网络地址为0xFFFF			

* 出口参数：无
* 说    明：
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void App_zigbee_app_write_cmd(u8  cmd ,u8 AddrMode)
{
	u8 buffer[16];
	u8 buffer_lenth=0;
	
	
	buffer[buffer_lenth] = ZIGBEE_DATA_SOF_APPLICAITON;//SOF
	buffer_lenth++;	
	
	buffer[ZIGBEE_DATA_LENTH_POS] = 0;//	LEN
	buffer_lenth++;	
	
	buffer[buffer_lenth] = cmd;//CMD	
	buffer_lenth++;		
	
	buffer[buffer_lenth] = AddrMode;//	AddrMode
	buffer_lenth++;	

	buffer[buffer_lenth] = 0;//nwkAddr0
	buffer_lenth++;	
	buffer[buffer_lenth] = 0;//	nwkAddr1
	buffer_lenth++;	

	buffer[buffer_lenth] = 0;//	seqID
	buffer_lenth++;	

	buffer[buffer_lenth] = ZIGBEE_CMD_READ;//	R|W
	buffer_lenth++;		

	buffer[ZIGBEE_DATA_LENTH_POS] = buffer_lenth;//	LEN
	
	HAL_UART_Transmit(&hlpuart1, buffer, buffer_lenth, 0xFFFF);

}




/**************************************************************************************                      
* 名    称：App_zigbee_app_write_cmd
* 功    能:
* 入口参数：
	Mode：	数据发送方式：			
	0x01 - 组播方式，网络地址为目标组Id			
	0x02 - 单播方式			
	0x0F - 广播方式，广播网络地址为0xFFFF		
	
0x00 - 关闭网络
0x00 - 一直打开网络
0x** - 打开网络若干秒后自动关闭

* 出口参数：无
* 说    明：
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
#define ZIGBEE_TurnOn_network 0x01   // 帧头 ，如应用功能帧头为 0xFE
#define ZIGBEE_TurnOff_network 0x00   // 帧头 ，如应用功能帧头为 0xFE

void App_zigbee_app_TurnOn_Off_network_cmd(u8  cmd ,u8 AddrMode,u8 OnOffnetwork)
{
	u8 buffer[16];
	u8 buffer_lenth=0;
	
	
	buffer[buffer_lenth] = ZIGBEE_DATA_SOF_APPLICAITON;//SOF
	buffer_lenth++;	
	
	buffer[ZIGBEE_DATA_LENTH_POS] = 0;//	LEN
	buffer_lenth++;	
	
	buffer[buffer_lenth] = cmd;//CMD	
	buffer_lenth++;		
	
	buffer[buffer_lenth] = AddrMode;//	AddrMode
	buffer_lenth++;	

	buffer[buffer_lenth] = g_tZigbeeCoordinator.nwkAddr0;//nwkAddr0
	buffer_lenth++;	
	buffer[buffer_lenth] = g_tZigbeeCoordinator.nwkAddr1;//	nwkAddr1
	buffer_lenth++;	

	buffer[buffer_lenth] = 0;//	seqID
	buffer_lenth++;	

	buffer[buffer_lenth] = OnOffnetwork;//	R|W
	buffer_lenth++;		

	buffer[ZIGBEE_DATA_LENTH_POS] = buffer_lenth;//	LEN
	
	HAL_UART_Transmit(&hlpuart1, buffer, buffer_lenth, 0xFFFF);

}

/**************************************************************************************                      
* 名    称：App_zigbee_loop
* 功    能：
* 入口参数：
* 出口参数：无
* 说    明：
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
uint8_t App_zigbee_rx_data_pro(void)
{
	uint8_t ucData;
	uint16_t cur_pos = 0;
	uint16_t pos = 0;
	uint8_t ret = 0xff ;
	uint8_t i;
	uint8_t rx_cmd = 0xff;
	uint8_t cmd = 0xff;
	uint8_t AddrMode = 0;
	
	if (bsp_CheckTimer(TMR_ID_ZIGBEE_UART_DATAIN))
	{
		memset(rx_data_diwen,0,sizeof(rx_data_diwen));
		
		while(1)
		{
		
			if(comGetChar(ZIGBEE_UART1, &ucData))
			{			
				rx_data_diwen[pos++] = ucData;
			}
			else
				break;

			if(pos >= DataSize_MAX)
				break;
		}
		/* log */
		if(pos <= 4)
		{
			ret = 250;
			return ret;
		}
		
		while(1)
		{
			/*搜索帧头*/
			if((rx_data_diwen[cur_pos] == ZIGBEE_DATA_SOF_CONFIG_Coordinator)||(rx_data_diwen[cur_pos] == ZIGBEE_DATA_SOF_APPLICAITON)||(rx_data_diwen[cur_pos] == 0))
			{
				break;
			}
			else
			{
				cur_pos++;
			}		
		}		
		
		/*  协调器配置命令回复*/
		if((rx_data_diwen[0+cur_pos] == ZIGBEE_DATA_SOF_CONFIG_Coordinator))//&&(rx_data_diwen[7+cur_pos] == ZIGBEE_CMD_READ))
		{
			cmd = rx_data_diwen[2+cur_pos];// cmd
			cmd &= ~0x80;// CMD&0x80
			switch(cmd)
			{
				case ZIGBEE_CMD_VERSION:
				{
					g_tZigbeeCoordinator.version[0] = rx_data_diwen[8+cur_pos];
					g_tZigbeeCoordinator.version[1] = rx_data_diwen[9+cur_pos];
					g_tZigbeeCoordinator.version[2] = rx_data_diwen[10+cur_pos];
					
					ret = cmd;
				}
					break;
				case ZIGBEE_CMD_CONFIG_DEVICE_TYPE:
				{
					g_tZigbeeCoordinator.devType = rx_data_diwen[8+cur_pos];
					ret = cmd;
				}
					break;
				case ZIGBEE_CMD_CONFIG_PANID:
				{
					g_tZigbeeCoordinator.PanIDH = rx_data_diwen[8+cur_pos];
					g_tZigbeeCoordinator.PanIDL = rx_data_diwen[9+cur_pos];
					ret = cmd;
				}
					break;
				case ZIGBEE_CMD_CONFIG_CHANNEL:
				{
					g_tZigbeeCoordinator.channel = rx_data_diwen[8+cur_pos];
					ret = cmd;
				}
					break;
				case ZIGBEE_CMD_CONFIG_NET_ADDRESS:
				{
					//g_tZigbeeCoordinator.PanIDH = rx_data_diwen[8+cur_pos];
					//g_tZigbeeCoordinator.PanIDL = rx_data_diwen[9+cur_pos];
					ret = cmd;
				}
					break;
				case ZIGBEE_CMD_CONFIG_MAC_ADDRESS:
				{
					for(i=0;i<8;i++)
						g_tZigbeeCoordinator.macAddr[i]= rx_data_diwen[8+cur_pos+i];
					ret = cmd;
				}
					break;
				case ZIGBEE_CMD_CONFIG_TXPOWER:
				{
					g_tZigbeeCoordinator.txPower = rx_data_diwen[8+cur_pos];
					ret = cmd;
				}
					break;		
				case ZIGBEE_CMD_CONFIG_BAUDRATE:
				{
					g_tZigbeeCoordinator.baudRate = rx_data_diwen[8+cur_pos];
					ret = cmd;
				}
					break;		
				default:
					break;
					
			}
		}
		/*  应用命令回复*/
		else if((rx_data_diwen[0+cur_pos] == ZIGBEE_DATA_SOF_APPLICAITON))//&&(rx_data_diwen[7+cur_pos] == ZIGBEE_CMD_READ))
		{
			rx_cmd = rx_data_diwen[2+cur_pos];// cmd
			/*其中，命令字节 或 0x40表示本地信息返回串口ACK，命令字节 或 0x80表示接收到的远端无线消息*/
			if((rx_cmd&0x40) == 0x40)//0x40|CMD //2.2.0.1 根据网络地址 查询 macAddr//2.2.1 串口透传数据
			{			
				cmd  = rx_cmd&(~0x40);// CMD&0x80
			}
			else if((rx_cmd&0x80) == 0x80)
			{
				cmd  = rx_cmd&(~0x80);// CMD&0x80
			}
				
			AddrMode = rx_data_diwen[3+cur_pos];// uint8_t
			
			switch(cmd)
			{
				case ZIGBEE_APPLICAITON_CMD_Device_Information:
				{
					if(rx_cmd == 0x40)
					{
						g_tZigbeeCoordinator.End_Device_nwkAddr[0] = rx_data_diwen[4+cur_pos];
						g_tZigbeeCoordinator.End_Device_nwkAddr[1] = rx_data_diwen[5+cur_pos];
						
						g_tZigbeeCoordinator.End_Device_seqID 	= rx_data_diwen[6+cur_pos];
						g_tZigbeeCoordinator.End_Device_feedback_state = rx_data_diwen[7+cur_pos];

					}
					else if(rx_cmd == 0x80)
					{
						g_tZigbeeCoordinator.End_Device_nwkAddr[0] = rx_data_diwen[4+cur_pos];
						g_tZigbeeCoordinator.End_Device_nwkAddr[1] = rx_data_diwen[5+cur_pos];

						g_tZigbeeCoordinator.End_Device_seqID 	= rx_data_diwen[6+cur_pos];

						g_tZigbeeCoordinator.End_Device_macAddr[0] = rx_data_diwen[7+cur_pos];
						g_tZigbeeCoordinator.End_Device_macAddr[1] = rx_data_diwen[8+cur_pos];
						g_tZigbeeCoordinator.End_Device_macAddr[2] = rx_data_diwen[9+cur_pos];
						g_tZigbeeCoordinator.End_Device_macAddr[3] = rx_data_diwen[10+cur_pos];
						g_tZigbeeCoordinator.End_Device_macAddr[4] = rx_data_diwen[11+cur_pos];
						g_tZigbeeCoordinator.End_Device_macAddr[5] = rx_data_diwen[12+cur_pos];
						g_tZigbeeCoordinator.End_Device_macAddr[6] = rx_data_diwen[13+cur_pos];
						g_tZigbeeCoordinator.End_Device_macAddr[7] = rx_data_diwen[14+cur_pos];						
					}

					ret = cmd;
				}
					break;
				case ZIGBEE_APPLICAITON_CMD_transparent_data:
				{
					g_tZigbeeCoordinator.End_Device_nwkAddr[0] = rx_data_diwen[4+cur_pos];
					g_tZigbeeCoordinator.End_Device_nwkAddr[1] = rx_data_diwen[5+cur_pos];
					
					g_tZigbeeCoordinator.End_Device_seqID	= rx_data_diwen[6+cur_pos];
					g_tZigbeeCoordinator.End_Device_feedback_state = rx_data_diwen[7+cur_pos];
					ret = cmd;
				}
					break;
				case ZIGBEE_APPLICAITON_CMD_Offline_device_notification:
				{
					g_tZigbeeCoordinator.End_Device_offline_nwkAddr[0]	= rx_data_diwen[7+cur_pos];
					g_tZigbeeCoordinator.End_Device_offline_nwkAddr[1] = rx_data_diwen[8+cur_pos];
					ret = cmd;
				}
					break;
				case ZIGBEE_APPLICAITON_CMD_Turn_ON_off_network:
				{
					g_tZigbeeCoordinator.End_Device_feedback_state = rx_data_diwen[7+cur_pos];
					ret = cmd;
				}
					break;
				case ZIGBEE_APPLICAITON_CMD_Remote_factory_reset:
				{
					g_tZigbeeCoordinator.End_Device_feedback_state = rx_data_diwen[7+cur_pos];
					ret = cmd;
				}
					break;
				case ZIGBEE_APPLICAITON_CMD_Remote_restart_command:
				{
					g_tZigbeeCoordinator.End_Device_feedback_state = rx_data_diwen[7+cur_pos];
					ret = cmd;
				}
					break;
				case ZIGBEE_APPLICAITON_CMD_Remove_device:
				{
					g_tZigbeeCoordinator.End_Device_feedback_state = rx_data_diwen[7+cur_pos];
					ret = cmd;
				}
					break;		
				case ZIGBEE_APPLICAITON_CMD_Remove_device_notifications:
				{
					g_tZigbeeCoordinator.End_Device_feedback_state = rx_data_diwen[7+cur_pos];

						g_tZigbeeCoordinator.Remove_device_notifications_macAddr[0] = rx_data_diwen[8+cur_pos];
						g_tZigbeeCoordinator.Remove_device_notifications_macAddr[1] = rx_data_diwen[9+cur_pos];
						g_tZigbeeCoordinator.Remove_device_notifications_macAddr[2] = rx_data_diwen[10+cur_pos];
						g_tZigbeeCoordinator.Remove_device_notifications_macAddr[3] = rx_data_diwen[11+cur_pos];
						g_tZigbeeCoordinator.Remove_device_notifications_macAddr[4] = rx_data_diwen[12+cur_pos];
						g_tZigbeeCoordinator.Remove_device_notifications_macAddr[5] = rx_data_diwen[13+cur_pos];
						g_tZigbeeCoordinator.Remove_device_notifications_macAddr[6] = rx_data_diwen[14+cur_pos];
						g_tZigbeeCoordinator.Remove_device_notifications_macAddr[7] = rx_data_diwen[15+cur_pos];		
					ret = cmd;
				}
					break;		

				case ZIGBEE_APPLICAITON_CMD_Query_signal_quality_of_the_local_device:
				{

					ret = cmd;
				}
					break;	
				case ZIGBEE_APPLICAITON_CMD_The_coordinator_queries_the_signal_quality_of_the_remote:
				{
					g_tZigbeeCoordinator.End_Device_Rssi = rx_data_diwen[7+cur_pos];
					ret = cmd;
				}
					break;	

				case ZIGBEE_APPLICAITON_CMD_Add_group:
				{
					g_tZigbeeCoordinator.End_Device_feedback_state = rx_data_diwen[7+cur_pos];

					ret = cmd;
				}
					break;	
				case ZIGBEE_APPLICAITON_CMD_Delete_group:
				{
					g_tZigbeeCoordinator.End_Device_feedback_state = rx_data_diwen[7+cur_pos];
					ret = cmd;
				}
					break;	
				case ZIGBEE_APPLICAITON_CMD_Query_group:
				{
					g_tZigbeeCoordinator.End_Device_feedback_state = rx_data_diwen[7+cur_pos];

					g_tZigbeeCoordinator.groupNum = rx_data_diwen[8+cur_pos];
					g_tZigbeeCoordinator.Group1_L= rx_data_diwen[9+cur_pos];
					g_tZigbeeCoordinator.Group1_H= rx_data_diwen[10+cur_pos];
					g_tZigbeeCoordinator.Group2_L= rx_data_diwen[11+cur_pos];
					g_tZigbeeCoordinator.Group2_H= rx_data_diwen[12+cur_pos];
					
					ret = cmd;
				}
					break;	
										
				default:
					break;
					
			}
		}


    }

    return ret;
}  


/**************************************************************************************                      
* 名    称：App_zigbee_loop
* 功    能：
* 入口参数：
* 出口参数：无
* 说    明：
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
void App_zigbee_loop(void )
{	
	u8 rx_data_type = 0;// 接收到的数据类型
	
	switch(g_tZigbeeCoordinator.work_state)
	{
		case ZIGBEE_INIT_GetVersion :
		{				
			/* 第一次发送，查询版本号命令*/
			if(g_tZigbeeCoordinator.flag_tx == FALSE)
			{
				g_tZigbeeCoordinator.flag_tx = TRUE ; 
				g_tZigbeeCoordinator.count_tx++;// ++
				if(g_tZigbeeCoordinator.count_tx >= 255)
					g_tZigbeeCoordinator.count_tx = 255;
				
				App_zigbee_config_read_cmd(ZIGBEE_CMD_VERSION);
				bsp_StartTimer(TMR_ID_ZIGBEE_TIMEROUT, 500);		/* 设置500ms	 超时 */		
			}
			/* 500ms 超时，再次发送查询命令*/
			if (bsp_CheckTimer(TMR_ID_ZIGBEE_TIMEROUT))
			{
				g_tZigbeeCoordinator.flag_tx = FALSE;// 再次发送版本号，查询命令
			}			
			/* 接收版本号数据*/
			if(App_zigbee_rx_data_pro() == ZIGBEE_CMD_VERSION)
			{
				g_tZigbeeCoordinator.work_state = ZIGBEE_INIT_GetDEVICETYPE;
				g_tZigbeeCoordinator.flag_tx = FALSE;
				g_tZigbeeCoordinator.count_tx = 0;// ++
				bsp_StopTimer(TMR_ID_ZIGBEE_TIMEROUT);// 停止定时器
			}
			
			break;
		}

		case ZIGBEE_INIT_GetDEVICETYPE :
		{	
			/* 第一次发送，查询设备类型命令*/
			if(g_tZigbeeCoordinator.flag_tx == FALSE)
			{
				g_tZigbeeCoordinator.flag_tx = TRUE ; 
				g_tZigbeeCoordinator.count_tx++;// ++
				if(g_tZigbeeCoordinator.count_tx >= 255)
					g_tZigbeeCoordinator.count_tx = 255;
				
				App_zigbee_config_read_cmd(ZIGBEE_CMD_CONFIG_DEVICE_TYPE);
				bsp_StartTimer(TMR_ID_ZIGBEE_TIMEROUT, 500);		/* 设置500ms	 超时 */		
			}
			/* 500ms 超时，再次发送查询命令*/
			if (bsp_CheckTimer(TMR_ID_ZIGBEE_TIMEROUT))
			{
				g_tZigbeeCoordinator.flag_tx = FALSE;// 再次发送版本号，查询命令
			}			
			/* 接收版本号数据*/
			if(App_zigbee_rx_data_pro() == ZIGBEE_CMD_CONFIG_DEVICE_TYPE)
			{
				g_tZigbeeCoordinator.work_state = ZIGBEE_INIT_GetPANID;
				g_tZigbeeCoordinator.flag_tx = FALSE;
				g_tZigbeeCoordinator.count_tx = 0;// ++
				bsp_StopTimer(TMR_ID_ZIGBEE_TIMEROUT);// 停止定时器
			}				
			break;
		}
		case ZIGBEE_INIT_GetPANID :
		{	
			/* 第一次发送，查询设备类型命令*/
			if(g_tZigbeeCoordinator.flag_tx == FALSE)
			{
				g_tZigbeeCoordinator.flag_tx = TRUE ; 
				g_tZigbeeCoordinator.count_tx++;// ++
				if(g_tZigbeeCoordinator.count_tx >= 255)
					g_tZigbeeCoordinator.count_tx = 255;
				
				App_zigbee_config_read_cmd(ZIGBEE_CMD_CONFIG_PANID);
				bsp_StartTimer(TMR_ID_ZIGBEE_TIMEROUT, 500);		/* 设置500ms	 超时 */		
			}
			/* 500ms 超时，再次发送查询命令*/
			if (bsp_CheckTimer(TMR_ID_ZIGBEE_TIMEROUT))
			{
				g_tZigbeeCoordinator.flag_tx = FALSE;// 再次发送版本号，查询命令
			}			
			/* 接收版本号数据*/
			if(App_zigbee_rx_data_pro() == ZIGBEE_CMD_CONFIG_PANID)
			{
				g_tZigbeeCoordinator.work_state = ZIGBEE_INIT_GetCHANNEL;
				g_tZigbeeCoordinator.flag_tx = FALSE;
				g_tZigbeeCoordinator.count_tx = 0;// ++
				bsp_StopTimer(TMR_ID_ZIGBEE_TIMEROUT);// 停止定时器
			}							
			break;
		}

		case ZIGBEE_INIT_GetCHANNEL:
		{	
			/* 第一次发送，查询设备类型命令*/
			if(g_tZigbeeCoordinator.flag_tx == FALSE)
			{
				g_tZigbeeCoordinator.flag_tx = TRUE ; 
				g_tZigbeeCoordinator.count_tx++;// ++
				if(g_tZigbeeCoordinator.count_tx >= 255)
					g_tZigbeeCoordinator.count_tx = 255;
				
				App_zigbee_config_read_cmd(ZIGBEE_CMD_CONFIG_CHANNEL);
				bsp_StartTimer(TMR_ID_ZIGBEE_TIMEROUT, 500);		/* 设置500ms	 超时 */		
			}
			/* 500ms 超时，再次发送查询命令*/
			if (bsp_CheckTimer(TMR_ID_ZIGBEE_TIMEROUT))
			{
				g_tZigbeeCoordinator.flag_tx = FALSE;// 再次发送版本号，查询命令
			}			
			/* 接收版本号数据*/
			if(App_zigbee_rx_data_pro() == ZIGBEE_CMD_CONFIG_CHANNEL)
			{
				g_tZigbeeCoordinator.work_state = ZIGBEE_INIT_GetNET_ADDRESS;
				g_tZigbeeCoordinator.flag_tx = FALSE;
				g_tZigbeeCoordinator.count_tx = 0;// ++
				bsp_StopTimer(TMR_ID_ZIGBEE_TIMEROUT);// 停止定时器
			}						
			break;
		}

		case ZIGBEE_INIT_GetNET_ADDRESS:
		{	
			/* 第一次发送，查询设备类型命令*/
			if(g_tZigbeeCoordinator.flag_tx == FALSE)
			{
				g_tZigbeeCoordinator.flag_tx = TRUE ; 
				g_tZigbeeCoordinator.count_tx++;// ++
				if(g_tZigbeeCoordinator.count_tx >= 255)
					g_tZigbeeCoordinator.count_tx = 255;
				
				App_zigbee_config_read_cmd(ZIGBEE_CMD_CONFIG_NET_ADDRESS);
				bsp_StartTimer(TMR_ID_ZIGBEE_TIMEROUT, 500);		/* 设置500ms	 超时 */		
			}
			/* 500ms 超时，再次发送查询命令*/
			if (bsp_CheckTimer(TMR_ID_ZIGBEE_TIMEROUT))
			{
				g_tZigbeeCoordinator.flag_tx = FALSE;// 再次发送版本号，查询命令
			}			
			/* 接收版本号数据*/
			if(App_zigbee_rx_data_pro() == ZIGBEE_CMD_CONFIG_NET_ADDRESS)
			{
				g_tZigbeeCoordinator.work_state = ZIGBEE_INIT_GetMAC_ADDRESS;
				g_tZigbeeCoordinator.flag_tx = FALSE;
				g_tZigbeeCoordinator.count_tx = 0;// ++
				bsp_StopTimer(TMR_ID_ZIGBEE_TIMEROUT);// 停止定时器
			}
						
	
			break;
		}
		case ZIGBEE_INIT_GetMAC_ADDRESS:
		{	
			/* 第一次发送，查询设备类型命令*/
			if(g_tZigbeeCoordinator.flag_tx == FALSE)
			{
				g_tZigbeeCoordinator.flag_tx = TRUE ; 
				g_tZigbeeCoordinator.count_tx++;// ++
				if(g_tZigbeeCoordinator.count_tx >= 255)
					g_tZigbeeCoordinator.count_tx = 255;
				
				App_zigbee_config_read_cmd(ZIGBEE_CMD_CONFIG_MAC_ADDRESS);
				bsp_StartTimer(TMR_ID_ZIGBEE_TIMEROUT, 500);		/* 设置500ms	 超时 */		
				
			}
			/* 500ms 超时，再次发送查询命令*/
			if (bsp_CheckTimer(TMR_ID_ZIGBEE_TIMEROUT))
			{
				g_tZigbeeCoordinator.flag_tx = FALSE;// 再次发送版本号，查询命令
			}			
			/* 接收版本号数据*/
			if(App_zigbee_rx_data_pro() == ZIGBEE_CMD_CONFIG_MAC_ADDRESS)
			{
				g_tZigbeeCoordinator.work_state = ZIGBEE_INIT_GetTXPOWER;
				g_tZigbeeCoordinator.flag_tx = FALSE;
				g_tZigbeeCoordinator.count_tx = 0;// ++
				bsp_StopTimer(TMR_ID_ZIGBEE_TIMEROUT);// 停止定时器
			}
			break;
		}
		case ZIGBEE_INIT_GetTXPOWER:
		{	
			/* 第一次发送，查询设备类型命令*/
			if(g_tZigbeeCoordinator.flag_tx == FALSE)
			{
				g_tZigbeeCoordinator.flag_tx = TRUE ; 
				g_tZigbeeCoordinator.count_tx++;// ++
				if(g_tZigbeeCoordinator.count_tx >= 255)
					g_tZigbeeCoordinator.count_tx = 255;
				
				App_zigbee_config_read_cmd(ZIGBEE_CMD_CONFIG_TXPOWER);
				bsp_StartTimer(TMR_ID_ZIGBEE_TIMEROUT, 500);		/* 设置500ms	 超时 */		
				
			}
			/* 500ms 超时，再次发送查询命令*/
			if (bsp_CheckTimer(TMR_ID_ZIGBEE_TIMEROUT))
			{
				g_tZigbeeCoordinator.flag_tx = FALSE;// 再次发送版本号，查询命令
			}			
			/* 接收版本号数据*/
			if(App_zigbee_rx_data_pro() == ZIGBEE_CMD_CONFIG_TXPOWER)
			{
				g_tZigbeeCoordinator.work_state = ZIGBEE_APP_OPEN_NETWORK;
				g_tZigbeeCoordinator.flag_tx = FALSE;
				g_tZigbeeCoordinator.count_tx = 0;// ++
				bsp_StopTimer(TMR_ID_ZIGBEE_TIMEROUT);// 停止定时器
			}
					
			break;
		}
		case ZIGBEE_APP_OPEN_NETWORK:
		{	
			/* 第一次发送，查询设备类型命令*/
			if(g_tZigbeeCoordinator.flag_tx == FALSE)
			{
				g_tZigbeeCoordinator.flag_tx = TRUE ; 
				g_tZigbeeCoordinator.count_tx++;// ++
				if(g_tZigbeeCoordinator.count_tx >= 255)
					g_tZigbeeCoordinator.count_tx = 255;
				
				App_zigbee_app_TurnOn_Off_network_cmd(ZIGBEE_APPLICAITON_CMD_Turn_ON_off_network,ZIGBEE_AddrMode_Signacast,ZIGBEE_TurnOn_network);//打开网络命令。
				
				bsp_StartTimer(TMR_ID_ZIGBEE_TIMEROUT, 500);		/* 设置500ms	 超时 */		
				
			}
			/* 500ms 超时，再次发送查询命令*/
			if (bsp_CheckTimer(TMR_ID_ZIGBEE_TIMEROUT))
			{
				g_tZigbeeCoordinator.flag_tx = FALSE;// 再次发送版本号，查询命令
			}			
			/* 接收版本号数据*/
			if((App_zigbee_rx_data_pro() == ZIGBEE_APPLICAITON_CMD_Turn_ON_off_network))
			{
				if((g_tZigbeeCoordinator.End_Device_feedback_state==FEEDBACK_OK))
				{
					g_tZigbeeCoordinator.work_state = ZIGBEE_LOOP;
					g_tZigbeeCoordinator.flag_tx = FALSE;
					g_tZigbeeCoordinator.count_tx = 0;// ++
					bsp_StopTimer(TMR_ID_ZIGBEE_TIMEROUT);// 停止定时器
				}
			}
					
			break;
		}
		
		
		case ZIGBEE_LOOP :
		{							

			rx_data_type = App_zigbee_rx_data_pro();// App_zigbee_rx_data_pro
			switch(rx_data_type)
			{
				case ZIGBEE_APPLICAITON_CMD_Device_Information:
				{

				}
					break;

				case ZIGBEE_APPLICAITON_CMD_transparent_data:
				{

				}
					break;


				case ZIGBEE_APPLICAITON_CMD_Offline_device_notification:
				{

				}
					break;


				case ZIGBEE_APPLICAITON_CMD_Turn_ON_off_network:
				{

				}
					break;


				case ZIGBEE_APPLICAITON_CMD_Remote_restart_command:
				{

				}
					break;


				case ZIGBEE_APPLICAITON_CMD_Remove_device:
				{

				}
					break;


				case ZIGBEE_APPLICAITON_CMD_Remove_device_notifications:
				{

				}
					break;


				case ZIGBEE_APPLICAITON_CMD_Query_signal_quality_of_the_local_device:
				{

				}
					break;


				case ZIGBEE_APPLICAITON_CMD_The_coordinator_queries_the_signal_quality_of_the_remote:
				{

				}
					break;
					
				case ZIGBEE_APPLICAITON_CMD_Add_group:
				{
				
				}
					break;
				
				
				case ZIGBEE_APPLICAITON_CMD_Delete_group:
				{
				
				}
					break;

				case ZIGBEE_APPLICAITON_CMD_Query_group:
				{
				
				}
					break;

				default:
					break;
			}

			break;
		}


		
		default:
			break;
	}
	
}

#endif

