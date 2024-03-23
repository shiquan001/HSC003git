#include "bsp.h"
#include "app.h"

#include "app_camera.h"
 CAMERA_STRCUT	  g_Camera;
static char rx_datas[CAMERA_DATA_LENTH];

 
 /*
u8 reset_cmd[4]={0x56,0x00,0x26,0x00};
u8 reset_rcv[4]={0x76,0x00,0x26,0x00};
u8 read_len_cmd[5]={0x56,0x00,0x34,0x01,0x00};
u8 read_len_rcv[7]={0x76,0x00,0x34,0x00,0x04,0x00,0x00};    
u8 start_photo_cmd[5]={0x56,0x00,0x36,0x01,0x00};    
u8 start_photo_rcv[5]={0x76,0x00,0x36,0x00,0x00};   
u8 get_photo_cmd [16] ={0x56,0x00,0x32,0x0C,0x00,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF }; 
u8 get_photo_rcv [5] ={0x76,0x00,0x32,0x00,0x00};
//设置压缩率命令,这里设为0x80 : 1,尽量使图像小
u8 set_compress_cmd [9] ={0x56,0x00,0x31,0x05,0x01,0x01,0x12,0x04,0x80};
u8 compress_rate_rcv [5] = {0x76,0x00,0x31,0x00,0x00};
u8 stop_photo_cmd [5] ={0x56,0x00,0x36,0x01,0x03};//停止拍照命令,它的返回命令与开始拍照的相同
u8 stop_photo_rcv[]={0x76,0x00,0x36,0x00,0x00};  	
u8 zoom_size_cmd [6] ={0x56,0x00, 0x54,0x01,0x11,0x00};  
u8 zoom_size_rcv [5] ={0x76,0x00,0x54,0x00,0x00,}; //设置图像大小命令
u8 photo_size_cmd [10] ={0x56,0x00,0x31,0x05,0x04,0x01,0x00,0x19,0x11,0x00};
u8 photo_size_rcv [5] ={0x76,0x00,0x31,0x00,0x00 };
u8 save_power_cmd [7] ={0x56, 0x00, 0x3E,0x03,0x00,0x01,0x00 }; 
u8 save_power_rcv [5] ={0x76,0x00,0x3E,0x00,0x00}; 	
u8 set_bps_cmd [7] ={0x56,0x00,0x24,0x03,0x01,0x0D,0xA6}; 
u8 set_bps_rcv [5] ={ 0x76,0x00,0x24,0x00,0x00};
u32 len1;//数据长度
u8 Camerbuff[5130];
u8 jpgname[50];
*/



 /**************************************************************************************					  
 * 名	  App_camera_tx_reset_camera_cmd
 * 功	  能:
 * 入口参数：
 * 出口参数：无
 * 说	  明：
 * 时	  间：
 * 作	  者: 王石泉
 * 调用方法：无
 ****************************************************************************************/
 void App_camera_tx_reset_camera_cmd(uint8_t address  )
 {
	 uint8_t buffer[16];
	 u8 buffer_lenth=0;
	 
	 memset(buffer,0x00,sizeof(buffer));

	 //协议标志(1 字节) + 序列号(1 字节) +命令字(1 字节) + 数据长度(1 字节) + 数据(0～16 字节)	 
	 buffer[buffer_lenth] = HEADER_TX;//协议标志
	 buffer_lenth++; 

	 buffer[buffer_lenth] = address;//序列号
	 buffer_lenth++; 
	 
	 buffer[buffer_lenth] = CMD_RESET;//功能码
	 buffer_lenth++; 
	 
	 buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
	 buffer_lenth++;		 	  	  

	 if (g_tUart5.SendBefor != 0)
	 {
		 g_tUart5.SendBefor();		 /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
	 }
	 HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
	 RS485_SendOver(); 
 }
/**************************************************************************************					   
* 名    称:App_camera_rx_reset_camera_cmd
* 功    能: 
* 入口	 参数：
* 出口	 参数：无
* 说    明： 
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
u8 App_camera_rx_reset_camera_cmd(void )
 {
  u16 pos = 0, i = 0;
  char ucData = 0,ret = 0;

  /*等待获得有效的485数据		*/		   
  if (bsp_CheckTimer(TMR_ID_485_UART))
  {
	   memset(rx_datas,0,sizeof(rx_datas));
#if USING_CAMERA_DEBUGE
	   p_info("camera rx_data:");
#endif
	   while(1)
	   {			   
		   if(comGetChar(Reserve_485_uart, (uint8_t *)&ucData))
		   {		   
			   rx_datas[pos++] = ucData;
	#if USING_CAMERA_DEBUGE
			   printf("%02X",ucData);
	#endif
		   }
		   else
			   break;

		   if(pos >= (CAMERA_DATA_LENTH))
			   break;
	   }
	   /* log */
	   if(pos <= 4)
	   {
		   ret = 2;
		   return ret;
	   }	   
	   for(i = 0;i<pos;i++)
	   {
		   if(rx_datas[i] == HEADER_RX)
		   {
			   break;
		   }
	   }
	   /* 数据长度太短了 丢弃*/
	   if((pos-i) <= 4)
	   {
		   ret = 3;
		   return ret;
	   }		   

	   if((rx_datas[i+1]==ADDRESS_CAMERA)&&(rx_datas[i+2]==CMD_RESET))
	   {
		   ret = 1;
		   p_info("  CMD_RESET ok");			   
	   }
  }
  return ret;	   


 }


 /**************************************************************************************					  
 * 名	  App_camera_tx_start_camera_cmd
 * 功	  能:
 * 入口参数：
 * 出口参数：无
 * 说	  明：
 * 时	  间：
 * 作	  者: 王石泉
 * 调用方法：无
 ****************************************************************************************/
 void App_camera_tx_start_camera_cmd(uint8_t address  )
 {
	 uint8_t buffer[16];
	 u8 buffer_lenth=0;
	 
	 memset(buffer,0x00,sizeof(buffer));

	 //协议标志(1 字节) + 序列号(1 字节) +命令字(1 字节) + 数据长度(1 字节) + 数据(0～16 字节)	 
	 buffer[buffer_lenth] = HEADER_TX;//协议标志
	 buffer_lenth++; 

	 buffer[buffer_lenth] = address;//序列号
	 buffer_lenth++; 
	 
	 buffer[buffer_lenth] = CMD_START_CAMERA;//功能码
	 buffer_lenth++; 
	 
	 buffer[buffer_lenth] = 0x01;//寄存器起始地址 2字节
	 buffer_lenth++;		 

	 buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
	 buffer_lenth++;		 	  	  

	 if (g_tUart5.SendBefor != 0)
	 {
		 g_tUart5.SendBefor();		 /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
	 }
	 HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
	 RS485_SendOver(); 
 }
 /**************************************************************************************					
 * 名	  称:App_camera_rx_start_camera_cmd
 * 功	  能: 
 * 入口	  参数：
 * 出口	  参数：无
 * 说	  明： 
 * 时	  间：
 * 作	  者: 王石泉
 * 调用方法：无
 ****************************************************************************************/
 u8 App_camera_rx_start_camera_cmd(void )
{
	u16 pos = 0, i = 0;
	char ucData = 0,ret = 0;

	/*等待获得有效的485数据 	 */ 		
	if (bsp_CheckTimer(TMR_ID_485_UART))
	{
		memset(rx_datas,0,sizeof(rx_datas));
#if USING_CAMERA_DEBUGE
		p_info("camera rx_data:");
#endif
		while(1)
		{				
			if(comGetChar(Reserve_485_uart, (uint8_t *)&ucData))
			{			
				rx_datas[pos++] = ucData;
#if USING_CAMERA_DEBUGE
				printf("%02X",ucData);
#endif
			}
			else
				break;

			if(pos >= (CAMERA_DATA_LENTH))
				break;
		}
		/* log */
		if(pos <= 4)
		{
			ret = 2;
			return ret;
		}		
		for(i = 0;i<pos;i++)
		{
			if(rx_datas[i] == HEADER_RX)
			{
				break;
			}
		}
		/* 数据长度太短了 丢弃*/
		if((pos-i) <= 4)
		{
			ret = 3;
			return ret;
		}			

		if((rx_datas[i+1]==ADDRESS_CAMERA)&&(rx_datas[i+2]==CMD_START_CAMERA))
		{
			ret = 1;
			p_info("  start_camera ok");				
		}
	}
	return ret;		
}

 /**************************************************************************************					  
 * 名	  App_camera_tx_read_datas_lenth_cmd
 * 功	  能:
 * 入口参数：
 * 出口参数：无
 * 说	  明：
 * 时	  间：
 * 作	  者: 王石泉
 * 调用方法：无
 ****************************************************************************************/
 void App_camera_tx_read_datas_lenth_cmd(uint8_t address  )
 {
	 uint8_t buffer[16];
	 u8 buffer_lenth=0;
	 
	 memset(buffer,0x00,sizeof(buffer));

	 //协议标志(1 字节) + 序列号(1 字节) +命令字(1 字节) + 数据长度(1 字节) + 数据(0～16 字节)	 
	 buffer[buffer_lenth] = HEADER_TX;//协议标志
	 buffer_lenth++; 

	 buffer[buffer_lenth] = address;//序列号
	 buffer_lenth++; 
	 
	 buffer[buffer_lenth] = CMD_READ_DATAS_LENTH;//功能码
	 buffer_lenth++; 
	 
	 buffer[buffer_lenth] = 0x01;//寄存器起始地址 2字节
	 buffer_lenth++;		 

	 buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
	 buffer_lenth++;		 	  	  

	 if (g_tUart5.SendBefor != 0)
	 {
		 g_tUart5.SendBefor();		 /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
	 }
	 HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
	 RS485_SendOver(); 
 }
/**************************************************************************************					 
* 名    称:App_camera_rx_start_camera_cmd
* 功    能: 
* 入口	   参数：
* 出口	   参数：无
* 说    明： 
* 时    间：
* 作    者: 王石泉
* 调用方法：无
****************************************************************************************/
u8 App_camera_rx_read_datas_lenth_cmd(void )
{
	 u16 pos = 0, i = 0;
	 char ucData = 0,ret = 0;

	 /*等待获得有效的485数据	  */		 
	 if (bsp_CheckTimer(TMR_ID_485_UART))
	 {
		 memset(rx_datas,0,sizeof(rx_datas));
#if USING_CAMERA_DEBUGE
		 p_info("camera rx_data:");
#endif
		 while(1)
		 {				 
			 if(comGetChar(Reserve_485_uart, (uint8_t *)&ucData))
			 {			 
				 rx_datas[pos++] = ucData;
#if USING_CAMERA_DEBUGE
				 printf("%02X",ucData);
#endif
			 }
			 else
				 break;

			 if(pos >= (CAMERA_DATA_LENTH))
				 break;
		 }
		 /* log */
		 if(pos <= 4)
		 {
			 ret = 2;
			 return ret;
		 }		 
		 for(i = 0;i<pos;i++)
		 {
			 if(rx_datas[i] == HEADER_RX)
			 {
				 break;
			 }
		 }
		 /* 数据长度太短了 丢弃*/
		 if((pos-i) <= 4)
		 {
			 ret = 3;
			 return ret;
		 }			 

		 if((rx_datas[i+1]==ADDRESS_CAMERA)&&(rx_datas[i+2]==CMD_READ_DATAS_LENTH))
		 {
			g_Camera.datas_lenth = 0;
	 		g_Camera.datas_lenth = rx_datas[i+7]<<8;// 高字节
	 		g_Camera.datas_lenth = g_Camera.datas_lenth|rx_datas[i+8];

			g_Camera.all_packages = g_Camera.datas_lenth/g_Camera.data_lenth_each_packages;
			if( g_Camera.datas_lenth%g_Camera.data_lenth_each_packages != 0)
			{
				g_Camera.all_packages ++;
			}
				
			g_Camera.rx_datas_count = 0;// 清零
			
			ret = 1;
			p_info("  read_datas_lenth ok");


			App_DeviceState_getpic_ask();

			APP_STATE = NB_CoAP_ST;   //发送数据
			p_info("App_DeviceState_getpic_ask 发送数据"); 				
		 }
	 }
	 return ret;	 
}

 /**************************************************************************************					  
 * 名	  App_camera_tx_read_datas_lenth_cmd
 * 功	  能: 
 * 入口参数：
 * 出口参数：无
 * 说	  明： 
 * 时	  间：
 * 作	  者: 王石泉
 * 调用方法：无
 ****************************************************************************************/
 void App_camera_tx_read_datas_cmd(uint8_t address ,u16 start_address,u16 lenth )
 {
	 uint8_t buffer[16];
	 u8 buffer_lenth=0;
	 
	 memset(buffer,0x00,sizeof(buffer));

	 //协议标志(1 字节) + 序列号(1 字节) +命令字(1 字节) + 数据长度(1 字节) + 数据(0～16 字节)	 
	 buffer[buffer_lenth] = HEADER_TX;//协议标志
	 buffer_lenth++; 

	 buffer[buffer_lenth] = address;//序列号
	 buffer_lenth++; 
	 
	 buffer[buffer_lenth] = CMD_READ_DATAS;//功能码
	 buffer_lenth++; 
	 
	 buffer[buffer_lenth] = 0x0C;//
	 buffer_lenth++;		 
	 buffer[buffer_lenth] = 0x00;//
	 buffer_lenth++;	
	 buffer[buffer_lenth] = 0x0A;//
	 buffer_lenth++;		 

	 buffer[buffer_lenth] = 0x00;//
	 buffer_lenth++;		 
	 buffer[buffer_lenth] = 0x00;//
	 buffer_lenth++;
	 buffer[buffer_lenth] = start_address>>8;//
	 buffer_lenth++;		 
	 buffer[buffer_lenth] = start_address;//
	 buffer_lenth++;	 

	 buffer[buffer_lenth] = 0x00;//
	 buffer_lenth++;		 
	 buffer[buffer_lenth] = 0x00;//
	 buffer_lenth++;
	 buffer[buffer_lenth] = lenth>>8;//
	 buffer_lenth++;		 
	 buffer[buffer_lenth] = lenth;//
	 buffer_lenth++;

	 buffer[buffer_lenth] = 0x00;//
	 buffer_lenth++;		 
	 buffer[buffer_lenth] = 0xFF;//
	 buffer_lenth++;

	 if (g_tUart5.SendBefor != 0)
	 {
		 g_tUart5.SendBefor();		 /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
	 }
	 HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
	 RS485_SendOver(); 
 }
 /**************************************************************************************				  
 * 名	  称:App_camera_rx_read_datas_cmd
 * 功	  能: 
 * 入口		参数：
 * 出口		参数：无
 * 说	  明： 
 * 时	  间：
 * 作	  者: 王石泉
 * 调用方法：无
 ****************************************************************************************/
 u8 App_camera_rx_read_datas_cmd(void )
 {
	  u16 pos = 0, i = 0,j=0;
	  char ucData = 0,ret = 0;
 
	  /*等待获得有效的485数据	   */		  
	  if (bsp_CheckTimer(TMR_ID_485_UART))
	  {
		  memset(rx_datas,0,sizeof(rx_datas));
#if USING_CAMERA_DEBUGE
		  p_info("camera rx_data:");
#endif
		  while(1)
		  { 			  
			  if(comGetChar(Reserve_485_uart, (uint8_t *)&ucData))
			  { 		  
				  rx_datas[pos++] = ucData;
#if USING_CAMERA_DEBUGE
				 // printf("%02X",ucData);
#endif
			  }
			  else
				  break;
 
			  if(pos >= (CAMERA_DATA_LENTH))
				  break;
		  }
		  p_info("camera rx_lenth:%d",pos);			   
		  
		  /* log */
		  if(pos <= 4)
		  {
			  ret = 2;
			  return ret;
		  } 	  
		  for(i = 0;i<pos;i++)
		  {
			  if(rx_datas[i] == HEADER_RX)
			  {
				  break;
			  }
		  }
	  
 
		  if((rx_datas[i+1]==ADDRESS_CAMERA)&&(rx_datas[i+2]==CMD_READ_DATAS))
		  {
			  memset(&g_Camera.data[0],0,CAMERA_DATA_LENTH);
		  
			for (j= 0; j < g_Camera.data_lenth_each_packages; j++)
			{
				g_Camera.data[j] = rx_datas[j+5];
			}

			App_DataReport_SensorExtend();
			APP_STATE = NB_CoAP_ST;   //发送数据
			g_Camera.server_ask_ok = 0;//
			bsp_StartTimer(TMR_ID_caiji_camera , TMR_DELAY_4S);//3S倒计时，
			p_info("App_DataReport_SensorExtend 发送数据"); 	

			g_Camera.rx_datas_count++;

			if(g_Camera.rx_datas_count*g_Camera.data_lenth_each_packages <g_Camera.datas_lenth )
			{
				ret = 1;
				p_info("  read_data  ok");				 
			}
			else
			{
				ret = 11;
				p_info("  read_data  over");				 
			}
		  }
	  }
	  return ret;	  
 }

 /**************************************************************************************					  
 * 名	  App_camera_tx_compress_rate_cmd
 * 功	  能: 
 * 入口参数：
 * 出口参数：无
 * 说	  明： XX 一般选 36 （范围：00 ----FF）
 * 时	  间：
 * 作	  者: 王石泉
 * 调用方法：无
 ****************************************************************************************/
 void App_camera_tx_compress_rate_cmd(uint8_t address ,u8 compress_rate )
 {
	 uint8_t buffer[16];
	 u8 buffer_lenth=0;
	 
	 memset(buffer,0x00,sizeof(buffer));

	 //协议标志(1 字节) + 序列号(1 字节) +命令字(1 字节) + 数据长度(1 字节) + 数据(0～16 字节)	 
	 buffer[buffer_lenth] = HEADER_TX;//协议标志
	 buffer_lenth++; 

	 buffer[buffer_lenth] = address;//序列号
	 buffer_lenth++; 
	 
	 buffer[buffer_lenth] = CMD_set_compress_cmd;//功能码
	 buffer_lenth++; 	 
	 buffer[buffer_lenth] = 0x05;//寄存器起始地址 2字节
	 buffer_lenth++;	
	 
	 buffer[buffer_lenth] = 0x01;//寄存器起始地址 2字节
	 buffer_lenth++;		 
	 buffer[buffer_lenth] = 0x01;//寄存器起始地址 2字节
	 buffer_lenth++;	
	 buffer[buffer_lenth] = 0x12;//寄存器起始地址 2字节
	 buffer_lenth++;		 	  	  
	 buffer[buffer_lenth] = 0x04;//寄存器起始地址 2字节
	 buffer_lenth++;	

	 buffer[buffer_lenth] = compress_rate;//寄存器起始地址 2字节
	 buffer_lenth++;	

	 if (g_tUart5.SendBefor != 0)
	 {
		 g_tUart5.SendBefor();		 /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
	 }
	 HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
	 RS485_SendOver(); 
 }
 /**************************************************************************************					  
 * 名	  称:App_camera_rx_compress_rate_cmd
 * 功	  能: 
 * 入口	参数：
 * 出口	参数：无
 * 说	  明： 
 * 时	  间：
 * 作	  者: 王石泉
 * 调用方法：无
 ****************************************************************************************/
 u8 App_camera_rx_compress_rate_cmd(void )
{
	 u16 pos = 0, i = 0;
	 char ucData = 0,ret = 0;

	 /*等待获得有效的485数据	   */		  
	 if (bsp_CheckTimer(TMR_ID_485_UART))
	 {
		  memset(rx_datas,0,sizeof(rx_datas));
#if USING_CAMERA_DEBUGE
		  p_info("camera rx_data:");
#endif
		  while(1)
		  { 			  
			  if(comGetChar(Reserve_485_uart, (uint8_t *)&ucData))
			  { 		  
				  rx_datas[pos++] = ucData;
		#if USING_CAMERA_DEBUGE
				  printf("%02X",ucData);
		#endif
			  }
			  else
				  break;

			  if(pos >= (CAMERA_DATA_LENTH))
				  break;
		  }
		  /* log */
		  if(pos <= 4)
		  {
			  ret = 2;
			  return ret;
		  } 	  
		  for(i = 0;i<pos;i++)
		  {
			  if(rx_datas[i] == HEADER_RX)
			  {
				  break;
			  }
		  }
		  /* 数据长度太短了 丢弃*/
		  if((pos-i) <= 4)
		  {
			  ret = 3;
			  return ret;
		  } 		  

		  if((rx_datas[i+1]==ADDRESS_CAMERA)&&(rx_datas[i+2]==CMD_set_compress_cmd))
		  {
			  ret = 1;
			  p_info("compress_rate ok");			  
		  }
	 }
	 return ret;	  


}

 /**************************************************************************************					  
 * 名	  App_camera_tx_photo_size_cmd
 * 功	  能: 
 * 入口参数：
 * 出口参数：无
 * 说	  明： 
 * 时	  间：
 * 作	  者: 王石泉
 * 调用方法：无
 ****************************************************************************************/
 void App_camera_tx_photo_size_cmd(uint8_t address ,u8 size_type )
 {
	 uint8_t buffer[16];
	 u8 buffer_lenth=0;
	 
	 memset(buffer,0x00,sizeof(buffer));

	 //协议标志(1 字节) + 序列号(1 字节) +命令字(1 字节) + 数据长度(1 字节) + 数据(0～16 字节)	 
	 buffer[buffer_lenth] = HEADER_TX;//协议标志
	 buffer_lenth++; 

	 buffer[buffer_lenth] = address;//序列号
	 buffer_lenth++; 
	 
	 buffer[buffer_lenth] = CMD_photo_size_cmd;//功能码
	 buffer_lenth++; 
	 
	 buffer[buffer_lenth] = 0x05;//寄存器起始地址 2字节
	 buffer_lenth++;		 

	 buffer[buffer_lenth] = 0x04;//寄存器起始地址 2字节
	 buffer_lenth++;	
	 buffer[buffer_lenth] = 0x01;//寄存器起始地址 2字节
	 buffer_lenth++;		 	  	  
	 buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
	 buffer_lenth++;	
	 buffer[buffer_lenth] = 0x19;//寄存器起始地址 2字节
	 buffer_lenth++;	
	 
	 buffer[buffer_lenth] = size_type;//寄存器起始地址 2字节
	 buffer_lenth++;	


	 if (g_tUart5.SendBefor != 0)
	 {
		 g_tUart5.SendBefor();		 /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
	 }
	 HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
	 RS485_SendOver(); 
 }
 /**************************************************************************************					  
 * 名	  App_camera_rx_photo_size_cmd
 * 功	  能: 
 * 入口参数：
 * 出口参数：无
 * 说	  明： 
 * 时	  间：
 * 作	  者: 王石泉
 * 调用方法：无
 ****************************************************************************************/
 u8 App_camera_rx_photo_size_cmd(void )
 {
	u16 pos = 0, i = 0;
	char ucData = 0,ret = 0;

	/*等待获得有效的485数据	  */		 
	if (bsp_CheckTimer(TMR_ID_485_UART))
	{
		 memset(rx_datas,0,sizeof(rx_datas));
#if USING_CAMERA_DEBUGE
		 p_info("camera rx_data:");
#endif
		 while(1)
		 {				 
			 if(comGetChar(Reserve_485_uart, (uint8_t *)&ucData))
			 {			 
				 rx_datas[pos++] = ucData;
			#if USING_CAMERA_DEBUGE
				 printf("%02X",ucData);
			#endif
			 }
			 else
				 break;

			 if(pos >= (CAMERA_DATA_LENTH))
				 break;
		 }
		 /* log */
		 if(pos <= 4)
		 {
			 ret = 2;
			 return ret;
		 }		 
		 for(i = 0;i<pos;i++)
		 {
			 if(rx_datas[i] == HEADER_RX)
			 {
				 break;
			 }
		 }
		 /* 数据长度太短了 丢弃*/
		 if((pos-i) <= 4)
		 {
			 ret = 3;
			 return ret;
		 }			 

		 if((rx_datas[i+1]==ADDRESS_CAMERA)&&(rx_datas[i+2]==CMD_photo_size_cmd))
		 {
			 ret = 1;
			 p_info("  CMD_photo_size_cmd ok");				 
		 }
	}
	return ret;		 
}

 /**************************************************************************************					  
 * 名	  App_camera_tx_clear_datas_cmd
 * 功	  能: 
 * 入口参数：
 * 出口参数：无
 * 说	  明： 
 * 时	  间：
 * 作	  者: 王石泉
 * 调用方法：无
 ****************************************************************************************/
 void App_camera_tx_clear_datas_cmd(uint8_t address  )
 {
	 uint8_t buffer[16];
	 u8 buffer_lenth=0;
	 
	 memset(buffer,0x00,sizeof(buffer));

	 //协议标志(1 字节) + 序列号(1 字节) +命令字(1 字节) + 数据长度(1 字节) + 数据(0～16 字节)	 
	 buffer[buffer_lenth] = HEADER_TX;//协议标志
	 buffer_lenth++; 

	 buffer[buffer_lenth] = address;//序列号
	 buffer_lenth++; 
	 
	 buffer[buffer_lenth] = CMD_CLEAR_DATAS;//功能码
	 buffer_lenth++; 
	 
	 buffer[buffer_lenth] = 0x01;//寄存器起始地址 2字节
	 buffer_lenth++;		 

	 buffer[buffer_lenth] = 0x03;//寄存器起始地址 2字节
	 buffer_lenth++;		 	  	  

	 if (g_tUart5.SendBefor != 0)
	 {
		 g_tUart5.SendBefor();		 /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
	 }
	 HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
	 RS485_SendOver(); 
 }

 /**************************************************************************************					  
 * 名	  App_camera_tx_clear_datas_cmd
 * 功	  能: 
 * 入口参数：
 * 出口参数：无
 * 说	  明： 
 * 时	  间：
 * 作	  者: 王石泉
 * 调用方法：无
 ****************************************************************************************/
 void App_camera_tx_set_address_cmd(uint8_t address, uint8_t new_address )
 {
	 uint8_t buffer[16];
	 u8 buffer_lenth=0;
	 
	 memset(buffer,0x00,sizeof(buffer));

	 //协议标志(1 字节) + 序列号(1 字节) +命令字(1 字节) + 数据长度(1 字节) + 数据(0～16 字节)	 
	 buffer[buffer_lenth] = HEADER_TX;//协议标志
	 buffer_lenth++; 

	 buffer[buffer_lenth] = address;//序列号
	 buffer_lenth++; 
	 
	 buffer[buffer_lenth] = CMD_set_address_cmd;//功能码
	 buffer_lenth++; 
	 
	 buffer[buffer_lenth] = 0x05;//寄存器起始地址 2字节
	 buffer_lenth++;	
	 
	 buffer[buffer_lenth] = 0x04;//寄存器起始地址 2字节
	 buffer_lenth++;	
	 buffer[buffer_lenth] = 0x01;//寄存器起始地址 2字节
	 buffer_lenth++;		 	  	  
	 buffer[buffer_lenth] = 0x00;//寄存器起始地址 2字节
	 buffer_lenth++;	
	 buffer[buffer_lenth] = 0x06;//寄存器起始地址 2字节
	 buffer_lenth++;	

	 buffer[buffer_lenth] = new_address;//寄存器起始地址 2字节
	 buffer_lenth++;

	 if (g_tUart5.SendBefor != 0)
	 {
		 g_tUart5.SendBefor();		 /* 如果是RS485通信，可以在这个函数中将RS485设置为发送模式 */
	 }
	 HAL_UART_Transmit(&huart5, buffer,buffer_lenth, 0xFFFF);
	 RS485_SendOver(); 
 }
#define CAMERA_TX_DATA_LENTH 200//240//192//96//200//512//200  //因为236字节的缘故； 96*2+18=210

/*
*********************************************************************************************************
*	函 数 名: App_camera_Init_Var
*	功能说明:  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_camera_Init_Var(void)
{
	g_Camera.start_state = FALSE;
	g_Camera.work_state = CAMERA_INIT;
	g_Camera.update_flag = 0;

	g_Camera.datas_lenth = 0;
	g_Camera.all_packages = 0;
	g_Camera.data_lenth_each_packages = CAMERA_TX_DATA_LENTH;

	g_Camera.rx_datas_count = 0;// 清零
	
	g_Camera.server_ask_ok = 0;// 清零

	g_Camera.poweroffpic = 0;
	g_Camera.getpicpack = 0;
	g_Camera.getpicpack_count = 0;
	
}
/*
*********************************************************************************************************
*	函 数 名: App_camera_timer_poweroff
*	功能说明:  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_camera_timer_poweroff(void)
{
	if(bsp_CheckTimer(TMR_ID_caiji_camera_poweroff))
	{
		g_Camera.work_state = CAMERA_clear_datas;					
		p_err("TMR_ID_caiji_camera_poweroff goto CAMERA_clear_datas");								
	}

}

/*
*********************************************************************************************************
*	函 数 名: App_camera_Loop
*	功能说明:  
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void App_camera_Loop(void)
{	
	u8 result = 0;
	if(g_Camera.start_state == FALSE)
	{
		return ;
	}
	
	switch(g_Camera.work_state)
	{
		case CAMERA_INIT :
		{				
			Bsp_12V_power_on();	//12 供电。				
			Bsp_sensor_power_on(); //sensor供电。	
			MX_UART5_Init();//初始化串口5防止死机； 20211007
			
			bsp_StartTimer(TMR_ID_caiji_camera , TMR_DELAY_3S);//3S
			g_Camera.work_state = CAMERA_delay;
			bsp_StartTimer(TMR_ID_caiji_camera_poweroff , TMR_DELAY_5min);//3S
			p_info("goto CAMERA_delay");				
		}
			break;
		
		case CAMERA_delay :
		{							
			if(bsp_CheckTimer(TMR_ID_caiji_camera))
			{
				g_Camera.work_state = CAMERA_set_size;
				comClearRxFifo(COM5);//COM5
				
				g_Camera.flag_tx = FALSE;
				g_Camera.count_tx = 0;// ++
				p_info("goto CAMERA_set_size");								
			}
			App_camera_timer_poweroff();//5分钟自动退出
		}
			break;
		case CAMERA_set_size :
		{
			/* 第一次发送，查询版本号命令*/
			if(g_Camera.flag_tx == FALSE)
			{
				g_Camera.flag_tx = TRUE ; 
				g_Camera.count_tx++;// ++
				if(g_Camera.count_tx >= 100)
					g_Camera.count_tx = 100;
				
				App_camera_tx_photo_size_cmd(ADDRESS_CAMERA,CMD_photo_size_320240);
				bsp_StartTimer(TMR_ID_camera_TIMEROUT, TMR_DELAY_0_25S);		/* 设置500ms	 超时 */		
			}		
			/* 500ms 超时，再次发送查询命令*/
			if (bsp_CheckTimer(TMR_ID_camera_TIMEROUT))
			{
				g_Camera.flag_tx = FALSE;// 再次发送版本号，查询命令				
				p_err("TMR_ID_camera_TIMEROUT");
			}		
			
			if(App_camera_rx_photo_size_cmd()==1)
			{
				g_Camera.work_state = CAMERA_reset;
				g_Camera.flag_tx = FALSE;
				g_Camera.count_tx = 0;// ++
				p_info("goto CAMERA_reset");								
			}
			App_camera_timer_poweroff();//5分钟自动退出
		}
			break;	
		case CAMERA_reset :
		{
			/* 第一次发送，查询版本号命令*/
			if(g_Camera.flag_tx == FALSE)
			{
				g_Camera.flag_tx = TRUE ; 
				g_Camera.count_tx++;// ++
				if(g_Camera.count_tx >= 100)
					g_Camera.count_tx = 100;
				
				App_camera_tx_reset_camera_cmd(ADDRESS_CAMERA);
				bsp_StartTimer(TMR_ID_camera_TIMEROUT, TMR_DELAY_0_25S);		/* 设置500ms	 超时 */		
			}		
			/* 500ms 超时，再次发送查询命令*/
			if (bsp_CheckTimer(TMR_ID_camera_TIMEROUT))
			{
				g_Camera.flag_tx = FALSE;// 再次发送版本号，查询命令				
				p_err("TMR_ID_camera_TIMEROUT");
			}			
			if(App_camera_rx_reset_camera_cmd()==1)
			{
				g_Camera.work_state = CAMERA_reset_delay;
				bsp_StartTimer(TMR_ID_caiji_camera , TMR_DELAY_3S);//3S
				p_info("goto CAMERA_reset_delay");								
			}
			App_camera_timer_poweroff();//5分钟自动退出
		}
			break;	
		case CAMERA_reset_delay :
		{							
			if(bsp_CheckTimer(TMR_ID_caiji_camera))
			{
				g_Camera.work_state = CAMERA_start_camera;
				
				g_Camera.flag_tx = FALSE;
				g_Camera.count_tx = 0;// ++
				
				comClearRxFifo(COM5);//COM5
				
				p_info("goto CAMERA_start_camera");								
			}
			App_camera_timer_poweroff();//5分钟自动退出
		}
			break;					
		case CAMERA_start_camera :
		{
			/* 第一次发送，查询版本号命令*/
			if(g_Camera.flag_tx == FALSE)
			{
				g_Camera.flag_tx = TRUE ; 
				g_Camera.count_tx++;// ++
				if(g_Camera.count_tx >= 100)
					g_Camera.count_tx = 100;
				
				App_camera_tx_start_camera_cmd(ADDRESS_CAMERA);
				
				bsp_StartTimer(TMR_ID_camera_TIMEROUT, TMR_DELAY_0_25S);		/* 设置500ms	 超时 */		
			}		
			/* 500ms 超时，再次发送查询命令*/
			if (bsp_CheckTimer(TMR_ID_camera_TIMEROUT))
			{
				g_Camera.flag_tx = FALSE;// 再次发送版本号，查询命令				
				p_err("TMR_ID_camera_TIMEROUT");
			}			
			if(App_camera_rx_start_camera_cmd()==1)
			{
				g_Camera.work_state = CAMERA_read_datas_lenth;
				g_Camera.flag_tx = FALSE;
				g_Camera.count_tx = 0;// ++
				p_info("goto CAMERA_read_datas_lenth");								
			}		
			App_camera_timer_poweroff();//5分钟自动退出
		}
			break;	
		
		case CAMERA_read_datas_lenth :
		{
			/* 第一次发送，查询版本号命令*/
			if(g_Camera.flag_tx == FALSE)
			{
				g_Camera.flag_tx = TRUE ; 
				g_Camera.count_tx++;// ++
				if(g_Camera.count_tx >= 100)
					g_Camera.count_tx = 100;
				App_camera_tx_read_datas_lenth_cmd(ADDRESS_CAMERA);
				
				bsp_StartTimer(TMR_ID_camera_TIMEROUT, TMR_DELAY_0_25S);		/* 设置500ms	 超时 */		
			}		
			/* 500ms 超时，再次发送查询命令*/
			if (bsp_CheckTimer(TMR_ID_camera_TIMEROUT))
			{
				g_Camera.flag_tx = FALSE;// 再次发送版本号，查询命令				
				p_err("TMR_ID_camera_TIMEROUT");
			}		
			if(App_camera_rx_read_datas_lenth_cmd()==1)
			{
				g_Camera.work_state = CAMERA_read_datas;
				App_camera_tx_read_datas_cmd(ADDRESS_CAMERA,g_Camera.rx_datas_count*g_Camera.data_lenth_each_packages,g_Camera.data_lenth_each_packages);
				p_info("goto CAMERA_read_datas");								
			}
			App_camera_timer_poweroff();//5分钟自动退出
		}
			break;
		
		case CAMERA_read_datas :
		{
			result = App_camera_rx_read_datas_cmd();
			if(result == 1)
			{
				g_Camera.work_state = CAMERA_tx_datas;
				p_info("goto CAMERA_tx_datas");								
			}
			else if(result == 11)
			{
				g_Camera.work_state = CAMERA_poweroffpic_getpicpack;
				p_info("goto CAMERA_poweroffpic_getpicpack");								
			}				
			App_camera_timer_poweroff();//5分钟自动退出
		}
			break;
		case CAMERA_tx_datas :
		{		
			if(bsp_CheckTimer(TMR_ID_caiji_camera))
			{
				g_Camera.rx_datas_count--;//发送失败，重新开始
				g_Camera.work_state = CAMERA_read_datas;
				App_camera_tx_read_datas_cmd(ADDRESS_CAMERA,g_Camera.rx_datas_count*g_Camera.data_lenth_each_packages,g_Camera.data_lenth_each_packages);
				p_err("goto CAMERA_read_datas");								
			}
			
			if(g_Camera.server_ask_ok == 1)
			{
				g_Camera.work_state = CAMERA_read_datas;
				App_camera_tx_read_datas_cmd(ADDRESS_CAMERA,g_Camera.rx_datas_count*g_Camera.data_lenth_each_packages,g_Camera.data_lenth_each_packages);
				p_info("goto CAMERA_read_datas");								
			}
			App_camera_timer_poweroff();//5分钟自动退出
		}
			break;

		case CAMERA_poweroffpic_getpicpack :
		{
			if(g_Camera.poweroffpic == 1)
			{
				App_DeviceState_poweroffpic_ask();
				APP_STATE = NB_CoAP_ST;   //发送数据
				p_info("App_DeviceState_poweroffpic_ask 发送数据"); 	

				g_Camera.work_state = CAMERA_clear_datas;					
				p_info("goto CAMERA_clear_datas");								
			}

			if(g_Camera.getpicpack == 1)
			{
				g_Camera.getpicpack = 0;
				g_Camera.rx_datas_count = g_Camera.getpicpack_count;
				App_camera_tx_read_datas_cmd(ADDRESS_CAMERA,g_Camera.rx_datas_count*g_Camera.data_lenth_each_packages,g_Camera.data_lenth_each_packages);
			}	

			result = App_camera_rx_read_datas_cmd();
			if(result == 1)
			{
				p_info("(result == 1)"); 							
			}
			else if(result == 11)
			{
				p_info("(result == 11)");								
			}							
			App_camera_timer_poweroff();//5分钟自动退出
		}
			break;

		case CAMERA_clear_datas :
		{
			App_camera_Init_Var();
			Bsp_12V_power_off();	//12 供电。				
			Bsp_sensor_power_off(); //sensor供电。				
		}
			break;
		
		default:
			break;
	}
}

